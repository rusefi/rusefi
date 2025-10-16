package com.rusefi.maintenance.migration.migrators;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.generateConstant;

import java.util.Locale;
import java.util.Optional;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

public enum CrankingIdleMigrator implements TuneMigrator {
	INSTANCE;

	public static final String CRANKING_IAC_POSITION_CURVE = "cltCrankingCorr";
	public static final String CRANKING_IAC_POSITION_BASE = "crankingIACposition";

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		final Optional<IniField> prevCrankingIdlePosition = context.getPrevIniFile()
				.findIniField(CRANKING_IAC_POSITION_BASE);
		final Optional<IniField> updatedCrankingIdlePosition = context.getUpdatedIniFile()
				.findIniField(CRANKING_IAC_POSITION_BASE);

		final Optional<IniField> prevCrankingCurve = context.getPrevIniFile()
				.findIniField(CRANKING_IAC_POSITION_CURVE);
		final Optional<IniField> updatedCrankingCurve = context.getUpdatedIniFile()
				.findIniField(CRANKING_IAC_POSITION_CURVE);

		// please note the missing ! on updatedCrankingIdlePosition.isPresent
		//  migration is invalid if we are migration from a tune that still contains this field (ie, too old)
		// TODO: we can skip migrations by build/signature/memory version/date?
		if (!prevCrankingIdlePosition.isPresent() || updatedCrankingIdlePosition.isPresent()
				|| !prevCrankingCurve.isPresent() || !updatedCrankingCurve.isPresent()) {
			return;
		}

		final Constant prevCrankingCurveData = context.getPrevTune().getConstantsAsMap()
				.get(CRANKING_IAC_POSITION_CURVE);
		final Constant updatedCrankingCurveData = context.getUpdatedTune().getConstantsAsMap()
				.get(CRANKING_IAC_POSITION_CURVE);

		final Constant prevIacBasePosition = context.getPrevTune().getConstantsAsMap().get(CRANKING_IAC_POSITION_BASE);

		if (prevCrankingCurveData != null && updatedCrankingCurveData != null && prevIacBasePosition != null) {

			final ArrayIniField prevTableField = (ArrayIniField) prevCrankingCurve.get();
			final ArrayIniField updatedCrankingCurveField = (ArrayIniField) updatedCrankingCurve.get();

			final String[][] migratedValues = prevTableField.getValues(prevCrankingCurveData.getValue());
			final Double prevCrankingPosition = Double.parseDouble(prevIacBasePosition.getValue());

			for (int row = 0; row < migratedValues.length; row++) {
				for (int column = 0; column < migratedValues[0].length; column++) {
					Double migratedIACPosition = Double.parseDouble(migratedValues[row][column]) * prevCrankingPosition;
					migratedValues[row][column] = formatArrayValue(migratedIACPosition, prevTableField.getDigits());
				}
			}

			context.addMigration(CRANKING_IAC_POSITION_CURVE,
					generateConstant(updatedCrankingCurveField, updatedCrankingCurveField.formatValue(migratedValues)));

		}

	}

	private static String formatArrayValue(final double value, final String digits) {
		final int numDigits = Integer.parseInt(digits);
		return String.format(Locale.US, "%." + numDigits + "f", value);
	}

}
