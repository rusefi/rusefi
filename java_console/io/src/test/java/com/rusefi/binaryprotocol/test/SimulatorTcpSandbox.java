package com.rusefi.binaryprotocol.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.IOException;
import java.net.Socket;

import static com.rusefi.io.tcp.TcpConnector.DEFAULT_PORT;
import static com.rusefi.io.tcp.TcpConnector.LOCALHOST;

public class SimulatorTcpSandbox {
    public static void main(String[] args) throws IOException, InterruptedException {
        BinaryProtocol.DISABLE_LOCAL_CACHE = true;

        Socket s = new Socket(LOCALHOST, DEFAULT_PORT);
        TcpIoStream tsStream = new TcpIoStream("sandbox", s);

        LinkManager linkManager = new LinkManager();
        SandboxCommon.verifyCrcNoPending(tsStream, linkManager);

        ConfigurationImage ci = SandboxCommon.readImage(tsStream, linkManager);

        System.exit(0);
    }

}
