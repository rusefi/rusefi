package com.rusefi.autodetect;

import com.rusefi.IoUtil;

public class PortDetectorSandbox {
    public static void main(String[] args) {
        while (true) {
            String port = PortDetector.autoDetectSerial(null).getSerialPort();
            System.out.println("Detected " + port);

            IoUtil.sleepSeconds(1);
        }
    }
}
