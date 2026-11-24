package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.PcVariableArrayIniField;
import com.opensr5.ini.field.ScalarIniField;
import com.opensr5.ini.field.StringIniField;
import com.rusefi.config.FieldType;
import org.junit.jupiter.api.Test;

import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * [tag:unburned_marks] Which settings and cells count as changed since the last burn (#10671).
 */
public class UnburnedChangesTest {
    private static final int SIZE = 64;

    private static final ScalarIniField FLOW = new ScalarIniField("injectorFlow", 0, "cc/min", FieldType.FLOAT, 1, "1", 0);
    private static final ScalarIniField LAG = new ScalarIniField("injectorLag", 4, "ms", FieldType.UINT16, 0.01, "2", 0);
    /** Two on/off settings sharing the 32-bit word at offset 8: bits 0 and 1. */
    private static final EnumIniField FLAG_A = bit("enableA", 8, 0);
    private static final EnumIniField FLAG_B = bit("enableB", 8, 1);
    /** A 3-bit enum in the same word, bits 4..6. */
    private static final EnumIniField MODE = new EnumIniField("mode", 8, FieldType.INT, enums(), 4, 2);
    /** 2 rows x 3 columns of UINT8 at offset 16. */
    private static final ArrayIniField TABLE = new ArrayIniField("table", 16, FieldType.UINT8, 3, 2, "deg", 1, "0", "100", "0");
    private static final StringIniField NAME = new StringIniField("name", 32, 8);

    private static EnumIniField bit(String name, int offset, int position) {
        // as the .ini reader builds a "bits, U32" setting: the storage type, not FieldType.BIT
        return new EnumIniField(name, offset, FieldType.INT, enums(), position, 0);
    }

    private static EnumIniField.EnumKeyValueMap enums() {
        Map<Integer, String> values = new HashMap<>();
        values.put(0, "false");
        values.put(1, "true");
        return new EnumIniField.EnumKeyValueMap(values);
    }

    private static ConfigurationImage image(ByteOrder order) {
        return new ConfigurationImage(SIZE, order);
    }

    private static ConfigurationImage image() {
        return image(ByteOrder.LITTLE_ENDIAN);
    }

    @Test
    void identicalTunesHaveNoChanges() {
        ConfigurationImage burned = image();
        UnburnedChanges changes = UnburnedChanges.of(burned.clone(), burned);
        for (IniField field : allFields()) {
            assertFalse(changes.isChanged(field), field.getName());
        }
        assertEquals(0, changes.countChanged(allFields()));
    }

    @Test
    void changedScalarIsMarkedAndItsNeighbourIsNot() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getByteBuffer(FLOW.getOffset(), 4).putFloat(470);

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertTrue(changes.isChanged(FLOW));
        assertFalse(changes.isChanged(LAG));
        assertEquals(1, changes.countChanged(allFields()));
    }

    @Test
    void valueSetBackToItsBurnedValueIsNotMarked() {
        ConfigurationImage burned = image();
        burned.getByteBuffer(FLOW.getOffset(), 4).putFloat(440);
        ConfigurationImage current = burned.clone();
        current.getByteBuffer(FLOW.getOffset(), 4).putFloat(470);
        assertTrue(UnburnedChanges.of(current, burned).isChanged(FLOW));

        current.getByteBuffer(FLOW.getOffset(), 4).putFloat(440);
        assertFalse(UnburnedChanges.of(current, burned).isChanged(FLOW));
    }

    @Test
    void onOffSettingsSharingAWordAreComparedBitByBit() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.setBitValue(FLAG_B, 1);

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertFalse(changes.isChanged(FLAG_A), "neighbour bit in the same word");
        assertTrue(changes.isChanged(FLAG_B));
        assertFalse(changes.isChanged(MODE), "enum in the same word");
        assertEquals(1, changes.countChanged(allFields()));
    }

    @Test
    void multiBitEnumIsComparedOnItsOwnBits() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.setBitValue(MODE, 5);

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertTrue(changes.isChanged(MODE));
        assertFalse(changes.isChanged(FLAG_A));
        assertFalse(changes.isChanged(FLAG_B));
    }

    @Test
    void bitFieldsWorkWithBigEndianTunes() {
        ConfigurationImage burned = image(ByteOrder.BIG_ENDIAN);
        ConfigurationImage current = burned.clone();
        current.setBitValue(FLAG_B, 1);

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertFalse(changes.isChanged(FLAG_A));
        assertTrue(changes.isChanged(FLAG_B));
    }

    @Test
    void tableCellsAreMarkedOneByOne() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[TABLE.getOffset(1, 2)] = 7;
        current.getContent()[TABLE.getOffset(0, 1)] = 3;

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertTrue(changes.isChanged(TABLE));
        assertTrue(changes.isCellChanged(TABLE, 1, 2));
        assertTrue(changes.isCellChanged(TABLE, 0, 1));
        assertFalse(changes.isCellChanged(TABLE, 0, 0));
        assertFalse(changes.isCellChanged(TABLE, 1, 1));
        assertEquals(2, changes.countChangedCells(TABLE));
        assertEquals(2, changes.countChanged(allFields()));
    }

    @Test
    void cellOutsideTheTableIsNotMarked() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[TABLE.getOffset(1, 2)] = 7;

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertFalse(changes.isCellChanged(TABLE, 2, 0));
        assertFalse(changes.isCellChanged(TABLE, 0, 3));
        assertFalse(changes.isCellChanged(TABLE, -1, 0));
    }

    @Test
    void textSettingIsMarked() {
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[NAME.getOffset() + 3] = 'x';

        assertTrue(UnburnedChanges.of(current, burned).isChanged(NAME));
    }

    @Test
    void valuesStoredAtTheSamePlaceCountOnce() {
        ArrayIniField sameTableOtherUnits = new ArrayIniField("tableAfr", 16, FieldType.UINT8, 3, 2, "afr", 0.1, "0", "100", "0");
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[TABLE.getOffset(0, 0)] = 9;

        assertEquals(1, UnburnedChanges.of(current, burned).countChanged(Arrays.asList(TABLE, sameTableOtherUnits)));
    }

    @Test
    void noBurnedTuneMarksNothing() {
        ConfigurationImage current = image();
        current.getByteBuffer(FLOW.getOffset(), 4).putFloat(470);

        UnburnedChanges changes = UnburnedChanges.of(current, null);
        assertFalse(changes.isChanged(FLOW));
        assertFalse(changes.isCellChanged(TABLE, 0, 0));
        assertEquals(0, changes.countChanged(allFields()));
        assertFalse(UnburnedChanges.of(null, current).isChanged(FLOW));
        assertFalse(UnburnedChanges.none().isChanged(FLOW));
    }

    @Test
    void fieldsOutsideTheMainPageAreNotMarked() {
        ScalarIniField onPage3 = new ScalarIniField("ltft", 0, "%", FieldType.FLOAT, 1, "1", 0);
        onPage3.setPageIndex(3);
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getByteBuffer(0, 4).putFloat(1);

        UnburnedChanges changes = UnburnedChanges.of(current, burned);
        assertTrue(changes.isChanged(FLOW), "same offset on the main page");
        assertFalse(changes.isChanged(onPage3));
        assertEquals(0, changes.countChanged(Collections.singletonList(onPage3)));
    }

    @Test
    void pcVariablesAreNotMarked() {
        PcVariableArrayIniField pcVariable = new PcVariableArrayIniField(TABLE);
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[TABLE.getOffset(0, 0)] = 9;

        assertFalse(UnburnedChanges.of(current, burned).isChanged(pcVariable));
    }

    @Test
    void fieldBeyondTheImageIsNotMarked() {
        ScalarIniField beyond = new ScalarIniField("beyond", SIZE - 2, "", FieldType.FLOAT, 1, "1", 0);
        ConfigurationImage burned = image();
        ConfigurationImage current = burned.clone();
        current.getContent()[SIZE - 1] = 1;

        assertFalse(UnburnedChanges.of(current, burned).isChanged(beyond));
    }

    private static List<IniField> allFields() {
        return Arrays.asList(FLOW, LAG, FLAG_A, FLAG_B, MODE, TABLE, NAME);
    }
}
