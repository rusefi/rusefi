package com.rusefi.ui.logview;

import javax.swing.SwingUtilities;
import java.awt.AWTException;
import java.awt.Component;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Robot;

/**
 * Moves the mouse pointer, so a pan-drag on the log chart can go on past the edge of the plots or the screen
 * (issue #10669): the chart wraps the pointer to the opposite side and keeps panning.
 */
interface PointerWarp {
    /** Moves the pointer to {@code (x, y)} in {@code component}'s coordinates; false when it cannot. */
    boolean moveTo(Component component, int x, int y);

    /**
     * Uses {@link Robot}. Headless, without permission or before the component shows it reports failure; some
     * desktops (e.g. Wayland, WSLg) accept the call but leave the pointer, which the chart detects separately.
     */
    PointerWarp ROBOT = new PointerWarp() {
        private Robot robot;
        private boolean unavailable;

        @Override
        public boolean moveTo(Component component, int x, int y) {
            if (unavailable || GraphicsEnvironment.isHeadless() || !component.isShowing()) {
                return false;
            }
            try {
                if (robot == null) {
                    robot = new Robot();
                }
                Point point = new Point(x, y);
                SwingUtilities.convertPointToScreen(point, component);
                robot.mouseMove(point.x, point.y);
                return true;
            } catch (AWTException | SecurityException e) {
                unavailable = true;
                return false;
            }
        }
    };
}
