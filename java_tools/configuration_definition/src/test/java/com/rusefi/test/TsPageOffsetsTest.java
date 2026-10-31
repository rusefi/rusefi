package com.rusefi.test;

import com.rusefi.ReaderStateImpl;
import com.rusefi.TsFileContent;
import com.rusefi.VariableRegistry;
import com.rusefi.output.PlainTsProjectConsumer;
import com.rusefi.output.TsOutput;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.TreeSet;

import static org.junit.jupiter.api.Assertions.*;

class TsPageOffsetsTest {
    @Test
    void mainPageOffsetsFollowAlignmentArraysNestedStructuresAndBits() {
        String definition = "struct config\n"
                + "struct sensor_s\n"
                + "uint8_t gain;;\"\", 1, 0, 0, 255, 0\n"
                + "end_struct\n"
                + "uint8_t header;;\"\", 1, 0, 0, 255, 0\n"
                + "float[2] correction;;\"ratio\", 1, 0, 0, 5, 2\n"
                + "sensor_s[2 iterate] sensors\n"
                + "bit enabled\n"
                + "bit active\n"
                + "uint16_t tail;;\"\", 1, 0, 0, 1000, 0\n"
                + "uint8_t[0] empty;;\"\", 1, 0, 0, 255, 0\n"
                + "end_struct\n";
        ReaderStateImpl state = new ReaderStateImpl();
        TestTSProjectConsumer consumer = new TestTSProjectConsumer(state);
        state.readBufferedReader(definition, consumer);
        VariableRegistry registry = state.getVariableRegistry();

        assertEquals("0", registry.get("TS_PAGE_1_OFFSET_header"));
        assertEquals("4", registry.get("TS_PAGE_1_OFFSET_correction"));
        assertEquals("12", registry.get("TS_PAGE_1_OFFSET_sensors1_gain"));
        assertEquals("16", registry.get("TS_PAGE_1_OFFSET_sensors2_gain"));
        assertEquals("20", registry.get("TS_PAGE_1_OFFSET_enabled"));
        assertEquals("20", registry.get("TS_PAGE_1_OFFSET_active"));
        assertEquals("24", registry.get("TS_PAGE_1_OFFSET_tail"));
        assertNull(registry.get("TS_PAGE_1_OFFSET_empty"));
        assertEquals(7, registry.getKeys().stream().filter(k -> k.startsWith("TS_PAGE_1_OFFSET_")).count());
        assertTrue(consumer.getContent().contains("correction = array, F32, 4, [2]"));
        assertTrue(consumer.getContent().contains("; total TS size = 28"));
        assertFalse(registry.getDefinesSection().contains("TS_PAGE_1_OFFSET_"));
        assertFalse(registry.getJavaConstants().contains("TS_PAGE_1_OFFSET_"));
    }

    @Test
    void secondaryOffsetsReachTheMainTemplateWithoutLeakingOtherPageVariables() throws IOException {
        ReaderStateImpl secondary = new ReaderStateImpl();
        secondary.getVariableRegistry().put("LOCAL_ONLY", "not a shared template variable");
        PlainTsProjectConsumer page = new PlainTsProjectConsumer(secondary,
                new TreeSet<>(String.CASE_INSENSITIVE_ORDER), 4);
        secondary.readBufferedReader("struct page4\n"
                + "uint32_t[2] preceding;;\"\", 1, 0, 0, 100, 0\n"
                + "float secondaryValue;;\"ratio\", 1, 0, 0, 5, 2\n"
                + "end_struct\n", page);

        ReaderStateImpl main = new ReaderStateImpl();
        page.registerPage(main.getVariableRegistry());
        TestTSProjectConsumer consumer = new TestTSProjectConsumer(main);
        main.readBufferedReader("struct config\n"
                + "float mainValue;;\"ratio\", 1, 0, 0, 5, 2\n"
                + "end_struct\n", consumer);

        VariableRegistry registry = main.getVariableRegistry();
        assertEquals("8", registry.get("TS_PAGE_4_OFFSET_secondaryValue"));
        assertEquals("0", registry.get("TS_PAGE_1_OFFSET_mainValue"));
        assertNull(registry.get("TS_PAGE_1_OFFSET_secondaryValue"));
        assertNull(registry.get("LOCAL_ONLY"));
        assertEquals("12", registry.get("PAGE_SIZE_4"));
        assertEquals(page.getContent(), registry.get("PAGE_CONTENT_4"));

        String template = "[Constants]\n"
                + "page = 1\nmainAlias = scalar, F32, @@TS_PAGE_1_OFFSET_mainValue@@, \"%\", 100, 0\n"
                + "page = 4\nsecondaryAlias = scalar, F32, @@TS_PAGE_4_OFFSET_secondaryValue@@, \"%\", 100, 0\n";
        TsFileContent expanded = consumer.getTsFileContent(new ByteArrayInputStream(template.getBytes(StandardCharsets.UTF_8)));
        assertEquals(template.replace("@@TS_PAGE_1_OFFSET_mainValue@@", "0")
                .replace("@@TS_PAGE_4_OFFSET_secondaryValue@@", "8"), expanded.getPrefix());
        assertFalse(registry.getDefinesSection().contains("TS_PAGE_4_OFFSET_"));
        assertFalse(registry.getJavaConstants().contains("TS_PAGE_4_OFFSET_"));
    }

    @Test
    void outputChannelsDoNotPublishCalibrationOffsets() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.readBufferedReader("struct output_s\nfloat measured;;\"\", 1, 0, 0, 5, 2\nend_struct\n");
        TsOutput output = new TsOutput(false);
        output.run(state, state.getStructures().get("output_s"), 0, "", "channel_");
        assertTrue(output.getContent().contains("channel_measured = scalar, F32, 0"));
        assertFalse(state.getVariableRegistry().getKeys().stream().anyMatch(k -> k.startsWith("TS_PAGE_")));
    }
}
