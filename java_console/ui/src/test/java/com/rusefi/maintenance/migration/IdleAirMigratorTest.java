package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static javax.management.ObjectName.quote;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

// [tag:idle_air_migration] Exercise the complete migration pipeline, including same-name preservation.
public class IdleAirMigratorTest {
    @Test
    void fixedAirBecomesFlatPressureCurve() {
        ArrayIniField curve = array("acIdleAdderByPressure");
        TestTuneMigrationContext context = createTestMigrationContext(
            value("acIdleExtraOffset", "13"), scalar("acIdleExtraOffset"),
            value(curve.getName(), "0 0 0 0 0 0 0 0"), curve);
        ComposedTuneMigrator.INSTANCE.migrateTune(context);
        String[][] values = curve.getValues(context.getMigratedValue(curve.getName()).getValue());
        for (String[] row : values) {
            for (String element : row) {
                assertEquals(13, Double.parseDouble(element));
            }
        }
        assertTrue(context.isFieldAlreadyMigrated("acIdleExtraOffset"));
    }

    @Test
    void absoluteRpmBecomesWarmIdleAddition() {
        checkRpm("950", "150");
        checkRpm("700", "0");
        checkRpm("2000", "1000");
    }

    private static void checkRpm(String previous, String expected) {
        TestTuneMigrationContext context = rpmContext(previous);
        ComposedTuneMigrator.INSTANCE.migrateTune(context);
        assertEquals(expected, context.getMigratedValue("acIdleRpmAdder").getValue());
        assertTrue(context.isFieldAlreadyMigrated("acIdleRpmTarget"));
        assertTrue(context.getTestCallbacks().getContent().contains("Review A/C idle RPM"));
    }

    @Test
    void malformedAbsoluteTargetIsNotReinterpreted() {
        for (String invalid : new String[]{"bad", "NaN", "Infinity"}) {
            TestTuneMigrationContext context = rpmContext(invalid);
            ComposedTuneMigrator.INSTANCE.migrateTune(context);
            assertFalse(context.getMigratedConstants().containsKey("acIdleRpmAdder"));
        }
    }

    @Test
    void malformedIdleCurveDoesNotAbortMigration() {
        TestTuneMigrationContext context = rpmContext("950");
        Map<String, Constant> values = new LinkedHashMap<>(context.getPrevTune().getConstantsAsMap());
        values.put("cltIdleRpm", value("cltIdleRpm", "800"));
        when(context.getPrevTune().getConstantsAsMap()).thenReturn(values);
        assertDoesNotThrow(() -> ComposedTuneMigrator.INSTANCE.migrateTune(context));
        assertFalse(context.getMigratedConstants().containsKey("acIdleRpmAdder"));
    }

    @Test
    void existingAdditionAndPressureCurveArePreserved() {
        for (String name : new String[]{"acIdleRpmAdder", "acIdleAdderByPressure"}) {
            IniField field = name.equals("acIdleRpmAdder") ? scalar(name) : array(name);
            String previous = name.equals("acIdleRpmAdder") ? "125" : "0 5 10 15 20 25 30 35";
            TestTuneMigrationContext context = createTestMigrationContext(
                value(name, previous), field, value(name, "0"), field);
            ComposedTuneMigrator.INSTANCE.migrateTune(context);
            assertEquals(previous, context.getMigratedValue(name).getValue());
        }
    }

    @Test
    void legacyCrankingAirStaysEnabled() {
        String name = "crankingAirAmountEnabled";
        TestTuneMigrationContext context = createTestMigrationContext(
            value("cltCrankingCorr", "50 50 50 50 50 50 50 50"), array("cltCrankingCorr"),
            value(name, quote("disabled")), enabledField());
        ComposedTuneMigrator.INSTANCE.migrateTune(context);
        assertEquals(quote("enabled"), context.getMigratedValue(name).getValue());
    }

    @Test
    void existingDisabledCrankingAirStaysDisabled() {
        String name = "crankingAirAmountEnabled";
        TestTuneMigrationContext context = createTestMigrationContext(
            value(name, quote("disabled")), enabledField(),
            value(name, quote("enabled")), enabledField());
        ComposedTuneMigrator.INSTANCE.migrateTune(context);
        assertEquals(quote("disabled"), context.getMigratedValue(name).getValue());
    }

    private static TestTuneMigrationContext rpmContext(String rpm) {
        TestTuneMigrationContext context = createTestMigrationContext(
            value("acIdleRpmTarget", rpm), scalar("acIdleRpmTarget"),
            value("acIdleRpmAdder", "0"), scalar("acIdleRpmAdder"));
        Map<String, Constant> values = new LinkedHashMap<>(context.getPrevTune().getConstantsAsMap());
        values.put("cltIdleRpm", value("cltIdleRpm", "1500 1400 1300 1200 1100 1000 900 800"));
        when(context.getPrevTune().getConstantsAsMap()).thenReturn(values);
        when(context.getPrevIniFile().findIniField("cltIdleRpm")).thenReturn(Optional.of(array("cltIdleRpm")));
        return context;
    }

    private static ArrayIniField array(String name) {
        return new ArrayIniField(name, 0, FieldType.FLOAT, 1, 8, "", 1, "0", "3000", "0");
    }

    private static ScalarIniField scalar(String name) {
        return new ScalarIniField(name, 0, "", FieldType.FLOAT, 1, "0", 0);
    }

    private static EnumIniField enabledField() {
        return new EnumIniField("crankingAirAmountEnabled", 0, FieldType.INT,
            new EnumIniField.EnumKeyValueMap(Map.of(0, "disabled", 1, "enabled")), 0, 0);
    }

    private static Constant value(String name, String text) {
        return new Constant(name, "", text, "0");
    }
}
