package com.rusefi.autotune;

/**
 * (c) Andrey Belomutskiy 2013-2017
 * 2/23/2016.
 */
public class Result {
    private final double[][] kgbcRES;

    public Result(double[][] kgbcRES) {
        this.kgbcRES = kgbcRES;
    }

    public double[][] getKgbcRES() {
        return kgbcRES;
    }
}
