package com.rusefi.ui.widgets;

import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniValue;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;

class GaugeScaleTest {
    private static GaugeModel model(double min, double max, double danger, int valueDecimals, int labelDecimals) {
        return new GaugeModel("ScaleTestGauge", "scaleTestChannel",
            IniValue.ofString("Air/fuel ratio"), IniValue.ofString("AFR"),
            IniValue.ofNumeric(min), IniValue.ofNumeric(max),
            IniValue.ofNumeric(min), IniValue.ofNumeric(min),
            IniValue.ofNumeric(danger), IniValue.ofNumeric(danger),
            IniValue.ofNumeric(valueDecimals), IniValue.ofNumeric(labelDecimals));
    }

    private static GaugeScale scale(GaugeModel model, GaugeScaleOverrides overrides) {
        return GaugeScale.resolve(model, null, null, overrides);
    }

    @Test
    void withoutOverridesTheIniDefinitionIsUsed() {
        GaugeScale scale = scale(model(10, 20, 18, 2, 1), GaugeScaleOverrides.NONE);
        assertEquals(10, scale.getMin());
        assertEquals(20, scale.getMax());
        assertEquals(18, scale.getHighDanger());
        assertEquals(2, scale.getValueDecimals());
        assertEquals(1, scale.getLabelDecimals());
        // model() sets both low limits to min and the high warning to the danger value.
        assertEquals(10, scale.getLowDanger());
        assertEquals(10, scale.getLowWarning());
        assertEquals(18, scale.getHighWarning());
    }

    @Test
    void eachOverrideReplacesOnlyItsOwnSetting() {
        GaugeModel model = model(10, 20, 18, 2, 1);
        GaugeScale min = scale(model, new GaugeScaleOverrides(5.0, null, null, null));
        assertEquals(5, min.getMin());
        assertEquals(20, min.getMax());
        assertEquals(18, min.getHighDanger());

        GaugeScale all = scale(model, new GaugeScaleOverrides(8.0, 22.0, 16.5, 0));
        assertEquals(8, all.getMin());
        assertEquals(22, all.getMax());
        assertEquals(16.5, all.getHighDanger());
        assertEquals(0, all.getValueDecimals());
        assertEquals(1, all.getLabelDecimals(), "Scale labels keep the .ini decimals");
        assertEquals(10, all.getLowDanger(), "Overrides keep the .ini warning and danger limits");
        assertEquals(18, all.getHighWarning(), "The round gauge clamps it below the overridden danger");
    }

    @Test
    void aRangeTheOverridesBreakFallsBackLikeABrokenIniRange() {
        // A saved min above a later, lower .ini max: the gauge still draws a usable range.
        GaugeScale scale = scale(model(0, 100, 80, 0, 0), new GaugeScaleOverrides(150.0, null, null, null));
        assertEquals(150, scale.getMin());
        assertEquals(160, scale.getMax());
    }

    @Test
    void theRoundGaugeRoundsToTheIniOrCustomDecimals() {
        GaugeModel model = model(0, 20, 18, 2, 0);
        CircularGauge ini = new CircularGauge(scale(model, GaugeScaleOverrides.NONE), model);
        ini.setValue(14.37);
        assertEquals("14.37", ini.getDisplayText(), "The .ini decimals");

        CircularGauge custom = new CircularGauge(scale(model, new GaugeScaleOverrides(null, null, null, 1)), model);
        custom.setValue(14.37);
        assertEquals("14.4", custom.getDisplayText());
        custom = new CircularGauge(scale(model, new GaugeScaleOverrides(null, null, null, 2)), model);
        custom.setValue(-0.006);
        assertEquals("-0.01", custom.getDisplayText(), "Ties and negatives round like the dashboard");

        GaugeModel rpm = model(0, 8000, 7000, 0, 0);
        CircularGauge whole = new CircularGauge(scale(rpm, new GaugeScaleOverrides(null, null, null, 0)), rpm);
        whole.setValue(3203);
        assertEquals("3203", whole.getDisplayText(), "Not the 4 RPM step of the .ini scale");
        whole = new CircularGauge(scale(rpm, new GaugeScaleOverrides(5.0, null, null, 0)), rpm);
        whole.setValue(3203);
        assertEquals("3203", whole.getDisplayText(), "Counted from zero, not from a custom min");
    }

