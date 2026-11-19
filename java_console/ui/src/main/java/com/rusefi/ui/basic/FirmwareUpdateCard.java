package com.rusefi.ui.basic;

import com.formdev.flatlaf.ui.FlatLineBorder;
import com.formdev.flatlaf.util.UIScale;

import javax.swing.*;
import java.awt.*;
import java.util.concurrent.atomic.AtomicReference;

/** One themed, responsive surface for firmware identity, actions, and operation feedback. */
public final class FirmwareUpdateCard extends JPanel {
    private FirmwareUpdateCard(String title) {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setBorder(BorderFactory.createCompoundBorder(
            new FlatLineBorder(new Insets(UIScale.scale(22), UIScale.scale(28),
                UIScale.scale(22), UIScale.scale(28)),
                color("Console.Card.borderColor", new Color(0xdfe3e9)), 1, 16),
            BorderFactory.createEmptyBorder(UIScale.scale(18), UIScale.scale(22),
                UIScale.scale(20), UIScale.scale(22))));
        setBackground(color("Console.Card.background", UIManager.getColor("Panel.background")));
        setOpaque(true);

        JLabel heading = new JLabel(title);
        heading.setFont(heading.getFont().deriveFont(Font.BOLD, heading.getFont().getSize2D() + 3f));
        JComponent help = com.rusefi.maintenance.ProgramSelector.createHelpButton();
        ResponsiveHeader header = new ResponsiveHeader(heading, help);
        add(header);
        add(Box.createVerticalStrut(UIScale.scale(10)));
        JSeparator separator = new JSeparator();
        separator.setForeground(color("Separator.foreground", heading.getForeground()));
        add(separator);
        add(Box.createVerticalStrut(UIScale.scale(14)));
    }

    @Override
    protected void addImpl(Component component, Object constraints, int index) {
        if (component instanceof JComponent) {
            ((JComponent) component).setAlignmentX(Component.LEFT_ALIGNMENT);
        }
        super.addImpl(component, constraints, index);
    }

    public static FirmwareUpdateCard create(String title) {
        return new FirmwareUpdateCard(title);
    }

    public static JScrollPane inResponsiveViewport(FirmwareUpdateCard card) {
        JPanel view = new CenteredCardView(card);
        JScrollPane scroll = new JScrollPane(view);
        scroll.setBorder(BorderFactory.createEmptyBorder());
        scroll.getViewport().setOpaque(false);
        scroll.setOpaque(false);
        scroll.getVerticalScrollBar().setUnitIncrement(16);
        return scroll;
    }

    /** Keeps no-INI consent in the firmware tab and discards it when navigation leaves that tab. */
    public static JPanel createFirmwareTabContent(
        JComponent firmwareContent, com.rusefi.maintenance.ProgramSelector selector,
        JTabbedPane tabs, Component tabContent
    ) {
        CardLayout layout = new CardLayout();
        JPanel views = new JPanel(layout);
        views.add(firmwareContent, "firmware");
        AtomicReference<JComponent> activeConfirmation = new AtomicReference<>();
        selector.setNoIniUpdatePanelHost(panel -> {
            if (views.getComponentCount() > 1) {
                views.remove(1);
            }
            JPanel confirmationHost = new JPanel(new BorderLayout());
            confirmationHost.setVisible(false);
            confirmationHost.setOpaque(false);
            confirmationHost.add(panel, BorderLayout.CENTER);
            activeConfirmation.set(panel);
            views.add(confirmationHost, "no-ini-confirmation");
            layout.show(views, "no-ini-confirmation");
        }, () -> {
            JComponent confirmation = activeConfirmation.getAndSet(null);
            if (confirmation != null) {
                confirmation.setVisible(false);
            }
            layout.show(views, "firmware");
        });
        if (tabs != null) {
            tabs.addChangeListener(event -> {
                if (tabs.getSelectedComponent() != tabContent) {
                    selector.cancelNoIniUpdateConfirmation();
                }
            });
        }
        return views;
    }

    private static Color color(String key, Color fallback) {
        Color value = UIManager.getColor(key);
        return value == null ? fallback : value;
    }

    private static final class CenteredCardView extends JPanel implements Scrollable {
        private final FirmwareUpdateCard card;

        private CenteredCardView(FirmwareUpdateCard card) {
            this.card = card;
            setOpaque(false);
            add(card);
        }

        @Override
        public void doLayout() {
            int available = getWidth();
            double fraction = available >= 1400 ? 0.50 : available >= 900 ? 0.75 : 1.0;
            int width = Math.max(0, Math.min(available - (fraction == 1.0 ? UIScale.scale(32) : 0),
                (int) Math.round(available * fraction)));
            int x = (available - width) / 2;
            for (int pass = 0; pass < 3; pass++) {
                card.setBounds(x, 16, width, card.getPreferredSize().height);
                layoutRecursively(card);
                if (card.getHeight() == card.getPreferredSize().height) {
                    break;
                }
            }
        }

        private static void layoutRecursively(Container container) {
            container.invalidate();
            if (container.getLayout() instanceof LayoutManager2) {
                ((LayoutManager2) container.getLayout()).invalidateLayout(container);
            }
            container.doLayout();
            for (Component component : container.getComponents()) {
                if (component instanceof Container) {
                    layoutRecursively((Container) component);
                }
            }
        }

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(1000, card.getPreferredSize().height + 32);
        }

        @Override
        public Dimension getPreferredScrollableViewportSize() {
            return getPreferredSize();
        }

        @Override
        public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
            return 16;
        }

        @Override
        public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
            return Math.max(16, visibleRect.height - 16);
        }

        @Override
        public boolean getScrollableTracksViewportWidth() {
            return true;
        }

        @Override
        public boolean getScrollableTracksViewportHeight() {
            return false;
        }
    }

    private static final class ResponsiveHeader extends JPanel {
        private final Component title;
        private final Component help;
        private boolean stacked;

        private ResponsiveHeader(Component title, Component help) {
            this.title = title;
            this.help = help;
            setLayout(null);
            setOpaque(false);
            add(title);
            add(help);
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension titleSize = title.getPreferredSize();
            Dimension helpSize = help.getPreferredSize();
            boolean wrap = getWidth() > 0 && getWidth() < titleSize.width + helpSize.width + UIScale.scale(12);
            return wrap
                ? new Dimension(Math.max(titleSize.width, helpSize.width),
                    titleSize.height + UIScale.scale(8) + helpSize.height)
                : new Dimension(titleSize.width + helpSize.width + UIScale.scale(12),
                    Math.max(titleSize.height, helpSize.height));
        }

        @Override
        public void doLayout() {
            Dimension titleSize = title.getPreferredSize();
            Dimension helpSize = help.getPreferredSize();
            boolean wrap = getWidth() < titleSize.width + helpSize.width + UIScale.scale(12);
            if (stacked != wrap) {
                stacked = wrap;
                if (getParent() != null) {
                    getParent().revalidate();
                }
            }
            if (wrap) {
                title.setBounds(0, 0, Math.min(getWidth(), titleSize.width), titleSize.height);
                help.setBounds(Math.max(0, getWidth() - helpSize.width),
                    titleSize.height + UIScale.scale(8), Math.min(getWidth(), helpSize.width), helpSize.height);
            } else {
                title.setBounds(0, 0, titleSize.width, getHeight());
                help.setBounds(getWidth() - helpSize.width, 0, helpSize.width, getHeight());
            }
        }
    }
}
