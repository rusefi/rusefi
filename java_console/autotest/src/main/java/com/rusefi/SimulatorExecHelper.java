package com.rusefi;

import com.devexperts.logging.Logging;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

import static com.rusefi.SimulatorFunctionalTestLauncher.isHappy;

/**
 * 3/18/14
 * @see SimulatorHelper
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class SimulatorExecHelper {
    private final static Logging log = Logging.getLogging(SimulatorExecHelper.class);

    private final static NamedThreadFactory THREAD_FACTORY = new NamedThreadFactory("SimulatorExecHelper", true);

    private static final String SIMULATOR_BUILD_RUSEFI_SIMULATOR = "simulator/build/rusefi_simulator";
    // see also SimulatorHelper
    private static final String SIMULATOR_BINARY = getSimulatorBinary();
    private static Process simulatorProcess;

    private static String getSimulatorBinary() {
        return FileLog.isWindows() ? SIMULATOR_BUILD_RUSEFI_SIMULATOR + ".exe" : SIMULATOR_BUILD_RUSEFI_SIMULATOR;
    }

    /**
     * This is currently used by auto-tests only. Todo: reuse same code for UI-launched simulator?
     */
    private static void runSimulator(CountDownLatch simulatorStarted) {
        Thread.currentThread().setName("Main simulation");
        AutotestLogging.INSTANCE.logLine("runSimulator...");

        try {
            AutotestLogging.INSTANCE.logLine("Binary size: " + new File(SIMULATOR_BINARY).length());

            AutotestLogging.INSTANCE.logLine("Executing " + SIMULATOR_BINARY);
            simulatorProcess = Runtime.getRuntime().exec(SIMULATOR_BINARY);
            AutotestLogging.INSTANCE.logLine("simulatorProcess: " + simulatorProcess);

            dumpProcessOutput(simulatorProcess, simulatorStarted);
        } catch (Exception err) {
            if (isHappy) {
                System.out.println("Already happy " + err);
            } else {
                throw new IllegalStateException(err);
            }
        }

        try {
            AutotestLogging.INSTANCE.logLine("exitValue: " + simulatorProcess.exitValue());
        } catch (Exception err) {
            log.warn("Error reading exit value", err);
        }

        System.out.println("end of console");
    }

    public static void dumpProcessOutput(Process process, CountDownLatch countDownLatch) throws IOException {
        BufferedReader input =
                new BufferedReader(new InputStreamReader(process.getInputStream()));
        Thread thread = THREAD_FACTORY.newThread(createErrorStreamEcho(process));
        thread.start();

        AtomicInteger counter = new AtomicInteger();
        String prefix = "from console: ";
        Consumer<String> PRINT_AND_LOG = string -> {
            if (countDownLatch != null)
                countDownLatch.countDown();
// looks like this is a performance issue since so many lines are printed? looks like it's helping to not write this?
            if (counter.incrementAndGet() < 1000)
                System.out.println(prefix + string);
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
                String prefix = "ERROR from console: ";
                Consumer<String> PRINT_AND_LOG = string -> {
                    System.out.println(prefix + string);
                    log.info(string);
                };

                readAndPrint(PRINT_AND_LOG, err);
            } catch (IOException e) {
                throw new IllegalStateException(e);
            }
        };
    }

    static void destroy() {
        if (simulatorProcess != null) {
            log.info("Destroying sub-process...");
            simulatorProcess.destroy();
        }
    }

    public static void startSimulator() throws InterruptedException {
        if (!new File(SIMULATOR_BINARY).exists())
            throw new IllegalStateException(SIMULATOR_BINARY + " not found");
        AutotestLogging.INSTANCE.logLine("startSimulator...");
        CountDownLatch simulatorStarted = new CountDownLatch(1);
        new Thread(() -> runSimulator(simulatorStarted), "simulator process").start();
        simulatorStarted.await(1, TimeUnit.MINUTES);
        log.info("Let's give it some time to start...");
        Thread.sleep(5 * Timeouts.SECOND);
    }
}
