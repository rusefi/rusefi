package com.rusefi.maintenance;

import com.rusefi.ui.FrameHelper;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;

/**
 * (c) Andrey Belomutskiy 2013-2015
 * 2/4/15
 */
public class FirmwareFlasher {
    public static final String IMAGE_ELF = "rusefi.elf";
    private static final String OPEN_OCD_COMMAND = "openocd/bin/openocd-0.8.0.exe " +
            "-f interface/stlink-v2.cfg " +
            "-f board/stm32f4discovery.cfg " +
            "-c init " +
            "-c targets " +
            "-c \"halt\" " +
            "-c \"flash write_image erase " + IMAGE_ELF + "\" " +
            "-c \"verify_image " + IMAGE_ELF + "\" " +
            "-c \"reset run\" " +
            "-c shutdown";

    private final JButton button = new JButton("wip");
    private final JTextArea log = new JTextArea();

    public FirmwareFlasher() {
        log.setLineWrap(true);

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                FrameHelper f = new FrameHelper();
                f.showFrame(log, false);

                if (!new File(IMAGE_ELF).exists()) {
                    appendMsg(IMAGE_ELF + " not found, cannot proceed !!!");
                    return;
                }

                doFlashFirmware();
            }
        });
    }

    private void doFlashFirmware() {
        appendMsg("Executing " + OPEN_OCD_COMMAND);
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        try {
            Process p = Runtime.getRuntime().exec(OPEN_OCD_COMMAND);
            startStreamThread(p, p.getInputStream(), "output", output);
            startStreamThread(p, p.getErrorStream(), "error", error);
            p.waitFor();
        } catch (IOException e) {
            appendMsg("IOError: " + e);
        } catch (InterruptedException e) {
            appendMsg("WaitError: " + e);
        }

        appendMsg("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
    }

    /**
     * This method listens to a data stream from the process, appends messages to UI
     * and accumulates output in a buffer
     */
    private void startStreamThread(final Process p, final InputStream stream, final String msg, final StringBuffer buffer) {
        final Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    BufferedReader bis = new BufferedReader(new InputStreamReader(stream));
                    while (isRunning(p)) {
                        String line = bis.readLine();
                        appendMsg(line);
                        buffer.append(line);
                    }
                } catch (IOException e) {
                    appendMsg("Stream " + e);
                }
                appendMsg("<EOS> " + msg);
            }
        });
        t.setDaemon(true);
        t.start();
    }

    private static boolean isRunning(Process p) {
        try {
            p.exitValue();
            return false;
        } catch (IllegalThreadStateException e) {
            return true;
        }
    }

    private void appendMsg(final String s) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                log.append(s + "\r\n");
            }
        });
    }

    public static boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("win");
    }

    public static Component getContent() {
        return new FirmwareFlasher().button;
    }
}
