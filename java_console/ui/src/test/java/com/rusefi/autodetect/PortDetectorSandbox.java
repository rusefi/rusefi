package com.rusefi.autodetect;

/**
 * see BltSwitchSandbox
 * See OpenBltSandbox
 */
public class PortDetectorSandbox {
    public static void main(String[] args) throws InterruptedException {
        while (true) {
            SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(null);
            String port = autoDetectResult.getSerialPort();
            System.out.println("Port Detected " + port + ", " + autoDetectResult.getSignature());

            Thread.sleep(1000);
        }
    }
}
