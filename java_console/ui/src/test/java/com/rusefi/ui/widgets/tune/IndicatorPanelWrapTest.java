package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;

class IndicatorPanelWrapTest {
    private static final int NARROW = 400;

    /** The Tuning strip stacks the chips in a BoxLayout; narrowing it must not cut chips off. */
    @Test
    void narrowStripWrapsEveryChipIntoView() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            List<IndicatorModel> indicators = new ArrayList<>();
            for (int i = 0; i < 30; i++) {
                indicators.add(new IndicatorModel("{ channel" + i + " }",
                    "Indicator " + i + " off", "Indicator " + i + " on", "white", "black", "red", "black"));
            }
            IndicatorPanel indicatorPanel = new IndicatorPanel(indicators, mock(IniFileModel.class), 0);
            JPanel strip = indicatorPanel.getPanel();
            JPanel parent = new JPanel();
            parent.setLayout(new BoxLayout(parent, BoxLayout.Y_AXIS));
            parent.add(strip);
            try {
                layOut(parent, strip, 10000);

                // the parent reserves the height the strip had at its old width
                layOut(parent, strip, NARROW);
                assertFalse(allChipsVisible(strip), "the first pass still has the wide strip's single row");

                // what the resize listener does once the strip has its new width
                indicatorPanel.fitRowsToWidth();
                // in a shown window that revalidate() invalidates the parent and its BoxLayout cache too
                parent.invalidate();
                layOut(parent, strip, NARROW);
                assertTrue(allChipsVisible(strip), "every chip lies inside the strip after wrapping");
            } finally {
                indicatorPanel.destroy();
            }
        });
    }

    /** One layout pass as BorderLayout.NORTH gives it: the full width and the preferred height. */
    private static void layOut(JPanel parent, JPanel strip, int width) {
        // validate() needs a native peer, so lay the undisplayed panels out directly
        parent.setSize(width, parent.getPreferredSize().height);
        parent.doLayout();
        strip.doLayout();
    }

    private static boolean allChipsVisible(JPanel strip) {
        for (Component chip : strip.getComponents()) {
            Rectangle bounds = chip.getBounds();
            if (bounds.x + bounds.width > strip.getWidth() || bounds.y + bounds.height > strip.getHeight()) {
                return false;
            }
        }
        return true;
    }
}
