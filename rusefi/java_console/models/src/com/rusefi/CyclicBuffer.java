package com.rusefi;

public class CyclicBuffer {
    private final double values[];

    private int size = 0;
    private int pointer = 0;

    public CyclicBuffer(int maxSize) {
        values = new double[maxSize];
    }

    public double getStandardDeviation() {
        double accumulator = 0;
        double avg = average();

        for (int i = 0; i < getSize(); i++) {
            double v = get(i) - avg;
            accumulator += v * v;
        }
        return Math.sqrt(accumulator);
    }

    public void add(double value) {
        if (size < values.length)
            size++;
        values[pointer] = value;
        pointer++;
        if (pointer == values.length)
            pointer = 0;
    }

    public double average() {
        return sum() / size;
    }

    public double sum() {
        double result = 0;
        for (int i = 0; i < size; i++)
            result += values[i];
        return result;
    }

    public int getSize() {
        return size;
    }

    public int getMaxSize() {
        return values.length;
    }

    public double get(int i) {
        return values[i];
    }
}