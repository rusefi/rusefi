package com.rusefi.maintenance.migration;

import com.opensr5.ini.RawIniFile;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.ScalarIniField;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.LinkedList;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.checkIfUnitsCanBeMigrated;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Regression coverage for the units-expression migration gap: when a firmware update changes a
 * field's TS units string from a literal (or an old expression) to a new {bitStringValue(...)}
 * expression, {@link IniFieldMigrationUtils#checkIfUnitsCanBeMigrated} used to compare the RAW
 * unevaluated strings and refuse migration - even though the physical unit (kPa) was unchanged.
 * The user's tuned value was then silently replaced by the new firmware's default.
 *
 * Real-world case: Harley hd81 Kansas -> Lima update lost the customer's 20..180 VE/ignition load
 * axes (replaced by 10..160 / 21..120 defaults) because Lima introduced kPa/psi display-unit
 * expressions on veLoadBins/ignitionLoadBins/boostCutPressure & friends.
 *
 * Note: in the production updater flow both tunes are generated from ECU images via
 * CalibrationsInfo.generateMsq(), so Constant units carry these raw ini strings; TunerStudio-saved
 * .msq files carry EVALUATED units ("kPa") and do not hit this path.
 *
 * The fix: `{...}` expression units never block migration - a raw-string mismatch involving an
 * expression says nothing about the physical unit. Literal-vs-literal comparison is unchanged
 * (afr vs lambda still refuses).
 */
class UnitsExpressionMigrationTest {
    // verbatim lines from the hd81 Kansas (lts-25kansas.2026.08.02) and Lima (lts-26lima.2026.08.02) inis
    private static final String KANSAS_VE_LOAD_BINS_LINE =
        "veLoadBins = array, U16, 33540, [16], {bitStringValue(fuelUnits, fuelAlgorithm) }, 1, 0, 0, 650, 0";
    private static final String LIMA_VE_LOAD_BINS_LINE =
        "veLoadBins = array, U16, 10768, [16], {bitStringValue(veLoadUnitLabels, veLoadUnitIdx)}, {!(veOverrideMode == 1 || (veOverrideMode == 0 && fuelAlgorithm == 0)) || useMetricOnInterface ? 1 : 0.145038}, 0, 0, {veOverrideMode == 1 || (veOverrideMode == 0 && fuelAlgorithm == 0) ? (useMetricOnInterface ? 650 : 650 * 0.145038) : 650}, 0";

    private static final String KANSAS_IGNITION_LOAD_BINS_LINE =
        "ignitionLoadBins = array, U16, 32692, [16], \"Load\", 1, 0, 0, 650, 0";
    private static final String LIMA_IGNITION_LOAD_BINS_LINE =
        "ignitionLoadBins = array, U16, 9920, [16], {bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}, {!(ignOverrideMode == 1 || (ignOverrideMode == 0 && fuelAlgorithm == 0)) || useMetricOnInterface ? 1 : 0.145038}, 0, 0, {ignOverrideMode == 1 || (ignOverrideMode == 0 && fuelAlgorithm == 0) ? (useMetricOnInterface ? 650 : 650 * 0.145038) : 650}, 0";

    private static final String KANSAS_BOOST_CUT_PRESSURE_LINE =
        "boostCutPressure = scalar, F32, 1592, \"kPa\", 1, 0, 0, 650, 0";
    private static final String LIMA_BOOST_CUT_PRESSURE_LINE =
        "boostCutPressure = scalar, F32, 1700, {bitStringValue(pressureUnitsLabels, useMetricOnInterface)}, {useMetricOnInterface ? 1 : 0.145038}, 0, {useMetricOnInterface ? 0 : 0}, {useMetricOnInterface ? 650 : 650 * 0.145038}, 0";

    private static String parseArrayUnits(final String iniLine) {
        return ArrayIniField.parse(tokenize(iniLine)).getUnits();
    }

    private static String parseScalarUnits(final String iniLine) {
        return ScalarIniField.parse(tokenize(iniLine)).getUnits();
    }

    private static LinkedList<String> tokenize(final String iniLine) {
        return new LinkedList<>(Arrays.asList(new RawIniFile.Line(iniLine).getTokens()));
    }

    @Test
    void veLoadBinsExpressionToExpressionIsMigratable() {
        final String kansasUnits = parseArrayUnits(KANSAS_VE_LOAD_BINS_LINE);
        final String limaUnits = parseArrayUnits(LIMA_VE_LOAD_BINS_LINE);

        // the ini parser keeps TS unit expressions raw - this is what DefaultTuneMigrator compares
        assertEquals("{bitStringValue(fuelUnits, fuelAlgorithm) }", kansasUnits);
        assertEquals("{bitStringValue(veLoadUnitLabels, veLoadUnitIdx)}", limaUnits);

        // both expressions evaluate to "kPa" on this vehicle; refusing here lost the customer's
        // 20..180 load axis
        assertTrue(checkIfUnitsCanBeMigrated(kansasUnits, limaUnits));
    }

    @Test
    void ignitionLoadBinsLiteralToExpressionIsMigratable() {
        final String kansasUnits = parseArrayUnits(KANSAS_IGNITION_LOAD_BINS_LINE);
        final String limaUnits = parseArrayUnits(LIMA_IGNITION_LOAD_BINS_LINE);

        assertEquals("Load", kansasUnits);
        assertEquals("{bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}", limaUnits);

        assertTrue(checkIfUnitsCanBeMigrated(kansasUnits, limaUnits));
    }

    @Test
    void scalarKpaToExpressionIsMigratable() {
        final String kansasUnits = parseScalarUnits(KANSAS_BOOST_CUT_PRESSURE_LINE);
        final String limaUnits = parseScalarUnits(LIMA_BOOST_CUT_PRESSURE_LINE);

        assertEquals("kPa", kansasUnits);
        assertEquals("{bitStringValue(pressureUnitsLabels, useMetricOnInterface)}", limaUnits);

        assertTrue(checkIfUnitsCanBeMigrated(kansasUnits, limaUnits));
    }

    @Test
    void identicalExpressionUnitsAreMigratable() {
        final String units = parseArrayUnits(LIMA_VE_LOAD_BINS_LINE);
        assertTrue(checkIfUnitsCanBeMigrated(units, units));
    }

    @Test
    void differentLiteralUnitsAreStillRefused() {
        // the expression tolerance must not weaken the literal-vs-literal guard
        assertFalse(checkIfUnitsCanBeMigrated("afr", "lambda"));
    }
}
