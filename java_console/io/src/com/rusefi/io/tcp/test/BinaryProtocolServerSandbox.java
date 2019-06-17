package com.rusefi.io.tcp.test;

import com.opensr5.ConfigurationImage;
import com.rusefi.FileLog;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolHolder;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.tcp.BinaryProtocolServer;
import com.rusefi.io.tcp.TcpIoStream;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

class BinaryProtocolServerSandbox {
    public static void main(String[] args) {
        TcpIoStream stream = new TcpIoStream(new ByteArrayInputStream(new byte[0]), new ByteArrayOutputStream());
        BinaryProtocol bp = BinaryProtocolHolder.create(FileLog.LOGGER, stream);
        bp.setController(new ConfigurationImage(new byte[14008]));
        bp.currentOutputs = new byte[1 + Fields.TS_OUTPUT_SIZE];
        BinaryProtocolServer.start();
    }
}