    @Test
    void theRoundGaugeKeepsTheIniWarningZonesWithOverrides() {
        // afr1Gauge from gauge_declarations.ini
        GaugeModel afr = new GaugeModel("afr1Gauge", "AFRValue",
            IniValue.ofString("Air/Fuel Ratio"), IniValue.ofString(""),
            IniValue.ofNumeric(10), IniValue.ofNumeric(19.4),
            IniValue.ofNumeric(12), IniValue.ofNumeric(13),
            IniValue.ofNumeric(15), IniValue.ofNumeric(16),
            IniValue.ofNumeric(2), IniValue.ofNumeric(2));
        CircularGauge gauge = new CircularGauge(scale(afr, new GaugeScaleOverrides(null, 18.0, 15.5, null)), afr);
        for (Object[] sample : new Object[][] {
            {11.0, CircularGauge.Zone.DANGER},
            {12.5, CircularGauge.Zone.WARNING},
            {14.0, CircularGauge.Zone.NORMAL},
            {15.2, CircularGauge.Zone.WARNING},
            {15.6, CircularGauge.Zone.DANGER},
        }) {
            gauge.setValue((Double) sample[0]);
            assertEquals(sample[1], gauge.getZone(), "AFR " + sample[0]);
        }

        // A user danger below the .ini high warning leaves no high warning band.
        gauge = new CircularGauge(scale(afr, new GaugeScaleOverrides(null, null, 14.5, null)), afr);
        gauge.setValue(14.4);
        assertEquals(CircularGauge.Zone.NORMAL, gauge.getZone());
        gauge.setValue(14.6);
        assertEquals(CircularGauge.Zone.DANGER, gauge.getZone());
    }

    @Test
    void cardGaugesUseTheOverriddenRangeAndDanger() {
        GaugeModel model = model(0, 100, 90, 0, 0);
        DigitalGauge gauge = new DigitalGauge(scale(model, new GaugeScaleOverrides(null, 50.0, 40.0, 1)), model);
        assertEquals(0.5, gauge.fraction(25));
        assertTrue(gauge.isDanger(40));
        assertFalse(gauge.isDanger(39.9));
        assertEquals("12.3", gauge.text(12.34));
        assertEquals("50", gauge.label(50));

        BarGauge bar = new BarGauge(scale(model, new GaugeScaleOverrides(null, null, 500.0, null)), model, false);
        assertEquals(1, bar.dangerFraction(), "A danger value past max leaves no danger zone");
    }

    @Test
    void overridesRoundTripThroughTheSlotNode() {
        Node node = new Node();
        node.setProperty("gauge", "ScaleTestGauge");
        GaugeScaleOverrides overrides = new GaugeScaleOverrides(-40.0, 150.5, 120.0, 3);
        overrides.writeTo(node);
        assertEquals(overrides, GaugeScaleOverrides.read(node));

        new GaugeScaleOverrides(null, 10.0, null, null).writeTo(node);
        assertEquals(new GaugeScaleOverrides(null, 10.0, null, null), GaugeScaleOverrides.read(node),
            "Writing replaces the previous settings");
        GaugeScaleOverrides.clear(node);
        assertTrue(GaugeScaleOverrides.read(node).isEmpty());
        assertEquals("ScaleTestGauge", node.getProperty("gauge"), "Clearing keeps the other slot settings");

        node.setProperty(GaugeScaleOverrides.MIN, "abc");
        node.setProperty(GaugeScaleOverrides.MAX, "Infinity");
        node.setProperty(GaugeScaleOverrides.DECIMALS, "7");
        assertTrue(GaugeScaleOverrides.read(node).isEmpty(), "Unreadable settings keep the .ini values");
    }

