package com.rusefi.ui.console;

import com.rusefi.ui.logview.RecentLogs;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.Popup;
import javax.swing.PopupFactory;
import javax.swing.Timer;
import javax.swing.UIManager;
import java.awt.Color;
import java.awt.Component;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.Point;
import java.awt.RenderingHints;
import java.io.File;
import java.io.IOException;
import java.util.Locale;

/**
 * Start Data Logging / Stop Logging button at the right end of the tabs' status strips: one click (or Ctrl+S)
 * starts a data log in the rusEFI logs folder and stops it again. While recording it shows a pulsing red stop
 * square with the elapsed time and the file size; after a recording stops, a notification offers to view it.
 * Logging > Show Data Logging Button hides it. [tag:console_theme]
 */
final class RecordButton extends JButton {
    static final String IDLE = "Start Data Logging";
    static final String STOP = "Stop Logging";
    /** Room for text that renders a little wider than measured, e.g. with fractional display scaling. */
    private static final int TEXT_SLACK = 4;
    private static final int TICK_MILLIS = 500;
    private static final int NOTICE_MILLIS = 8000;

    private final BinaryLoggingMenu logging;
    private final RecordIcon icon = new RecordIcon();
    private final Timer ticker = new Timer(TICK_MILLIS, e -> refresh(true));
    private int recordingWidth;
    private Popup notice;
    private final Timer noticeTimer = new Timer(NOTICE_MILLIS, e -> hideNotice());
    /** Text of the last notification, for tests. */
    String lastNotice;

    RecordButton(BinaryLoggingMenu logging) {
        super(IDLE);
        this.logging = logging;
        setIcon(icon);
        setIconTextGap(8);
        setMargin(new Insets(0, 12, 0, 12));
        noticeTimer.setRepeats(false);
        addActionListener(e -> logging.toggle());
        logging.addStateListener(() -> refresh(false));
        logging.addRecordingStoppedListener(this::recordingStopped);
        refresh(false);
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension size = super.getPreferredSize();
        size.height = JoinedButtons.HEIGHT;
        // never narrower than the idle label, so "Starting…" does not shrink it and the label is never cut off
        Insets insets = getInsets();
        int idleWidth = insets.left + insets.right + icon.getIconWidth() + getIconTextGap()
                + getFontMetrics(getFont()).stringWidth(IDLE);
        size.width = Math.max(size.width, idleWidth) + TEXT_SLACK;
        if (logging != null && logging.isRecording()) {
            // grow only while recording, so the button does not jitter as the numbers change
            recordingWidth = Math.max(recordingWidth, size.width);
            size.width = recordingWidth;
        }
        return size;
    }

    /** As wide as preferred: a squeezed layout would otherwise cut the label off ("Recor…"). */
    @Override
    public Dimension getMinimumSize() {
        return getPreferredSize();
    }

    void refresh(boolean tick) {
        setVisible(logging.isRecordButtonShown());
        boolean recording = logging.isRecording();
        if (recording) {
            if (!ticker.isRunning()) {
                ticker.start();
            }
            icon.recording = true;
            icon.dim = tick && !icon.dim;
            File file = logging.lastLog();
            setText(STOP + "  ·  " + elapsed(logging.recordingMillis()) + "  ·  "
                    + RecentLogs.size(file == null ? 0 : file.length()));
            setForeground(themed("Console.Status.disconnected.foreground", Color.RED));
            setToolTipText("Recording " + (file == null ? "" : file.getName()) + " - click to stop (Ctrl+S)");
            setEnabled(true);
        } else {
            ticker.stop();
            recordingWidth = 0;
            icon.recording = false;
            icon.dim = false;
            setText(logging.isStarting() ? "Starting…" : IDLE);
            setForeground(UIManager.getColor("Button.foreground"));
            setEnabled(logging.isStarting() || logging.canStart());
            setToolTipText(logging.isStarting() ? "Saving the tune before recording - click to cancel (Ctrl+S)"
                    : isEnabled() ? "Start recording a data log into the rusEFI logs folder (Ctrl+S)"
                    : "Connect to an ECU to record a data log");
        }
        revalidate();
        repaint();
    }

    /** "00:42", "12:05", "1:02:09". */
    static String elapsed(long millis) {
        long seconds = Math.max(0, millis / 1000);
        if (seconds < 3600) {
            return String.format(Locale.ROOT, "%02d:%02d", seconds / 60, seconds % 60);
        }
        return String.format(Locale.ROOT, "%d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);
    }

    private void recordingStopped(File file, double seconds) {
        String duration = RecentLogs.duration(seconds);
        lastNotice = "Log saved  ·  " + (duration.isEmpty() ? "" : duration + "  ·  ") + RecentLogs.size(file.length());
        showNotice(file);
    }

    /** A small panel above the button: what was saved, View in the Log Viewer, Show folder, close. */
    JPanel noticePanel(File file) {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 6));
        panel.setBackground(themed("Console.Card.background", Color.WHITE));
        panel.setBorder(BorderFactory.createLineBorder(themed("Console.Card.borderColor", Color.LIGHT_GRAY)));
        JLabel text = new JLabel("✓ " + lastNotice);
        text.setForeground(themed("Console.Status.connected.foreground", text.getForeground()));
        panel.add(text);
        JButton view = new JButton("View");
        view.addActionListener(e -> {
            hideNotice();
            logging.view(file);
        });
        panel.add(view);
        JButton folder = new JButton("Show folder");
        folder.setEnabled(Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.OPEN));
        folder.addActionListener(e -> {
            hideNotice();
            try {
                Desktop.getDesktop().open(file.getAbsoluteFile().getParentFile());
            } catch (IOException | RuntimeException ignored) {
                // the file manager is optional; the log is saved either way
            }
        });
        panel.add(folder);
        JButton close = new JButton("×");
        close.setBorderPainted(false);
        close.setContentAreaFilled(false);
        close.addActionListener(e -> hideNotice());
        panel.add(close);
        return panel;
    }

    private void showNotice(File file) {
        hideNotice();
        if (!isShowing()) {
            return;
        }
        JPanel panel = noticePanel(file);
        Point button = getLocationOnScreen();
        Dimension size = panel.getPreferredSize();
        // above the button, which sits in the status strip at the bottom of the window
        notice = PopupFactory.getSharedInstance().getPopup(this, panel,
                button.x + getWidth() - size.width, button.y - size.height - 6);
        notice.show();
        noticeTimer.restart();
    }

    private void hideNotice() {
        noticeTimer.stop();
        if (notice != null) {
            notice.hide();
            notice = null;
        }
    }

    private static Color themed(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    /** A red dot (record) or a red rounded square (stop) that pulses while recording. */
    static final class RecordIcon implements Icon {
        private static final int SIZE = 12;
        boolean recording;
        boolean dim;

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                Color red = themed("Console.Status.disconnected.dot", Color.RED);
                if (!c.isEnabled()) {
                    red = themed("Label.disabledForeground", Color.GRAY);
                }
                g2.setColor(dim ? new Color(red.getRed(), red.getGreen(), red.getBlue(), 110) : red);
                if (recording) {
                    g2.fillRoundRect(x + 1, y + 1, SIZE - 2, SIZE - 2, 4, 4);
                } else {
                    g2.fillOval(x, y, SIZE, SIZE);
                }
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return SIZE;
        }

        @Override
        public int getIconHeight() {
            return SIZE;
        }
    }
}
