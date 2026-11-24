package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

/**
 * The offline/dirty toolbar label wording (#9730). Pure logic, no Swing.
 */
public class TuningToolbarStateLabelTest {

    @Test
    void offlineAndDirty() {
        assertEquals("Local changes not burned to ECU",
                TuningToolbarWidget.stateLabelText(true, true));
    }

    @Test
    void connectedAndDirty() {
        assertEquals("Pending changes not burned",
                TuningToolbarWidget.stateLabelText(false, true));
    }

    @Test
    void offlineAndClean() {
        assertEquals("Offline tune",
                TuningToolbarWidget.stateLabelText(true, false));
    }

    @Test
    void connectedAndClean() {
        // Empty label = invisible; nothing to say when connected with no pending edits.
        assertEquals("", TuningToolbarWidget.stateLabelText(false, false));
    }

    @Test
    void countsChangedValues() {
        // [tag:unburned_marks] #10671
        assertEquals("Pending changes not burned (3 values)",
                TuningToolbarWidget.stateLabelText(false, true, 3));
        assertEquals("Pending changes not burned (1 value)",
                TuningToolbarWidget.stateLabelText(false, true, 1));
        assertEquals("Local changes not burned to ECU (2 values)",
                TuningToolbarWidget.stateLabelText(true, true, 2));
    }

    @Test
    void noCountWhenNoSettingIsChanged() {
        // e.g. only bytes that no setting covers differ: still dirty, nothing to count
        assertEquals("Pending changes not burned", TuningToolbarWidget.stateLabelText(false, true, 0));
        assertEquals("", TuningToolbarWidget.stateLabelText(false, false, 0));
    }

    @Test
    void noIniCountsNothing() {
        ConfigurationImage burned = ConfigurationImageTestHelper.newLittleEndian(4);
        ConfigurationImage current = ConfigurationImageTestHelper.newLittleEndian(new byte[]{1, 0, 0, 0});
        assertEquals(0, TuningToolbarWidget.changedValueCount(null, current, burned));
    }

    @Test
    void savesCurrentTuneWithoutEdits() {
        ConfigurationImage working = ConfigurationImageTestHelper.newLittleEndian(1);
        ConfigurationImage session = ConfigurationImageTestHelper.newLittleEndian(2);
        ConfigurationImage ecu = ConfigurationImageTestHelper.newLittleEndian(3);
        ConfigurationImage baseline = ConfigurationImageTestHelper.newLittleEndian(4);

        assertSame(working, TuningToolbarWidget.imageToSave(working, session, ecu, baseline));
        assertSame(session, TuningToolbarWidget.imageToSave(null, session, ecu, baseline));
        assertSame(ecu, TuningToolbarWidget.imageToSave(null, null, ecu, baseline));
        assertSame(baseline, TuningToolbarWidget.imageToSave(null, null, null, baseline));
    }
}
