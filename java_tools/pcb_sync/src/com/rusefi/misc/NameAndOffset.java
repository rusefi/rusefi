package com.rusefi.misc;

/**
 * @author Andrey Belomutskiy
 *         1/24/14
 */
public class NameAndOffset {
    private final String name;
    public final double x;
    public final double y;

    public NameAndOffset(String name, double x, double y) {
        this.name = name;
        this.x = x;
        this.y = y;
    }

    static NameAndOffset parseNameAndOffset(String request) {
        String[] tokens = request.split(" ");
        NameAndOffset result;
        if (tokens.length == 1) {
            result = new NameAndOffset(tokens[0], 0, 0);
        } else if (tokens.length == 3) {
            double x = Double.parseDouble(tokens[1]);
            double y = Double.parseDouble(tokens[2]);
            result = new NameAndOffset(tokens[0], x, y);
        } else {
            throw new IllegalArgumentException("Invalid: " + request);
        }
        return result;
    }

    public String getName() {
        return name;
    }
}
