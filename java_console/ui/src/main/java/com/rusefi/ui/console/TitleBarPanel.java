package com.rusefi.ui.console;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;

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
    private final JLabel name = caption(new JLabel("rusEFI Console"));
    private final JLabel version = caption(new JLabel());
    private final JPanel chips = new JPanel(new FlowLayout(FlowLayout.CENTER, 6, 0));

    TitleBarPanel() {
        super(new FlowLayout(FlowLayout.CENTER, 8, 0));
        setOpaque(false);
        caption(this);
        chips.setOpaque(false);
        caption(chips);
        name.setFont(name.getFont().deriveFont(Font.BOLD));
        add(name);
        add(version);
        add(chips);
    }

    void update(String consoleVersion, List<String> chipTexts, String tooltip) {
        version.setText(consoleVersion);
        version.setForeground(mutedForeground());
        chips.removeAll();
        for (String text : chipTexts) {
            if (text != null && !text.isEmpty()) {
                chips.add(caption(new ChipLabel(text)));
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

    /** Short monospaced label on a rounded background. */
    private static class ChipLabel extends JLabel {
        ChipLabel(String text) {
            super(text);
            setBorder(BorderFactory.createEmptyBorder(2, 8, 2, 8));
        }

        @Override
        public void updateUI() {
            super.updateUI();
            Font base = UIManager.getFont("Label.font");
            if (base != null) {
                setFont(new Font(Font.MONOSPACED, Font.PLAIN, Math.max(base.getSize() - 1, 10)));
            }
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
}
