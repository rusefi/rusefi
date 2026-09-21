package com.rusefi.ts_plugin.knock;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Graphics2D;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.image.BufferedImage;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class KnockCanvasTest {
    @Test
    public void paintSixCylinderCanvasWiderThanHistory() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            KnockCanvas canvas = createCanvas(6, 800, 480);
            assertDoesNotThrow(() -> paint(canvas));
        });
    }

    @Test
    public void paintMouseAtBottomEdge() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            KnockCanvas canvas = createCanvas(1, 640, 480);
            canvas.setMousePosition(100, 480);
            assertDoesNotThrow(() -> paint(canvas));
        });
    }

    @Test
    public void mouseLookupStaysWithinHistoryAtEdgesAndAfterUpdates() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            for (int divider : new int[]{1, 6, 8}) {
                for (int width : new int[]{320, 800}) {
                    KnockCanvas canvas = createCanvas(divider, width, 480);
                    for (int update = 0; update < 2; update++) {
                        for (int x : new int[]{-width, 0, width / 2, width - 1, width, width * 2}) {
                            for (int y : new int[]{-480, 0, 479, 480, 960}) {
                                canvas.setMousePosition(x, y);
                                assertDoesNotThrow(() -> paint(canvas));
                                assertEquals(64, canvas.getCurrentMouseMagnitudes().length);
                            }
                        }
                        canvas.processValues(new float[64]);
                    }
                }
            }
        });
    }

    private static KnockCanvas createCanvas(int divider, int width, int height) {
        KnockCanvas canvas = new KnockCanvas(1, divider, false);
        JComponent component = canvas.getComponent();
        component.setSize(width, height);
        // Deliver the resize before painting, without waiting for the event queue.
        for (ComponentListener listener : component.getComponentListeners()) {
            listener.componentResized(new ComponentEvent(component, ComponentEvent.COMPONENT_RESIZED));
        }
        canvas.setFrequencyStart(1000);
        canvas.setFrequencyStep(200);
        return canvas;
    }

    private static void paint(KnockCanvas canvas) {
        JComponent component = canvas.getComponent();
        BufferedImage image = new BufferedImage(component.getWidth(), component.getHeight(), BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = image.createGraphics();
        try {
            component.paint(graphics);
        } finally {
            graphics.dispose();
        }
    }
}
