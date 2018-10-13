package com.rusefi;

import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.function.Consumer;

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
        FileLog.MAIN.logLine("runSimulator...");

        try {
            FileLog.MAIN.logLine("Binary size: " + new File(SIMULATOR_COMMAND).length());

            FileLog.MAIN.logLine("Executing " + SIMULATOR_COMMAND);
            ExecHelper.simulatorProcess = Runtime.getRuntime().exec(SIMULATOR_COMMAND);
            FileLog.MAIN.logLine("simulatorProcess: " + ExecHelper.simulatorProcess);

            dumpProcessOutput(ExecHelper.simulatorProcess);

            FileLog.MAIN.logLine("exitValue: " + simulatorProcess.exitValue());

            System.out.println("end of console");
        } catch (Exception err) {
            throw new IllegalStateException(err);
        }
    }

    public static void dumpProcessOutput(Process process) throws IOException {
        BufferedReader input =
                new BufferedReader(new InputStreamReader(process.getInputStream()));
        Thread thread = new Thread(createErrorStreamEcho(process));
        thread.setDaemon(true);
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
        FileLog.MAIN.logLine("startSimulator...");
        new Thread(new Runnable() {
            @Override
            public void run() {
                runSimulator();
            }
        }, "simulator process").start();
    }
}
