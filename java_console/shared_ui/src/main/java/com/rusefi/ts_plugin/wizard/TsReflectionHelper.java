package com.rusefi.ts_plugin.wizard;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

/**
 * Utilities for reflecting into TunerStudio Swing UI.
 */
final class TsReflectionHelper {

    private TsReflectionHelper() {
    }

    static Frame findTsMainFrame(long timeout, TimeUnit unit) throws InterruptedException {
        long deadline = System.currentTimeMillis() + unit.toMillis(timeout);
        while (System.currentTimeMillis() < deadline) {
            for (Frame f : JFrame.getFrames()) {
                try {
                    String title = f.getTitle();
                    if (title != null && title.contains(TsAccess.TS_TITLE_FRAGMENT)) {
                        return f;
                    }
                } catch (Throwable ignore) {
                }
            }
            Thread.sleep(250);
        }
        return null;
    }

    static List<AbstractButton> findTopLevelButtons(Frame frame) {
        List<AbstractButton> result = new ArrayList<>();
        visitComponents(frame, c -> {
            if (c instanceof AbstractButton) {
                String cn = c.getClass().getName();
                if (TsAccess.getTsTopLevelButtonClass().equals(cn)) {
                    result.add((AbstractButton) c);
                }
            }
        });
        return result;
    }

    static List<AbstractButton> findAlternativeButtons(Frame frame) {
        List<AbstractButton> result = new ArrayList<>();
        visitComponents(frame, c -> {
            if (c instanceof AbstractButton) {
                AbstractButton b = (AbstractButton) c;
                String text = b.getText();
                if ("Setup".equals(text) || "Fuel".equals(text)) {
                    result.add(b);
                }
            }
        });
        return result;
    }

    static JMenuItem findMenuItem(Frame frame, String text) {
        final List<JMenuItem> candidates = new ArrayList<>();
        visitComponents(frame, c -> {
            if (c instanceof JMenuItem) {
                JMenuItem mi = (JMenuItem) c;
                String cls = c.getClass().getName();
                // Prefer TS-specific popup menu class when available
                boolean isTsMenu = cls.endsWith(TsAccess.getTsPopupMainMenuClassSuffix());
                boolean textMatches = Objects.equals(mi.getText(), text);
                if (textMatches && (isTsMenu || candidates.isEmpty())) {
                    candidates.add(mi);
                }
            }
        });
        return candidates.isEmpty() ? null : candidates.get(0);
    }

    static JDialog waitForDialogWithTitle(String title, long timeout, TimeUnit unit) throws InterruptedException {
        long deadline = System.currentTimeMillis() + unit.toMillis(timeout);
        while (System.currentTimeMillis() < deadline) {
            for (Window w : Dialog.getWindows()) {
                if (w instanceof JDialog && w.isVisible()) {
                    JDialog d = (JDialog) w;
                    if (title.equals(d.getTitle())) {
                        return d;
                    }
                }
            }
            Thread.sleep(100);
        }
        return null;
    }

    static void bringToFront(JDialog dialog) {
        SwingUtilities.invokeLater(() -> {
            dialog.setAlwaysOnTop(true);
            dialog.toFront();
            dialog.requestFocus();
            dialog.setVisible(true);
            dialog.setAlwaysOnTop(false);
        });
    }

    private static void visitComponents(Component root, Consumer<Component> visitor) {
        visitor.accept(root);
        if (root instanceof Container) {
            for (Component c : ((Container) root).getComponents()) {
                visitComponents(c, visitor);
            }
        }
    }
}
