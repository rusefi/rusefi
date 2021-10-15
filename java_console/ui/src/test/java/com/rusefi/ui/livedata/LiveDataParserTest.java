package com.rusefi.ui.livedata;

import com.rusefi.livedata.LiveDataParserPanel;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.jetbrains.annotations.NotNull;
import org.junit.Test;

import java.awt.*;
import java.io.IOException;
import java.util.Map;
import java.util.TreeMap;

public class LiveDataParserTest {
    @Test
    public void testParsing() throws IOException {
        Map<String, Object> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", Boolean.TRUE);
        values.put("engineTooFast", Boolean.FALSE);

        VariableValueSource valueSource = new VariableValueSource() {
            @Override
            public Object getValue(String name) {
                return values.get(name);
            }
        };

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

        SourceCodePainter painter = new SourceCodePainter() {
            @Override
            public void paintBackground(Color color, Range range) {
                System.out.println("paint " + color + " " + range);
            }

            @Override
            public void paintForeground(Color color, Range range) {
                System.out.println("paintForeground");

            }
        };
        ParseTree tree = LiveDataParserPanel.getParseTree(sourceCode);
        new ParseTreeWalker().walk(new PrintCPP14ParserListener(), tree);


        LiveDataParserPanel.applyVariables(valueSource, sourceCode, painter, tree);

    }

}