    @Test
    void parsingTheDialogEntry() {
        GaugeScale ini = GaugeScale.of(0, 100, 80, 0, 0);
        GaugeScaleOverrides.Parsed empty = GaugeScaleOverrides.parse(" ", "", "", null, ini);
        assertEquals(GaugeScaleOverrides.NONE, empty.overrides);

        GaugeScaleOverrides.Parsed all = GaugeScaleOverrides.parse("-10", " 50.5 ", "45", 2, ini);
        assertEquals(new GaugeScaleOverrides(-10.0, 50.5, 45.0, 2), all.overrides);

        assertEquals("Max must be a number", GaugeScaleOverrides.parse("", "fast", "", null, ini).error);
        assertEquals("Danger from must be a number", GaugeScaleOverrides.parse("", "", "NaN", null, ini).error);
        assertEquals("Min must be below Max", GaugeScaleOverrides.parse("100", "", "", null, ini).error,
            "An empty Max means the .ini max");
        assertEquals("Min must be below Max", GaugeScaleOverrides.parse("", "-5", "", null, ini).error);
        assertNotNull(GaugeScaleOverrides.parse("", "", "", 7, ini).error);
        assertNotNull(GaugeScaleOverrides.parse("", "", "500", null, ini).overrides,
            "A danger value past max is allowed: it means no danger zone");
    }

    @Test
    void gaugeBodyAppliesTheSlotOverridesAndChangingTheGaugeDropsThem() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "[GaugeConfigurations]\ngaugeCategory = Scale\n"
            + "firstScaleGauge = firstScaleChannel, \"First\", \"kPa\", 0, 100, 0, 0, 80, 80, 0, 0\n"
            + "otherScaleGauge = otherScaleChannel, \"Other\", \"kPa\", 0, 100, 0, 0, 80, 80, 0, 0\n";
        IniFileModel ini = IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))),
            "scale.ini", mock(IniFileMetaInfo.class));
        SwingUtilities.invokeAndWait(() -> {
            UIContext context = new UIContext();
            context.iniFileState.setIniFileModelForTest(ini);
            JPanelWithListener wrapper = new JPanelWithListener(new BorderLayout());
            String[] changed = {null};
            try {
                SensorGauge.createGaugeBody(context, "firstScaleGauge", wrapper, name -> changed[0] = name,
                    Collections.emptyList(), GaugeDisplayKind.ROUND, new GaugeScaleOverrides(null, 50.0, null, null));
                CircularGauge gauge = (CircularGauge) wrapper.getComponent(0);
                gauge.setValue(25);
                assertEquals(135, gauge.getProgressDegrees(), "Half of the overridden 0..50 range");

                JPopupMenu menu = SensorGauge.createPopupMenu(context, wrapper, name -> changed[0] = name,
                    Collections.emptyList(), GaugeDisplayKind.ROUND);
                findItem(menu, "Other").doClick(0);
                assertEquals("otherScaleGauge", changed[0]);
                CircularGauge other = (CircularGauge) wrapper.getComponent(0);
                other.setValue(25);
                // A quarter of 0..100: 67.5 degrees, rounded.
                assertEquals(68, other.getProgressDegrees(), "The new gauge uses its own .ini range");
            } finally {
                wrapper.removeAllChildrenAndListeners();
            }
        });
    }

    private static JMenuItem findItem(MenuElement menu, String text) {
        for (MenuElement element : menu.getSubElements()) {
            if (element instanceof JMenuItem && !(element instanceof JMenu) && text.equals(((JMenuItem) element).getText())) {
                return (JMenuItem) element;
            }
            JMenuItem found = findItem(element, text);
            if (found != null) {
                return found;
            }
        }
        return null;
    }
}
