package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.function.Supplier;

/**
 * Shows a card beside a component while the mouse rests on it, e.g. where a Tuning pin is on its
 * connector. Unlike a tooltip it stays until the mouse leaves (the console keeps Swing's short tooltip
 * timeout), and it replaces the component's tooltip meanwhile, since the card says the same and more.
 * Leaving, clicking or hiding the component closes it.
 */
final class HoverPreview {
    /** Shorter than the tooltip delay, so the plain tooltip does not show first. */
    static final int DELAY_MS = 400;
    private static final int GAP = 8;

    private final JComponent owner;
    private final Supplier<JComponent> content;
    private final Timer timer;
    private Popup popup;
    private String hiddenToolTip;

    private HoverPreview(JComponent owner, Supplier<JComponent> content) {
        this.owner = owner;
        this.content = content;
        timer = new Timer(DELAY_MS, e -> show());
        timer.setRepeats(false);
    }

    /** @param content builds the card for the current state, or returns null for no card (the tooltip stays) */
    static HoverPreview install(JComponent owner, Supplier<JComponent> content) {
        HoverPreview preview = new HoverPreview(owner, content);
        owner.putClientProperty(HoverPreview.class, preview);
        owner.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                preview.timer.restart();
            }

            @Override
            public void mouseExited(MouseEvent e) {
                preview.hide();
            }

            @Override
            public void mousePressed(MouseEvent e) {
                preview.hide();
            }
        });
        owner.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && !owner.isShowing()) {
                preview.hide();
            }
        });
        return preview;
    }

    /** The preview installed on {@code owner}, or null. */
    static HoverPreview of(JComponent owner) {
        return (HoverPreview) owner.getClientProperty(HoverPreview.class);
    }

    /** The card for the current state, without showing it; null for none. */
    JComponent createCard() {
        return content.get();
    }

    boolean isShowing() {
        return popup != null;
    }

    /** Shows the card now; false when there is none to show. Package-private so tests skip the delay. */
    boolean show() {
        timer.stop();
        if (popup != null || !owner.isShowing()) {
            return popup != null;
        }
        JComponent card = createCard();
        if (card == null) {
            return false;
        }
        Point location = place(owner, card.getPreferredSize());
        hiddenToolTip = owner.getToolTipText();
        owner.setToolTipText(null);
        popup = PopupFactory.getSharedInstance().getPopup(owner, card, location.x, location.y);
        popup.show();
        return true;
    }

    void hide() {
        timer.stop();
        if (popup == null) {
            return;
        }
        popup.hide();
        popup = null;
        owner.setToolTipText(hiddenToolTip);
        hiddenToolTip = null;
    }

    /** Right of the component, or left when that leaves the screen; top-aligned, kept on screen. */
    static Point place(JComponent owner, Dimension size) {
        Point at = owner.getLocationOnScreen();
        GraphicsConfiguration gc = owner.getGraphicsConfiguration();
        Rectangle screen = gc.getBounds();
        Insets insets = Toolkit.getDefaultToolkit().getScreenInsets(gc);
        screen = new Rectangle(screen.x + insets.left, screen.y + insets.top,
            screen.width - insets.left - insets.right, screen.height - insets.top - insets.bottom);
        return place(new Rectangle(at, owner.getSize()), size, screen);
    }

    static Point place(Rectangle owner, Dimension size, Rectangle screen) {
        int x = owner.x + owner.width + GAP;
        if (x + size.width > screen.x + screen.width) {
            x = Math.max(screen.x, owner.x - GAP - size.width);
        }
        int y = Math.max(screen.y, Math.min(owner.y, screen.y + screen.height - size.height));
        return new Point(x, y);
    }
}
