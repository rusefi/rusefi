package com.rusefi.maintenance.migration.migrators;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.generateConstant;

import java.util.Locale;
import java.util.Optional;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

public enum IdleCurveMigrator implements TuneMigrator {
	INSTANCE;

	public static final String IDLE_CURVE_NAME = "cltIdleCorr";

	public static final String IDLE_TABLE_NAME = "cltIdleCorrTable";

	@Override
	public void migrateTune(final TuneMigrationContext context) {
		final Optional<IniField> prevIdleCurve = context.getPrevIniFile().findIniField(IDLE_CURVE_NAME);
		final Optional<IniField> updatedIdleTable = context.getUpdatedIniFile().findIniField(IDLE_TABLE_NAME);

		if (!prevIdleCurve.isPresent() || !updatedIdleTable.isPresent()) {
			return;
		}

		final Constant prevIdleCurveData = context.getPrevTune().getConstantsAsMap().get(IDLE_CURVE_NAME);
        final ArrayIniField prevCurveField = (ArrayIniField) prevIdleCurve.get();
		final int prevCurveRowSize = prevCurveField.getRows();

        final ArrayIniField updatedTableField = (ArrayIniField) updatedIdleTable.get();
		final int columnSize = updatedTableField.getCols();

		if (prevCurveRowSize == columnSize) {
			// same bin size, so we copy as-is
			final String[][] prevValues = prevCurveField.getValues(prevIdleCurveData.getValue());
			final int colCount = updatedTableField.getCols();

			final String[][] migratedTableValues = new String[updatedTableField.getRows()][colCount];

			//TODO: extract if useful for another new migration?
			for (int row = 0; row < updatedTableField.getRows(); row++) {
				for (int col = 0; col < colCount; col++) {
					migratedTableValues[row][col] = prevValues[col][0];
				}
			}

			context.addMigration(IDLE_CURVE_NAME, IDLE_TABLE_NAME,
					generateConstant(updatedTableField, updatedTableField.formatValue(migratedTableValues)));
		} else if (prevCurveRowSize < columnSize) {
			// newer table has more bins
			final String[][] prevValues = prevCurveField.getValues(prevIdleCurveData.getValue());
			final String[][] migratedTableValues = upsampleCurveToTable(prevValues, prevCurveRowSize, columnSize,
					updatedTableField.getRows(), prevCurveField.getDigits());
			
			context.addMigration(IDLE_CURVE_NAME, IDLE_TABLE_NAME,
					generateConstant(updatedTableField, updatedTableField.formatValue(migratedTableValues)));
		} else {
			// newer table has less bins, keep values from the start, middle, and end of the curve
			final String[][] prevValues = prevCurveField.getValues(prevIdleCurveData.getValue());
			final String[][] migratedTableValues = downsampleCurveToTable(prevValues, prevCurveRowSize,
					columnSize, updatedTableField.getRows());

			context.addMigration(IDLE_CURVE_NAME, IDLE_TABLE_NAME,
					generateConstant(updatedTableField, updatedTableField.formatValue(migratedTableValues)));
		}
	}

	/**
	 * Downsamples a curve to a table by selecting values from the start, middle,
	 * and end sections. The downsampled values are replicated across all rows of
	 * the resulting table.
	 *
	 * @param prevValues        The original curve values as a 2D array
	 * @param curveRowSize  	The size of the original curve
	 * @param columnSize 		The number of columns in the resulting table
	 * @param rowSize           The number of rows in the resulting table
	 * @return A 2D array representing the downsampled table values
	 */
	 //TODO: extract if useful for another new migration?
	public static String[][] downsampleCurveToTable(final String[][] prevValues, final int curveRowSize,
			final int columnSize, final int rowSize) {
		// Calculate how many values to take from the start, middle, and end (ideally 1/3 ?)
		final int newStartSize = columnSize / 3;
		final int newEndSize = columnSize / 3;
		final int newMiddleSize = columnSize - newStartSize - newEndSize;

		final int middleStartIndex = (curveRowSize - newMiddleSize) / 2;
		final String[][] migratedTableValues = new String[rowSize][columnSize];

		int destCol = 0;

		// Copy from start section
		for (int i = 0; i < newStartSize; i++) {
			migratedTableValues[0][destCol++] = prevValues[i][0];
		}

		// Copy from middle section
		for (int i = 0; i < newMiddleSize; i++) {
			migratedTableValues[0][destCol++] = prevValues[middleStartIndex + i][0];
		}

		// Copy from end section
		for (int i = 0; i < newEndSize; i++) {
			migratedTableValues[0][destCol++] = prevValues[curveRowSize - newEndSize + i][0];
		}

		// replicate on the other rows:
		for (int i = 1; i < rowSize; i++) {
			migratedTableValues[i] = migratedTableValues[0];
		}

		return migratedTableValues;
	}

	/**
	 * Upsamples a curve to a table by interpolating values between existing data
	 * points. The upsampled values are replicated across all columns of the
	 * resulting table.
	 *
	 * @param prevValues        The original curve values as a 2D array
	 * @param curveRowSize		The size of the original curve
	 * @param columnSize 		The number of columns in the resulting table
	 * @param rowSize           The number of rows in the resulting table
	 * @param digits            The number of decimal places for formatting, same as the .ini ones!!
	 * @return A 2D array representing the upsampled table values
	 */
	public static String[][] upsampleCurveToTable(final String[][] prevValues, final int curveRowSize,
			final int columnSize, final int rowSize, final String digits) {
		// Create new table array for migrated values
		final String[][] migratedTableValues = new String[rowSize][columnSize];

		// Parse previous values to doubles for interpolation
		final double[] prevDoubleValues = new double[curveRowSize];
		for (int i = 0; i < curveRowSize; i++) {
			prevDoubleValues[i] = Double.parseDouble(prevValues[i][0]);
		}

		// Calculate the scaling factor for interpolation
		final double scale = (double) (curveRowSize - 1) / (columnSize - 1);

		// Interpolate values for the new table
		for (int i = 0; i < columnSize; i++) {
			final double position = i * scale;
			final int lowerIndex = (int) position;
			final int upperIndex = Math.min(lowerIndex + 1, curveRowSize - 1);

			final double interpolatedValue;
			if (lowerIndex == upperIndex) {
				interpolatedValue = prevDoubleValues[lowerIndex];
			} else {
				// Linear interpolation
				final double fraction = position - lowerIndex;
				interpolatedValue = prevDoubleValues[lowerIndex] * (1.0 - fraction)
						+ prevDoubleValues[upperIndex] * fraction;
			}

			// Format the value according to the field's digits precision
			migratedTableValues[0][i] = formatArrayValue(interpolatedValue, digits);
		}

		// Replicate on the other rows
		for (int i = 1; i < rowSize; i++) {
			migratedTableValues[i] = migratedTableValues[0];
		}

		return migratedTableValues;
	}

	public static String formatArrayValue(final double value, final String digits) {
		final int numDigits = Integer.parseInt(digits);
		return String.format(Locale.US, "%." + numDigits + "f", value);
	}

}
