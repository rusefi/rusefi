package com.rusefi.io.tcp.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.ConnectionStateListener;
import com.rusefi.io.IoStream;
import com.rusefi.io.LinkConnector;
import com.rusefi.io.LinkManager;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

class BinaryProtocolServerSandbox {
    public static void main(String[] args) {
        LinkManager linkManager = new LinkManager();
        TcpIoStream stream = new TcpIoStream(linkManager, new ByteArrayInputStream(new byte[0]), new ByteArrayOutputStream());
        BinaryProtocol bp = new BinaryProtocol(linkManager, FileLog.LOGGER, stream);
        linkManager.setConnector(new LinkConnector() {
            @Override
            public void connectAndReadConfiguration(ConnectionStateListener listener) {
                throw new UnsupportedOperationException();
            }

            @Override
            public void send(String command, boolean fireEvent) throws InterruptedException {
                throw new UnsupportedOperationException();
            }

            @Override
            public void restart() {
                throw new UnsupportedOperationException();
            }

            @Override
            public BinaryProtocol getBinaryProtocol() {
                return bp;
            }
        });
        bp.setController(new ConfigurationImage(new byte[Fields.TOTAL_CONFIG_SIZE]));
        bp.currentOutputs = new byte[1 + Fields.TS_OUTPUT_SIZE];
        BinaryProtocolServer.start(linkManager);
    }
}