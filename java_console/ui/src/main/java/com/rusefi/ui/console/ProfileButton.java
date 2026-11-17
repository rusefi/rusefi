package com.rusefi.ui.console;

import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingConstants;
import java.awt.BasicStroke;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

/**
 * "Profile: Trigger ^" beside the Start Data Logging button: shows which signals the next recording keeps and
 * opens a menu to choose another logging profile. The choice is locked while recording, since the file's
 * fields are fixed when it starts. Hidden together with the record button. [tag:console_theme]
 */
final class ProfileButton extends JButton {
    static final String PREFIX = "Profile: ";
    /** Longer profile names are shortened with an ellipsis; the tooltip gives the whole name. */
    static final int MAX_NAME = 24;
    /** Room for text that renders a little wider than measured, e.g. with fractional display scaling. */
    private static final int TEXT_SLACK = 4;

    private final BinaryLoggingMenu logging;
    /** The menu shown last, for tests. */
    JPopupMenu popup;

    ProfileButton(BinaryLoggingMenu logging) {
        this.logging = logging;
        setIcon(new ChevronIcon());
        setHorizontalTextPosition(SwingConstants.LEFT);
        setIconTextGap(6);
        addActionListener(e -> showMenu());
        logging.addStateListener(this::refresh);
        refresh();
    }

    /** The record button and this one as one outlined control, for the tabs' status strips. */
    static JComponent withRecordButton(BinaryLoggingMenu logging) {
        return new JoinedButtons(new RecordButton(logging), new ProfileButton(logging));
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension size = super.getPreferredSize();
        size.height = JoinedButtons.HEIGHT;
        size.width += TEXT_SLACK;
        return size;
    }

    /** As wide as preferred: a squeezed layout would otherwise cut the name off ("Profile: F…"). */
    @Override
    public Dimension getMinimumSize() {
        return getPreferredSize();
    }

    void refresh() {
        setVisible(logging.isRecordButtonShown());
        String name = logging.profiles().selected().getName();
        setText(PREFIX + shorten(name));
        boolean locked = logging.isRecording() || logging.isStarting();
        setEnabled(!locked);
        setToolTipText(locked ? "Stop logging to change the profile"
                : name + ": " + logging.profiles().selected().describe() + " · click to choose another profile");
        revalidate();
        repaint();
    }

    /** "My very long profile n…" for names longer than {@link #MAX_NAME}. */
    static String shorten(String name) {
        return name.length() <= MAX_NAME ? name : name.substring(0, MAX_NAME - 1).trim() + "\u2026";
    }

    JPopupMenu menu() {
        JPopupMenu menu = new JPopupMenu();
        for (JMenuItem item : logging.profileItems()) {
            menu.add(item);
        }
        menu.addSeparator();
        menu.add(logging.editProfilesItem());
        return menu;
    }

    private void showMenu() {
        popup = menu();
        if (isShowing()) {
            // above the button, which sits in the status strip at the bottom of the window
            popup.show(this, 0, -popup.getPreferredSize().height);
        }
    }

    /** A small upward chevron: the menu opens above the button. */
    private static final class ChevronIcon implements Icon {
        private static final int SIZE = 10;

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(c.getForeground());
                g2.setStroke(new BasicStroke(1.6f, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                g2.drawPolyline(new int[]{x + 1, x + SIZE / 2, x + SIZE - 1},
                        new int[]{y + 7, y + 3, y + 7}, 3);
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
