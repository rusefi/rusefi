package com.rusefi.pinout;

import com.rusefi.EnumsReader;
import com.rusefi.ReaderState;
import com.rusefi.ReaderStateImpl;
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
        runPinoutTest("pins:\n" +
                "  - pin: 1\n" +
                "    id: [E11, E11]\n" +
                "    class: [event_inputs, switch_inputs]\n" +
                "    function: Digital trigger/switch input for instance Hall type CAM\n" +
                "    ts_name: Digital 2\n" +
                "    type: din", "");

    }

    private static void runPinoutTest(String inputYaml, String expected) throws IOException {
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

        ReaderStateImpl state = new ReaderStateImpl();

        state.getEnumsReader().read(new StringReader("enum class Gpio : uint16_t {\n" +
                                                        "Unassigned = 0,\n" +
                                                        "Invalid = 1,\n" +
                                                        "E11 = 2,\n" +
                                                        "};"));

        ParseState definitionState = new ParseState(state.getEnumsReader());

        PinoutLogic logic = new PinoutLogic(testBoard);

        logic.registerBoardSpecificPinNames(state.getVariableRegistry(), definitionState, state.getEnumsReader());

        //        assertEquals(expected, testWriter.getBuffer().toString());
    }
}
