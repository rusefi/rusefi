package com.rusefi.ui.livedata;

import com.rusefi.CodeWalkthrough;
import com.rusefi.livedata.LiveDataParserPanel;
import com.rusefi.livedata.LiveDataView;
import com.rusefi.livedata.ParseResult;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.junit.Test;

import java.awt.*;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.TreeMap;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
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

        String sourceCode = "bool AcState::getAcState() {\n" +
                "\tauto rpm = Sensor::getOrZero(SensorType::Rpm);\n" +
                "\n" +
                "\tif (engineTooSlow) {\n" +
                "\t\tinvokeMethod();\n" +
                "\t\treturn true;\n" +
                "\t} else {\n  " +
                "auto ff2 = engineConfiguration->Alternatorcontrolpin;\n" +
                "\t}\n  " +
                "auto ff = engineConfiguration->tpsMax;\n" +
                "\tif (engineTooFast) {\n" +
                "\t\treturn false;\n" +
                "\t} \n  " +
                "return ff;\n" +
                "}\n" +
                "bool AcState::getAcState2() {\n" +
                "return ff;\n" +
                "}\n";

        SourceCodePainter painter = mock(SourceCodePainter.class);
        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);

        System.out.println("******************************************* Just print everything for educational purposes");
        new ParseTreeWalker().walk(new PrintCPP14ParserListener(), tree);

        System.out.println("******************************************* Now running FOR REAL");


        CodeWalkthrough.applyVariables(valueSource, sourceCode, painter, tree);
        verify(painter, times(2)).paintForeground(eq(Color.blue), any());

        verify(painter).paintBackground(eq(Color.red), any());
        verify(painter).paintBackground(eq(Color.green), any());

        verify(painter, times(4)).paintBackground(eq(CodeWalkthrough.ACTIVE_STATEMENT), any());
        verify(painter, times(1)).paintBackground(eq(CodeWalkthrough.INACTIVE_BRANCH), any());

        verify(painter, times(3)).paintBackground(eq(CodeWalkthrough.PASSIVE_CODE), any());
    }

    @Test
    public void testConfigurationInRealSourceCode() throws IOException, URISyntaxException {
        String sourceCode = LiveDataParserPanel.getContent(LiveDataParserPanel.class, LiveDataView.BOOST_CONTROL.getFileName());
        assertTrue(sourceCode.length() > 100);

        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);
        ParseResult parseResult = CodeWalkthrough.applyVariables(VariableValueSource.VOID, sourceCode, SourceCodePainter.VOID, tree);
        assertFalse(parseResult.getConfigTokens().isEmpty());
    }
}
