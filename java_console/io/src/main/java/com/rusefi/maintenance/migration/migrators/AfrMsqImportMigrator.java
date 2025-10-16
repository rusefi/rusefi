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

			String[][] migratedValues = prevTableField.getValues(prevAfrTableData.getValue());
			final Double prevStoichRatioValue = Double.parseDouble(prevStoichRatioConstant.getValue());

			// Check if table sizes match, if not, resample first
			final int prevRows = prevTableField.getRows();
			final int prevCols = prevTableField.getCols();
			final int updatedRows = updatedTableField.getRows();
			final int updatedCols = updatedTableField.getCols();

			if (prevRows != updatedRows || prevCols != updatedCols) {
				if (prevRows < updatedRows || prevCols < updatedCols) {
					// Need to upsample
					migratedValues = upsampleTable(migratedValues, updatedCols, updatedRows,
							prevTableField.getDigits());
				} else {
					// Need to downsample
					migratedValues = downsampleTable(migratedValues, updatedCols, updatedRows);
				}
			}

			// we check the previous units as a way to know what the user configuration was
			// (since only one of the two branches of the #if LAMBDA is saved on the msq)
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

			 context.addMigration(
					 updatedTableField.getName(),
					 new Constant(
                         updatedTableField.getName(),
                         updatedTableField.getUnits(),
                         updatedTableField.formatValue(migratedValues),
                         updatedTableField.getDigits(),
                         Integer.toString(updatedRows),
                         Integer.toString(updatedCols)
                     )
                 );
		}

	}

	private static String formatArrayValue(final double value, final String digits) {
		final int numDigits = Integer.parseInt(digits);
		return String.format(Locale.US, "%." + numDigits + "f", value);
	}

	/**
	 * Upsamples a table by interpolating values between existing data points. The
	 * upsampled values are replicated across all rows of the resulting table if the
	 * input is a 1D curve. If the input is a 2D table, it interpolates in both
	 * dimensions.
	 *
	 * @param prevValues The original curve/table values as a 2D array
	 * @param columnSize The number of columns in the resulting table
	 * @param rowSize    The number of rows in the resulting table
	 * @param digits     The number of decimal places for formatting, same as the
	 *                   .ini ones!!
	 * @return A 2D array representing the upsampled table values
	 */
	public static String[][] upsampleTable(final String[][] prevValues, final int columnSize, final int rowSize,
			final String digits) {
		final String[][] migratedTableValues = new String[rowSize][columnSize];

		final int prevRowSize = prevValues.length;
		final int prevColSize = prevValues[0].length;

		// 2D table interpolation - interpolate in both dimensions
		// Parse previous values to doubles for interpolation
		final double[][] prevDoubleValues = new double[prevRowSize][prevColSize];
		for (int row = 0; row < prevRowSize; row++) {
			for (int col = 0; col < prevColSize; col++) {
				prevDoubleValues[row][col] = Double.parseDouble(prevValues[row][col]);
			}
		}

		// Calculate the scaling factors for interpolation
		final double colScale = (double) (prevColSize - 1) / (columnSize - 1);
		final double rowScale = (double) (prevRowSize - 1) / (rowSize - 1);

		// Interpolate values for the new table in both dimensions
		for (int row = 0; row < rowSize; row++) {
			final double rowPosition = row * rowScale;
			final int lowerRowIndex = (int) rowPosition;
			final int upperRowIndex = Math.min(lowerRowIndex + 1, prevRowSize - 1);
			final double rowFraction = rowPosition - lowerRowIndex;

			for (int col = 0; col < columnSize; col++) {
				final double colPosition = col * colScale;
				final int lowerColIndex = (int) colPosition;
				final int upperColIndex = Math.min(lowerColIndex + 1, prevColSize - 1);
				final double colFraction = colPosition - lowerColIndex;

				// Bilinear interpolation
				final double interpolatedValue;
				if (lowerRowIndex == upperRowIndex && lowerColIndex == upperColIndex) {
					// No interpolation needed
					interpolatedValue = prevDoubleValues[lowerRowIndex][lowerColIndex];
				} else if (lowerRowIndex == upperRowIndex) {
					// Only column interpolation
					interpolatedValue = prevDoubleValues[lowerRowIndex][lowerColIndex] * (1.0 - colFraction)
							+ prevDoubleValues[lowerRowIndex][upperColIndex] * colFraction;
				} else if (lowerColIndex == upperColIndex) {
					// Only row interpolation
					interpolatedValue = prevDoubleValues[lowerRowIndex][lowerColIndex] * (1.0 - rowFraction)
							+ prevDoubleValues[upperRowIndex][lowerColIndex] * rowFraction;
				} else {
					// Bilinear interpolation in both dimensions
					final double topInterpolated = prevDoubleValues[lowerRowIndex][lowerColIndex] * (1.0 - colFraction)
							+ prevDoubleValues[lowerRowIndex][upperColIndex] * colFraction;
					final double bottomInterpolated = prevDoubleValues[upperRowIndex][lowerColIndex]
							* (1.0 - colFraction) + prevDoubleValues[upperRowIndex][upperColIndex] * colFraction;
					interpolatedValue = topInterpolated * (1.0 - rowFraction) + bottomInterpolated * rowFraction;
				}

				// Format the value according to the field's digits precision
				migratedTableValues[row][col] = formatArrayValue(interpolatedValue, digits);
			}
		}

		return migratedTableValues;
	}

	/**
	 * Downsamples a curve/table by selecting values from the start, middle, and end
	 * sections. For 1D curves, the downsampled values are replicated across all
	 * rows of the resulting table. For 2D tables, downsampling is applied in both
	 * dimensions.
	 *
	 * @param prevValues The original curve/table values as a 2D array
	 * @param columnSize The number of columns in the resulting table
	 * @param rowSize    The number of rows in the resulting table
	 * @return A 2D array representing the downsampled table values
	 */
	public static String[][] downsampleTable(final String[][] prevValues, final int columnSize, final int rowSize) {
		final int prevRowSize = prevValues.length;
		final int prevColSize = prevValues[0].length;

		final String[][] migratedTableValues = new String[rowSize][columnSize];

		// 2D table downsampling - downsample in both dimensions
		// Calculate how many values to take from the start, middle, and end for columns
		final int newColStartSize = columnSize / 3;
		final int newColEndSize = columnSize / 3;
		final int newColMiddleSize = columnSize - newColStartSize - newColEndSize;

		final int colMiddleStartIndex = (prevColSize - newColMiddleSize) / 2;

		// Calculate how many values to take from the start, middle, and end for rows
		final int newRowStartSize = rowSize / 3;
		final int newRowEndSize = rowSize / 3;
		final int newRowMiddleSize = rowSize - newRowStartSize - newRowEndSize;

		final int rowMiddleStartIndex = (prevRowSize - newRowMiddleSize) / 2;

		int destRow = 0;

		// Copy from start row section
		for (int srcRow = 0; srcRow < newRowStartSize; srcRow++) {
			int destCol = 0;

			// Copy from start column section
			for (int srcCol = 0; srcCol < newColStartSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[srcRow][srcCol];
			}

			// Copy from middle column section
			for (int srcCol = 0; srcCol < newColMiddleSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[srcRow][colMiddleStartIndex + srcCol];
			}

			// Copy from end column section
			for (int srcCol = 0; srcCol < newColEndSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[srcRow][prevColSize - newColEndSize + srcCol];
			}

			destRow++;
		}

		// Copy from middle row section
		for (int srcRow = 0; srcRow < newRowMiddleSize; srcRow++) {
			int destCol = 0;

			// Copy from start column section
			for (int srcCol = 0; srcCol < newColStartSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[rowMiddleStartIndex + srcRow][srcCol];
			}

			// Copy from middle column section
			for (int srcCol = 0; srcCol < newColMiddleSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[rowMiddleStartIndex + srcRow][colMiddleStartIndex
						+ srcCol];
			}

			// Copy from end column section
			for (int srcCol = 0; srcCol < newColEndSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[rowMiddleStartIndex + srcRow][prevColSize
						- newColEndSize + srcCol];
			}

			destRow++;
		}

		// Copy from end row section
		for (int srcRow = 0; srcRow < newRowEndSize; srcRow++) {
			int destCol = 0;

			// Copy from start column section
			for (int srcCol = 0; srcCol < newColStartSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[prevRowSize - newRowEndSize + srcRow][srcCol];
			}

			// Copy from middle column section
			for (int srcCol = 0; srcCol < newColMiddleSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[prevRowSize - newRowEndSize
						+ srcRow][colMiddleStartIndex + srcCol];
			}

			// Copy from end column section
			for (int srcCol = 0; srcCol < newColEndSize; srcCol++) {
				migratedTableValues[destRow][destCol++] = prevValues[prevRowSize - newRowEndSize + srcRow][prevColSize
						- newColEndSize + srcCol];
			}

			destRow++;
		}

		return migratedTableValues;
	}

}
