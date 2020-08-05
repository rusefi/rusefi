package com.rusefi.ts_plugin;

import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;

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

    private LinkManager controllerConnector;

    public ConnectPanel() {
        JPanel flow = new JPanel(new FlowLayout());


        JButton connect = new JButton("Connect");
        connect.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

                controllerConnector = new LinkManager()
                        .setCompositeLogicEnabled(false)
                        .setNeedPullData(false);

                controllerConnector.startAndConnect(":2390", ConnectionStateListener.VOID);

            }
        });


        JButton poke = new JButton("poke");
        poke.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
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


                    packet = new byte[17];
                    packet[0] = Fields.TS_SD_W_COMMAND;
                    packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
                    packet[6] = Fields.TS_SD_PROTOCOL_READ_DIR;
                    stream.sendPacket(packet);
                    response = stream.getDataBuffer().getPacket("read dir command");
                    System.out.println("read dir command " + IoStream.printHexBinary(response));


                    packet = new byte[8];
                    packet[0] = Fields.TS_SD_R_COMMAND;
                    packet[1] = 0;
                    packet[2] = TS_SD_PROTOCOL_FETCH_INFO;
                    packet[5] = 0x02;
                    packet[6] = 0x02;
                    stream.sendPacket(packet);
                    response = stream.getDataBuffer().getPacket("read command", true);
                    System.out.println("read command " + IoStream.printHexBinary(response));

                    int fileIndex = 0;

                    int offset = 32 * fileIndex;
                    String fileNamePart = new String(response, 1 + offset, 8).trim();
                    String fileExt =  new String(response, 1 + offset + 8, 3).trim();
                    String fileName = fileNamePart + "." + fileExt;

                    ByteBuffer bb = ByteBuffer.wrap(response, 1 + offset + 28, 4);
                    bb.order(ByteOrder.LITTLE_ENDIAN);
                    int size = bb.getInt();

                    System.out.println("Filename " + fileName + " size " + size);




                } catch (IOException ioException) {
                    ioException.printStackTrace();
                }


            }
        });

        flow.add(connect);
        flow.add(poke);

        content.add(flow);
    }

    public JComponent getContent() {
        return content;
    }
}
