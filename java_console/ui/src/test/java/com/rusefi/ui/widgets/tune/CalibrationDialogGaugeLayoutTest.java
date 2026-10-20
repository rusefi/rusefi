package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.SwingUtil;
import com.rusefi.ui.widgets.CircularGauge;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

/** Dialog gauges use the space a wide window offers, as in the CAN O2 sensors dialog. */
class CalibrationDialogGaugeLayoutTest {
    // Same shape as egoSettings in secondary_panels.ini: two border-layout status panels with
    // gauges on one side, a plain settings column between them, all in an xAxis dialog.
    private static final String INI = "[Constants]\npage = 1\n"
        + "[GaugeConfigurations]\ngaugeCategory = O2\n"
        + gauges(0) + gauges(1)
        + "[UserDefined]\n"
        + status(0, "West", "East") + status(1, "East", "West")
        + "dialog = middle, \"Middle\"\n"
        + "\tfield = \"Support for AEM or RusEFI CAN UEGO\"\n"
        + "dialog = ego, \"\", xAxis\n"
        + "\tpanel = status0\n\tpanel = middle\n\tpanel = status1\n";

    private static String gauges(int side) {
        StringBuilder text = new StringBuilder();
        for (int i = 0; i < 4; i++) {
            text.append("g").append(side).append(i).append(" = c").append(side).append(i)
                .append(", \"G\", \"V\", 0, 1, 0, 0, 1, 1, 2, 2\n");
        }
        return text.toString();
    }

    private static String status(int side, String gaugePlacement, String otherPlacement) {
        return "dialog = gauges" + side + "\n"
            + "\tgauge = g" + side + "0\n\tgauge = g" + side + "1\n\tgauge = g" + side + "2\n\tgauge = g" + side + "3\n"
            + "dialog = config" + side + "\n\tfield = \"UEGO type\"\n"
            + "dialog = side" + side + "\n\tfield = \"AFR: Invalid\"\n"
            + "dialog = status" + side + ", \"CAN UEGO " + (side + 1) + "\", border\n"
            + "\tpanel = config" + side + ", North\n"
            + "\tpanel = gauges" + side + ", " + gaugePlacement + "\n"
            + "\tpanel = side" + side + ", " + otherPlacement + "\n";
    }

    @Test
    void gaugesGrowIntoAWideWindowAndShrinkBackInANarrowOne() throws Throwable {
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(INI.getBytes(StandardCharsets.US_ASCII))), "ego.ini", mock(IniFileMetaInfo.class));
        SwingUtilities.invokeAndWait(() -> {
            CalibrationDialogWidget widget = new CalibrationDialogWidget(new UIContext());
            try {
                widget.update(ini.getDialogs().get("ego"), ini, null);
                JScrollPane scroll = new JScrollPane(widget.getViewportPane());
                List<CircularGauge> gauges = new ArrayList<>();
                collect(widget.getContentPane(), CircularGauge.class, gauges);
                assertEquals(8, gauges.size());

                // Text widths depend on the platform fonts (CI runs Linux), so only the 300 px
                // cap is an exact size: a very wide window reaches it with any fonts.
                layout(scroll, 2400, 2000);
                Component middle = find(widget.getContentPane(), "Middle");
                assertNotNull(middle);
                for (CircularGauge gauge : gauges) {
                    assertEquals(gauge.getWidth(), gauge.getHeight(), "Gauge cells stay square");
                    assertEquals(300, gauge.getWidth(), "Very wide window: gauges reach the cap");
                }
                assertEquals(middle.getPreferredSize().width, middle.getWidth(),
                    "The settings column keeps its width; spare width goes to the gauges");

                layout(scroll, 1600, 2000);
                for (CircularGauge gauge : gauges) {
                    assertTrue(gauge.getWidth() > 150 && gauge.getWidth() <= 300,
                        "Wide window: gauges grow from 150 px, got " + gauge.getWidth());
                }

                layout(scroll, 900, 2000);
                for (CircularGauge gauge : gauges) {
                    assertTrue(gauge.getWidth() >= 100 && gauge.getWidth() <= 150,
                        "Narrow window: back to the base size, got " + gauge.getWidth());
                }
            } finally {
                widget.destroy();
            }
        });
    }

    /** Lays out twice: the gauge grid's height follows the width of the first pass. */
    private static void layout(JScrollPane scroll, int width, int height) {
        scroll.setSize(width, height);
        // Off screen, validate() does nothing, so lay out top-down by hand.
        for (int pass = 0; pass < 3; pass++) {
            SwingUtil.invalidateTree(scroll);
            doLayoutTree(scroll);
        }
    }

    private static void doLayoutTree(Container container) {
        container.doLayout();
        for (Component component : container.getComponents()) {
            if (component instanceof Container) {
                doLayoutTree((Container) component);
            }
        }
    }

    private static <T> void collect(Container container, Class<T> type, List<T> found) {
        for (Component component : container.getComponents()) {
            if (type.isInstance(component)) {
                found.add(type.cast(component));
            }
            if (component instanceof Container) {
                collect((Container) component, type, found);
            }
        }
    }

    private static Component find(Container container, String name) {
        for (Component component : container.getComponents()) {
            if (name.equals(component.getName())) {
                return component;
            }
            if (component instanceof Container) {
                Component found = find((Container) component, name);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }
}
