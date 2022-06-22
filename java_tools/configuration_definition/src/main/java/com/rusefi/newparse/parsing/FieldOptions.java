package com.rusefi.newparse.parsing;

import java.io.PrintStream;

public class FieldOptions {
    public double min;
    public double max;
    public double scale;
    public double offset;
    public int digits;
    public String units;
    public String comment;

    public FieldOptions() {
        min = 0;
        max = 0;
        scale = 1;
        offset = 0;
        digits = 0;
        units = "\"\"";
        comment = "";
    }

    // Produce a deep copy of this object
    public FieldOptions copy() {
        FieldOptions other = new FieldOptions();

        other.min = this.min;
        other.max = this.max;
        other.scale = this.scale;
        other.offset = this.offset;
        other.digits = this.digits;
        other.units = this.units;
        other.comment = this.comment;

        return other;
    }

    public static String tryRound(double value) {
        long longVal = Math.round(value);

        // If the rounded value can exactly represent this float, then print as an integer
        if (value == longVal) {
            return Long.toString(longVal);
        } else {
            return Double.toString(value);
        }
    }

    public void printTsFormat(PrintStream ps) {
        ps.print(units);
        ps.print(", ");
        ps.print(tryRound(scale));
        ps.print(", ");
        ps.print(tryRound(offset));
        ps.print(", ");
        ps.print(tryRound(min));
        ps.print(", ");
        ps.print(tryRound(max));
        ps.print(", ");
        ps.print(digits);
    }
}
