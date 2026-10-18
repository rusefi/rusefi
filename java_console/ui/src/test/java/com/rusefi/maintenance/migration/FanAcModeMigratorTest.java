package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

import static com.rusefi.config.FieldType.INT;
import static com.rusefi.config.FieldType.UINT8;
import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContextWithDisappearedConst;
import static javax.management.ObjectName.quote;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

// [tag:fan_ac_mode_migration] Exercise the full pipeline for historical Boolean targets
// and the replacement A/C modes, including a mode selected by an already-updated tune.
public class FanAcModeMigratorTest {
    @Test
    void trueToRelay() {
        checkLegacyMode("true", "Relay", "Disabled");
    }

    @Test
    void falseToDisabled() {
        checkLegacyMode("false", "Disabled", "Relay");
    }

    @Test
    void yesToRelay() {
        checkLegacyMode("yes", "Relay", "Disabled");
    }

    @Test
    void noToDisabled() {
        checkLegacyMode("no", "Disabled", "Relay");
    }

    private static void checkLegacyMode(String previous, String expected, String destinationDefault) {
        for (int fan = 1; fan <= 2; fan++) {
            TestTuneMigrationContext context = legacyContext(fan, previous, destinationDefault);

            ComposedTuneMigrator.INSTANCE.migrateTune(context);

            assertEquals(constant(modeName(fan), expected), context.getMigratedValue(modeName(fan)));
            assertTrue(context.isFieldAlreadyMigrated(legacyName(fan)));
            assertFalse(context.getMigratedConstants().containsKey(legacyName(fan)));
            assertEquals("", context.getTestCallbacks().getContent());
        }
    }

    @Test
    void historicalBooleanTargets() {
        for (int fan = 1; fan <= 2; fan++) {
            String name = legacyName(fan);
            TestTuneMigrationContext context = createTestMigrationContext(
                constant(name, "false"), legacyField(fan, false),
                constant(name, "no"), legacyField(fan, true));

            ComposedTuneMigrator.INSTANCE.migrateTune(context);

            assertEquals(constant(name, "no"), context.getMigratedValue(name));
            assertEquals("", context.getTestCallbacks().getContent());
        }
    }

    @Test
    void existingModesArePreserved() {
        for (int fan = 1; fan <= 2; fan++) {
            for (String mode : new String[]{"Disabled", "Relay", "Pressure"}) {
                String name = modeName(fan);
                TestTuneMigrationContext context = createTestMigrationContext(
                    constant(name, mode), modeField(fan),
                    constant(name, mode.equals("Disabled") ? "Relay" : "Disabled"), modeField(fan));

                ComposedTuneMigrator.INSTANCE.migrateTune(context);

                assertEquals(constant(name, mode), context.getMigratedValue(name));
                assertEquals("", context.getTestCallbacks().getContent());
            }
        }
    }

    @Test
    void existingPressureModeTakesPrecedenceOverStaleLegacyValue() {
        for (int fan = 1; fan <= 2; fan++) {
            TestTuneMigrationContext context = legacyContext(fan, "yes", "Disabled");
            Map<String, Constant> previous = new LinkedHashMap<>(context.getPrevTune().getConstantsAsMap());
            previous.put(modeName(fan), constant(modeName(fan), "Pressure"));
            when(context.getPrevTune().getConstantsAsMap()).thenReturn(previous);
            when(context.getPrevIniFile().findIniField(modeName(fan))).thenReturn(Optional.of(modeField(fan)));
            Map<String, IniField> fields =
                new LinkedHashMap<>(context.getPrevIniFile().getAllIniFields());
            fields.put(modeName(fan), modeField(fan));
            when(context.getPrevIniFile().getAllIniFields()).thenReturn(fields);

            ComposedTuneMigrator.INSTANCE.migrateTune(context);

            assertEquals(constant(modeName(fan), "Pressure"), context.getMigratedValue(modeName(fan)));
        }
    }

    @Test
    void missingSourceValueLeavesDestinationAlone() {
        TestTuneMigrationContext context = legacyContext(1, "yes", "Pressure");
        when(context.getPrevTune().getConstantsAsMap()).thenReturn(Collections.emptyMap());

        ComposedTuneMigrator.INSTANCE.migrateTune(context);

        assertTrue(context.getMigratedConstants().isEmpty());
        assertEquals("", context.getTestCallbacks().getContent());
    }

    @Test
    void missingDestinationFieldIsNotInvented() {
        TestTuneMigrationContext context = createTestMigrationContextWithDisappearedConst(
            constant(legacyName(1), "yes"), legacyField(1, true));

        ComposedTuneMigrator.INSTANCE.migrateTune(context);

        assertTrue(context.getMigratedConstants().isEmpty());
    }

    @Test
    void missingDestinationValue() {
        TestTuneMigrationContext context = legacyContext(1, "yes", "Disabled");
        when(context.getUpdatedTune().getConstantsAsMap()).thenReturn(Collections.emptyMap());

        ComposedTuneMigrator.INSTANCE.migrateTune(context);

        assertEquals(constant(modeName(1), "Relay"), context.getMigratedValue(modeName(1)));
        assertEquals("", context.getTestCallbacks().getContent());
    }

    @Test
    void invalidSourceValuesAreNotConverted() {
        for (String value : new String[]{"INVALID", "", "1"}) {
            TestTuneMigrationContext context = legacyContext(1, value, "Pressure");

            ComposedTuneMigrator.INSTANCE.migrateTune(context);

            assertTrue(context.getMigratedConstants().isEmpty());
        }
    }

    @Test
    void unsupportedDestinationModeIsNotWritten() {
        TestTuneMigrationContext context = legacyContext(1, "yes", "Disabled");
        EnumIniField field = new EnumIniField(modeName(1), 0, UINT8, labels("Disabled", "Pressure"), 0, 1);
        when(context.getUpdatedIniFile().findIniField(modeName(1))).thenReturn(Optional.of(field));

        ComposedTuneMigrator.INSTANCE.migrateTune(context);

        assertTrue(context.getMigratedConstants().isEmpty());
    }

    private static TestTuneMigrationContext legacyContext(int fan, String value, String destinationDefault) {
        return createTestMigrationContext(
            constant(legacyName(fan), value), legacyField(fan, value.equals("yes") || value.equals("no")),
            constant(modeName(fan), destinationDefault), modeField(fan));
    }

    private static EnumIniField legacyField(int fan, boolean yesNo) {
        return new EnumIniField(legacyName(fan), 188, INT,
            yesNo ? labels("no", "yes") : labels("false", "true"), fan + 3, 0);
    }

    private static EnumIniField modeField(int fan) {
        return new EnumIniField(modeName(fan), 588 + (fan - 1) * 6, UINT8,
            labels("Disabled", "Relay", "Pressure", "INVALID"), 0, 1);
    }

    private static EnumIniField.EnumKeyValueMap labels(String... values) {
        Map<Integer, String> result = new LinkedHashMap<>();
        for (int i = 0; i < values.length; i++) {
            result.put(i, values[i]);
        }
        return new EnumIniField.EnumKeyValueMap(result);
    }

    private static Constant constant(String name, String value) {
        return new Constant(name, null, quote(value), null);
    }

    private static String legacyName(int fan) {
        return "enableFan" + fan + "WithAc";
    }

    private static String modeName(int fan) {
        return "fan" + fan + "AcMode";
    }
}
