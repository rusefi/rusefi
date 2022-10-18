package com.rusefi.maintenance;

import com.devexperts.util.TimeUtil;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.TimeUnit;

/**
 * @see SimulatorExecHelper
 */
public class ExecHelper {
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
    private static void startStreamThread(final Process p, final InputStream stream, final StringBuffer buffer, final StatusConsumer wnd) {
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
                    wnd.append(line);
                    buffer.append(line);
                    wasRunningTime = System.currentTimeMillis();
                }
            } catch (IOException e) {
                wnd.append("Stream " + e);
            }
        });
        t.setDaemon(true);
        t.start();
    }

    @NotNull
    public static String executeCommand(String workingDirPath, String command, String binaryRelativeName, StatusConsumer wnd) {
        return executeCommand(workingDirPath, command, binaryRelativeName, wnd, new StringBuffer());
    }

        /**
         * @param output out parameter with stdout content
         * @return stderr of invoked command
         */
    @NotNull
    public static String executeCommand(String workingDirPath, String command, String binaryRelativeName, StatusConsumer wnd, StringBuffer output) {
        StringBuffer error = new StringBuffer();
        String binaryFullName = workingDirPath + File.separator + binaryRelativeName;
        if (!new File(binaryFullName).exists()) {
            wnd.append(binaryFullName + " not found :(");
            return error.toString();
        }

        File workingDir = new File(workingDirPath);
        return executeCommand(command, wnd, output, error, workingDir);
    }

    @NotNull
    public static String executeCommand(String command, StatusConsumer wnd, StringBuffer output, StringBuffer error, File workingDir) {
        wnd.append("Executing " + command);
        try {
            Process p = Runtime.getRuntime().exec(command, null, workingDir);
            startStreamThread(p, p.getInputStream(), output, wnd);
            startStreamThread(p, p.getErrorStream(), error, wnd);
            p.waitFor(3, TimeUnit.MINUTES);
        } catch (IOException e) {
            wnd.append("IOError: " + e);
        } catch (InterruptedException e) {
            wnd.append("WaitError: " + e);
        }
        wnd.append("Done!");
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
