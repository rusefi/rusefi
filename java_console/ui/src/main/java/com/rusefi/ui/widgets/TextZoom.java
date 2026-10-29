package com.rusefi.ui.widgets;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.tune.TuningStyle;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.fife.ui.rsyntaxtextarea.Style;
import org.fife.ui.rsyntaxtextarea.SyntaxScheme;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseWheelEvent;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Bigger or smaller text in a text-heavy tab (Lua Scripting, Messages, Device, AI Troubleshooting): Ctrl+wheel
 * over the text, or Ctrl + plus / minus with focus in the tab, Ctrl+0 back to normal. Only the text
 * components given change size; toolbars and buttons stay. Each tab keeps its own size between runs.
 * <p>
 * The wheel listener sits on the text itself, which would stop wheel events from reaching the scroll pane
 * around it, so a plain wheel is passed on to the parent to keep scrolling as before. The keys are bound on
 * the tab, so a key the focused component uses itself keeps its own meaning (Ctrl + numpad plus / minus
 * fold and unfold code in the Lua editor). The Lua editor draws each kind of word in a style with its own
 * font, which changing the editor's font leaves as it was, so those fonts are resized as well.
 */
public final class TextZoom {
    static final float STEP = 1.1f;
    static final float MIN = 0.7f;
    static final float MAX = 2.5f;
    private static final int BUBBLE_MS = 900;

    private final JComponent scope;
    private final Node config;
    private final String key;
    /** Each text component and its font at normal size. */
    private final Map<JComponent, Font> normal = new LinkedHashMap<>();
    /** A code editor's style fonts at normal size, by style index; null where a style has none. */
    private final Map<RSyntaxTextArea, Font[]> normalStyles = new LinkedHashMap<>();
    private final Timer hideBubble;
    private float zoom;
    private double wheelTurn;
    private Popup bubble;

    private TextZoom(JComponent scope, Node config, String key) {
        this.scope = scope;
        this.config = config;
        this.key = key;
        hideBubble = new Timer(BUBBLE_MS, e -> hideBubble());
        hideBubble.setRepeats(false);
    }

    /**
     * @param scope the tab; its keys work while focus is anywhere in it
     * @param config where each tab's size is kept, under {@code key}
     * @param texts the components whose font changes, at their normal font now
     */
    public static TextZoom install(JComponent scope, Node config, String key, JComponent... texts) {
        TextZoom zoom = new TextZoom(scope, config, key);
        for (JComponent text : texts) {
            zoom.normal.put(text, text.getFont());
            if (text instanceof RSyntaxTextArea) {
                SyntaxScheme scheme = ((RSyntaxTextArea) text).getSyntaxScheme();
                Font[] fonts = new Font[scheme.getStyleCount()];
                for (int i = 0; i < fonts.length; i++) {
                    Style style = scheme.getStyle(i);
                    fonts[i] = style == null ? null : style.font;
                }
                zoom.normalStyles.put((RSyntaxTextArea) text, fonts);
            }
            text.addMouseWheelListener(zoom::wheel);
        }
        zoom.bindKeys();
        zoom.zoom = clamp(parse(config.getProperty(key, null)));
        zoom.apply();
        return zoom;
    }

    public float getZoom() {
        return zoom;
    }

    public void zoomIn() {
        setZoom(zoom * STEP);
    }

    public void zoomOut() {
        setZoom(zoom / STEP);
    }

    public void reset() {
        setZoom(1);
    }

    private void setZoom(float requested) {
        float next = clamp(requested);
        // repeated steps land on 100% again instead of 99.9%
        if (Math.abs(next - 1) < 0.03f) {
            next = 1;
        }
        if (next == zoom) {
            showBubble();
            return;
        }
        zoom = next;
        config.setProperty(key, Float.toString(zoom));
        apply();
        showBubble();
    }

