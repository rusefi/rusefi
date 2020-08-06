package com.rusefi.io.tcp;

import java.io.IOException;
import java.net.ServerSocket;

public interface ServerSocketFunction {
    ServerSocket apply(int port) throws IOException;
}
