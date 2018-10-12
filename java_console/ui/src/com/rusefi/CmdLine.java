package com.rusefi;

import com.rusefi.io.LinkManager;

/**
 * (c) Andrey Belomutskiy 2013-2018
 * 2/22/2015
 */
public class CmdLine {
    public static void main(String[] args) {
        if (args.length == 0 || args.length > 2) {
            System.out.println("CmdLine COMMAND [PORT]");
            return;
        }
        String command = args[0];
        if (args.length == 1) {
            String port = LinkManager.getDefaultPort();
            if (port == null)
                return;
            executeCommand(command, port);
        } else {
            executeCommand(command, args[1]);
        }
    }

    private static void executeCommand(String command, String port) {
        System.out.println("Sending " + command);
        System.out.println("Sending to " + port);
        IoUtil.realHardwareConnect(port);

        IoUtil.sendCommand(command);
        System.out.println("Done!");
        System.exit(-1);
    }

}
