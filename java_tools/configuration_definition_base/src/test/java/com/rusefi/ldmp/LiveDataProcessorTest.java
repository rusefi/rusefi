package com.rusefi.ldmp;

import com.rusefi.ReaderProvider;
import org.junit.Test;

import java.io.*;
import java.util.Map;

import static org.junit.Assert.assertEquals;

public class LiveDataProcessorTest {
    @Test
    public void testTwoSections() throws IOException {
        String testYaml = "Usages:\n" +
                "#  output_channels always goes first at least because it has protocol version at well-known offset\n" +
                "  - name: output_channels\n" +
                "    java: TsOutputs.java\n" +
                "    folder: console/binary\n" +
                "    cppFileName: status_loop\n" +
                "    constexpr: \"engine->outputChannels\"\n";


        Map<String, Object> data = LiveDataProcessor.getStringObjectMap(new StringReader(testYaml));

        TestFileCaptor captor = new TestFileCaptor();
        LiveDataProcessor liveDataProcessor = new LiveDataProcessor("test", new ReaderProvider() {
            @Override
            public Reader read(String fileName) {
                System.out.println("read " + fileName);
                return new StringReader("struct_no_prefix wideband_state_s\n" +
                        "\tuint16_t tempC;WBO: Temperature;\"C\", 1, 0, 500, 1000, 0\n" +
                        "\tuint16_t esr;WBO: ESR;\"ohm\", 1, 0, 0, 10000, 0\n" +
                        "end_struct");
            }
        }, captor);
        liveDataProcessor.handleYaml(data);
        assertEquals(7, captor.fileCapture.size());

        assertEquals("tempC = scalar, U16, 0, \"C\", 1, 0\n" +
                "esr = scalar, U16, 2, \"ohm\", 1, 0\n" +
                "; total TS size = 4\n", captor.fileCapture.get(LiveDataProcessor.OUTPUTS_SECTION_FILE_NAME).sb.toString());

        assertEquals("entry = tempC, \"WBO: Temperature\", int,    \"%d\"\n" +
                "entry = esr, \"WBO: ESR\", int,    \"%d\"\n", captor.fileCapture.get(LiveDataProcessor.DATA_LOG_FILE_NAME).sb.toString());

    }
}
