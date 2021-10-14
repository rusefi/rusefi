package com.rusefi.livedata;

import com.devexperts.logging.Logging;
import com.rusefi.livedata.generated.CPP14Lexer;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserBaseListener;
import com.rusefi.ui.livedata.Range;
import com.rusefi.ui.livedata.SourceCodePainter;
import com.rusefi.ui.livedata.VariableValueSource;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;
import java.awt.*;
import java.io.*;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Stack;

import static com.devexperts.logging.Logging.getLogging;
import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED;

/**
 * this panel shows a live view of rusEFI firmware C/C++ code
 */
public class LiveDataParserPanel {
    private static final Logging log = getLogging(LiveDataParserPanel.class);

    private final JPanel content = new JPanel(new BorderLayout());
    private final JTextPane text = new JTextPane();
    private final VariableValueSource valueSource;
    private String sourceCode;

    public LiveDataParserPanel(VariableValueSource valueSource) {
        this.valueSource = valueSource;

        JScrollPane rightScrollPane = new JScrollPane(text,
                VERTICAL_SCROLLBAR_AS_NEEDED, HORIZONTAL_SCROLLBAR_AS_NEEDED);

        content.add(rightScrollPane);

        try {
            sourceCode = getContent(getClass(), "ac_control.cpp");
            text.setText(sourceCode);

            refresh();
        } catch (IOException | URISyntaxException e) {
            log.warn("Error reading: " + e);
        }
    }

    public static String getContent(Class<?> clazz, String fileName) throws IOException, URISyntaxException {
        URL cpp = clazz.getResource("/c_sources/" + fileName);
        String line;

        StringBuilder result = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new FileReader(new File(cpp.toURI())))) {
            while ((line = br.readLine()) != null) {
                result.append(line + "\n");
            }
        }
        return result.toString();
    }

    @NotNull
    public static ParseTree getParseTree(String sourceCode) {
        CharStream in;
        try {
            in = new ANTLRInputStream(new ByteArrayInputStream(sourceCode.getBytes()));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
        CPP14Parser parser = new CPP14Parser(new CommonTokenStream(new CPP14Lexer(in)));
        return parser.translationUnit();
    }

    public static void applyVariables(VariableValueSource valueSource, String s, SourceCodePainter painter, ParseTree tree) {
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
                    painter.paint(Color.GREEN, new Range(ctx));
                } else {
                    painter.paint(Color.RED, new Range(ctx));
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
    private static String getOrigin(ParserRuleContext ctx, String s) {
        Range range = new Range(ctx);
        return s.substring(range.getStart(), range.getStop());
    }

    public JPanel getContent() {
        return content;
    }

    public void refresh() {
        ParseTree tree = getParseTree(sourceCode);

        StyleContext sc = StyleContext.getDefaultStyleContext();

        StyledDocument styledDocument = text.getStyledDocument();
        AttributeSet oldSet = styledDocument.getCharacterElement(0).getAttributes();
        styledDocument.setCharacterAttributes(0, sourceCode.length(), sc.getEmptySet(), true);

        applyVariables(valueSource, sourceCode, new SourceCodePainter() {
            @Override
            public void paint(Color color, Range range) {
                AttributeSet s = sc.addAttribute(oldSet, StyleConstants.Background, color);
                styledDocument.setCharacterAttributes(range.getStart(), range.getLength(), s, true);
            }
        }, tree);
    }
}
