package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.rusefi.core.MessagesCentral;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Lets the ECU (typically a Lua script via print()) control the background color of all
 * Message panes: any message containing "set_bg_color=RRGGBB" (hex, optional leading '#',
 * or decimal "R,G,B") repaints every registered messages view. Color is reset back to
 * white when a new Lua script is written to the ECU, see
 * {@link com.rusefi.ui.lua.LuaScriptPanel}.
 *
 * @see MessagesView
 */
public class MessagesPaneBackgroundColor {
    private static final Logging log = getLogging(MessagesPaneBackgroundColor.class);

    public static final String SET_BG_COLOR_PREFIX = "set_bg_color=";
    public static final Color DEFAULT_COLOR = Color.white;

    private static final List<JTextPane> panes = new CopyOnWriteArrayList<>();
    private static volatile Color currentColor = DEFAULT_COLOR;

    static {
        // plain addListener, not addListenerAndReplay: we do not want a stale backlog message
        // to re-apply a color side effect, see MessagesCentral#history javadoc
        MessagesCentral.getInstance().addListener((clazz, message) -> onMessage(message));
    }

    private MessagesPaneBackgroundColor() {
    }

    /**
     * Registers one messages text pane; immediately applies the current color so panes
     * created after a set_bg_color message match the rest.
     */
    static void register(JTextPane pane) {
        panes.add(pane);
        pane.setBackground(currentColor);
    }

    /**
     * Invoked for every message from {@link MessagesCentral}, already on the AWT thread.
     */
    static void onMessage(String message) {
        int index = message.indexOf(SET_BG_COLOR_PREFIX);
        if (index < 0) {
            return;
        }
        String value = message.substring(index + SET_BG_COLOR_PREFIX.length()).trim();
        // take only the first token in case something follows the color on the same line
        int end = 0;
        while (end < value.length() && !Character.isWhitespace(value.charAt(end))) {
            end++;
        }
        value = value.substring(0, end);
        Color color = parseColor(value);
        if (color == null) {
            log.info("Ignoring invalid " + SET_BG_COLOR_PREFIX + value);
            return;
        }
        applyColor(color);
    }

    /**
     * Back to default white, e.g. when a new Lua script is written to the ECU.
     */
    public static void reset() {
        applyColor(DEFAULT_COLOR);
    }

    private static void applyColor(Color color) {
        currentColor = color;
        SwingUtilities.invokeLater(() -> {
            for (JTextPane pane : panes) {
                pane.setBackground(color);
            }
        });
    }

    /**
     * @param value "RRGGBB" hex (optional leading '#') or decimal "R,G,B"
     * @return parsed color or null if not a valid color
     */
    public static Color parseColor(String value) {
        if (value.startsWith("#")) {
            value = value.substring(1);
        }
        if (value.isEmpty()) {
            return null;
        }
        if (value.indexOf(',') >= 0) {
            String[] parts = value.split(",");
            if (parts.length != 3) {
                return null;
            }
            try {
                int r = Integer.parseInt(parts[0].trim());
                int g = Integer.parseInt(parts[1].trim());
                int b = Integer.parseInt(parts[2].trim());
                if (isOutOfRange(r) || isOutOfRange(g) || isOutOfRange(b)) {
                    return null;
                }
                return new Color(r, g, b);
            } catch (NumberFormatException e) {
                return null;
            }
        }
        if (!value.matches("[0-9a-fA-F]{6}")) {
            return null;
        }
        return new Color(Integer.parseInt(value, 16));
    }

    private static boolean isOutOfRange(int component) {
        return component < 0 || component > 255;
    }
}
