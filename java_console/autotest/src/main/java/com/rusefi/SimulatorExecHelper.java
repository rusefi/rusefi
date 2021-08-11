package com.rusefi;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.function.Consumer;

/**
 * 3/18/14
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class SimulatorExecHelper {
    private final static NamedThreadFactory THREAD_FACTORY = new NamedThreadFactory("SimulatorExecHelper", true);

    // see also SimulatorHelper
    private static final String SIMULATOR_BINARY = "../simulator/build/rusefi_simulator.exe";
    static Process simulatorProcess;

    /**
     * This is currently used by auto-tests only. Todo: reuse same code for UI-launched simulator?
     */
    private static void runSimulator() {
        Thread.currentThread().setName("Main simulation");
        FileLog.MAIN.logLine("runSimulator...");

        try {
            FileLog.MAIN.logLine("Binary size: " + new File(SIMULATOR_BINARY).length());

            FileLog.MAIN.logLine("Executing " + SIMULATOR_BINARY);
            SimulatorExecHelper.simulatorProcess = Runtime.getRuntime().exec(SIMULATOR_BINARY);
            FileLog.MAIN.logLine("simulatorProcess: " + SimulatorExecHelper.simulatorProcess);

            dumpProcessOutput(SimulatorExecHelper.simulatorProcess);

            FileLog.MAIN.logLine("exitValue: " + simulatorProcess.exitValue());

            System.out.println("end of console");
        } catch (Exception err) {
            throw new IllegalStateException(err);
        }
    }

    public static void dumpProcessOutput(Process process) throws IOException {
        BufferedReader input =
                new BufferedReader(new InputStreamReader(process.getInputStream()));
        Thread thread = THREAD_FACTORY.newThread(createErrorStreamEcho(process));
        thread.start();

        String prefix = "from console: ";
        Consumer<String> PRINT_AND_LOG = string -> {
// looks like this is a performance issue since so many lines are printed? looks like it's helping to not write this?

//            System.out.println(prefix + string);
//            FileLog.SIMULATOR_CONSOLE.logLine(string);
        };


        readAndPrint(PRINT_AND_LOG, input);
        input.close();
    }

    private static void readAndPrint(Consumer<String> consumer, BufferedReader input) throws IOException {
        String line;
        while ((line = input.readLine()) != null) {
            consumer.accept(line);
        }
    }

    private static Runnable createErrorStreamEcho(final Process process) {
        return () -> {
            BufferedReader err =
                    new BufferedReader(new InputStreamReader(process.getErrorStream()));
            try {
                String prefix = "from console: ";
                Consumer<String> PRINT_AND_LOG = string -> {
                    System.out.println(prefix + string);
                    FileLog.SIMULATOR_CONSOLE.logLine(string);
                };

                readAndPrint(PRINT_AND_LOG, err);
            } catch (IOException e) {
                throw new IllegalStateException(e);
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
        if (!new File(SIMULATOR_BINARY).exists())
            throw new IllegalStateException(SIMULATOR_BINARY + " not found");
        FileLog.MAIN.logLine("startSimulator...");
        new Thread(new Runnable() {
            @Override
            public void run() {
                runSimulator();
            }
        }, "simulator process").start();
    }
}
