package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import static javax.management.ObjectName.quote;

/**
 * [tag:fan_ac_mode_migration] The old Enable with AC Boolean became an A/C mode:
 * true/yes means Relay, false/no means Disabled. Match labels explicitly because
 * the old U32 bit and new U08 enum have different names and storage types.
 * BooleanIniFieldMigrator still handles upgrades between historical Boolean schemas.
 */
public enum FanAcModeMigrator implements TuneMigrator {
    INSTANCE;

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (int fan = 1; fan <= 2; fan++) {
            migrateFan(context, "enableFan" + fan + "WithAc", "fan" + fan + "AcMode");
        }
    }

    private static void migrateFan(TuneMigrationContext context, String oldName, String newName) {
        if (context.isAdditionalIniFieldToIgnore(oldName) || context.isAdditionalIniFieldToIgnore(newName)
            || context.isFieldAlreadyMigrated(oldName) || context.getMigratedConstants().containsKey(newName)) {
            return;
        }

        // An existing mode (especially Pressure) belongs to the normal same-name migration.
        // A destination that still uses the old Boolean belongs to BooleanIniFieldMigrator.
        if (context.getPrevIniFile().findIniField(newName).isPresent()
            || context.getUpdatedIniFile().findIniField(oldName).isPresent()) {
            return;
        }

        IniField oldField = context.getPrevIniFile().findIniField(oldName).orElse(null);
        IniField newField = context.getUpdatedIniFile().findIniField(newName).orElse(null);
        Constant oldValue = context.getPrevTune().getConstantsAsMap().get(oldName);
        if (!(oldField instanceof EnumIniField) || !(newField instanceof EnumIniField)
            || oldValue == null || oldValue.getValue() == null) {
            return;
        }

        EnumIniField.EnumKeyValueMap oldLabels = ((EnumIniField) oldField).getEnums();
        String oldLabel = oldLabels.get(oldLabels.indexOf(oldValue.getValue()));
        final String mode;
        if ("true".equals(oldLabel) || "yes".equals(oldLabel)) {
            mode = "Relay";
        } else if ("false".equals(oldLabel) || "no".equals(oldLabel)) {
            mode = "Disabled";
        } else {
            return;
        }

        String newValue = quote(mode);
        if (((EnumIniField) newField).getEnums().indexOf(newValue) == -1) {
            return;
        }

        // Mark the old name as handled so DefaultTuneMigrator does not report it missing.
        context.addMigration(oldName, newName,
            new Constant(newName, newField.getUnits(), newValue, newField.getDigits()));
    }
}
