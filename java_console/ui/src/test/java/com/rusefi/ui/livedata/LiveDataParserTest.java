package com.rusefi.ui.livedata;

import com.rusefi.livedata.LiveDataParserPanel;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.junit.Test;

import java.awt.*;
import java.util.Map;
import java.util.TreeMap;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.*;

public class LiveDataParserTest {
    @Test
    public void testParsing() {
        Map<String, Object> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", Boolean.TRUE);
        values.put("engineTooFast", Boolean.FALSE);

        VariableValueSource valueSource = values::get;

        String sourceCode = "bool AcState::getAcState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {\n" +
                "\tauto rpm = Sensor::getOrZero(SensorType::Rpm);\n" +
                "\n" +
                "\tengineTooSlow = rpm < 500;\n" +
                "\n" +
                "\tif (engineTooSlow) {\n" +
                "\t\treturn true;\n" +
                "\t} else {\n  " +
                "auto ff2 = CONFIG(Alternatorcontrolpin);\n" +
                "\t}\n  " +
                "auto ff = CONFIG(tpsMax);\n" +
                "\tif (engineTooFast) {\n" +
                "\t\treturn false;\n" +
                "\t} \n  " +
                "return ff;\n" +
                "}";

        SourceCodePainter painter = mock(SourceCodePainter.class);
        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);
        new ParseTreeWalker().walk(new PrintCPP14ParserListener(), tree);


        LiveDataParserPanel.applyVariables(valueSource, sourceCode, painter, tree);
        verify(painter, times(2)).paintForeground(eq(Color.blue), any());

        verify(painter).paintBackground(eq(Color.red), any());
        verify(painter).paintBackground(eq(Color.green), any());
    }
}
