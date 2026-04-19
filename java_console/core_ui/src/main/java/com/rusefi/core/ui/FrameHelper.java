package com.rusefi.core.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Arrays;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FrameHelper {
    private static final Logging log = getLogging(FrameHelper.class);
    private final JFrame frame = new JFrame();

    public FrameHelper() {
        this(JDialog.DISPOSE_ON_CLOSE);
    }

    public FrameHelper(int operation) {
        AutoupdateUtil.assertAwtThread();
        frame.setDefaultCloseOperation(operation);
        AutoupdateUtil.setAppIcon(frame);
    }

    @NotNull
    public static String appendBundleName(String title) {
        String bundleName = BundleUtil.readBundleFullNameNotNull().getUiLabel();
        return title + " " + bundleName;
    }

    // note hard-coded 'EXIT_ON_CLOSE' - that's the best choice at the moment
    public static FrameHelper createFrame(String title) {
        FrameHelper frame = new FrameHelper(JDialog.EXIT_ON_CLOSE);
        frame.frame.setTitle(appendBundleName(title));
        return frame;
    }

    public JFrame getFrame() {
        return frame;
    }

    public void showFrame(JComponent content) {
        showFrame(content, true);
    }

    public void showFrame(JComponent content, final boolean maximizeOnStart) {
        initFrame(content, maximizeOnStart);
        frame.setVisible(true);
    }

    public void initFrame(JComponent content, final boolean maximizeOnStart) {
        if (maximizeOnStart) {
            frame.setSize(GraphicsEnvironment.getLocalGraphicsEnvironment().getMaximumWindowBounds().getSize());
        } else if (frame.getSize().getWidth() < 1 || frame.getSize().getHeight() < 1){
            frame.setSize(800, 500);
        }
        frame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                onWindowOpened();
                if (maximizeOnStart)
                    frame.setExtendedState(frame.getExtendedState() | JFrame.MAXIMIZED_BOTH);
            }

            @Override
            public void windowClosed(WindowEvent ev) {
                onWindowClosed();
                for (Thread t : Thread.getAllStackTraces().keySet()) {
                    if (!t.isDaemon())
                       log.info("Non-daemon thread: " + t);
                }
                log.info(Arrays.toString(Frame.getFrames()));
            }
        });
        frame.add(content);
    }

    protected void onWindowOpened() {
    }

    protected void onWindowClosed() {
    }
}
