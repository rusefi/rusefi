package com.rusefi.ui.widgets;

import com.rusefi.ui.basic.UiHelper;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * [tag:console_theme] GaugeStyle's fallback colours, used without the console theme (tests, sandboxes),
 * are the light theme's Console.Gauge.* values, and the dark theme defines every gauge key too.
 */
public class GaugeStyleTest {
    @Test
    public void fallbacksMatchTheLightThemeAndDarkDefinesEveryKey() throws Exception {
        List<String> problems = new ArrayList<>();
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            for (boolean dark : new boolean[]{false, true}) {
                SwingUtilities.invokeAndWait(() -> {
                    UiHelper.configureLaf(dark);
                    for (Map.Entry<String, Integer> fallback : GaugeStyle.FALLBACKS.entrySet()) {
                        Color themed = UIManager.getColor("Console.Gauge." + fallback.getKey());
                        if (themed == null) {
                            problems.add((dark ? "dark" : "light") + " theme has no Console.Gauge." + fallback.getKey());
                        } else if (!dark && themed.getRGB() != fallback.getValue()) {
                            problems.add(String.format("Console.Gauge.%s: light theme #%08x, fallback #%08x",
                                fallback.getKey(), themed.getRGB(), fallback.getValue()));
                        }
                    }
                    UiHelper.unregisterThemeForUnitTest();
                });
            }
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                JFrame.setDefaultLookAndFeelDecorated(previousFrameDecorated);
                JDialog.setDefaultLookAndFeelDecorated(previousDialogDecorated);
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
        assertTrue(problems.isEmpty(), String.join("\n", problems));
    }
}
