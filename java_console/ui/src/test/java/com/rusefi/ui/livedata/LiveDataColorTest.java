package com.rusefi.ui.livedata;

import com.rusefi.CodeWalkthrough;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;
import com.rusefi.livedata.LiveDataParserPanel;
import org.antlr.v4.runtime.tree.ParseTree;
import org.junit.Ignore;
import org.junit.Test;

import java.awt.*;
import java.io.IOException;
import java.net.URISyntaxException;

import static com.rusefi.livedata.LiveDataParserPanel.getContentOrNull;
import static com.rusefi.ui.LiveDataPane.CPP_SUFFIX;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

@Ignore // todo: https://github.com/rusefi/rusefi/issues/4669
public class LiveDataColorTest {
    @Test
    public void testAllFiles() throws IOException {
        int counter = 0;
        for (live_data_e view : live_data_e.values()) {
            String fileName = StateDictionary.INSTANCE.getFileName(view) + CPP_SUFFIX;

            try {
                testSpecificFile(fileName);
            } catch (RuntimeException e) {
                throw new IllegalStateException("During " + fileName, e);
            }

            counter++;
        }

        assertTrue(counter > 4);

    }

    private void testSpecificFile(String fileName) throws IOException {
        String sourceCode = getContentOrNull(getClass(), fileName);
        assertNotNull("Not found: sourceCode for " + fileName, sourceCode);

        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);

        CodeWalkthrough.applyVariables(VariableValueSource.VOID, sourceCode, new SourceCodePainter() {
            @Override
            public void paintBackground(Color color, Range range) {
            }

            @Override
            public void paintForeground(Color color, Range range) {
            }
        }, tree);
    }
}
