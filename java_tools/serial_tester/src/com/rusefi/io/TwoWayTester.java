package com.rusefi.io;

import com.rusefi.io.serial.SerialPortReader;
import jssc.SerialPort;
import jssc.SerialPortException;

public class TwoWayTester {

//    private static final int BAUD_RATE = 38400;
    private static final int BAUD_RATE = 115200;
    public static final String TEST_LINE = "a0123456789b0123456789c0123456789d0123456789e0123456789f0123456789g0123456789h0123456789d0123456789";
//    public static final String TEST_LINE = "a0123456789b0123456789";
    public static final DataListener VOID = new DataListener() {
        @Override
        public void onStringArrived(String string) {
            // nothing is expected to arrive
        }
    };

    public static void main(String[] args) throws SerialPortException, InterruptedException {
        if(args.length<1) {
            System.out.println("Parameter expected");
        }


        String port1 = args[0];
        String port2 = "COM25";

        DataListener listener = new DataListener() {

            int counter;
            StringBuffer sb = new StringBuffer();

            @Override
            public void onStringArrived(String string) {
                sb.append(string);
                if (sb.length() < TEST_LINE.length()) {
                    logLine("Partial");
                    return;
                }


                String l = sb.substring(0, TEST_LINE.length());

                if (!l.equalsIgnoreCase(TEST_LINE)) {
                    logLine("WOW   " + l);
                    logLine("not   " + TEST_LINE);

                    int index = sb.indexOf(TEST_LINE.charAt(0) + "", 1);
                    if (index != -1)
                        sb.delete(0, index);

                } else {
                    logLine("Ok " + counter++);
                    sb.delete(0, TEST_LINE.length());
                }
            }
        };
        //SerialPort in = open(port2, listener);


        SerialPort out = open(port1, listener);


        while (true) {
            out.writeString(TEST_LINE);
            //Thread.sleep(50);
        }
    }

    public static SerialPort open(String port, DataListener listener) throws SerialPortException {
        SerialPort serialPort = new SerialPort(port);
        logLine("Opening " + port + " @ " + BAUD_RATE);
        boolean opened = serialPort.openPort();//Open serial port
        if (!opened)
            logLine("not opened");
        serialPort.setParams(BAUD_RATE, 8, 1, 0);//Set params.
        int mask = SerialPort.MASK_RXCHAR;
        //Set the prepared mask
        serialPort.setEventsMask(mask);
        serialPort.addEventListener(new SerialPortReader(serialPort, listener));
        return serialPort;
    }

    private static void logLine(String s) {
        System.out.println(s);
    }
}
