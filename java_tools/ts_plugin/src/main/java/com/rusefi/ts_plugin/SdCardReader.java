package com.rusefi.ts_plugin;

import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static com.rusefi.config.generated.Fields.TS_SD_PROTOCOL_FETCH_INFO;
import static com.rusefi.shared.FileUtil.close;

public class SdCardReader {
    private final static int TRANSFER_HEADER_SIZE = 3;
    private final JPanel content = new JPanel(new BorderLayout());

    private final JPanel fileList = new JPanel(new VerticalFlowLayout());
    private final JLabel status = new JLabel();

    private final ConnectPanel connectPanel = new ConnectPanel(new ConnectionStateListener() {
        public void onConnectionEstablished() {
            requestFileList();
        }

        public void onConnectionFailed() {
        }
    });

    public SdCardReader() {
        JButton refresh = new JButton("Refresh");
        refresh.addActionListener(e -> requestFileList());

        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.add(connectPanel.getContent(), BorderLayout.NORTH);
        topPanel.add(status, BorderLayout.CENTER);

        content.add(topPanel, BorderLayout.NORTH);
        content.add(fileList, BorderLayout.CENTER);
        content.add(refresh, BorderLayout.SOUTH);
    }

    public Component getContent() {
        return content;
    }

    private void requestFileList() {
        int fileCount = 0;

        try {
            byte[] packet;
            byte[] response;
            IoStream stream = connectPanel.getControllerConnector().getConnector().getBinaryProtocol().getStream();

            packet = new byte[3];
            packet[0] = Fields.TS_SD_R_COMMAND;
            packet[2] = Fields.TS_SD_PROTOCOL_RTC;
            stream.sendPacket(packet);
            response = stream.getDataBuffer().getPacket("RTC status");
            System.out.println("RTC response " + IoStream.printHexBinary(response));
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
            System.out.println("read dir command " + IoStream.printHexBinary(response));

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
            System.out.println("read command " + IoStream.printHexBinary(response));

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
                int size = bb.getInt();

                JPanel filePanel = new JPanel(new FlowLayout());

                filePanel.add(new JLabel(fileName + " " + size + " byte(s)"));

                JButton download = new JButton("Download");
                download.addActionListener(e -> downloadFile(fileName));

                filePanel.add(download);
                JButton delete = new JButton("Delete");
                delete.addActionListener(e1 -> {
                    int result = JOptionPane.showConfirmDialog(null, "Are you sure you want to remove " + fileName,
                            "rusEfi", JOptionPane.YES_NO_OPTION);
                    if (result == JOptionPane.YES_OPTION) {
                        deleteFile(fileName);
                        status.setText("Deleted " + fileName);
                        requestFileList();
                    }
                });
                filePanel.add(delete);

                fileList.add(filePanel);

                System.out.println("Filename " + fileName + " size " + size);

                AutoupdateUtil.trueLayout(content.getParent());
            }

        } catch (IOException ioException) {
            ioException.printStackTrace();
        }

        if (fileCount == 0) {
            status.setText("No files found.");
        }
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
            System.out.println("Download file " + IoStream.printHexBinary(response));

            status.setText("Downloading " + fileName);


            fos = new FileOutputStream("downloaded_" + fileName, false);

            int chunk = 0;

            int totalSize = 0;

            while (true) {
                packet = new byte[17];
                packet[0] = Fields.TS_SD_R_COMMAND;
                packet[2] = Fields.TS_SD_PROTOCOL_FETCH_DATA;
                packet[3] = (byte) chunk;
                packet[4] = (byte) (chunk >> 8);

                stream.sendPacket(packet);
                response = stream.getDataBuffer().getPacket("Get file", true);

                if (response == null) {
                    System.out.println("No content response");
                    break;
                }

                System.out.println("Got content package size "  + response.length);


                int dataBytes = response.length - TRANSFER_HEADER_SIZE;
                fos.write(response, TRANSFER_HEADER_SIZE, dataBytes);

                totalSize += dataBytes;

                if (dataBytes != 2048) {
                    System.out.println(response.length + " must be the last packet");
                    status.setText(fileName + " downloaded " + totalSize + " byte(s)");
                    break;
                }
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
            System.out.println("Delete file " + IoStream.printHexBinary(response));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    private void applyLastFour(String lastFour, byte[] packet) {
        for (int i = 0; i < 4; i++)
            packet[7 + i] = (byte) lastFour.charAt(i);
    }
}
