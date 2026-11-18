package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import javax.swing.JTable;
import javax.swing.SwingUtilities;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * A click on a free pin's "Free · Assign…" Tune use opens the assign picker (#10627 review). AWT sends no
 * MOUSE_CLICKED when the pointer moves a pixel or two between press and release, so the test sends the events a
 * real hand produces; the page's table listeners are called directly because tests run headless (no displayed
 * table).
 */
class PinoutTuneUseClickTest {
    @Test
    void aClickThatMovesSlightlyStillOpensThePicker() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<PinoutPin> assigned = new ArrayList<>();
            PinoutConnectorPage page = page(assigned);
            Point at = tuneUseCell(page, "A3");

            // a still click: press, release, then the click AWT adds
            press(page.table, at, 1);
            release(page.table, at, 1);
            clicked(page.table, at, 1);
            assertEquals(1, assigned.size(), "a still click opens the picker, once");

            // the same click with the pointer moved 2 px: AWT sends no MOUSE_CLICKED
            Point moved = new Point(at.x + 2, at.y + 1);
            press(page.table, at, 1);
            release(page.table, moved, 1);
            assertEquals(2, assigned.size(), "a click that moved slightly opens the picker");
            assertEquals("A3", assigned.get(1).name);

            // a drag across the table is not a click
            press(page.table, at, 1);
            release(page.table, new Point(at.x + 40, at.y), 1);
            assertEquals(2, assigned.size(), "a drag does not open the picker");

            // the second click of a double-click does not open it again
            press(page.table, at, 1);
            release(page.table, at, 1);
            press(page.table, at, 2);
            release(page.table, at, 2);
            assertEquals(3, assigned.size(), "a double-click opens the picker once");
        });
    }

    private static PinoutConnectorPage page(List<PinoutPin> assigned) {
        PinoutPin a1 = new PinoutPin("A1", "Analog input 1", "av", "analog_inputs", "A1 AV1", "blue");
        a1.tuneUse = "Fan output";
        PinoutPin a3 = new PinoutPin("A3", "Analog input 3", "av", "", "A3 AV3", "");
        PinoutConnectorPage page = new PinoutConnectorPage("Connector A", null, new ArrayList<>(Arrays.asList(a1, a3)),
                PinoutDrawing.ColorMode.TYPE, pin -> null, assignment -> { });
        page.setAssigner(assigned::add);
        page.table.setSize(900, 200);
        page.table.doLayout();
        return page;
    }

    private static Point tuneUseCell(PinoutConnectorPage page, String pinName) {
        JTable table = page.table;
        int row = table.convertRowIndexToView(page.pins.indexOf(page.find(pinName)));
        int column = table.convertColumnIndexToView(PinoutConnectorPage.TUNE_USE_COLUMN);
        Rectangle cell = table.getCellRect(row, column, false);
        return new Point(cell.x + cell.width / 2, cell.y + cell.height / 2);
    }

    private static void press(JTable table, Point at, int clickCount) {
        send(table, MouseEvent.MOUSE_PRESSED, at, clickCount);
    }

    private static void release(JTable table, Point at, int clickCount) {
        send(table, MouseEvent.MOUSE_RELEASED, at, clickCount);
    }

    private static void clicked(JTable table, Point at, int clickCount) {
        send(table, MouseEvent.MOUSE_CLICKED, at, clickCount);
    }

    private static void send(JTable table, int id, Point at, int clickCount) {
        MouseEvent event = new MouseEvent(table, id, System.currentTimeMillis(),
                id == MouseEvent.MOUSE_RELEASED || id == MouseEvent.MOUSE_CLICKED ? 0 : MouseEvent.BUTTON1_DOWN_MASK,
                at.x, at.y, clickCount, false, MouseEvent.BUTTON1);
        for (MouseListener listener : table.getMouseListeners()) {
            // the page's own listeners only: the look and feel's selection handler throws headless
            if (!listener.getClass().getName().startsWith(PinoutConnectorPage.class.getName())) {
                continue;
            }
            switch (id) {
                case MouseEvent.MOUSE_PRESSED:
                    listener.mousePressed(event);
                    break;
                case MouseEvent.MOUSE_RELEASED:
                    listener.mouseReleased(event);
                    break;
                default:
                    listener.mouseClicked(event);
            }
        }
    }
}
