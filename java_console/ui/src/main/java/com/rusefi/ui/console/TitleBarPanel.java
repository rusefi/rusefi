package com.rusefi.ui.console;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.UiProperties;
import com.rusefi.ui.widgets.ConsoleFonts;

import javax.swing.*;
import java.awt.*;
import java.util.List;

/**
 * Centre of the console title bar: product name, console version and short labels for the
 * connected firmware and link, replacing FlatLaf's single title string. The frame title itself
 * (taskbar, Alt+Tab) keeps the full text, which this panel also shows as its tooltip.
 * Colours come from the Console.TitleBar.* keys in the theme files [tag:console_theme].
 */
class TitleBarPanel extends JPanel {
    static final int GAP = 8;
    private static final int CORNER_ICON_GAP = 6;
    private final JLabel name = caption(new JLabel(UiProperties.getDisplayName()));
    private final JLabel version = caption(new JLabel());
    private final JLabel leftBranchIcon = branchIcon();
    private final JLabel rightBranchIcon = branchIcon();

    TitleBarPanel() {
        super(new DroppingRowLayout());
        setOpaque(false);
        caption(this);
        name.setFont(name.getFont().deriveFont(Font.BOLD));
        add(name);
        add(version);
        add(leftBranchIcon);
        add(rightBranchIcon);
    }

    private static JLabel branchIcon() {
        JLabel label = caption(new JLabel(new ClownIcon()));
        label.setVisible(false);
        label.setToolTipText("Development firmware branch");
        return label;
    }

    /** Shows corner markers only for selected connected firmware branches. */
    void setConnectedBranch(String branch) {
        boolean show = "master".equals(branch) || "development".equals(branch) || "lts-26lima-dev".equals(branch);
        leftBranchIcon.setVisible(show);
        rightBranchIcon.setVisible(show);
        revalidate();
        repaint();
    }

    void update(String consoleVersion, List<String> chipTexts, String tooltip) {
        version.setText(consoleVersion);
        version.setForeground(mutedForeground());
        while (getComponentCount() > 4) {
            remove(2);
        }
        for (String text : chipTexts) {
            if (text != null && !text.isEmpty()) {
                add(caption(new ChipLabel(text)), getComponentCount() - 2);
            }
        }
        setToolTipText(tooltip);
        name.setToolTipText(tooltip);
        version.setToolTipText(tooltip);
        revalidate();
        repaint();
    }

    @Override
    public void updateUI() {
        super.updateUI();
        if (version != null) {
            version.setForeground(mutedForeground());
        }
    }

    /**
     * "uaefi_pro · master.2026.08.13" from "rusEFI master.2026.08.13.uaefi_pro.3151813420";
     * an unparsable signature is shown as is, without the "rusEFI " prefix.
     */
    static String firmwareLabel(String signature) {
        if (signature == null || signature.trim().isEmpty()) {
            return null;
        }
        RusEfiSignature s = SignatureHelper.parse(signature);
        if (s == null) {
            String trimmed = signature.trim();
            return trimmed.startsWith("rusEFI ") ? trimmed.substring("rusEFI ".length()).trim() : trimmed;
        }
        return s.getBundleTarget() + " · " + s.getBranch() + "." + s.getYear() + "." + s.getMonth() + "." + s.getDay();
    }

    /**
     * "simulator :29001" or "uaefi121 COM7": the firmware build id from the version report
     * ("&lt;version&gt;@&lt;hash&gt; &lt;buildId&gt; &lt;engineType&gt; &lt;uptime&gt;", see
     * printVersionForConsole in status_loop.cpp) followed by the port; a bare number is a TCP port.
     */
    static String connectionLabel(String firmwareVersion, String port) {
        String buildId = null;
        if (firmwareVersion != null) {
            String[] parts = firmwareVersion.trim().split("\\s+");
            if (parts.length >= 2 && parts[0].contains("@")) {
                buildId = parts[1];
            }
        }
        String portLabel = null;
        if (port != null && !port.trim().isEmpty()) {
            portLabel = port.trim().matches("\\d+") ? ":" + port.trim() : port.trim();
        }
        if (buildId == null) {
            return portLabel;
        }
        return portLabel == null ? buildId : buildId + " " + portLabel;
    }

    static Color mutedForeground() {
        Color c = UIManager.getColor("Console.TitleBar.mutedForeground");
        return c != null ? c : UIManager.getColor("Label.disabledForeground");
    }

    private static <T extends JComponent> T caption(T component) {
        // Clicking and dragging the title text still moves the window, like an empty title bar.
        component.putClientProperty(FlatClientProperties.COMPONENT_TITLE_BAR_CAPTION, true);
        return component;
    }

