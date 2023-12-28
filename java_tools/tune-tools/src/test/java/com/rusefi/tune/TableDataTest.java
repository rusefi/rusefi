package com.rusefi.tune;

import com.rusefi.tools.tune.TableData;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;

import static org.junit.Assert.assertEquals;

public class TableDataTest {
    @Test
    public void read() throws IOException {

        String s = "<constant cols=\"10\" digits=\"0\" name=\"scriptTable4\" rows=\"8\" units=\"value\">\n" +
            "         0.0 0.0 0.0 333 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         444 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "         0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n" +
            "      </constant>\n";

        Reader r = new StringReader(s);
        float[][] data = TableData.readTable("x", "scriptTable4", 8, s1 -> r, 10);

        assertEquals(333, (int)data[0][3]);
    }
}
