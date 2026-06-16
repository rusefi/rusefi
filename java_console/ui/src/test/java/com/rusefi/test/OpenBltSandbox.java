package com.rusefi.test;

import com.rusefi.libopenblt.XcpLoader;
import com.rusefi.libopenblt.XcpSettings;
import com.rusefi.libopenblt.transport.IXcpTransport;
import com.rusefi.libopenblt.transport.XcpNet;

import java.io.IOException;

/**
 * Manual harness that clears flash memory over OpenBLT XCP-on-Ethernet.
 */
public class OpenBltSandbox {
    public static void main(String[] args) throws IOException {
        IXcpTransport transport = new XcpNet("192.168.10.1", 29001);
        try {
            XcpLoader loader = new XcpLoader(transport, new XcpSettings());

            loader.start();

            int start = 0x08000000 + 128 * 1024;
            int size = 768 * 1024;
            int chunk = 1024;

            for (int i = 0; i < size; i += chunk) {
                loader.clearMemory(start + i, chunk);
            }
        } catch (Exception e) {
            System.out.print(e);
        } finally {
            transport.disconnect();
        }
    }
}
