package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Arrays;

import static javax.management.ObjectName.quote;

/**
 * [tag:idle_air_migration] A/C RPM is an addition, not an absolute floor. Never copy the
 * old numeric target unchanged. Preserve its effect at the last (warmest) idle-curve
 * point and warn that cold operation differs. Fixed A/C air becomes a flat curve.
 */
public enum IdleAirMigrator implements TuneMigrator {
    INSTANCE;

    @Override
    public void migrateTune(TuneMigrationContext context) {
        migrateAcAir(context);
        migrateAcRpm(context);
        String enabled = "crankingAirAmountEnabled";
        if (canMigrate(context, "cltCrankingCorr", enabled)) {
            IniField field = context.getUpdatedIniFile().findIniField(enabled).orElse(null);
            if (field instanceof EnumIniField
                && ((EnumIniField) field).getEnums().indexOf(quote("enabled")) >= 0) {
                // Do not consume cltCrankingCorr: the original curve must migrate normally too.
                context.addMigration(enabled, new Constant(enabled, field.getUnits(), quote("enabled"), field.getDigits()));
            }
        }
    }

    private static boolean canMigrate(TuneMigrationContext context, String oldName, String newName) {
        return !context.isAdditionalIniFieldToIgnore(oldName)
            && !context.isAdditionalIniFieldToIgnore(newName)
            && !context.isFieldAlreadyMigrated(oldName)
            && !context.getMigratedConstants().containsKey(newName)
            && context.getPrevIniFile().findIniField(oldName).isPresent()
            && !context.getPrevIniFile().findIniField(newName).isPresent()
            && context.getUpdatedIniFile().findIniField(newName).isPresent();
    }

    private static void migrateAcAir(TuneMigrationContext context) {
        String oldName = "acIdleExtraOffset";
        String newName = "acIdleAdderByPressure";
        if (!canMigrate(context, oldName, newName)) {
            return;
        }
        IniField field = context.getUpdatedIniFile().findIniField(newName).orElse(null);
        Double value = readScalar(context, oldName);
        if (!(field instanceof ArrayIniField) || value == null || value < 0 || value > 100) {
            return;
        }
        ArrayIniField array = (ArrayIniField) field;
        String[][] values = new String[array.getRows()][array.getCols()];
        for (String[] row : values) {
            Arrays.fill(row, IdleCurveMigrator.formatArrayValue(value, array.getDigits()));
        }
        context.addMigration(oldName, newName,
            new Constant(newName, field.getUnits(), array.formatValue(values), field.getDigits()));
    }

    private static void migrateAcRpm(TuneMigrationContext context) {
        String oldName = "acIdleRpmTarget";
        String newName = "acIdleRpmAdder";
        if (!canMigrate(context, oldName, newName)) {
            return;
        }
        Double target = readScalar(context, oldName);
        IniField curve = context.getPrevIniFile().findIniField("cltIdleRpm").orElse(null);
        Constant curveValue = context.getPrevTune().getConstantsAsMap().get("cltIdleRpm");
        if (target == null || !(curve instanceof ArrayIniField) || curveValue == null || curveValue.getValue() == null) {
            return;
        }
        try {
            String[][] values = ((ArrayIniField) curve).getValues(curveValue.getValue());
            String[] lastRow = values[values.length - 1];
            double warmIdle = Double.parseDouble(lastRow[lastRow.length - 1]);
            if (!Double.isFinite(warmIdle) || warmIdle <= 0) {
                return;
            }
            double adder = Math.max(0, Math.min(1000, target - warmIdle));
            IniField field = context.getUpdatedIniFile().findIniField(newName).get();
            context.addMigration(oldName, newName, new Constant(newName, field.getUnits(),
                IdleCurveMigrator.formatArrayValue(adder, field.getDigits()), field.getDigits()));
            context.logWarn("A/C idle RPM converted from an absolute target to an addition using the last "
                + "(warmest) idle RPM curve point. Review A/C idle RPM across coolant temperatures.");
        } catch (NumberFormatException | IndexOutOfBoundsException | IllegalStateException e) {
            context.logWarn("A/C idle RPM conversion skipped: invalid idle RPM curve. Review A/C idle calibration.");
        }
    }

    private static Double readScalar(TuneMigrationContext context, String name) {
        Constant constant = context.getPrevTune().getConstantsAsMap().get(name);
        if (constant == null || constant.getValue() == null) {
            return null;
        }
        try {
            double value = Double.parseDouble(constant.getValue());
            return Double.isFinite(value) ? value : null;
        } catch (NumberFormatException e) {
            return null;
        }
    }
}
