package com.rusefi.ui.logview;

import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/** Chart display options are kept in the console config and read back by the next viewer. */
class ChartSettingsPersistenceTest {
    @Test
    void optionsSurviveARestart() {
        Node node = new Node();
        ChartSettings settings = new ChartSettings(node);
        int[] saves = {0};
        settings.addListener(() -> saves[0]++);

        settings.setAlwaysMinMax(true);
        settings.setShowRawValues(false);
        settings.setCursorValues(ChartSettings.CursorValues.LIST);
        settings.setPinValues(true);
        settings.setSnapCursor(false);
        settings.setVertical(true);
        settings.setVerticalMode(ChartSettings.VerticalMode.SAMPLES);
        settings.setTimeSpacing(150);
        settings.setHorizontal(true);
        settings.setHorizontalDivisions(7);
        settings.setSmoothingWindowSeconds(0.2);
        settings.setSmoothingWindowSeconds("RPM", 0.05);
        assertEquals(12, saves[0], "every change is saved");

        ChartSettings restored = new ChartSettings(node);
        assertTrue(restored.isAlwaysMinMax());
        assertFalse(restored.isShowRawValues());
        assertEquals(ChartSettings.CursorValues.LIST, restored.getCursorValues());
        assertTrue(restored.isPinValues());
        assertFalse(restored.isSnapCursor());
        assertTrue(restored.isVertical());
        assertEquals(ChartSettings.VerticalMode.SAMPLES, restored.getVerticalMode());
        assertEquals(150, restored.getTimeSpacing());
        assertTrue(restored.isHorizontal());
        assertEquals(7, restored.getHorizontalDivisions());
        assertEquals(0.2, restored.getSmoothingWindowSeconds());
        assertEquals(0.05, restored.getSmoothingWindowSeconds("RPM"));
        assertTrue(restored.usesGlobalSmoothing("MAP"));

        restored.useGlobalSmoothing("RPM");
        assertTrue(new ChartSettings(node).usesGlobalSmoothing("RPM"));
    }

    @Test
    void defaultsWhenNothingIsStored() {
        ChartSettings settings = new ChartSettings(new Node());
        assertFalse(settings.isAlwaysMinMax(), "min/max on hover");
        assertTrue(settings.isShowRawValues(), "raw values beside smoothed ones");
        assertEquals(ChartSettings.CursorValues.LABELS, settings.getCursorValues());
        assertTrue(settings.isSnapCursor());
        assertEquals(ChartSettings.DEFAULT_TIME_SPACING, settings.getTimeSpacing());
    }

    @Test
    void corruptValuesFallBackToDefaults() {
        Node node = new Node();
        node.setProperty("cursorValues", "SOMETHING");
        node.setProperty("timeSpacing", 100_000);
        node.setProperty("smoothing", "-3");
        node.getChild("smoothingSignals").setProperty("RPM", "NaN");
        ChartSettings settings = new ChartSettings(node);
        assertEquals(ChartSettings.CursorValues.LABELS, settings.getCursorValues());
        assertEquals(ChartSettings.MAX_TIME_SPACING, settings.getTimeSpacing());
        assertEquals(0, settings.getSmoothingWindowSeconds());
        assertTrue(settings.usesGlobalSmoothing("RPM"));
    }

    /** The plot brightness is stored only when it differs from the console theme, so it otherwise follows it. */
    @Test
    void plotBrightnessFollowsTheThemeUntilChanged() {
        Node node = new Node();
        ChartSettings settings = new ChartSettings(node);
        boolean themeDark = settings.isDarkPlot();
        settings.setSnapCursor(false);
        assertNull(node.getProperty("darkPlot", null));

        settings.setDarkPlot(!themeDark);
        assertEquals(!themeDark, new ChartSettings(node).isDarkPlot());
    }

    @Test
    void rawValuesSwitchIsUsableOnlyWhileSmoothing() {
        ChartSettings settings = new ChartSettings();
        ChartToolbar toolbar = new ChartToolbar(settings, () -> {
        });
        assertFalse(toolbar.rawShow.isEnabled());
        assertFalse(toolbar.smoothingNote.isVisible());

        settings.setSmoothingWindowSeconds("RPM", 0.1);
        // what opening the Values popup does
        toolbar.updateControls();
        assertTrue(toolbar.rawShow.isEnabled(), "per-signal smoothing counts too");
        assertTrue(toolbar.smoothingNote.isVisible(), "warns that min/max stay raw");
        toolbar.rawHide.doClick();
        assertFalse(settings.isShowRawValues());
    }

    @Test
    void valuesPopupSwitchesMinMax() {
        ChartSettings settings = new ChartSettings();
        ChartToolbar toolbar = new ChartToolbar(settings, () -> {
        });
        assertTrue(toolbar.minMaxHover.isSelected());

        toolbar.minMaxAlways.doClick();
        assertTrue(settings.isAlwaysMinMax());
        toolbar.minMaxHover.doClick();
        assertFalse(settings.isAlwaysMinMax());
    }
}
