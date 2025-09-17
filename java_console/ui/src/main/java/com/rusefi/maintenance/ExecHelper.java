package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.devexperts.util.TimeUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

/**
 * @see SimulatorExecHelper
 */
public class ExecHelper {
    private static final Logging log = getLogging(ExecHelper.class);

    private static boolean isRunning(Process p) {
        try {
            p.exitValue();
            return false;
        } catch (IllegalThreadStateException e) {
            return true;
        }
    }

    /**
     * This method listens to a data stream from the process, appends messages to UI
     * and accumulates output in a buffer
     */
    private static void startStreamThread(final Process p, final InputStream stream, final StringBuffer buffer, final UpdateOperationCallbacks callbacks) {
        final Thread t = new Thread(() -> {
            try {
                BufferedReader bis = new BufferedReader(new InputStreamReader(stream, StandardCharsets.ISO_8859_1));
                /*
                 * Sometimes process has already finished but we still want to read output, so give it extra half a second
                 * TODO: are we supposed to just NOT check process status and just wait for 'null' from readLine?
                 */
                long wasRunningTime = System.currentTimeMillis();
                while (isRunning(p) || (System.currentTimeMillis() - wasRunningTime) < 0.5 * TimeUtil.SECOND) {
                    String line = bis.readLine();
                    if (line == null)
                        break;
                    callbacks.logLine(line);
                    buffer.append(line);
                    wasRunningTime = System.currentTimeMillis();
                }
            } catch (IOException e) {
                callbacks.logLine("Stream " + e);
                callbacks.error();
            }
        });
        t.setDaemon(true);
        t.start();
    }

    @NotNull
    public static String executeCommand(String workingDirPath, String command, String binaryRelativeName, UpdateOperationCallbacks callbacks) throws FileNotFoundException {
        return executeCommand(workingDirPath, command, binaryRelativeName, callbacks, new StringBuffer());
    }

        /**
         * @param output out parameter with stdout content
         * @return stderr of invoked command
         */
    @NotNull
    public static String executeCommand(String workingDirPath, String command, String binaryRelativeName, UpdateOperationCallbacks callbacks, StringBuffer output) throws FileNotFoundException {
        StringBuffer error = new StringBuffer();
        String binaryFullName = getBinaryFullFileName(workingDirPath, binaryRelativeName);
        if (!new File(binaryFullName).exists()) {
            callbacks.logLine(binaryFullName + " not found :(");
            throw new FileNotFoundException(binaryFullName);
        }

        File workingDir = new File(workingDirPath);
        try {
            return executeCommand(command, callbacks, output, error, workingDir);
        } catch (ErrorExecutingCommand e) {
            callbacks.logLine("ErrorExecutingCommand: " + e);
            callbacks.error();
            throw new RuntimeException(e);
        }
    }

    private static @NotNull String getBinaryFullFileName(String workingDirPath, String binaryRelativeName) {
        return workingDirPath + File.separator + binaryRelativeName;
    }

    @NotNull
    public static String executeCommand(String command, UpdateOperationCallbacks callbacks, StringBuffer output, StringBuffer error, File workingDir) throws ErrorExecutingCommand {
        callbacks.logLine("Executing command=" + command);
        try {
            Process p = Runtime.getRuntime().exec(command, null, workingDir);
            startStreamThread(p, p.getInputStream(), output, callbacks);
            startStreamThread(p, p.getErrorStream(), error, callbacks);
            p.waitFor(3, TimeUnit.MINUTES);
        } catch (IOException e) {
            log.info("executeCommand " + e);
            throw new ErrorExecutingCommand(e);
        } catch (InterruptedException e) {
            callbacks.logLine("WaitError: " + e);
            callbacks.error();
        }

        return error.toString();
    }

    public static void submitAction(Runnable runnable, String threadName) {
        Thread thread = new Thread(runnable, threadName);
        thread.setDaemon(true);
        thread.start();
    }

    @NotNull
    public static String getBatchCommand(String batch) {
        return "cmd /c start " + batch;
    }
}
