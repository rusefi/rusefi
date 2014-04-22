package com.rusefi.misc;

/**
 * @author Andrey Belomutskiy
 *         1/24/14
 */
public class TwoFileRequest {
    public final String input;
    public final String output;

    public TwoFileRequest(String input, String output) {
        this.input = input;
        this.output = output;
    }

    static TwoFileRequest parseTwoFile(String request, int lineIndex) {
        String[] tokens = request.split(" ");
        if (tokens.length != 2)
            throw new IllegalArgumentException("Unexpected token count in [" + request + "] @" + lineIndex);

        return new TwoFileRequest(tokens[0], tokens[1]);
    }
}