    private void apply() {
        for (Map.Entry<JComponent, Font> entry : normal.entrySet()) {
            Font font = entry.getValue();
            if (font == null) {
                continue;
            }
            JComponent text = entry.getKey();
            text.setFont(font.deriveFont(font.getSize2D() * zoom));
            Font[] styles = normalStyles.get(text);
            if (styles != null) {
                RSyntaxTextArea editor = (RSyntaxTextArea) text;
                SyntaxScheme scheme = editor.getSyntaxScheme();
                for (int i = 0; i < styles.length && i < scheme.getStyleCount(); i++) {
                    Style style = scheme.getStyle(i);
                    if (style != null && styles[i] != null) {
                        style.font = styles[i].deriveFont(styles[i].getSize2D() * zoom);
                    }
                }
                // re-measures the lines for the new fonts
                editor.setSyntaxScheme(scheme);
            }
            text.revalidate();
            text.repaint();
        }
    }

    private void wheel(MouseWheelEvent event) {
        if (!event.isControlDown()) {
            // the listener stops the event here; hand it to the nearest wheel handler around the
            // text, normally its scroll pane, so scrolling works as before
            Component source = event.getComponent();
            for (Container parent = source.getParent(); parent != null; parent = parent.getParent()) {
                if (parent.getMouseWheelListeners().length > 0) {
                    parent.dispatchEvent(SwingUtilities.convertMouseEvent(source, event, parent));
                    return;
                }
            }
            return;
        }
        event.consume();
        // trackpads send small fractions of a notch
        wheelTurn += event.getPreciseWheelRotation();
        while (wheelTurn <= -1) {
            wheelTurn += 1;
            zoomIn();
        }
        while (wheelTurn >= 1) {
            wheelTurn -= 1;
            zoomOut();
        }
    }

    private void bindKeys() {
        InputMap keys = scope.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        int ctrl = InputEvent.CTRL_DOWN_MASK;
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, ctrl), "textZoomIn");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, ctrl | InputEvent.SHIFT_DOWN_MASK), "textZoomIn");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_PLUS, ctrl), "textZoomIn");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_ADD, ctrl), "textZoomIn");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, ctrl), "textZoomOut");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, ctrl), "textZoomOut");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_0, ctrl), "textZoomReset");
        keys.put(KeyStroke.getKeyStroke(KeyEvent.VK_NUMPAD0, ctrl), "textZoomReset");
        scope.getActionMap().put("textZoomIn", action(this::zoomIn));
        scope.getActionMap().put("textZoomOut", action(this::zoomOut));
        scope.getActionMap().put("textZoomReset", action(this::reset));
    }

    private static Action action(Runnable run) {
        return new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                run.run();
            }
        };
    }

    /** "120%" at the top right of the first visible text for a moment, so the change is noticed. */
    private void showBubble() {
        JComponent anchor = null;
        for (JComponent text : normal.keySet()) {
            if (text.isShowing()) {
                anchor = text;
                break;
            }
        }
        if (anchor == null) {
            return;
        }
        hideBubble();
        JLabel label = new JLabel(percent(zoom));
        label.setFont(label.getFont().deriveFont(Font.BOLD));
        JPanel card = new JPanel(new BorderLayout());
        TuningStyle.styleCard(card, 4, 10);
        card.add(label);
        Rectangle visible = anchor.getVisibleRect();
        Point at = new Point(visible.x + visible.width - card.getPreferredSize().width - 12, visible.y + 10);
        SwingUtilities.convertPointToScreen(at, anchor);
        bubble = PopupFactory.getSharedInstance().getPopup(anchor, card, at.x, at.y);
        bubble.show();
        hideBubble.restart();
    }

    private void hideBubble() {
        if (bubble != null) {
            bubble.hide();
            bubble = null;
        }
    }

    static String percent(float zoom) {
        return Math.round(zoom * 100) + "%";
    }

    private static float parse(String saved) {
        try {
            return saved == null ? 1 : Float.parseFloat(saved);
        } catch (NumberFormatException e) {
            return 1;
        }
    }

    private static float clamp(float value) {
        return Float.isFinite(value) ? Math.max(MIN, Math.min(MAX, value)) : 1;
    }
}
