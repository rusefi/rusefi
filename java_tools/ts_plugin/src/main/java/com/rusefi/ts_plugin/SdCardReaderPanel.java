package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.IoStream;
import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.text.CharacterIterator;
import java.text.StringCharacterIterator;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.TS_SD_PROTOCOL_FETCH_INFO;
import static com.rusefi.shared.FileUtil.close;

public class SdCardReaderPanel {
    private final static int TRANSFER_HEADER_SIZE = 3;
    private static final Logging log = getLogging(SdCardReaderPanel.class);

    private final JPanel fileList = new JPanel(new VerticalFlowLayout());
    private final JLabel status = new JLabel();
    private final Supplier<ControllerAccess> controllerAccessSupplier;
    private final Supplier<IoStream> ioStreamSupplier;
    private final Container parent;

    private final JPanel content = new JPanel(new BorderLayout());

    public SdCardReaderPanel(Supplier<ControllerAccess> controllerAccessSupplier,
                             Supplier<IoStream> ioStreamSupplier, Container parent) {
        this.controllerAccessSupplier = controllerAccessSupplier;
        this.ioStreamSupplier = ioStreamSupplier;
        this.parent = parent;
        JButton refresh = new JButton("Refresh");
        refresh.addActionListener(e -> ConnectPanel.IO_THREAD.execute(this::requestFileList));

        JButton open = new JButton("Open Destination Folder");
        JPanel lowPanel = new JPanel(new FlowLayout());
        lowPanel.add(refresh);
        lowPanel.add(open);

        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(status, BorderLayout.CENTER);
        topPanel.add(lowPanel, BorderLayout.SOUTH);

        content.add(topPanel, BorderLayout.NORTH);
        content.add(fileList, BorderLayout.CENTER);

        open.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String folder = getDestinationFolder();
                    Runtime.getRuntime().exec("explorer.exe /select," + folder + File.separator);
                } catch (IOException ex) {
                    log.error("Error", ex);
                }
            }
        });
    }

    public JPanel getContent() {
        return content;
    }

    @NotNull
    private byte[] getDirContent() throws IOException {
        byte[] packet;
        byte[] response;
        IoStream stream = ioStreamSupplier.get();

        packet = new byte[3];
        packet[0] = Fields.TS_SD_R_COMMAND;
        packet[2] = Fields.TS_SD_PROTOCOL_RTC;
        response = stream.sendAndGetPacket(packet, "RTC status", false);
        log.info("RTC response " + IoStream.printHexBinary(response));
        if (response == null)
            throw new IOException("RTC No packet");

        packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_READ_DIR;
        response = stream.sendAndGetPacket(packet, "read dir command", false);
        if (response == null)
            throw new IOException("Read Dir No packet");
        log.info("read dir command " + IoStream.printHexBinary(response));

        packet = new byte[8];
        packet[0] = Fields.TS_SD_R_COMMAND;
        packet[1] = 0;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[5] = 0x02;
        packet[6] = 0x02;
        response = stream.sendAndGetPacket(packet, "read command", true);
        if (response == null)
            throw new IOException("No packet");
        log.info("read command " + IoStream.printHexBinary(response));
        return response;
    }

    @NotNull
    public String getDestinationFolder() {
        return LogUploadSelector.getLogsFolderDir(controllerAccessSupplier.get().getEcuConfigurationNames()[0]);
    }

    private void downloadFile(String fileName) {
        String lastFour = ConnectPanel.getLastFour(fileName);

        byte[] packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_FETCH_COMPRESSED;
        applyLastFour(lastFour, packet);

        IoStream stream = ioStreamSupplier.get();

        FileOutputStream fos = null;
        try {
            byte[] response = stream.sendAndGetPacket(packet, "Download file", false);
            log.info("Download file " + IoStream.printHexBinary(response));
            setStatus("Downloading " + fileName);

            fos = new FileOutputStream(getDestinationFolder() + File.separator + fileName, false);
            int chunk = 0;
            int totalSize = 0;
            long start = System.currentTimeMillis();
            while (true) {
                packet = new byte[17];
                packet[0] = Fields.TS_SD_R_COMMAND;
                packet[2] = Fields.TS_SD_PROTOCOL_FETCH_DATA;
                packet[3] = (byte) chunk;
                packet[4] = (byte) (chunk >> 8);

                // we are competing with gauge poking thread for instance
                response = stream.sendAndGetPacket(packet, "Get file", true);

                if (response == null) {
                    log.info("No content response");
                    break;
                }

                int dataBytes = response.length - TRANSFER_HEADER_SIZE;
                totalSize += dataBytes;

                if (chunk % 10 == 0)
                    log.info("Got content package size " + response.length + "/total=" + totalSize);

                fos.write(response, TRANSFER_HEADER_SIZE, dataBytes);

                if (dataBytes != 2048) {
                    log.info(response.length + " must be the last packet");
                    long duration = System.currentTimeMillis() - start;
                    setStatus(fileName + " downloaded " + humanReadableByteCountBin(totalSize) + " in " + duration + " ms");
                    break;
                }
                if (chunk % 10 == 0)
                    setStatus(humanReadableByteCountBin(totalSize) + " so far");
                chunk++;
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        } finally {
            close(fos);
        }
    }

    private void applyLastFour(String lastFour, byte[] packet) {
        for (int i = 0; i < 4; i++)
            packet[7 + i] = (byte) lastFour.charAt(i);
    }

    private static String humanReadableByteCountBin(long bytes) {
        long absB = bytes == Long.MIN_VALUE ? Long.MAX_VALUE : Math.abs(bytes);
        if (absB < 1024) {
            return bytes + " B";
        }
        long value = absB;
        CharacterIterator ci = new StringCharacterIterator("KMGTPE");
        for (int i = 40; i >= 0 && absB > 0xfffccccccccccccL >> i; i -= 10) {
            value >>= 10;
            ci.next();
        }
        value *= Long.signum(bytes);
        return String.format("%.1f %ciB", value / 1024.0, ci.current());
    }

    private void deleteFile(String fileName) {
        String lastFour = ConnectPanel.getLastFour(fileName);

        byte[] packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_REMOVE_FILE;
        applyLastFour(lastFour, packet);

        IoStream stream = ioStreamSupplier.get();

        try {
            stream.sendPacket(packet);
            byte[] response = stream.getDataBuffer().getPacket("delete file");
            log.info("Delete file " + IoStream.printHexBinary(response));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private void requestFileList() {
        int fileCount = 0;
        status.setText("Reading file list...");
        try {
            byte[] response = getDirContent();

            fileList.removeAll();

            for (int fileIndex = 0; fileIndex < 512 / 32; fileIndex++) {
                int offset = 32 * fileIndex;
                String fileNamePart = new String(response, 1 + offset, 8).trim();
                if (fileNamePart.trim().isEmpty())
                    break;
                fileCount++;
                String fileExt = new String(response, 1 + offset + 8, 3).trim();
                String fileName = fileNamePart + "." + fileExt;

                ByteBuffer bb = ByteBuffer.wrap(response, 1 + offset + 28, 4);
                bb.order(ByteOrder.LITTLE_ENDIAN);
                int fileSize = bb.getInt();

                JPanel filePanel = new JPanel(new FlowLayout());

                filePanel.add(new JLabel(fileName + " " + humanReadableByteCountBin(fileSize)));

                JButton download = new JButton("Download");
                download.addActionListener(e -> ConnectPanel.IO_THREAD.execute(() -> downloadFile(fileName)));

                filePanel.add(download);
                JButton delete = new JButton("Delete");
                delete.addActionListener(e1 -> {
                    int result = JOptionPane.showConfirmDialog(null, "Are you sure you want to remove " + fileName,
                            "rusEfi", JOptionPane.YES_NO_OPTION);
                    if (result == JOptionPane.YES_OPTION) {

                        ConnectPanel.IO_THREAD.execute(() -> {
                            deleteFile(fileName);
                            setStatus("Deleted " + fileName);
                            requestFileList();
                        });
                    }
                });
                filePanel.add(delete);

                fileList.add(filePanel);
                fileList.revalidate();
                AutoupdateUtil.trueLayout(fileList);

                log.info("Filename " + fileName + " size " + fileSize);

                AutoupdateUtil.trueLayout(parent);
            }

        } catch (IOException ioException) {
            ioException.printStackTrace();
        }

        if (fileCount == 0) {
            status.setText("No files found.");
        }
    }

    public void setStatus(String message) {
        SwingUtilities.invokeLater(() -> status.setText(message));
    }

    public void onConnectionEstablished() {
        ConnectPanel.IO_THREAD.execute(this::requestFileList);
    }
}
