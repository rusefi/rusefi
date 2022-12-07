package com.rusefi.newparse.parsing;

public class ArrayField<PrototypeType extends PrototypeField> implements Field {
    public final int[] length;
    public final Boolean iterate;
    public final PrototypeType prototype;

    public ArrayField(PrototypeType prototype, int[] length, Boolean iterate) {
        this.length = length;
        this.iterate = iterate;
        this.prototype = prototype;
    }
}
