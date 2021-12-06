package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkManager;
import com.rusefi.io.serial.StreamConnector;
import com.rusefi.io.tcp.TcpIoStream;
import jdk.nashorn.internal.runtime.regexp.joni.constants.Arguments;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.io.tcp.TcpConnector.DEFAULT_PORT;
import static com.rusefi.io.tcp.TcpConnector.LOCALHOST;

public class SimulatorTcpSandbox {
    public static void main(String[] args) throws IOException {
        BinaryProtocol.DISABLE_LOCAL_CACHE = true;

        Socket s = new Socket(LOCALHOST, DEFAULT_PORT);
        TcpIoStream tsStream = new TcpIoStream("sandbox", s);

        LinkManager linkManager = new LinkManager();
        verifyCrcNoPending(tsStream, linkManager);


        StreamConnector streamConnector = new StreamConnector(linkManager, () -> tsStream);
        linkManager.setConnector(streamConnector);
        streamConnector.connectAndReadConfiguration(new BinaryProtocol.Arguments(   false), new ConnectionStateListener() {
            @Override
            public void onConnectionEstablished() {
                System.out.println("onConnectionEstablished");

                BinaryProtocol currentStreamState = linkManager.getCurrentStreamState();
                if (currentStreamState == null) {
                    System.out.println("No BinaryProtocol");
                } else {
                    BinaryProtocolState binaryProtocolState = currentStreamState.getBinaryProtocolState();
                    ConfigurationImage ci = binaryProtocolState.getControllerConfiguration();
                    System.out.println("Got ConfigurationImage " + ci);
                    System.exit(0);
                }
            }

            @Override
            public void onConnectionFailed() {
                System.out.println("onConnectionFailed");
            }
        });

    }

    private static void verifyCrcNoPending(IoStream tsStream, LinkManager linkManager) {
        BinaryProtocol bp = new BinaryProtocol(linkManager, tsStream);
        linkManager.COMMUNICATION_EXECUTOR.submit(() -> {
            if (tsStream.getDataBuffer().dropPending() != 0)
                System.out.println("ERROR Extra data before CRC");
            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
//            bp.getCrcFromController(Fields.TOTAL_CONFIG_SIZE);
            if (tsStream.getDataBuffer().dropPending() != 0)
                System.out.println("ERROR Extra data after CRC");
        });
    }
}
