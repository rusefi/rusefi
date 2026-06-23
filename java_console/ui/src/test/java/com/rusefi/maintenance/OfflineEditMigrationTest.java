package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.MsqFactory;
import jakarta.xml.bind.JAXBException;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.load;
import static com.rusefi.maintenance.migration.migrators.CltIdleCorrMigrator.MAN_IDLE_POSITION_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.*;

class OfflineEditMigrationTest {

    @Test
    void noEditsYieldsEmptyPlan() throws JAXBException {
        TestTuneMigrationContext ctx = load();
        IniFileModel ini = ctx.getPrevIniFile();
        ConfigurationImage baseline = ctx.getPrevTune().asImage(ini);

        OfflineEditMigration.Plan plan = OfflineEditMigration.computePlan(baseline, baseline.clone(), ini, ini);

        assertTrue(plan.isEmpty(), "unchanged image must produce no edits");
    }

    @Test
    void editedFieldPresentInTargetIsMigratable() throws JAXBException {
        TestTuneMigrationContext ctx = load();
        IniFileModel sourceIni = ctx.getPrevIniFile();
        IniFileModel targetIni = ctx.getUpdatedIniFile();

        ConfigurationImage baseline = ctx.getPrevTune().asImage(sourceIni);
        ConfigurationImage edited = baseline.clone();

        // Change a single scalar field that exists in both signatures.
        IniField field = sourceIni.findIniField(MAN_IDLE_POSITION_FIELD_NAME).orElseThrow();
        Constant original = MsqFactory.valueOf(baseline, sourceIni).getConstantsAsMap().get(MAN_IDLE_POSITION_FIELD_NAME);
        ConfigurationImageGetterSetter2.setValue(field, edited, original.cloneWithValue("42.0"));

        OfflineEditMigration.Plan plan = OfflineEditMigration.computePlan(baseline, edited, sourceIni, targetIni);

        assertEquals(1, plan.migratable.size(), "exactly one edited field expected");
        assertEquals(MAN_IDLE_POSITION_FIELD_NAME, plan.migratable.get(0).getName());
        assertTrue(plan.incompatible.isEmpty(), "field exists on target, so nothing incompatible");

        // And applying it actually writes the edited value onto a target-layout image
        // (compare numerically — target .ini may render with different digits, e.g. "42" vs "42.0").
        ConfigurationImage targetImage = ctx.getUpdatedTune().asImage(targetIni);
        ConfigurationImage merged = OfflineEditMigration.apply(targetImage, targetIni, plan.migratable);
        String mergedValue = MsqFactory.valueOf(merged, targetIni).getConstantsAsMap().get(MAN_IDLE_POSITION_FIELD_NAME).getValue();
        assertEquals(42.0, Double.parseDouble(mergedValue), 1e-6);
    }
}
