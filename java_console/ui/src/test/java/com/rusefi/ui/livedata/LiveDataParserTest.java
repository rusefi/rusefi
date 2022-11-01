package com.rusefi.ui.livedata;

import com.rusefi.CodeWalkthrough;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;
import com.rusefi.livedata.LiveDataParserPanel;
import com.rusefi.livedata.LiveDataParserSandbox;
import com.rusefi.livedata.ParseResult;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.junit.Ignore;
import org.junit.Test;

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.TreeMap;

import static com.rusefi.CodeWalkthrough.TRUE_CONDITION;
import static com.rusefi.ui.LiveDataPane.CPP_SUFFIX;
import static org.junit.Assert.*;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.*;

public class LiveDataParserTest {
    @Test
    public void testGreenCode() {
        String sourceCode = "void AcController::onSlowCallback() {\n" +
                "\tbool isEnabled = getAcState();\n" +
                "\n" +
                "\tm_acEnabled = isEnabled;\n" +
                "auto [cltValid, clt] = Sensor::get(SensorType::Clt);\n" +
                "\n" +
                "\tenginePins.acRelay.setValue(isEnabled);\n" +
                "\n" +
                "#if EFI_TUNER_STUDIO\n" +
                "\tengine->outputChannels.acState = isEnabled;\n" +
                "#endif // EFI_TUNER_STUDIO\n" +
                "}\n";

        SourceCodePainter painter = run(name -> null, sourceCode);
        verify(painter, times(7)).paintBackground(eq(CodeWalkthrough.ACTIVE_STATEMENT), any());
    }

    @Test
    public void testMethodNamesCode() {
        String sourceCode = "void AcController::onSlowCallback() {\n" +
                "}\n" +
                "void onSlowCallback2() {\n" +
                "                }\n" +
                "int onSlowCallback3() {\n" +
                "                }\n" +
                ""
                ;

        SourceCodePainter painter = mock(SourceCodePainter.class);
        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);

        printTree(tree);
        ParseResult result = CodeWalkthrough.applyVariables(name -> null, sourceCode, painter, tree);
        assertEquals(3, result.getFunctions().size());
    }

    @Test
    public void testParsing() {
        Map<String, Double> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", 1.0);
        values.put("engineTooFast", 1.0);

        VariableValueSource valueSource = LiveDataParserSandbox.getVariableValueSource(values);

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

        SourceCodePainter painter = run(valueSource, sourceCode);

        verify(painter, times(2)).paintForeground(eq(CodeWalkthrough.CONFIG), any());

        verify(painter).paintBackground(eq(TRUE_CONDITION), any());

        verify(painter, times(4)).paintBackground(eq(CodeWalkthrough.ACTIVE_STATEMENT), any());
        verify(painter, times(5)).paintBackground(eq(CodeWalkthrough.PASSIVE_CODE), any());
    }

    private SourceCodePainter run(VariableValueSource valueSource, String sourceCode) {
        SourceCodePainter painter = mock(SourceCodePainter.class);
        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);

        printTree(tree);
        CodeWalkthrough.applyVariables(valueSource, sourceCode, painter, tree);
        return painter;
    }

    private static void printTree(ParseTree tree) {
        System.out.println("******************************************* Just print everything for educational purposes");
        new ParseTreeWalker().walk(new PrintCPP14ParserListener(), tree);
        System.out.println("******************************************* Now running FOR REAL");
    }

    @Test
    public void testConfigurationInRealSourceCode() throws IOException, URISyntaxException {
        String fileName = StateDictionary.INSTANCE.getFileName(live_data_e.LDS_boost_control);
        String sourceCode = LiveDataParserPanel.getContent(LiveDataParserPanel.class, fileName + CPP_SUFFIX);
        assertTrue(sourceCode.length() > 100);

        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);
        LiveDataParserTest.printTree(tree);
        ParseResult parseResult = CodeWalkthrough.applyVariables(VariableValueSource.VOID, sourceCode, SourceCodePainter.VOID, tree);
        assertFalse(parseResult.getConfigTokens().isEmpty());
    }
}
