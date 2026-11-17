package com.opensr5.ini.field;

/**
 * An array declared in the [PcVariables] section, for example the gear numbers used as curve bins.
 * It has no ECU storage: its values come from the INI 'defaultValue' line and are zero without one.
 * The console keeps no per-tune copy, so these values cannot be edited.
 */
public class PcVariableArrayIniField extends ArrayIniField {
    private final Double[][] values;

    public PcVariableArrayIniField(ArrayIniField definition) {
        super(definition.getName(), 0, definition.getType(), definition.getCols(), definition.getRows(),
            definition.getUnits(), definition.getMultiplier(), definition.getMin(), definition.getMax(),
            definition.getDigits());
        values = new Double[getRows()][getCols()];
        for (Double[] row : values) {
            java.util.Arrays.fill(row, 0.0);
        }
    }

    /**
     * @param defaults values in display units, row by row; a list of another length is ignored
     * @return true if the values were accepted
     */
    public boolean setDefaultValues(double[] defaults) {
        if (defaults.length != getRows() * getCols()) {
            return false;
        }
        for (int i = 0; i < defaults.length; i++) {
            values[i / getCols()][i % getCols()] = defaults[i];
        }
        return true;
    }

    public Double[][] getValues() {
        Double[][] copy = new Double[values.length][];
        for (int i = 0; i < values.length; i++) {
            copy[i] = values[i].clone();
        }
        return copy;
    }
}
