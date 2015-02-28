package com.rusefi.maintenance;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class ProcessStatusWindow {
    protected final JTextArea log = new JTextArea();
    private final JScrollPane messagesScroll = new JScrollPane(log, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(400, 400);
        }
    };

    public ProcessStatusWindow() {
        log.setLineWrap(true);
    }

    public static boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("win");
    }

    protected static boolean isRunning(Process p) {
        try {
            p.exitValue();
            return false;
        } catch (IllegalThreadStateException e) {
            return true;
        }
    }

    protected void showFrame() {
        FrameHelper f = new FrameHelper();
        f.getFrame().setTitle("rusEfi Firmware Flasher");
        f.showFrame(messagesScroll, false);
        UiUtils.centerWindow(f.getFrame());
        log.setText(""); // let's remove stuff from previous invocation
    }

    protected void appendMsg(final String s) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                log.append(s + "\r\n");
                UiUtils.trueLayout(log);
            }
        });
    }

    /**
     * This method listens to a data stream from the process, appends messages to UI
     * and accumulates output in a buffer
     */
    protected void startStreamThread(final Process p, final InputStream stream, final StringBuffer buffer) {
        final Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    BufferedReader bis = new BufferedReader(new InputStreamReader(stream));
                    while (isRunning(p)) {
                        String line = bis.readLine();
                        if (line == null)
                            break;
                        appendMsg(line);
                        buffer.append(line);
                    }
                } catch (IOException e) {
                    appendMsg("Stream " + e);
                }
            }
        });
        t.setDaemon(true);
        t.start();
    }

    protected StringBuffer executeCommand(String command) {
        appendMsg("Executing " + command);
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        try {
            Process p = Runtime.getRuntime().exec(command);
            startStreamThread(p, p.getInputStream(), output);
            startStreamThread(p, p.getErrorStream(), error);
            p.waitFor();
        } catch (IOException e) {
            appendMsg("IOError: " + e);
        } catch (InterruptedException e) {
            appendMsg("WaitError: " + e);
        }
        return error;
    }
}