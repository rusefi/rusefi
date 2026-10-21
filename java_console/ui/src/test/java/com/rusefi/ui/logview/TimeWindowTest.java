package com.rusefi.ui.logview;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

class TimeWindowTest {
    private static final double EPSILON = 1e-9;

    @Test
    void startsShowingWholeLog() {
        TimeWindow window = new TimeWindow(10, 70, 0.1);
        assertEquals(10, window.start(), EPSILON);
        assertEquals(70, window.end(), EPSILON);
        assertTrue(window.isFull());
    }

    @Test
    void zoomKeepsAnchorAtSameScreenPosition() {
        TimeWindow window = new TimeWindow(0, 100, 0.1);
        window.zoom(25, 0.5);
        assertEquals(50, window.span(), EPSILON);
        // The anchor was a quarter of the way in, and still is.
        assertEquals(0.25, (25 - window.start()) / window.span(), EPSILON);
        assertFalse(window.isFull());
    }

    @Test
    void zoomInStopsAtMinimumSpan() {
        TimeWindow window = new TimeWindow(0, 100, 2);
        for (int i = 0; i < 100; i++) {
            window.zoom(50, 0.5);
        }
        assertEquals(2, window.span(), EPSILON);
        assertEquals(49, window.start(), EPSILON);
    }

    @Test
    void zoomOutStopsAtWholeLog() {
        TimeWindow window = new TimeWindow(0, 100, 1);
        window.zoom(90, 0.1);
        window.zoom(90, 1000);
        assertEquals(0, window.start(), EPSILON);
        assertEquals(100, window.end(), EPSILON);
    }

    @Test
    void zoomNearEdgeStaysInsideLog() {
        TimeWindow window = new TimeWindow(0, 100, 1);
        window.zoom(95, 0.2);
        window.zoom(2, 3);
        assertTrue(window.start() >= 0);
        assertTrue(window.end() <= 100 + EPSILON);
        assertEquals(60, window.span(), EPSILON);
    }

    @Test
    void panIsClampedToLogEdges() {
        TimeWindow window = new TimeWindow(0, 100, 1);
        window.show(40, 60);
        window.pan(15);
        assertEquals(55, window.start(), EPSILON);
        window.pan(1000);
        assertEquals(80, window.start(), EPSILON);
        assertEquals(100, window.end(), EPSILON);
        window.pan(-1000);
        assertEquals(0, window.start(), EPSILON);
        assertEquals(20, window.span(), EPSILON);
    }

    @Test
    void showAcceptsEitherOrderAndWidensTinyRanges() {
        TimeWindow window = new TimeWindow(0, 100, 4);
        window.show(30, 20);
        assertEquals(20, window.start(), EPSILON);
        assertEquals(30, window.end(), EPSILON);
        window.show(50, 51);
        assertEquals(48.5, window.start(), EPSILON);
        assertEquals(52.5, window.end(), EPSILON);
    }

    @Test
    void resetReturnsToWholeLog() {
        TimeWindow window = new TimeWindow(0, 100, 1);
        window.show(10, 20);
        window.reset();
        assertTrue(window.isFull());
    }

    @Test
    void singleInstantLogDoesNotBreak() {
        TimeWindow window = new TimeWindow(5, 5, 1);
        window.zoom(5, 0.5);
        window.pan(3);
        assertEquals(5, window.start(), EPSILON);
        assertTrue(Double.isFinite(window.span()));
    }
}
