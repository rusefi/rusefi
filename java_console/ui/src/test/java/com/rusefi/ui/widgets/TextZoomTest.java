package com.rusefi.ui.widgets;

import com.rusefi.core.preferences.storage.Node;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class TextZoomTest {
    private static final Font MONO = new Font(Font.MONOSPACED, Font.PLAIN, 12);

    @Test
    void textGrowsAndShrinksInStepsWithinLimitsAndTheSizeIsKept() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            Node config = new Node();
            JPanel tab = new JPanel();
            JTextArea log = text();
            JButton button = new JButton("Copy log");
            Font buttonFont = button.getFont();
            tab.add(log);
            tab.add(button);
            TextZoom zoom = TextZoom.install(tab, config, "device", log);

            zoom.zoomIn();
            assertEquals(12 * TextZoom.STEP, log.getFont().getSize2D(), 0.01);
            assertEquals(MONO.getFamily(), log.getFont().getFamily(), "the same font, only bigger");
            assertSame(buttonFont, button.getFont(), "only the text changes, not the buttons");
            assertEquals("110%", TextZoom.percent(zoom.getZoom()));
            zoom.zoomOut();
            assertEquals(1f, zoom.getZoom(), "back to exactly 100%");
            assertEquals(12f, log.getFont().getSize2D(), 0.01);

            for (int i = 0; i < 30; i++) {
                zoom.zoomIn();
            }
            assertEquals(TextZoom.MAX, zoom.getZoom(), "never larger than the limit");
            for (int i = 0; i < 30; i++) {
                zoom.zoomOut();
            }
            assertEquals(TextZoom.MIN, zoom.getZoom(), "never smaller than the limit");
            assertEquals(Float.toString(TextZoom.MIN), config.getProperty("device"), "kept for the next run");

            JTextArea next = text();
            TextZoom.install(new JPanel(), config, "device", next);
            assertEquals(12 * TextZoom.MIN, next.getFont().getSize2D(), 0.01, "restored at startup");

            zoom.reset();
            assertEquals(12f, log.getFont().getSize2D(), 0.01);
            config.setProperty("broken", "large");
            JTextArea other = text();
            TextZoom.install(new JPanel(), config, "broken", other);
            assertEquals(12f, other.getFont().getSize2D(), 0.01, "an unreadable setting means normal size");
        });
    }

    @Test
    void ctrlWheelResizesAndAPlainWheelStillScrolls() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JTextArea log = text();
            JScrollPane scroll = new JScrollPane(log);
            List<MouseWheelEvent> scrolled = new ArrayList<>();
            scroll.addMouseWheelListener(scrolled::add);
            TextZoom zoom = TextZoom.install(scroll, new Node(), "messages", log);

            log.dispatchEvent(wheel(log, -1, InputEvent.CTRL_DOWN_MASK));
            assertEquals(1.1f, zoom.getZoom(), 0.001, "wheel up with Ctrl: bigger");
            assertTrue(scrolled.isEmpty(), "and no scrolling");
            log.dispatchEvent(wheel(log, 1, InputEvent.CTRL_DOWN_MASK));
            assertEquals(1f, zoom.getZoom(), 0.001, "wheel down with Ctrl: smaller");

            log.dispatchEvent(wheel(log, 1, 0));
            assertEquals(1f, zoom.getZoom(), 0.001);
            assertEquals(1, scrolled.size(), "a plain wheel reaches the scroll pane");
        });
    }

    @Test
    void aPlainWheelOverZoomableTextScrollsTheRealScrollPane() throws Exception {
        org.junit.jupiter.api.Assumptions.assumeFalse(GraphicsEnvironment.isHeadless());
        JFrame[] frame = new JFrame[1];
        try {
            SwingUtilities.invokeAndWait(() -> {
                StringBuilder lines = new StringBuilder();
                for (int i = 0; i < 300; i++) {
                    lines.append("line ").append(i).append('\n');
                }
                JTextArea log = new JTextArea(lines.toString());
                log.setFont(MONO);
                JScrollPane scroll = new JScrollPane(log);
                TextZoom zoom = TextZoom.install(scroll, new Node(), "messages", log);
                frame[0] = new JFrame();
                frame[0].add(scroll);
                frame[0].setSize(400, 300);
                frame[0].setVisible(true);
                frame[0].validate();
                int before = scroll.getVerticalScrollBar().getValue();
                log.dispatchEvent(wheel(log, 3, 0));
                assertTrue(scroll.getVerticalScrollBar().getValue() > before, "the text scrolled down");
                assertEquals(1f, zoom.getZoom(), "and kept its size");
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                if (frame[0] != null) {
                    frame[0].dispose();
                }
            });
        }
    }

    @Test
    void ctrlPlusMinusAndZeroWorkWithFocusAnywhereInTheTab() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            JPanel tab = new JPanel();
            JTextArea log = text();
            tab.add(log);
            TextZoom zoom = TextZoom.install(tab, new Node(), "ai", log);
            press(tab, KeyEvent.VK_EQUALS, 0);
            press(tab, KeyEvent.VK_EQUALS, InputEvent.SHIFT_DOWN_MASK);
            press(tab, KeyEvent.VK_ADD, 0);
            assertEquals(TextZoom.STEP * TextZoom.STEP * TextZoom.STEP, zoom.getZoom(), 0.001, "Ctrl+= / Ctrl++ / numpad");
            press(tab, KeyEvent.VK_MINUS, 0);
            assertEquals(TextZoom.STEP * TextZoom.STEP, zoom.getZoom(), 0.001);
            press(tab, KeyEvent.VK_0, 0);
            assertEquals(1f, zoom.getZoom(), "Ctrl+0: normal size");
        });
    }

    @Test
    void theLuaEditorKeepsItsOwnCtrlNumpadKeysAndTakesTheOthers() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            RSyntaxTextArea editor = new RSyntaxTextArea();
            editor.setCodeFoldingEnabled(true);
            InputMap own = editor.getInputMap(JComponent.WHEN_FOCUSED);
            for (int key : new int[]{KeyEvent.VK_EQUALS, KeyEvent.VK_MINUS, KeyEvent.VK_0}) {
                assertNull(own.get(KeyStroke.getKeyStroke(key, InputEvent.CTRL_DOWN_MASK)),
                    KeyEvent.getKeyText(key) + " with Ctrl reaches the tab's text size keys");
            }
            assertNotNull(own.get(KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, InputEvent.CTRL_DOWN_MASK)),
                "Ctrl + numpad minus still folds code in the editor");
        });
    }

    @Test
    void theLuaEditorsColouredCodeGrowsToo() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            RSyntaxTextArea editor = new RSyntaxTextArea("local x = 1");
            editor.setSyntaxEditingStyle(org.fife.ui.rsyntaxtextarea.SyntaxConstants.SYNTAX_STYLE_LUA);
            editor.setFont(MONO);
            TextZoom zoom = TextZoom.install(new JPanel(), new Node(), "lua", editor);
            zoom.zoomIn();
            zoom.zoomIn();
            Font keyword = editor.getSyntaxScheme().getStyle(org.fife.ui.rsyntaxtextarea.Token.RESERVED_WORD).font;
            assertEquals(editor.getFont().getSize2D(), keyword.getSize2D(), 0.01,
                "keywords are drawn at the new size, not only the plain text");
        });
    }

    private static JTextArea text() {
        JTextArea text = new JTextArea("line\n");
        text.setFont(MONO);
        return text;
    }

    private static MouseWheelEvent wheel(Component target, int rotation, int modifiers) {
        return new MouseWheelEvent(target, MouseEvent.MOUSE_WHEEL, 0, modifiers, 5, 5, 0, false,
            MouseWheelEvent.WHEEL_UNIT_SCROLL, 3, rotation);
    }

    private static void press(JComponent tab, int key, int modifiers) {
        KeyStroke stroke = KeyStroke.getKeyStroke(key, InputEvent.CTRL_DOWN_MASK | modifiers);
        Object name = tab.getInputMap(JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT).get(stroke);
        assertNotNull(name, "bound: " + stroke);
        tab.getActionMap().get(name).actionPerformed(new ActionEvent(tab, ActionEvent.ACTION_PERFORMED, ""));
    }
}
