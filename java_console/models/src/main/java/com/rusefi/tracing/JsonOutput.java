package com.rusefi.tracing;

import com.rusefi.PerfTraceTool;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.List;

/**
 * This class helps to write JSON files readable by chrome://tracing/
 * <p>
 * See https://github.com/catapult-project/catapult/blob/master/tracing/README.md
 * @see PerfTraceTool code generator for EnumNames.java
 */
public class JsonOutput {

    /**
     * those are special entries that change display settings
     * those set thread names and sort index based on those thread IDs
     * (those process IDs are interrupt numbers, and the name is the name of the interrupt handler)
     */
    private static final String FORMATTING_SETTINGS =
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":-16,\"tid\":0,\"args\":{\"name\":\"Main\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":-1,\"tid\":0,\"args\":{\"name\":\"SysTick\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":18,\"tid\":0,\"args\":{\"name\":\"ADC\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":19,\"tid\":0,\"args\":{\"name\":\"CAN1 TX\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":20,\"tid\":0,\"args\":{\"name\":\"CAN1 RX 0\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":21,\"tid\":0,\"args\":{\"name\":\"CAN1 RX 1\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":22,\"tid\":0,\"args\":{\"name\":\"CAN1 SCE\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":29,\"tid\":0,\"args\":{\"name\":\"TIM3\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":30,\"tid\":0,\"args\":{\"name\":\"TIM4\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":44,\"tid\":0,\"args\":{\"name\":\"TIM8/13\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":50,\"tid\":0,\"args\":{\"name\":\"TIM5\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":54,\"tid\":0,\"args\":{\"name\":\"TIM6\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":57,\"tid\":0,\"args\":{\"name\":\"DMA2s1\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":58,\"tid\":0,\"args\":{\"name\":\"DMA2s2\"}}," +
			"{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":59,\"tid\":0,\"args\":{\"name\":\"DMA2s3\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":60,\"tid\":0,\"args\":{\"name\":\"DMA2s4\"}}," +
            "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":67,\"tid\":0,\"args\":{\"name\":\"USB\"}}"
            ;
    private static final String EOL = "\n";

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
