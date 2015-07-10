package com.rusefi;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * 3/18/14
 * (c) Andrey Belomutskiy
 */
public class ExecHelper {
    private static final String SIMULATOR_COMMAND = "../win32_functional_tests/build/rusefi_simulator.exe";
    static Process simulatorProcess;

    /**
     * This is currently used by auto-tests only. Todo: reuse same code for UI-launched simulator?
     */
    private static void runSimulator() {
        Thread.currentThread().setName("Main simulation");

        try {
            FileLog.MAIN.logLine("Binary size: " + new File(SIMULATOR_COMMAND).length());

            FileLog.MAIN.logLine("Executing " + SIMULATOR_COMMAND);
            ExecHelper.simulatorProcess = Runtime.getRuntime().exec(SIMULATOR_COMMAND);
            FileLog.MAIN.logLine("simulatorProcess: " + ExecHelper.simulatorProcess);

            BufferedReader input =
                    new BufferedReader(new InputStreamReader(ExecHelper.simulatorProcess.getInputStream()));
            Thread thread = new Thread(createErrorStreamEcho());
            thread.setDaemon(true);
            thread.start();

            String line;
            while ((line = input.readLine()) != null) {
                System.out.println("from console: " + line);
                FileLog.SIMULATOR_CONSOLE.logLine(line);
            }

            FileLog.MAIN.logLine("exitValue: " + simulatorProcess.exitValue());

            System.out.println("end of console");
            input.close();
        } catch (Exception err) {
            throw new IllegalStateException(err);
        }
    }

    private static Runnable createErrorStreamEcho() {
        return new Runnable() {
            @Override
            public void run() {
                BufferedReader err =
                        new BufferedReader(new InputStreamReader(ExecHelper.simulatorProcess.getErrorStream()));
                String errLine;
                try {
                    while ((errLine = err.readLine()) != null) {
                        System.out.println("from err: " + errLine);
                        FileLog.SIMULATOR_CONSOLE.logLine(errLine);
                    }
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        };
    }

    static void destroy() {
        if (simulatorProcess != null) {
            FileLog.MAIN.logLine("Destroying sub-process...");
            simulatorProcess.destroy();
        }
    }

    public static void startSimulator() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                runSimulator();
            }
        }, "simulator process").start();
    }
}
