package com.irnems;

import jssc.SerialPortList;

/**
 * Date: 12/25/12
 * (c) Andrey Belomutskiy
 */
public class Test {

    public static void main(String[] args) {
        String[] portNames = SerialPortList.getPortNames();
        System.out.println("Total " + portNames.length + " serial port(s)");
        for (String portName : portNames) {
            System.out.println(portName);
        }
        System.exit(0);
    }
}
