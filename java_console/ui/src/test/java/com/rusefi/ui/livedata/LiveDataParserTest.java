package com.rusefi.ui.livedata;

import com.rusefi.livedata.generated.CPP14Lexer;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserBaseListener;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.jetbrains.annotations.NotNull;
import org.junit.Test;

import java.awt.*;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;
import java.util.TreeMap;

public class LiveDataParserTest {
    @Test
    public void testParsing() throws IOException {
        Map<String, Object> values = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
        values.put("engineTooSlow", Boolean.TRUE);
        values.put("engineTooFast", Boolean.FALSE);

        ValueSource valueSource = new ValueSource() {
            @Override
            public Object getValue(String name) {
                return values.get(name);
            }
        };


        String s = "bool AcState::getAcState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {\n" +
                "\tauto rpm = Sensor::getOrZero(SensorType::Rpm);\n" +
                "\n" +
                "\tengineTooSlow = rpm < 500;\n" +
                "\n" +
                "\tif (engineTooSlow) {\n" +
                "\t\treturn true;\n" +
                "\t} else {\n  " +
                "auto ff2 = 1;\n" +
                "\t}\n  " +
                "auto ff = 1;\n" +
                "\tif (engineTooFast) {\n" +
                "\t\treturn false;\n" +
                "\t} \n  " +
                "return ff;\n" +
                "}";
        CharStream in = new ANTLRInputStream(new ByteArrayInputStream(s.getBytes()));

        CPP14Parser parser = new CPP14Parser(new CommonTokenStream(new CPP14Lexer(in)));

        ParseTree tree = parser.translationUnit();
        new ParseTreeWalker().walk(new PrintCPP14ParserListener(), tree);


        Stack<Boolean> currentState = new Stack<>();
        currentState.add(Boolean.TRUE);

        new ParseTreeWalker().walk(new CPP14ParserBaseListener() {
            @Override
            public void enterStatement(CPP14Parser.StatementContext ctx) {
                String origin = getOrigin(ctx, s);
                System.out.println("enter statement [" + origin + "]");
            }

            @Override
            public void enterCondition(CPP14Parser.ConditionContext ctx) {
                String conditionVariable = ctx.getText();
                System.out.println("REQUESTING VALUE " + conditionVariable);
                System.out.println("exp " + getOrigin(ctx.expression(), s));

                Boolean state = (Boolean) valueSource.getValue(conditionVariable);
                if (state == null) {
                    // todo: fail on unknown condition variables
                    return;
                }
                if (state) {
                    //paint(Color.GREEN, ctx.)
                }
            }

            @Override
            public void enterSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
                super.enterSelectionStatement(ctx);

                System.out.println("Else terminal " + ctx.Else());
            }

            @Override
            public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {

            }
        }, tree);

    }

    @NotNull
    private String getOrigin(ParserRuleContext ctx, String s) {
        Range range = new Range(ctx);
        return s.substring(range.start, range.stop);
    }

    interface ValueSource {
        Object getValue(String name);
    }

    static class Range {
        private final int start;
        private final int stop;

        public Range(int start, int stop) {
            this.start = start;
            this.stop = stop;
        }

        public Range(ParserRuleContext context) {
            this(context.start.getStartIndex(), context.stop.getStopIndex() + 1);
        }

        public int getStart() {
            return start;
        }

        public int getStop() {
            return stop;
        }
    }

}
