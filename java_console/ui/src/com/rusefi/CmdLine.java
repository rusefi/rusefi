package com.rusefi;

import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
import jssc.SerialPortList;

/**
 * (c) Andrey Belomutskiy 2013-2015
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
            String[] ports = SerialPortList.getPortNames();
            if (ports.length == 0) {
                System.out.println("Port not specified and no ports found");
                return;
            }
            String port = ports[ports.length - 1];
            System.out.println("Using last of " + ports.length + " port(s)");
            executeCommand(command, port);
        } else {
            executeCommand(command, args[1]);
        }
    }

    private static void executeCommand(String command, String port) {
        System.out.println("Sending " + command);
        System.out.println("Sending to " + port);
        LinkManager.start(port);
        LinkManager.open();
        LinkManager.engineState.registerStringValueAction(EngineState.RUS_EFI_VERSION_TAG, (EngineState.ValueCallback<String>) EngineState.ValueCallback.VOID);

        IoUtil.sendCommand(command);
        System.out.println("Done!");
        System.exit(-1);
    }
}
