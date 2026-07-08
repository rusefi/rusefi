package com.rusefi;

import com.rusefi.io.LinkManager;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Manual sandbox for testing {@link SerialPortScanner#inspectPort} against real hardware.
 * Run {@code main()} to continuously poll all serial ports and print their detected type
 * (OpenBLT bootloader, ECU, EcuWithOpenblt, or Unknown).
 *
 * All ports are probed in parallel (via {@link SerialPortScanner#inspectPorts}) so that
 * a slow or unresponsive port does not block detection of other ports.
 *
 * @see com.rusefi.autodetect.PortDetectorSandbox for ECU-only detection
 */
public class InspectPortSandbox {
    public static void main(String[] args) throws InterruptedException {
        while (true) {
            Set<String> ports = LinkManager.getCommPorts();
            System.out.println("Available ports: " + ports);
            List<PortResult> results = SerialPortScanner.inspectPorts(new ArrayList<>(ports), null);
            for (PortResult result : results) {
                System.out.println("  " + result.port + " => " + result.type);
            }
            Thread.sleep(2000);
        }
    }
}
