package com.rusefi.core.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.io.BundleUtil;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Arrays;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Date: 3/24/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FrameHelper {
    private static final Logging log = getLogging(FrameHelper.class);
    private final JFrame frame;
    /**
     * True when this helper wraps an already-visible frame handed off from another window
     * (#9715) — in that case {@link #showFrame} swaps content in place instead of creating and
     * showing a new window.
     */
    private final boolean reusing;

    public FrameHelper() {
        this(JDialog.DISPOSE_ON_CLOSE);
    }

    public FrameHelper(int operation) {
        this(null, operation);
    }

    /**
     * When {@code existingFrame} is non-null, reuse that already-visible frame instead of creating a
     * new window (#9715) — used to hand the maximized splash frame to the main console so there is no
     * second window flashing in. When null, a fresh {@link JFrame} is created as usual.
     */
    public FrameHelper(JFrame existingFrame, int operation) {
        AutoupdateUtil.assertAwtThread();
        this.reusing = existingFrame != null;
        this.frame = reusing ? existingFrame : new JFrame();
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
        if (reusing) {
            swapContent(content);
        } else {
            initFrame(content, maximizeOnStart);
            frame.setVisible(true);
        }
    }

    public void initFrame(JComponent content, final boolean maximizeOnStart) {
        if (maximizeOnStart) {
            frame.setSize(GraphicsEnvironment.getLocalGraphicsEnvironment().getMaximumWindowBounds().getSize());
        } else if (frame.getSize().getWidth() < 1 || frame.getSize().getHeight() < 1){
            frame.setSize(800, 500);
        }
        installWindowListener(maximizeOnStart);
        frame.add(content);
    }

    /**
     * Swap the content of an already-visible, maximized frame (#9715). The previous owner's window
     * listeners (e.g. the splash's "save config on close") and its default button are dropped, our
     * close-handling listener is installed, and {@link #onWindowOpened()} is invoked explicitly
     * because {@code WINDOW_OPENED} does not re-fire on a frame that is already showing.
     */
    private void swapContent(JComponent content) {
        for (WindowListener l : frame.getWindowListeners()) {
            frame.removeWindowListener(l);
        }
        installWindowListener(false);
        frame.getRootPane().setDefaultButton(null);
        frame.getContentPane().removeAll();
        frame.add(content);
        AutoupdateUtil.trueLayoutAndRepaint(frame);
        onWindowOpened();
        // The loading overlay raised by StartupFrame.connect stays up across this swap and is taken
        // down once the new content has painted (#9715): invokeLater drains after the repaint queued
        // above, so the overlay lifts as the gauges become visible — no flicker, no fixed delay.
        SwingUtilities.invokeLater(() -> LoadingOverlay.hide(frame));
    }

    private void installWindowListener(final boolean maximizeOnStart) {
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

            @Override
            public void windowClosing(WindowEvent e) {
                onWindowClosing();
            }
        });
    }

    protected void onWindowOpened() {
    }

    protected void onWindowClosed() {
    }

    protected void onWindowClosing() {
    }
}
