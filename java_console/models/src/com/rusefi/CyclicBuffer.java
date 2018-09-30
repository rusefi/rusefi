package com.rusefi;

import java.util.Arrays;

/**
 * Fixed-size array backed buffer
 */
public class CyclicBuffer implements DataBuffer {
    private final double values[];

    private int size = 0;
    private int pointer = 0;

    public CyclicBuffer(int maxSize) {
        values = new double[maxSize];
    }

    public void add(double value) {
        if (size < values.length)
            size++;
        values[pointer] = value;
        pointer++;
        if (pointer == values.length)
            pointer = 0;
    }

    public int getSize() {
        return size;
    }

    @Override
    public double[] getValues() {
        return Arrays.copyOfRange(values, 0, size);
    }

    public int getMaxSize() {
        return values.length;
    }

    public double get(int i) {
        return values[i];
    }
}