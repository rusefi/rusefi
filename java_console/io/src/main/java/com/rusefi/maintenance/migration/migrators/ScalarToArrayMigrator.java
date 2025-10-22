package com.rusefi.maintenance.migration.migrators;

import java.util.Optional;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

class scalarMigration {
	private String prevFieldName; // ex: afterCrankingIACtaperDuration
	private String updatedFieldName;

	public scalarMigration(String prevFieldName, String updatedFieldName) {
		this.prevFieldName = prevFieldName;
		this.updatedFieldName = updatedFieldName;
	}

	public String getPrevFieldName() {
		return prevFieldName;
	}

	public String getUpdatedFieldName() {
		return updatedFieldName;
	}

}

public enum ScalarToArrayMigrator implements TuneMigrator {
	INSTANCE;

	private static final scalarMigration[] fieldsToMigrate = {
			new scalarMigration("afterCrankingIACtaperDuration", "afterCrankingIACtaperDuration"), };

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		for (scalarMigration field : fieldsToMigrate) {

			final Optional<IniField> prevField = context.getPrevIniFile().findIniField(field.getPrevFieldName());
			final Optional<IniField> updatedField = context.getUpdatedIniFile()
					.findIniField(field.getUpdatedFieldName());
			final Constant prevConst = context.getPrevTune().getConstantsAsMap().get(field.getPrevFieldName());
			final Constant updatedConst = context.getUpdatedTune().getConstantsAsMap().get(field.getUpdatedFieldName());

			if (!prevField.isPresent() || !updatedField.isPresent() || prevConst == null || updatedConst == null) {
				continue;
			}

			if (updatedField.get() instanceof ArrayIniField && prevField.get() instanceof ScalarIniField) {
				ArrayIniField migratedField = (ArrayIniField) updatedField.get();
				
				String[][] migratedValues = migratedField.getValues(updatedConst.getValue());
				final String prevValue = prevConst.getValue();

				for (int row = 0; row < migratedValues.length; row++) {
					for (int column = 0; column < migratedValues[0].length; column++) {
						migratedValues[row][column] = prevValue;
					}
				}
				
				
				 context.addMigration(
						 migratedField.getName(),
						 new Constant(
								 migratedField.getName(),
								 migratedField.getUnits(),
	                         migratedField.formatValue(migratedValues),
	                         migratedField.getDigits(),
	                         Integer.toString(migratedField.getRows()),
	                         Integer.toString(migratedField.getCols())
	                     )
	                 );
			}
		}
	}
}