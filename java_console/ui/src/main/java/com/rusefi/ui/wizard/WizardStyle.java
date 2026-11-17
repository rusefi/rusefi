package com.rusefi.ui.wizard;

import com.formdev.flatlaf.ui.FlatLineBorder;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.TransparentPanels;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Path2D;

/**
 * Shared look of the setup wizard: white rounded cards on the grey console page, muted
 * section labels and themed status colours, matching the startup Connect card and Log Viewer.
 * Colours: Console.Card.* and Console.Wizard.* theme keys [tag:console_theme].
 */
final class WizardStyle {
    static final int GAP = 12;
    static final int LARGE_GAP = 20;
    static final int ARC = 10;

    static Color color(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color != null ? color : fallback;
    }

    static Color background() {
        return color("Panel.background", new Color(0xf5f6f8));
    }

    static Color surface() {
        return color("TextField.background", Color.WHITE);
    }

    static Color border() {
        return color("Component.borderColor", new Color(0xc8ccd2));
    }

    static Color text() {
        return color("Label.foreground", new Color(0x20242a));
    }

    static Color muted() {
        return color("Console.Wizard.mutedForeground", color("Label.disabledForeground", new Color(0x586273)));
    }

    static Color accent() {
        return color("Component.accentColor", new Color(0x2563eb));
    }

    static Color accentText() {
        return color("Console.Wizard.accentForeground", new Color(0x1d4ed8));
    }

    static Color success() {
        return color("Console.Wizard.success.foreground", new Color(0x15803d));
    }

    static Color successBackground() {
        return color("Console.Wizard.success.background", new Color(0xe7f6ec));
    }

    static Color error() {
        return color("Console.Wizard.error.foreground", new Color(0xb91c1c));
    }

    static Color errorBackground() {
        return color("Console.Wizard.error.background", new Color(0xfdecec));
    }

    static Color focus() {
        return color("Component.focusColor", new Color(0x4d86f7));
    }

    static Color cardBackground() {
        return color("Console.Card.background", Color.WHITE);
    }

    static Color cardBorder() {
        return color("Console.Card.borderColor", new Color(0xdfe3e9));
    }

    static Color sectionBackground() {
        return color("Console.Card.sectionBackground", new Color(0xf7f8fa));
    }

    /** Background of the wizard's header and footer strips, like the console's tab row and status bar. */
    static Color barBackground() {
        return color("Console.TabArea.background", Color.WHITE);
    }

    static Color separator() {
        return color("Separator.foreground", new Color(0xdfe3e9));
    }

    /**
     * Paints {@code card} as a white rounded card with the given padding. Plain child panels become
     * transparent (also ones added later) so the card colour shows through behind every row.
     */
    static void styleCard(JComponent card, int vertical, int horizontal) {
        styleRounded(card, cardBackground(), vertical, horizontal);
        transparentPanels(card);
    }

    /** A tinted rounded block inside a card, e.g. a live sensor readout. */
    static void styleSection(JComponent section, int vertical, int horizontal) {
        styleRounded(section, sectionBackground(), vertical, horizontal);
    }

    private static void styleRounded(JComponent component, Color background, int vertical, int horizontal) {
        // FlatPanelUI paints a rounded background when the border is a FlatLineBorder with an arc
        component.setBorder(new FlatLineBorder(new Insets(vertical, horizontal, vertical, horizontal), cardBorder(), 1, ARC));
        // a plain Color, not the theme's UIResource, so transparentPanels() keeps this block tinted
        component.setBackground(new Color(background.getRGB(), true));
        component.setOpaque(true);
    }

    /** A step page: the card centred at the top, scrolling vertically when taller than the window. */
    static JScrollPane cardPage(JComponent card) {
        JPanel page = new ScrollablePanel();
        page.setLayout(new GridBagLayout());
        page.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        GridBagConstraints c = new GridBagConstraints();
        c.anchor = GridBagConstraints.NORTH;
        c.weightx = 1;
        c.weighty = 1;
        page.add(card, c);
        return pageScrollPane(page);
    }

    /** Scroll pane for a step page: no frame of its own, so the card sits directly on the page. */
    static JScrollPane pageScrollPane(JComponent view) {
        JScrollPane scrollPane = new JScrollPane(view);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        scrollPane.getVerticalScrollBar().setUnitIncrement(16);
        return scrollPane;
    }

    /** See {@link TransparentPanels#install}. */
    static void transparentPanels(JComponent root) {
        TransparentPanels.install(root);
    }

    /** Small bold muted capitals above a group: a card's eyebrow or a field's section label. */
    static JLabel sectionLabel(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(muted());
        label.setFont(label.getFont().deriveFont(Font.BOLD, label.getFont().getSize2D() * 0.9f));
        return label;
    }

    /** Card heading: the step's name in a larger bold font. */
    static JLabel cardTitle(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(text());
        label.setFont(label.getFont().deriveFont(Font.BOLD, label.getFont().getSize2D() * 1.6f));
        return label;
    }

    /** One-line explanation under a card heading. */
    static JLabel description(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(muted());
        return label;
    }

    static void styleError(JLabel label) {
        label.setForeground(error());
    }

    /** Shapes keep confirmation and cancellation distinguishable without relying on colour. */
    enum ActionIcon implements Icon {
        CONFIRM, CANCEL, NEXT;

        @Override
        public void paintIcon(Component component, Graphics graphics, int x, int y) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.translate(x, y);
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setColor(component.getForeground());
                g.setStroke(new BasicStroke(2, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
                if (this == CONFIRM) {
                    Path2D check = new Path2D.Float();
                    check.moveTo(3, 8);
                    check.lineTo(6, 11);
                    check.lineTo(13, 4);
                    g.draw(check);
                } else if (this == NEXT) {
                    g.drawLine(3, 8, 12, 8);
                    g.drawLine(8, 4, 12, 8);
                    g.drawLine(8, 12, 12, 8);
                } else {
                    g.drawLine(4, 4, 12, 12);
                    g.drawLine(12, 4, 4, 12);
                }
            } finally {
                g.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return 16;
        }

        @Override
        public int getIconHeight() {
            return 16;
        }
    }

    private WizardStyle() {
    }
}
