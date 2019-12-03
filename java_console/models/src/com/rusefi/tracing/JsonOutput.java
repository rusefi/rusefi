package com.rusefi.tracing;

import java.io.*;
import java.util.Arrays;
import java.util.List;

/**
 * This class helps to write JSON files readable by chrome://tracing/
 * <p>
 * See https://github.com/catapult-project/catapult/blob/master/tracing/README.md
 * @see PerfTraceTool
 */
public class JsonOutput {

    /**
     * those are special entries that change display settings
     * those set thread names and sort index based on those thread IDs
     * (those thread IDs are interrupt numbers, and the name is the name of the interrupt handler)
     */
    private static final String FORMATTING_SETTINGS
            = "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":18,\"pid\":0,\"args\":{\"name\":\"ADC\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":29,\"pid\":0,\"args\":{\"name\":\"TIM3\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":30,\"pid\":0,\"args\":{\"name\":\"TIM4\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":44,\"pid\":0,\"args\":{\"name\":\"TIM8/13\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":50,\"pid\":0,\"args\":{\"name\":\"TIM5\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":58,\"pid\":0,\"args\":{\"name\":\"DMA2s2\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":60,\"pid\":0,\"args\":{\"name\":\"DMA2s4\"}}," +
            "{\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":67,\"pid\":0,\"args\":{\"name\":\"USB\"}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":18,\"pid\":0,\"args\":{\"sort_index\":4}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":29,\"pid\":0,\"args\":{\"sort_index\":2}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":30,\"pid\":0,\"args\":{\"sort_index\":5}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":44,\"pid\":0,\"args\":{\"sort_index\":7}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":50,\"pid\":0,\"args\":{\"sort_index\":3}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":58,\"pid\":0,\"args\":{\"sort_index\":6}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":60,\"pid\":0,\"args\":{\"sort_index\":8}}," +
            "{\"name\":\"thread_sort_index\",\"ph\":\"M\",\"tid\":67,\"pid\":0,\"args\":{\"sort_index\":9}}"
            ;
    private static final String EOL = "\r\n";

    public static void main(String[] args) throws IOException {
        List<Entry> testEntries = Arrays.asList(
                new Entry("hello", Phase.B, 0.1),
                new Entry("hello2", Phase.B, 0.2),
                new Entry("hello2", Phase.E, 0.3),
                new Entry("hello", Phase.E, 0.4)
        );

        writeToStream(testEntries, new FileOutputStream("hello_trace.json"));
    }

    public static void writeToStream(List<Entry> testEntries, OutputStream outputStream) throws IOException {

        Writer out = new OutputStreamWriter(outputStream);
        out.write("{\"traceEvents\": [" + EOL);

        out.write(FORMATTING_SETTINGS + EOL);

        for (Entry e : testEntries) {
            out.write(",");
            out.write(e.toString() + EOL);
        }

        out.write("]}");
        out.close();
    }
}
