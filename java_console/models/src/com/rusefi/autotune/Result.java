package com.rusefi.autotune;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 2/23/2016.
 */
public class Result {
    private final float[][] kgbcRES;

    public Result(float[][] kgbcRES) {
        this.kgbcRES = kgbcRES;
    }

    public float[][] getKgbcRES() {
        return kgbcRES;
    }
}
