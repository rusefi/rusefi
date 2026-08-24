package com.rusefi.maintenance.migration.cranking_flex_table_migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestCallbacks;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.CrankingFlexTableMigrator;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import org.junit.jupiter.api.Test;

import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class CrankingFlexTableMigratorTest {

    // 1D curve: ArrayIniField.parse of "[size]" yields cols=1, rows=size
    private static ArrayIniField curve(final String name, final int size) {
        return new ArrayIniField(name, 0, FieldType.FLOAT, 1, size, "ratio", 1.0, "0", "50", "2");
    }

    // C [ethanol][coolant] is emitted as .ini "[coolant x ethanol]" -> cols=coolant, rows=ethanol
    private static ArrayIniField flexTable(final String name, final int coolantCols, final int ethanolRows) {
        return new ArrayIniField(name, 0, FieldType.UINT8, coolantCols, ethanolRows, "mult", 1.0, "0", "5", "2");
    }

    private static ArrayIniField bins(final String name, final int size) {
        return new ArrayIniField(name, 0, FieldType.UINT8, 1, size, "%", 1.0, "0", "100", "0");
    }

    private static Constant constant(final ArrayIniField field, final String value) {
        return new Constant(field.getName(), field.getUnits(), value, field.getDigits(),
            String.valueOf(field.getRows()), String.valueOf(field.getCols()));
    }

    @Test
    void migratesLegacyE0E100CurvesIntoFlexTable() {
        final int coolantSize = 8;
        final int ethanolSize = 4;

        final ArrayIniField e0Field = curve("crankingFuelCoef", coolantSize);
        final ArrayIniField e100Field = curve("crankingFuelCoefE100", coolantSize);
        final ArrayIniField tableField = flexTable("crankingFuelFlexTable", coolantSize, ethanolSize);
        final ArrayIniField binsField = bins("crankingFuelFlexBins", ethanolSize);

        // E0 = 1.0 at every coolant point, E100 = 3.0 at every coolant point
        final Constant e0Const = constant(e0Field, "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0");
        final Constant e100Const = constant(e100Field, "3.0 3.0 3.0 3.0 3.0 3.0 3.0 3.0");
        final Constant binsConst = constant(binsField, "0 35 65 100");

        // Previous tune still has the two 1D curves
        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.findIniField(any())).thenReturn(Optional.empty());
        when(prevIni.findIniField("crankingFuelCoef")).thenReturn(Optional.of(e0Field));
        when(prevIni.findIniField("crankingFuelCoefE100")).thenReturn(Optional.of(e100Field));
        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Map.of(
            "crankingFuelCoef", e0Const,
            "crankingFuelCoefE100", e100Const));

        // Updated tune: crankingFuelCoefE100 retired, new 2D table + ethanol axis present
        final IniFileModel updatedIni = mock(IniFileModel.class);
        when(updatedIni.findIniField(any())).thenReturn(Optional.empty());
        when(updatedIni.findIniField("crankingFuelCoef")).thenReturn(Optional.of(e0Field));
        when(updatedIni.findIniField("crankingFuelFlexTable")).thenReturn(Optional.of(tableField));
        when(updatedIni.findIniField("crankingFuelFlexBins")).thenReturn(Optional.of(binsField));
        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Map.of("crankingFuelFlexBins", binsConst));

        final TestTuneMigrationContext context =
            new TestTuneMigrationContext(prevMsq, prevIni, updatedMsq, updatedIni, new TestCallbacks());

        CrankingFlexTableMigrator.INSTANCE.migrateTune(context);

        final Constant migrated = context.getMigratedConstants().get("crankingFuelFlexTable");
        assertNotNull(migrated, "crankingFuelFlexTable should have been migrated");

        // Parse back into [ethanol rows][coolant cols]
        final String[][] values = tableField.getValues(migrated.getValue());
        assertEquals(ethanolSize, values.length);
        assertEquals(coolantSize, values[0].length);

        // 0% ethanol -> the E0 curve (1.0)
        assertEquals(1.0, Double.parseDouble(values[0][0]), 1e-3);
        // 100% ethanol -> clamped to the legacy 85% anchor -> the E100 curve (3.0)
        assertEquals(3.0, Double.parseDouble(values[3][0]), 1e-3);
        // 35% ethanol -> 1 + (35/85) * (3 - 1)
        assertEquals(1.0 + (35.0 / 85.0) * 2.0, Double.parseDouble(values[1][0]), 1e-2);
        // 65% ethanol -> 1 + (65/85) * (3 - 1)
        assertEquals(1.0 + (65.0 / 85.0) * 2.0, Double.parseDouble(values[2][0]), 1e-2);
    }

    @Test
    void doesNothingWhenLegacyE100FieldIsAbsent() {
        // Not the upgrade this migrator handles (no retired crankingFuelCoefE100 in the previous .ini): do nothing.
        final IniFileModel prevIni = mock(IniFileModel.class);
        when(prevIni.findIniField(any())).thenReturn(Optional.empty());
        final Msq prevMsq = mock(Msq.class);
        when(prevMsq.getConstantsAsMap()).thenReturn(Map.of());
        final IniFileModel updatedIni = mock(IniFileModel.class);
        when(updatedIni.findIniField(any())).thenReturn(Optional.empty());
        final Msq updatedMsq = mock(Msq.class);
        when(updatedMsq.getConstantsAsMap()).thenReturn(Map.of());

        final TestTuneMigrationContext context =
            new TestTuneMigrationContext(prevMsq, prevIni, updatedMsq, updatedIni, new TestCallbacks());
        CrankingFlexTableMigrator.INSTANCE.migrateTune(context);

        assertNull(context.getMigratedConstants().get("crankingFuelFlexTable"));
    }
}
