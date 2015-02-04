package com.rusefi.ui;

import com.irnems.FileLog;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import static com.rusefi.Launcher.*;
import static javax.swing.JOptionPane.OK_OPTION;

/**
 * 6/30/13
 * (c) Andrey Belomutskiy
 */
public class DefaultExceptionHandler implements Thread.UncaughtExceptionHandler {
    public void uncaughtException(Thread t, Throwable e) {
        handleException(e);
    }

    public static void handleException(Throwable e) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream ps = new PrintStream(baos);
        e.printStackTrace(ps);

        JPanel content = new JPanel(new BorderLayout());
        content.add(new JLabel(e.toString()), BorderLayout.NORTH);

        JTextArea textArea = new JTextArea(baos.toString());

        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        JScrollPane scrollPane = new JScrollPane(textArea) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(500, 400);
            }
        };

        content.add(scrollPane, BorderLayout.CENTER);

        JOptionPane.showConfirmDialog(findActiveFrame(), content, CONSOLE_VERSION + ": Exception Occurred", JOptionPane.DEFAULT_OPTION);
        e.printStackTrace();
        FileLog.MAIN.logLine("handleException: " + baos.toString());
    }

    private static Frame findActiveFrame() {
        Frame[] frames = JFrame.getFrames();
        for (Frame frame : frames) {
            if (frame.isVisible())
                return frame;
        }
        return null;
    }
}
