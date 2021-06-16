package com.rusefi.newparse.parsing;

import java.io.PrintStream;

public class FieldOptions {
    public float min;
    public float max;
    public float scale;
    public float offset;
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

    public void printTsFormat(PrintStream ps) {
        ps.print(units);
        ps.print(", ");
        ps.print(scale);
        ps.print(", ");
        ps.print(offset);
        ps.print(", ");
        ps.print(min);
        ps.print(", ");
        ps.print(max);
        ps.print(", ");
        ps.print(digits);
    }
}
