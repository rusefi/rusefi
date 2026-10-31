package com.rusefi.tune;

import com.rusefi.tools.tune.TableData;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TableDataTest {
    @Test
    public void read() throws IOException {
        String input = "<constant cols=\"10\" digits=\"0\" name=\"scriptTable4\" rows=\"8\" units=\"value\">\n" +
            "         0.0 0.0 0.0 333 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         444 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         771 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 777 \n" +
            "      </constant>\n";

        Reader r = new StringReader(input);
        TableData data = TableData.readTable("x", "scriptTable4", 8, s1 -> r, 10);

        assertEquals(333, (int) data.floats[0][3]);



        assertEqualsWithLocalFormattingHack("{0.000,\t0.000,\t0.000,\t333.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{444.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{771.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t777.000,\t},\n", data.getCTable());


        assertEqualsWithLocalFormattingHack("static const float hardCodedscriptTable4[8][10] = {\n" +
            "{0.000,\t0.000,\t0.000,\t333.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{444.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t},\n" +
            "{771.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t0.000,\t777.000,\t},\n" +
            "};\n", data.getCsourceCode());
    }

    private void assertEqualsWithLocalFormattingHack(String s, String s2) {
        assertEquals(localFormattingHack(s), localFormattingHack(s2));
    }

    private String localFormattingHack(String cTable) {
        return cTable.replaceAll(",", " ").replaceAll("\\.", " ");
    }
}
