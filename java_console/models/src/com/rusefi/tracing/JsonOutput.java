package com.rusefi.tracing;

import java.io.*;
import java.util.Arrays;
import java.util.List;

/**
 * This class helps to write JSON files readable by chrome://tracing/
 * <p>
 * See https://github.com/catapult-project/catapult/blob/master/tracing/README.md
 */
public class JsonOutput {

    public static void main(String[] args) throws IOException {
        List<Entry> testEntries = Arrays.asList(
                new Entry("hello", Phase.B, 0.1),
                new Entry("hello2", Phase.B, 0.2),
                new Entry("hello2", Phase.E, 0.3),
                new Entry("hello", Phase.E, 0.4)
        );

        writeToStream(testEntries, new FileOutputStream("hello_trace.json"));
    }

    private static void writeToStream(List<Entry> testEntries, OutputStream outputStream) throws IOException {

        Writer out = new OutputStreamWriter(outputStream);
        out.write("{\"traceEvents\": [\n");

        boolean isFirst = true;
        for (Entry e : testEntries) {
            if (isFirst) {
                isFirst = false;
            } else {
                out.write(",");
            }
            out.write(e.toString() + "\r\n");
        }

        out.write("]}");
        out.close();
    }
}
