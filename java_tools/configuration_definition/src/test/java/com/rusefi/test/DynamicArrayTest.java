package com.rusefi.test;

import com.rusefi.ConfigFieldImpl;
import com.rusefi.ReaderStateImpl;
import com.rusefi.output.BaseCHeaderConsumer;
import com.rusefi.output.TsOutput;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class DynamicArrayTest {
    @Test
    public void parsesCapacitySeparatelyFromRuntimeDimensions() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.getVariableRegistry().register("CAPACITY", 24);
        ConfigFieldImpl field = ConfigFieldImpl.parse(state, "uint16_t[32{rows} x CAPACITY{cols}] table");
        assertNotNull(field);
        assertArrayEquals(new int[]{32, 24}, field.getArraySizes());
        assertArrayEquals(new String[]{"{rows}", "{cols}"}, field.getTsArrayDimensions());
        assertEquals("32][CAPACITY", field.getArraySizeVariableName());
        assertEquals(1536, field.getSize(null));
    }

    @Test
    public void generatesDynamicDimensionsWithoutChangingStorage() throws Exception {
        String fields = "uint8_t rows\nuint8_t cols\n"
            + "uint16_t[24{cols}] rpm\n"
            + "uint16_t[32{rows}] load\n"
            + "uint16_t[32{rows} x 24{cols}] table\n"
            + "uint16_t following\n";
        ReaderStateImpl dynamic = new ReaderStateImpl();
        TestTSProjectConsumer ts = new TestTSProjectConsumer(dynamic);
        BaseCHeaderConsumer header = new BaseCHeaderConsumer();
        dynamic.readBufferedReader("struct_no_prefix config_s\n" + fields + "end_struct\n", ts, header);
        ReaderStateImpl fixed = new ReaderStateImpl();
        TestTSProjectConsumer fixedTs = new TestTSProjectConsumer(fixed);
        BaseCHeaderConsumer fixedHeader = new BaseCHeaderConsumer();
        fixed.readBufferedReader("struct_no_prefix config_s\n"
            + fields.replace("{rows}", "").replace("{cols}", "") + "end_struct\n", fixedTs, fixedHeader);
        assertEquals(fixedHeader.getContent(), header.getContent());
        assertEquals(fixedTs.getContent(), ts.getContent().replace("{rows}", "32").replace("{cols}", "24"));
        assertTrue(ts.getContent().contains("rpm = array, U16, 2, [{cols}]"));
        assertTrue(ts.getContent().contains("table = array, U16, 114, [{cols}x{rows}]"));
        assertTrue(ts.getContent().contains("following = scalar, U16, 1650,"));
    }

    @Test
    public void supportsMixedFixedAndDynamicDimensions() {
        ConfigFieldImpl field = ConfigFieldImpl.parse(new ReaderStateImpl(), "float[8{rows} x 16] table");
        assertArrayEquals(new String[]{"{rows}", "16"}, field.getTsArrayDimensions());
        assertEquals(512, field.getSize(null));
    }

    @Test
    public void preservesDimensionsThroughTemplateInstantiation() {
        ReaderStateImpl state = new ReaderStateImpl();
        TestTSProjectConsumer ts = new TestTSProjectConsumer(state);
        state.readBufferedReader("struct table_s<SCALE>\n"
            + "uint16_t[8{rows} x 16{cols}] autoscale table;;\"\", @@SCALE@@, 0, 0, 100, 0\n"
            + "end_struct\nstruct_no_prefix config_s\n"
            + "table_s<1> fuel\nend_struct\n", ts);
        assertTrue(ts.getContent().contains("fuel_table = array, U16, 0, [{cols}x{rows}]"));
    }

    @Test
    public void rejectsDynamicOutputChannels() {
        ReaderStateImpl state = new ReaderStateImpl();
        state.readBufferedReader("struct_no_prefix config_s\nuint16_t[8{rows}] table\nend_struct\n");
        assertThrows(IllegalArgumentException.class, () -> new TsOutput(false).run(
            state, state.getStructures().get("config_s"), 0, "", ""));
    }

    @Test
    public void rejectsUnsupportedDeclarations() {
        ReaderStateImpl state = new ReaderStateImpl();
        assertThrows(IllegalArgumentException.class,
            () -> ConfigFieldImpl.parse(state, "uint16_t[8{rows} iterate] table"));
        assertThrows(IllegalArgumentException.class,
            () -> ConfigFieldImpl.parse(state, "uint16_t[0{rows}] table"));
        assertThrows(IllegalArgumentException.class,
            () -> ConfigFieldImpl.parse(state, "some_struct[8{rows}] table"));
        for (String size : new String[]{"8{}", "8{rows+1}", "{rows}", "8{rows", "8{1rows}"}) {
            assertNull(ConfigFieldImpl.parse(state, "uint16_t[" + size + "] table"), size);
        }
    }
}
