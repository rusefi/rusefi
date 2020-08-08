package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
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
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.function.Supplier;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.config.generated.Fields.TS_SD_PROTOCOL_FETCH_INFO;
import static com.rusefi.shared.FileUtil.close;

public class SdCardReader {
    private static final Logging log = getLogging(SdCardReader.class);
    private final static int TRANSFER_HEADER_SIZE = 3;
    private final JPanel content = new JPanel(new BorderLayout());

    private final JPanel fileList = new JPanel(new VerticalFlowLayout());
    private final JLabel status = new JLabel();

    private static final Executor IO_THREAD = Executors.newSingleThreadExecutor();

    private final ConnectPanel connectPanel = new ConnectPanel(new ConnectionStateListener() {
        public void onConnectionEstablished() {
            IO_THREAD.execute(() -> requestFileList());
        }

        public void onConnectionFailed() {
        }
    });
    private final Supplier<ControllerAccess> controllerAccessSupplier;

    public SdCardReader(Supplier<ControllerAccess> controllerAccessSupplier) {
        this.controllerAccessSupplier = controllerAccessSupplier;
        JButton refresh = new JButton("Refresh");
        refresh.addActionListener(e -> IO_THREAD.execute(this::requestFileList));

        JPanel topPanel = new JPanel(new BorderLayout());
        JPanel lowPanel = new JPanel(new FlowLayout());

        JButton open = new JButton("Open Destination Folder");
        lowPanel.add(refresh);
        lowPanel.add(open);

        open.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    String folder = getDestinationFolder(controllerAccessSupplier);
                    Runtime.getRuntime().exec("explorer.exe /select," + folder + File.separator);
                } catch (IOException ex) {
                    log.error("Error", ex);
                }
            }
        });

        topPanel.add(connectPanel.getContent(), BorderLayout.NORTH);
        topPanel.add(status, BorderLayout.CENTER);
        topPanel.add(lowPanel, BorderLayout.SOUTH);

        content.add(topPanel, BorderLayout.NORTH);

        JScrollPane fileListScroll = new JScrollPane(fileList, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        content.add(fileListScroll, BorderLayout.CENTER);

        content.add(new JLabel("<html>This tab allows direct access to SD card<br/>Please be sure to disconnect Tuner Studio from ECU while downloading files using this tab"), BorderLayout.SOUTH);
    }

    @NotNull
    private String getDestinationFolder(Supplier<ControllerAccess> controllerAccessSupplier) {
        return LogUploadSelector.getLogsFolderDir(controllerAccessSupplier.get().getEcuConfigurationNames()[0]);
    }

    public Component getContent() {
        return content;
    }

    private void requestFileList() {
        int fileCount = 0;

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
                download.addActionListener(e -> IO_THREAD.execute(() -> downloadFile(fileName)));

                filePanel.add(download);
                JButton delete = new JButton("Delete");
                delete.addActionListener(e1 -> {
                    int result = JOptionPane.showConfirmDialog(null, "Are you sure you want to remove " + fileName,
                            "rusEfi", JOptionPane.YES_NO_OPTION);
                    if (result == JOptionPane.YES_OPTION) {

                        IO_THREAD.execute(() -> {
                            deleteFile(fileName);
                            setStatus("Deleted " + fileName);
                            requestFileList();
                        });
                    }
                });
                filePanel.add(delete);

                fileList.add(filePanel);

                log.info("Filename " + fileName + " size " + fileSize);

                AutoupdateUtil.trueLayout(content.getParent());
            }

        } catch (IOException ioException) {
            ioException.printStackTrace();
        }

        if (fileCount == 0) {
            status.setText("No files found.");
        }
    }

    @NotNull
    private byte[] getDirContent() throws IOException {
        byte[] packet;
        byte[] response;
        IoStream stream = connectPanel.getControllerConnector().getConnector().getBinaryProtocol().getStream();

        packet = new byte[3];
        packet[0] = Fields.TS_SD_R_COMMAND;
        packet[2] = Fields.TS_SD_PROTOCOL_RTC;
        stream.sendPacket(packet);
        response = stream.getDataBuffer().getPacket("RTC status");
        log.info("RTC response " + IoStream.printHexBinary(response));
        if (response == null)
            throw new IOException("RTC No packet");

        packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_READ_DIR;
        stream.sendPacket(packet);
        response = stream.getDataBuffer().getPacket("read dir command");
        if (response == null)
            throw new IOException("Read Dir No packet");
        log.info("read dir command " + IoStream.printHexBinary(response));

        packet = new byte[8];
        packet[0] = Fields.TS_SD_R_COMMAND;
        packet[1] = 0;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[5] = 0x02;
        packet[6] = 0x02;
        stream.sendPacket(packet);
        response = stream.getDataBuffer().getPacket("read command", true);
        if (response == null)
            throw new IOException("No packet");
        log.info("read command " + IoStream.printHexBinary(response));
        return response;
    }

    private void downloadFile(String fileName) {
        String lastFour = ConnectPanel.getLastFour(fileName);

        byte[] packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_FETCH_COMPRESSED;
        applyLastFour(lastFour, packet);

        IoStream stream = connectPanel.getControllerConnector().getConnector().getBinaryProtocol().getStream();

        FileOutputStream fos = null;
        try {
            stream.sendPacket(packet);
            byte[] response = stream.getDataBuffer().getPacket("Download file");
            log.info("Download file " + IoStream.printHexBinary(response));
            setStatus("Downloading " + fileName);

            fos = new FileOutputStream(getDestinationFolder(controllerAccessSupplier) + File.separator + fileName, false);
            int chunk = 0;
            int totalSize = 0;
            long start = System.currentTimeMillis();
            while (true) {
                packet = new byte[17];
                packet[0] = Fields.TS_SD_R_COMMAND;
                packet[2] = Fields.TS_SD_PROTOCOL_FETCH_DATA;
                packet[3] = (byte) chunk;
                packet[4] = (byte) (chunk >> 8);

                stream.sendPacket(packet);
                response = stream.getDataBuffer().getPacket("Get file", true);

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

    private void deleteFile(String fileName) {
        String lastFour = ConnectPanel.getLastFour(fileName);

        byte[] packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_REMOVE_FILE;
        applyLastFour(lastFour, packet);

        IoStream stream = connectPanel.getControllerConnector().getConnector().getBinaryProtocol().getStream();

        try {
            stream.sendPacket(packet);
            byte[] response = stream.getDataBuffer().getPacket("delete file");
            log.info("Delete file " + IoStream.printHexBinary(response));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private void applyLastFour(String lastFour, byte[] packet) {
        for (int i = 0; i < 4; i++)
            packet[7 + i] = (byte) lastFour.charAt(i);
    }

    private void setStatus(String message) {
        SwingUtilities.invokeLater(() -> status.setText(message));
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
}