    /**
     * One centred row. When the title bar is too narrow for everything, the last items (the
     * connection chip first, then the firmware chip, then the version) are hidden instead of
     * wrapping onto a second line the title bar would cut off; the tooltip keeps the full text.
     */
    static class DroppingRowLayout implements LayoutManager {
        @Override
        public void layoutContainer(Container parent) {
            Insets insets = parent.getInsets();
            int available = parent.getWidth() - insets.left - insets.right;
            int height = parent.getHeight() - insets.top - insets.bottom;
            Component leftIcon = parent.getComponentCount() > 2 ? parent.getComponent(parent.getComponentCount() - 2) : null;
            Component rightIcon = parent.getComponentCount() > 2 ? parent.getComponent(parent.getComponentCount() - 1) : null;
            int leftWidth = leftIcon != null && leftIcon.isVisible() ? leftIcon.getPreferredSize().width + CORNER_ICON_GAP : 0;
            int rightWidth = rightIcon != null && rightIcon.isVisible() ? rightIcon.getPreferredSize().width + CORNER_ICON_GAP : 0;
            int contentLeft = insets.left + leftWidth;
            int contentWidth = Math.max(0, available - leftWidth - rightWidth);
            int shown = 0;
            int width = 0;
            for (int i = 0; i < parent.getComponentCount() - (leftIcon == null ? 0 : 2); i++) {
                Component c = parent.getComponent(i);
                int next = width + (shown == 0 ? 0 : GAP) + c.getPreferredSize().width;
                // the product name always stays, clipped if even it does not fit
                if (shown > 0 && next > contentWidth) {
                    break;
                }
                width = next;
                shown++;
            }
            int x = contentLeft + Math.max(0, (contentWidth - width) / 2);
            int contentCount = parent.getComponentCount() - (leftIcon == null ? 0 : 2);
            for (int i = 0; i < contentCount; i++) {
                Component c = parent.getComponent(i);
                if (i >= shown) {
                    c.setBounds(0, 0, 0, 0);
                    continue;
                }
                Dimension size = c.getPreferredSize();
                int w = Math.min(size.width, contentLeft + contentWidth - x);
                int h = Math.min(size.height, height);
                c.setBounds(x, insets.top + (height - h) / 2, w, h);
                x += w + GAP;
            }
            if (leftIcon != null) {
                placeCornerIcon(leftIcon, insets.left, insets.top, height);
                placeCornerIcon(rightIcon, insets.left + available - rightIcon.getPreferredSize().width, insets.top, height);
            }
        }

        private static void placeCornerIcon(Component icon, int x, int top, int height) {
            if (!icon.isVisible()) {
                icon.setBounds(0, 0, 0, 0);
                return;
            }
            Dimension size = icon.getPreferredSize();
            int h = Math.min(size.height, height);
            icon.setBounds(x, top + (height - h) / 2, size.width, h);
        }

        @Override
        public Dimension preferredLayoutSize(Container parent) {
            Insets insets = parent.getInsets();
            int width = 0;
            int height = 0;
            for (Component c : parent.getComponents()) {
                if (!c.isVisible()) {
                    continue;
                }
                Dimension size = c.getPreferredSize();
                width += (width == 0 ? 0 : GAP) + size.width;
                height = Math.max(height, size.height);
            }
            return new Dimension(width + insets.left + insets.right, height + insets.top + insets.bottom);
        }

        /** The product name only, so the menu bar can shrink this panel instead of the menus. */
        @Override
        public Dimension minimumLayoutSize(Container parent) {
            Dimension preferred = preferredLayoutSize(parent);
            Insets insets = parent.getInsets();
            int first = parent.getComponentCount() == 0 ? 0 : parent.getComponent(0).getPreferredSize().width;
            return new Dimension(first + insets.left + insets.right, preferred.height);
        }

        @Override
        public void addLayoutComponent(String name, Component comp) {
        }

        @Override
        public void removeLayoutComponent(Component comp) {
        }
    }

    /** Short monospaced label on a rounded background. */
    private static class ChipLabel extends JLabel {
        ChipLabel(String text) {
            super(text);
            setBorder(BorderFactory.createEmptyBorder(3, 10, 3, 10));
        }

        @Override
        public void updateUI() {
            super.updateUI();
            // full label size in a modern monospace; the logical Monospaced font is Courier on Windows
            setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
            setForeground(mutedForeground());
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                Color background = UIManager.getColor("Console.TitleBar.chipBackground");
                g2.setColor(background != null ? background : UIManager.getColor("Panel.background"));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    // yes, a custom IconClass, cheaper on rendering time than a png or a complete icon set
    private static class ClownIcon implements Icon {
        private static final int SIZE = 20;
        private static final Color MAKEUP_RED = new Color(220, 55, 65);

        @Override
        public int getIconWidth() { return SIZE; }

        @Override
        public int getIconHeight() { return SIZE; }

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setColor(MAKEUP_RED);
                g.fillOval(x + 1, y + 2, 8, 8);
                g.fillOval(x + 11, y + 2, 8, 8);
                g.setColor(Color.WHITE);
                g.fillOval(x + 3, y + 4, 14, 14);
                g.setColor(Color.BLACK);
                g.fillOval(x + 6, y + 8, 2, 2);
                g.fillOval(x + 12, y + 8, 2, 2);
                g.setColor(MAKEUP_RED);
                g.fillOval(x + 8, y + 10, 4, 4);
                g.drawArc(x + 7, y + 10, 7, 5, 200, 140);
            } finally {
                g.dispose();
            }
        }
    }
}
