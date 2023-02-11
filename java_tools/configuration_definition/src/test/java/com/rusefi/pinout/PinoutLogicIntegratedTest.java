package com.rusefi.pinout;

import com.rusefi.EnumsReader;
import com.rusefi.VariableRegistry;
import com.rusefi.newparse.ParseState;
import org.junit.Test;

import java.io.*;
import java.util.Collections;
import java.util.List;

import static org.junit.Assert.assertEquals;

public class PinoutLogicIntegratedTest {
    @Test
    public void testWholeThing() throws IOException {
        runPinputTest("pins:\n" +
                "  - pin: 1\n" +
                "    id: [E11, E11]\n" +
                "    class: [event_inputs, switch_inputs]\n" +
                "    function: Digital trigger/switch input for instance Hall type CAM\n" +
                "    ts_name: Digital 2\n" +
                "    type: din", "");

    }

    private static void runPinputTest(String inputYaml, String expected) throws IOException {
        StringWriter testWriter = new StringWriter();

        Reader input = new StringReader(inputYaml);

        BoardInputs testBoard = new BoardInputs() {
            @Override
            public List<?> getBoardYamlKeys() {
                return Collections.singletonList("key");
            }

            @Override
            public Reader getReader(Object yamlKey) {
                return input;
            }

            @Override
            public String getName() {
                return "test";
            }

            @Override
            public List<String> getInputFiles() {
                throw new UnsupportedOperationException();
            }

            @Override
            public Writer getWriter() {
                return testWriter;
            }
        };

        VariableRegistry registry = new VariableRegistry();
        EnumsReader enumsReader = new EnumsReader();

        enumsReader.read(new StringReader("#define output_pin_e_enum \"NONE\", \"INVALID\", \"PA0\", \"PA1\", \"PA2\"\n" +
                "custom output_pin_e 2 bits, U16, @OFFSET@, [0:7], @@output_pin_e_enum@@\n"));

        enumsReader.read(new StringReader("#define Gpio_enum \"NONE\", \"INVALID\", \"PA0\", \"PA1\", \"PA2\", \"PA3\"\n" +
                "\n" +
                "custom Gpio 2 bits, U16, @OFFSET@, [0:7], @@Gpio_enum@@"));

        ParseState definitionState = new ParseState();

        PinoutLogic logic = new PinoutLogic(testBoard);

        //logic.registerBoardSpecificPinNames(registry, definitionState, enumsReader);

        //        assertEquals(expected, testWriter.getBuffer().toString());
    }
}
