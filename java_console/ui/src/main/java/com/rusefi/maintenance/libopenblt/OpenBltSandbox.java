package com.rusefi.maintenance.libopenblt;

import com.rusefi.maintenance.libopenblt.transport.IXcpTransport;
import com.rusefi.maintenance.libopenblt.transport.XcpNet;

public class OpenBltSandbox {
    public static void main(String[] args) {
        XcpSettings settings = new XcpSettings();
        settings.timeoutT1 = 1000;
        settings.timeoutT3 = 2000;
        settings.timeoutT4 = 10000;
        settings.timeoutT5 = 1000;
        settings.timeoutT6 = 50;
        settings.timeoutT7 = 2000;

        try (IXcpTransport transport = new XcpNet("192.168.10.1", 29000)) {
            XcpLoader loader = new XcpLoader(transport, settings);

            loader.start();

            int start = 0x08000000 + 128 * 1024;
            int size = 768 * 1024;
            int chunk = 1024;

            for (int i = 0; i < size; i += chunk) {
                loader.clearMemory(start + i, chunk);
            }
        } catch (Exception e) {
            System.out.print(e);
        }
    }
}
