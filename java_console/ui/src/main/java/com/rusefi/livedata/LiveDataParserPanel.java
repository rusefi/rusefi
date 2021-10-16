package com.rusefi.livedata;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.enums.live_data_e;
import com.rusefi.io.LinkConnector;
import com.rusefi.livedata.generated.CPP14Lexer;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserBaseListener;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.livedata.Range;
import com.rusefi.ui.livedata.SourceCodePainter;
import com.rusefi.ui.livedata.VariableValueSource;
import com.rusefi.ui.livedocs.LiveDocHolder;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
import com.rusefi.ui.livedocs.RefreshActions;
import com.rusefi.ui.livedocs.RefreshActionsMap;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.antlr.v4.runtime.tree.TerminalNode;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.io.*;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Stack;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;
import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED;

/**
 * this panel shows a live view of rusEFI firmware C/C++ code
 */
public class LiveDataParserPanel {
    private static final Logging log = getLogging(LiveDataParserPanel.class);

    private final JPanel content = new JPanel(new BorderLayout());
    private ParseResult parseResult = ParseResult.VOID;
    private final UIContext uiContext;
    private final JTextPane text = new JTextPane() {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            BinaryProtocol binaryProtocol = uiContext.getLinkManager().getConnector().getBinaryProtocol();
            if (binaryProtocol == null)
                return;
            BinaryProtocolState bps = binaryProtocol.getBinaryProtocolState();
            if (bps == null)
                return;
            ConfigurationImage ci = bps.getControllerConfiguration();
            if (ci == null)
                return;

            g.setColor(Color.red);

            for (Token setting : parseResult.getConfigTokens()) {
                Field field = Field.findFieldOrNull(Fields.VALUES, "", setting.getText());
                if (field == null)
                    continue;
                Number value = field.getValue(ci);
                Rectangle r;
                try {
                    r = getUI().modelToView(text, setting.getStopIndex());
                } catch (BadLocationException e) {
                    throw new IllegalStateException(e);
                }
                g.drawString(value.toString(), r.x, r.y);


            }


        }
    };
    private final VariableValueSource valueSource;
    private String sourceCode;

    public LiveDataParserPanel(UIContext uiContext, VariableValueSource valueSource, String fileName) {
        this.uiContext = uiContext;
        this.valueSource = valueSource;

        JScrollPane rightScrollPane = new JScrollPane(text,
                VERTICAL_SCROLLBAR_AS_NEEDED, HORIZONTAL_SCROLLBAR_AS_NEEDED);

        content.add(rightScrollPane);

        try {
            sourceCode = getContent(getClass(), fileName);
            text.setText(sourceCode);

            refresh();
        } catch (IOException | URISyntaxException e) {
            log.warn("Error reading: " + e);
        }
    }

    public static String getContent(Class<?> clazz, String fileName) throws IOException, URISyntaxException {
        InputStream cpp = clazz.getResourceAsStream("/c_sources/" + fileName);
        String line;

        StringBuilder result = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new InputStreamReader(cpp))) {
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

    public static ParseResult applyVariables(VariableValueSource valueSource, String s, SourceCodePainter painter, ParseTree tree) {
        Stack<Boolean> currentState = new Stack<>();
        currentState.add(Boolean.TRUE);

        java.util.List<TerminalNode> allTerminals = new java.util.ArrayList<>();

        new ParseTreeWalker().walk(new CPP14ParserBaseListener() {
            @Override
            public void enterStatement(CPP14Parser.StatementContext ctx) {
                String origin = getOrigin(ctx, s);
//                System.out.println("enter statement [" + origin + "]");
            }

            @Override
            public void visitTerminal(TerminalNode node) {
                allTerminals.add(node);
            }

            @Override
            public void enterCondition(CPP14Parser.ConditionContext ctx) {
                String conditionVariable = ctx.getText();
//                System.out.println("REQUESTING VALUE " + conditionVariable);
//                System.out.println("exp " + getOrigin(ctx.expression(), s));

                Boolean state = (Boolean) valueSource.getValue(conditionVariable);
                if (state == null) {
                    // todo: fail on unknown condition variables
                    return;
                }
                if (state) {
                    painter.paintBackground(Color.GREEN, new Range(ctx));
                } else {
                    painter.paintBackground(Color.RED, new Range(ctx));
                }
            }

            @Override
            public void enterSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
                super.enterSelectionStatement(ctx);
//                System.out.println("Else terminal " + ctx.Else());
            }

            @Override
            public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {

            }
        }, tree);

        java.util.List<Token> configTokens = new java.util.ArrayList<>();

        for (int i = 0; i < allTerminals.size() - 3; i++) {

            if (allTerminals.get(i).getText().equals("CONFIG") &&
                    allTerminals.get(i + 1).getText().equals("(") &&
                    allTerminals.get(i + 3).getText().equals(")")
            ) {
                Token token = allTerminals.get(i + 2).getSymbol();
                painter.paintForeground(Color.BLUE, new Range(token, token));
                configTokens.add(token);
            }
        }
        return new ParseResult(configTokens);
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

        // todo: technically we do not need to do the complete re-compile on fresh data arrival just repaint!
        // todo: split compilation and painting/repainting
        parseResult = applyVariables(valueSource, sourceCode, new SourceCodePainter() {
            @Override
            public void paintBackground(Color color, Range range) {
                AttributeSet s = sc.addAttribute(oldSet, StyleConstants.Background, color);
                styledDocument.setCharacterAttributes(range.getStart(), range.getLength(), s, true);
            }

            @Override
            public void paintForeground(Color color, Range range) {
                AttributeSet s = sc.addAttribute(oldSet, StyleConstants.Foreground, color);
                styledDocument.setCharacterAttributes(range.getStart(), range.getLength(), s, true);
            }
        }, tree);
    }

    @Nullable
    public static LiveDataParserPanel getLiveDataParserPanel(UIContext uiContext, final live_data_e live_data_e, final Field[] values, String fileName) {
        AtomicReference<byte[]> reference = new AtomicReference<>();

        LiveDataParserPanel livePanel = new LiveDataParserPanel(uiContext, new VariableValueSource() {
            @Override
            public Object getValue(String name) {
                byte[] bytes = reference.get();
                if (bytes == null)
                    return null;
                Field f = Field.findField(values, "", name);
                int number = f.getValue(new ConfigurationImage(bytes)).intValue();
//                System.out.println("getValue " + f);
                // convert Number to Boolean
                return number != 0;
            }
        }, fileName);
        RefreshActionsMap refreshActionsMap = new RefreshActionsMap();
        refreshActionsMap.put(live_data_e, new RefreshActions() {
            @Override
            public void refresh(BinaryProtocol bp, byte[] response) {
                reference.set(response);
                livePanel.refresh();
            }
        });

        LiveDocsRegistry.INSTANCE.register(new LiveDocHolder(live_data_e, refreshActionsMap) {
            @Override
            public boolean isVisible() {
                JPanel panel = livePanel.getContent();
                boolean isVisible = !panel.getVisibleRect().isEmpty();
                return isVisible && isRecursivelyVisible(panel);
            }
        });
        return livePanel;
    }

    private static boolean isRecursivelyVisible(Container c) {
        Container parent = c.getParent();
        return c.isVisible() && (parent == null || isRecursivelyVisible(parent));
    }
}
