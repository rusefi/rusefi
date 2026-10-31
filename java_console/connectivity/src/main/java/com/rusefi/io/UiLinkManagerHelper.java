package com.rusefi.io;

import com.rusefi.io.serial.BufferedSerialIoStream;
import com.rusefi.io.tcp.TcpConnector;
import com.rusefi.io.tcp.TcpIoStream;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

public class UiLinkManagerHelper {
    @NotNull
    public static IoStream open(String port) throws IOException {
        if (TcpConnector.isTcpPort(port))
            return TcpIoStream.open(port);
        return BufferedSerialIoStream.openPort(port);
    }
}
