package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.SignatureHelper;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import static javax.management.ObjectName.quote;

/** Kansas used inverted tuning-detector semantics (rusEFI #9309); enable it when upgrading to Lima. */
public enum KansasLimaTuningDetectorMigrator implements TuneMigrator {
    INSTANCE;

    private static final String FIELD = "isTuningDetectorEnabled";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        if (context.isAdditionalIniFieldToIgnore(FIELD) || context.isFieldAlreadyMigrated(FIELD)) {
            return;
        }
        if (context.getPrevTune().versionInfo == null || context.getUpdatedTune().versionInfo == null
            || !isRelease(context.getPrevTune().versionInfo.getSignature(), "lts-25kansas")
            || !isRelease(context.getUpdatedTune().versionInfo.getSignature(), "lts-26lima")) {
            return;
        }

        final IniField previousField = context.getPrevIniFile().findIniField(FIELD).orElse(null);
        final IniField updatedField = context.getUpdatedIniFile().findIniField(FIELD).orElse(null);
        final Constant previousValue = context.getPrevTune().getConstantsAsMap().get(FIELD);
        if (!(previousField instanceof EnumIniField) || !(updatedField instanceof EnumIniField)
            || previousValue == null || previousValue.getValue() == null) {
            return;
        }
        final EnumIniField.EnumKeyValueMap labels = ((EnumIniField) previousField).getEnums();
        final String previousLabel = labels.get(labels.indexOf(previousValue.getValue()));
        final String enabledValue = quote("yes");
        if (!("no".equals(previousLabel) || "yes".equals(previousLabel))
            || ((EnumIniField) updatedField).getEnums().indexOf(enabledValue) == -1) {
            return;
        }

        context.addMigration(FIELD,
            new Constant(FIELD, updatedField.getUnits(), enabledValue, updatedField.getDigits()));
    }

    private static boolean isRelease(String signature, String release) {
        final RusEfiSignature parsed = SignatureHelper.parse(signature);
        return parsed != null && (release.equals(parsed.getBranch()) || (release + "-dev").equals(parsed.getBranch()));
    }
}
