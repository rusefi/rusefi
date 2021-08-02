package com.rusefi.newparse.parsing;

import java.util.Arrays;
import java.util.Optional;

public enum Type {
    U08("uint8_t", "U08", 1),
    S08("int8_t", "S08", 1),
    U16("uint16_t", "U16", 2),
    S16("int16_t", "S16", 2),
    U32("uint32_t", "U32", 4),
    S32("int32_t", "S32", 4),
    F32("float", "F32", 4),
    ANGLE_T("angle_t", "F32", 4),

    // TODO: remove I32
    I32("int", "S32", 4);

    public final String cType;
    public final String tsType;
    public final int size;

    Type(String cType, String tunerstudioType, int size) {
        this.cType = cType;
        this.tsType = tunerstudioType;
        this.size = size;
    }

    public static Optional<Type> findByCtype(String cType) {
        return Arrays.stream(Type.values()).filter(t -> t.cType.equals(cType)).findFirst();
    }

    public static Type findByTsType(String tsType) {
        return Arrays.stream(Type.values()).filter(t -> t.tsType.equals(tsType)).findFirst().get();
    }
}
