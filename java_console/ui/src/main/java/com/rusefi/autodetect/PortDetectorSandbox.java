package com.rusefi.autodetect;

public class PortDetectorSandbox {
    public static void main(String[] args) throws InterruptedException {
        while (true) {
            String port = PortDetector.autoDetectSerial(null).getSerialPort();
            System.out.println("Detected " + port);

            Thread.sleep(1000);
        }
    }
}
