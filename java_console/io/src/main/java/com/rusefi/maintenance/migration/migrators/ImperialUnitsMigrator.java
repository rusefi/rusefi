package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

public enum ImperialUnitsMigrator implements TuneMigrator {
	INSTANCE;

	public static final String USE_METRIC_FIELD_NAME = "useMetricOnInterface";
    public static final String USE_METRIC_FIELD_VALUE = "Metric";

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		final Optional<IniField> prevUseMetric = context.getPrevIniFile().findIniField(USE_METRIC_FIELD_NAME);
		final Optional<IniField> updatedUseMetric = context.getUpdatedIniFile().findIniField(USE_METRIC_FIELD_NAME);

        // migration is only valid where the useMetric is introduced, not if it exists on the prev tune, otherwise we are overriding the user preference
		if (prevUseMetric.isPresent() || !updatedUseMetric.isPresent()) {
			return;
		}

		final Constant updatedUseMetricConstant = context.getUpdatedTune().getConstantsAsMap().get(USE_METRIC_FIELD_NAME);

        if(updatedUseMetricConstant != null ){
            final Constant migratedBooleanConst = updatedUseMetricConstant.cloneWithValue(USE_METRIC_FIELD_VALUE);
            context.addMigration(USE_METRIC_FIELD_NAME, migratedBooleanConst);
        }

	}

}
