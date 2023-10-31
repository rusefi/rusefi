package com.rusefi.tracing.test;

import com.rusefi.tracing.Entry;
import com.rusefi.tracing.JsonOutput;
import com.rusefi.tracing.Phase;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class PerfJsonOutputTest {
    @Test
    public void test() throws IOException {
        List<Entry> testEntries = Arrays.asList(
                new Entry("hello", Phase.B, 0.1, 0, 0),
                new Entry("hello2", Phase.B, 0.2, 0, 0),
                new Entry("hello2", Phase.E, 0.3, 0, 0),
                new Entry("hello", Phase.E, 0.4, 0, 0)
        );

        ByteArrayOutputStream baos = new ByteArrayOutputStream();

        JsonOutput.writeToStream(testEntries, baos);

        assertEquals("{\"traceEvents\": [\n" +
                "{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":-16,\"tid\":0,\"args\":{\"name\":\"Main\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":-1,\"tid\":0,\"args\":{\"name\":\"SysTick\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":18,\"tid\":0,\"args\":{\"name\":\"ADC\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":19,\"tid\":0,\"args\":{\"name\":\"CAN1 TX\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":20,\"tid\":0,\"args\":{\"name\":\"CAN1 RX 0\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":21,\"tid\":0,\"args\":{\"name\":\"CAN1 RX 1\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":22,\"tid\":0,\"args\":{\"name\":\"CAN1 SCE\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":29,\"tid\":0,\"args\":{\"name\":\"TIM3\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":30,\"tid\":0,\"args\":{\"name\":\"TIM4\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":44,\"tid\":0,\"args\":{\"name\":\"TIM8/13\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":50,\"tid\":0,\"args\":{\"name\":\"TIM5\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":54,\"tid\":0,\"args\":{\"name\":\"TIM6\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":57,\"tid\":0,\"args\":{\"name\":\"DMA2s1\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":58,\"tid\":0,\"args\":{\"name\":\"DMA2s2\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":59,\"tid\":0,\"args\":{\"name\":\"DMA2s3\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":60,\"tid\":0,\"args\":{\"name\":\"DMA2s4\"}},{\"name\":\"process_name\",\"ph\":\"M\",\"pid\":67,\"tid\":0,\"args\":{\"name\":\"USB\"}}\n" +
                ",{\"name\":\"hello\",\"ph\":\"B\",\"tid\":0,\"pid\":0,\"ts\":0.1}\n" +
                ",{\"name\":\"hello2\",\"ph\":\"B\",\"tid\":0,\"pid\":0,\"ts\":0.2}\n" +
                ",{\"name\":\"hello2\",\"ph\":\"E\",\"tid\":0,\"pid\":0,\"ts\":0.3}\n" +
                ",{\"name\":\"hello\",\"ph\":\"E\",\"tid\":0,\"pid\":0,\"ts\":0.4}\n" +
                "]}", baos.toString());
    }
}
