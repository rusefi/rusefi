package com.irnems.models;

/**
 * @author Andrey Belomutskiy
 *         1/29/13
 */
public class MafValue {

    private final int value;

    public MafValue(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public static MafValue valueOf(String value) {
        return new MafValue(Integer.parseInt(value));
    }

    @Override
    public String toString() {
        return "Maf{" +
                value +
                '}';
    }
}
