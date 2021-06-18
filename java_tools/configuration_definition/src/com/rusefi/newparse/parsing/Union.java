package com.rusefi.newparse.parsing;

import java.util.List;

public class Union implements Field {
    public final List<Field> fields;

    public Union(List<Field> fields) {
        this.fields = fields;
    }
}
