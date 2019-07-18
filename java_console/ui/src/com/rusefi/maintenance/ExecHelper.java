package com.rusefi.maintenance;

import com.rusefi.SimulatorExecHelper;
import com.rusefi.ui.StatusConsumer;
import org.jetbrains.annotations.NotNull;

import java.io.*;

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
                BufferedReader bis = new BufferedReader(new InputStreamReader(stream));
                while (isRunning(p)) {
                    String line = bis.readLine();
                    if (line == null)
                        break;
                    wnd.appendMsg(line);
                    buffer.append(line);
                }
            } catch (IOException e) {
                wnd.appendMsg("Stream " + e);
            }
        });
        t.setDaemon(true);
        t.start();
    }

    @NotNull
    public static StringBuffer executeCommand(String workingDirPath, String command, String binaryRelativeName, StatusConsumer wnd) {
        StringBuffer error = new StringBuffer();
        String binaryFullName = workingDirPath + File.separator + binaryRelativeName;
        if (!new File(binaryFullName).exists()) {
            wnd.appendMsg(binaryFullName + " not found :(");
            return error;
        }

        wnd.appendMsg("Executing " + command);
        StringBuffer output = new StringBuffer();
        try {
            File workingDir = new File(workingDirPath);
            Process p = Runtime.getRuntime().exec(command, null, workingDir);
            startStreamThread(p, p.getInputStream(), output, wnd);
            startStreamThread(p, p.getErrorStream(), error, wnd);
            p.waitFor();
        } catch (IOException e) {
            wnd.appendMsg("IOError: " + e);
        } catch (InterruptedException e) {
            wnd.appendMsg("WaitError: " + e);
        }
        wnd.appendMsg("Done!");
        return error;
    }

    protected static void submitAction(Runnable runnable, String threadName) {
        Thread thread = new Thread(runnable, threadName);
        thread.setDaemon(true);
        thread.start();
    }

    @NotNull
    public static String getBatchCommand(String batch) {
        return "cmd /c start " + batch;
    }
}
