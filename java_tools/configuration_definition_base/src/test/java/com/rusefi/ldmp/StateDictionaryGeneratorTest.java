package com.rusefi.ldmp;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import static com.rusefi.AssertCompatibility.assertEquals;

public class StateDictionaryGeneratorTest {
    @Test
    public void test() throws IOException {

        String testYaml = "Usages:\n" +
                "#  output_channels always goes first at least because it has protocol version at well-known offset\n" +
                "  - name: output_channels\n" +
                "    java: TsOutputs.java\n" +
                "    folder: console/binary\n" +
                "    cppFileName: status_loop\n" +
                "    constexpr: \"engine->outputChannels\"\n" +
                "\n" +
                "  - name: fuel_computer\n" +
                "    java: FuelComputer.java\n" +
                "    folder: controllers/algo/fuel\n" +
                "    constexpr: [\"engine->fuelComputer\", \"x\"]\n" +
            "    output_name: [ \"wb1\", \"wb2\" ]\n" +
                "    conditional_compilation: \"EFI_ENGINE_CONTROL\"\n";


        List<LinkedHashMap> data = LiveDataProcessor.getStringObjectMap(new StringReader(testYaml));

        TestFileCaptor captor = new TestFileCaptor();
        LiveDataProcessor liveDataProcessor = new LiveDataProcessor("test", fileName -> new StringReader(""), captor, "./");
        liveDataProcessor.handleYaml(data);
        assertEquals("number of outputs", 12, captor.fileCapture.size());

        assertEquals("        stateDictionary.register(live_data_e.LDS_output_channels, \"status_loop\");\n" +
            "        stateDictionary.register(live_data_e.LDS_fuel_computer0, \"fuel_computer\");\n" +
            "        stateDictionary.register(live_data_e.LDS_fuel_computer1, \"fuel_computer\");\n",
            liveDataProcessor.stateDictionaryGenerator.content.toString());
    }
}
