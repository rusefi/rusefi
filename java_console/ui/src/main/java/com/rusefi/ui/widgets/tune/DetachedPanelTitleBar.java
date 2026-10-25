package com.rusefi.ui.widgets.tune;

import com.formdev.flatlaf.FlatLaf;
import com.formdev.flatlaf.icons.FlatWindowAbstractIcon;
import com.formdev.flatlaf.ui.FlatNativeWindowBorder;
import com.formdev.flatlaf.ui.FlatRootPaneUI;
import com.formdev.flatlaf.ui.FlatTitlePane;

import javax.swing.*;
import java.awt.*;
import java.util.function.Consumer;

/** Adds a panel-layout action beside Close while retaining FlatLaf's dragging and resizing. */
final class DetachedPanelTitleBar {
    private DetachedPanelTitleBar() {
    }

    static Consumer<Boolean> install(JDialog dialog, Runnable split, boolean canSplit) {
        if (UIManager.getLookAndFeel() instanceof FlatLaf) {
            // With a native window border (Windows 10+), FlatLaf only resizes decorated dialogs:
            // it enables the border and PLAIN_DIALOG decorations itself once the dialog is
            // displayable. An undecorated dialog would get neither that border nor FlatLaf's
            // Swing resizer, leaving the window a fixed size.
            if (!FlatNativeWindowBorder.isSupported()) {
                dialog.setUndecorated(true);
                dialog.getRootPane().setWindowDecorationStyle(JRootPane.PLAIN_DIALOG);
            }
            SplitRootPaneUI ui = new SplitRootPaneUI(split, canSplit);
            dialog.getRootPane().setUI(ui);
            return ui::setCanSplit;
        }
        // Standalone Swing callers may use native decorations, whose title bars cannot host Swing controls.
        JMenuBar menu = new JMenuBar();
        JButton button = new JButton("Split panel");
        button.setToolTipText("Split into separate panels");
        button.addActionListener(event -> split.run());
        menu.add(Box.createHorizontalGlue());
        menu.add(button);
        menu.setVisible(canSplit);
        dialog.setJMenuBar(menu);
        return menu::setVisible;
    }

    private static final class SplitRootPaneUI extends FlatRootPaneUI {
        private final Runnable split;
        private boolean canSplit;

        SplitRootPaneUI(Runnable split, boolean canSplit) {
            this.split = split;
            this.canSplit = canSplit;
        }

        @Override
        protected FlatTitlePane createTitlePane() {
            return new SplitTitlePane(rootPane, split, canSplit);
        }

        void setCanSplit(boolean canSplit) {
            this.canSplit = canSplit;
            if (titlePane instanceof SplitTitlePane) {
                ((SplitTitlePane) titlePane).splitButton.setVisible(canSplit);
                titlePane.revalidate();
                rootPane.revalidate();
            }
        }
    }

    private static final class SplitTitlePane extends FlatTitlePane {
        private final JButton splitButton;

        SplitTitlePane(JRootPane root, Runnable split, boolean canSplit) {
            super(root);
            splitButton = createButton("TitlePane.maximizeIcon", "Split panel", event -> split.run());
            splitButton.setIcon(new SplitIcon(windowStyle));
            splitButton.setToolTipText("Split into separate panels");
            splitButton.setForeground(closeButton.getForeground());
            splitButton.setBackground(closeButton.getBackground());
            splitButton.setVisible(canSplit);
            buttonPanel.add(splitButton, buttonPanel.getComponentCount() - 1);
        }

        @Override
        protected void activeChanged(boolean active) {
            super.activeChanged(active);
            if (splitButton != null) {
                splitButton.setForeground(closeButton.getForeground());
                splitButton.setBackground(closeButton.getBackground());
            }
        }
    }

    private static final class SplitIcon extends FlatWindowAbstractIcon {
        SplitIcon(String windowStyle) {
            super(windowStyle);
        }

        @Override
        protected void paintIconAt1x(Graphics2D graphics, int x, int y, int width, int height, double scale) {
            int paneWidth = Math.max(4, (int) (5 * scale));
            int paneHeight = Math.max(7, (int) (10 * scale));
            int gap = Math.max(2, (int) (3 * scale));
            int left = x + (width - 2 * paneWidth - gap) / 2;
            int top = y + (height - paneHeight) / 2;
            graphics.setStroke(new BasicStroke((float) Math.max(1, scale)));
            graphics.drawRect(left, top, paneWidth, paneHeight);
            graphics.drawRect(left + paneWidth + gap, top, paneWidth, paneHeight);
        }
    }
}
