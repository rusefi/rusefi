package com.rusefi.maintenance.migration.migrators;

import java.util.Optional;

import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.config.FieldType;

public enum STFTFieldMigrator implements TuneMigrator {
	INSTANCE;

	private static final String stftMaxAddTemplate = "stft_cellCfgs%_maxAdd";
	private static final String stftMaxRemoveTemplate = "stft_cellCfgs%_maxRemove";
	private static final int stftCellCount = 4;

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		for (int stftCurrentCell = 1; stftCurrentCell <= stftCellCount; stftCurrentCell++) {

			// try migrating first the maxAdd:
			{
				final String stftMaxAddField = stftMaxAddTemplate.replace("%", Integer.toString(stftCurrentCell));

				final Optional<IniField> prevMaxAddIniField = context.getPrevIniFile().findIniField(stftMaxAddField);
				final Optional<IniField> updatedMaxAddIniField = context.getUpdatedIniFile()
						.findIniField(stftMaxAddField);

				if (prevMaxAddIniField.isPresent() && updatedMaxAddIniField.isPresent()) {
					// we only want to migrate if the previous is INT8
					final Boolean ifValidMigration = ((ScalarIniField) prevMaxAddIniField.get())
							.getType() == FieldType.INT8;

					if (ifValidMigration) {
						final Constant prevMaxAddConst = context.getPrevTune().getConstantsAsMap().get(stftMaxAddField);
						final Constant updatedMaxAddConst = context.getUpdatedTune().getConstantsAsMap()
								.get(stftMaxAddField);
						final String prevMaxAddValue = prevMaxAddConst.getValue();

						if (prevMaxAddConst != null && updatedMaxAddConst != null) {
							final Constant migratedMaxAddConst = updatedMaxAddConst
									.cloneWithValue(String.valueOf(prevMaxAddValue));
							context.addMigration(stftMaxAddField, migratedMaxAddConst);
						}
					}
				}
			}

			{
				final String stftMaxRemoveField = stftMaxRemoveTemplate.replace("%", Integer.toString(stftCurrentCell));

				final Optional<IniField> prevMaxRemoveIniField = context.getPrevIniFile()
						.findIniField(stftMaxRemoveField);
				final Optional<IniField> updatedMaxRemoveIniField = context.getUpdatedIniFile()
						.findIniField(stftMaxRemoveField);

				if (prevMaxRemoveIniField.isPresent() && updatedMaxRemoveIniField.isPresent()) {
					// we only want to migrate if the previous is INT8
					final Boolean ifValidMigration = ((ScalarIniField) prevMaxRemoveIniField.get())
							.getType() == FieldType.INT8;

					if (ifValidMigration) {
						final Constant prevMaxRemoveConst = context.getPrevTune().getConstantsAsMap()
								.get(stftMaxRemoveField);
						final Constant updatedMaxRemoveConst = context.getUpdatedTune().getConstantsAsMap()
								.get(stftMaxRemoveField);
						final Double prevMaxRemoveValue = Double.parseDouble(prevMaxRemoveConst.getValue());

						if (prevMaxRemoveConst != null && updatedMaxRemoveConst != null) {
							final Constant migratedStftMaxRemoveConst = updatedMaxRemoveConst
									// abs to change sign
									.cloneWithValue(String.valueOf(Math.abs(prevMaxRemoveValue)));
							context.addMigration(stftMaxRemoveField, migratedStftMaxRemoveConst);
						}
					}
				}
			}

		}
	}
}
