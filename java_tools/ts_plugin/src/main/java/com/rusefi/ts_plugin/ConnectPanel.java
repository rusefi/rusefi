package com.rusefi.ts_plugin;

import com.rusefi.autoupdate.AutoupdateUtil;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import static com.rusefi.config.generated.Fields.TS_SD_PROTOCOL_FETCH_INFO;

public class ConnectPanel {
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel fileList = new JPanel(new VerticalFlowLayout());
    private final JLabel status = new JLabel();

    private LinkManager controllerConnector;

    public ConnectPanel() {
        JPanel flow = new JPanel(new FlowLayout());


        JButton connect = new JButton("Connect");
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                connect.setEnabled(false);

                controllerConnector = new LinkManager()
                        .setCompositeLogicEnabled(false)
                        .setNeedPullData(false);

                //controllerConnector.startAndConnect(":2390", ConnectionStateListener.VOID);
                controllerConnector.startAndConnect(":29001", new ConnectionStateListener() {
                    public void onConnectionEstablished() {
                        status.setText("Connected to rusEFI");
                        requestFileList();
                    }

                    public void onConnectionFailed() {
                    }
                });

            }
        });

        JButton poke = new JButton("poke");
        poke.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                requestFileList();
            }
        });

        flow.add(connect);
        flow.add(poke);

        content.add(flow, BorderLayout.NORTH);
        content.add(fileList, BorderLayout.CENTER);
        content.add(status, BorderLayout.SOUTH);
    }

    private void requestFileList() {
        try {
            byte[] packet;
            byte[] response;
            IoStream stream = controllerConnector.getConnector().getBinaryProtocol().getStream();

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
                String fileExt = new String(response, 1 + offset + 8, 3).trim();
                String fileName = fileNamePart + "." + fileExt;

                ByteBuffer bb = ByteBuffer.wrap(response, 1 + offset + 28, 4);
                bb.order(ByteOrder.LITTLE_ENDIAN);
                int size = bb.getInt();

                JPanel filePanel = new JPanel(new FlowLayout());

                filePanel.add(new JLabel(fileName + " " + size + " byte(s)"));

                JButton download = new JButton("Download");
                download.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        downloadFile(fileName);


                    }
                });

                filePanel.add(download);
                JButton delete = new JButton("Delete");
                delete.addActionListener(e1 -> {
                    int result = JOptionPane.showConfirmDialog(null, "Are you sure you want to remove " + fileName,
                            "rusEfi", JOptionPane.YES_NO_OPTION);
                    if (result == JOptionPane.YES_OPTION) {
                        deleteFile(fileName);
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
    }

    public static String getLastFour(String fileName) {
        int dotIndex = fileName.indexOf(".");
        fileName = fileName.substring(0, dotIndex);
        if (fileName.length() < 5)
            return fileName;
        return fileName.substring(fileName.length() - 4);
    }

    private void downloadFile(String fileName) {
        fileName = ConnectPanel.getLastFour(fileName);

    }

    private void deleteFile(String fileName) {
        fileName = ConnectPanel.getLastFour(fileName);

        byte[] packet = new byte[17];
        packet[0] = Fields.TS_SD_W_COMMAND;
        packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
        packet[6] = Fields.TS_SD_PROTOCOL_REMOVE_FILE;
        for (int i = 0; i < 4; i++)
            packet[7 + i] = (byte) fileName.charAt(i);

        IoStream stream = controllerConnector.getConnector().getBinaryProtocol().getStream();

        try {
            stream.sendPacket(packet);
            byte[] response = stream.getDataBuffer().getPacket("delete file");
            System.out.println("delete file " + IoStream.printHexBinary(response));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public JComponent getContent() {
        return content;
    }
}
