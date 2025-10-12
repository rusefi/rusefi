package com.rusefi.maintenance.migration.migrators;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.generateConstant;

import java.util.Locale;
import java.util.Optional;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

public enum AfrMsqImportMigrator implements TuneMigrator {
	INSTANCE;

	public static final String STOICH_RATIO_NAME = "stoichRatioPrimary";
	public static final String AFR_TABLE_NAME = "lambdaTable";

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		final Optional<IniField> prevAfrTable = context.getPrevIniFile().findIniField(AFR_TABLE_NAME);
		final Optional<IniField> updatedAfrTable = context.getUpdatedIniFile().findIniField(AFR_TABLE_NAME);
		final Optional<IniField> prevStoichRatio = context.getPrevIniFile().findIniField(STOICH_RATIO_NAME);

		if (!prevAfrTable.isPresent() || !updatedAfrTable.isPresent() || !prevStoichRatio.isPresent()) {
			return;
		}

		final Constant prevAfrTableData = context.getPrevTune().getConstantsAsMap().get(AFR_TABLE_NAME);
		final Constant updatedAfrTableData = context.getUpdatedTune().getConstantsAsMap().get(AFR_TABLE_NAME);
		final Constant prevStoichRatioConstant = context.getPrevTune().getConstantsAsMap().get(STOICH_RATIO_NAME);

		if (prevAfrTableData != null && updatedAfrTableData != null && prevStoichRatioConstant != null) {

			final String prevLambdaTableUnits = prevAfrTableData.getUnits();
			final String updatedLambdaTableUnits = updatedAfrTableData.getUnits();

			if (prevLambdaTableUnits == null || updatedLambdaTableUnits == null) {
				return;
			}

			// migration if valid only if units are different!
			if (prevLambdaTableUnits.contains(updatedLambdaTableUnits)) {
				return;
			}

			final ArrayIniField prevTableField = (ArrayIniField) prevAfrTable.get();
			final ArrayIniField updatedTableField = (ArrayIniField) updatedAfrTable.get();

			final String[][] migratedValues = prevTableField.getValues(prevAfrTableData.getValue());
			final Double prevStoichRatioValue = Double.parseDouble(prevStoichRatioConstant.getValue());

            // we check the previous units as a way to know what the user configuration was (since only one of the two branches of the #if LAMBDA is saved on the msq)
			if (prevLambdaTableUnits.contains("afr")) {
				for (int row = 0; row < migratedValues.length; row++) {
					for (int column = 0; column < migratedValues[0].length; column++) {
						Double migratedLambda = Double.parseDouble(migratedValues[row][column]) / prevStoichRatioValue;
						migratedValues[row][column] = formatArrayValue(migratedLambda, prevTableField.getDigits());
					}
				}
			} else {
				for (int row = 0; row < migratedValues.length; row++) {
					for (int column = 0; column < migratedValues[0].length; column++) {
						Double migratedAfr = Double.parseDouble(migratedValues[row][column]) * prevStoichRatioValue;
						migratedValues[row][column] = formatArrayValue(migratedAfr, prevTableField.getDigits());
					}
				}
			}

			context.addMigration(AFR_TABLE_NAME,
					generateConstant(updatedTableField, updatedTableField.formatValue(migratedValues)));

		}

	}

	private static String formatArrayValue(final double value, final String digits) {
		final int numDigits = Integer.parseInt(digits);
		return String.format(Locale.US, "%." + numDigits + "f", value);
	}

}
