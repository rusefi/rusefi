package com.rusefi.maintenance;

import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.util.UiUtils;

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
    public static final String IMAGE_FILE = "rusefi.bin";
    private static final String OPEN_OCD_COMMAND = "openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c \"program " +
            IMAGE_FILE +
            " verify reset exit 0x08000000\"";
    public static final String SUCCESS_MESSAGE_TAG = "shutdown command invoked";

    private final JButton button = new JButton("Program Firmware");
    private final JTextArea log = new JTextArea();
    private final JScrollPane messagesScroll = new JScrollPane(log, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(400, 400);
        }
    };

    public FirmwareFlasher() {
        log.setLineWrap(true);

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                FrameHelper f = new FrameHelper();
                f.getFrame().setTitle("rusEfi Firmware Flasher");
                f.showFrame(messagesScroll, false);
                UiUtils.centerWindow(f.getFrame());
                log.setText(""); // let's remove stuff from previous invocation

                Thread openOcdThread = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        doFlashFirmware();
                    }
                });
                openOcdThread.setDaemon(true);
                openOcdThread.start();
            }
        });
    }

    private void doFlashFirmware() {
        if (!new File(IMAGE_FILE).exists()) {
            appendMsg(IMAGE_FILE + " not found, cannot proceed !!!");
            return;
        }
        appendMsg("Executing " + OPEN_OCD_COMMAND);
        StringBuffer output = new StringBuffer();
        StringBuffer error = new StringBuffer();
        try {
            Process p = Runtime.getRuntime().exec(OPEN_OCD_COMMAND);
            startStreamThread(p, p.getInputStream(), output);
            startStreamThread(p, p.getErrorStream(), error);
            p.waitFor();
        } catch (IOException e) {
            appendMsg("IOError: " + e);
        } catch (InterruptedException e) {
            appendMsg("WaitError: " + e);
        }
        if (error.toString().contains(SUCCESS_MESSAGE_TAG)) {
            appendMsg("!!! Looks good!!!");
        } else {
            appendMsg("!!! FIRMWARE FLASH: DOES NOT LOOK RIGHT !!!");
        }
    }

    /**
     * This method listens to a data stream from the process, appends messages to UI
     * and accumulates output in a buffer
     */
    private void startStreamThread(final Process p, final InputStream stream, final StringBuffer buffer) {
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
                UiUtils.trueLayout(log);
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
