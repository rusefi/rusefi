package com.rusefi.livedata;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.rusefi.CodeWalkthrough;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.BinaryProtocolState;
import com.rusefi.config.Field;
import com.rusefi.enums.live_data_e;
import com.rusefi.livedata.generated.CPP14Lexer;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.livedata.Range;
import com.rusefi.ui.livedata.SourceCodePainter;
import com.rusefi.ui.livedata.VariableValueSource;
import com.rusefi.ui.livedocs.LiveDocHolder;
import com.rusefi.ui.livedocs.LiveDocsRegistry;
import com.rusefi.ui.livedocs.RefreshActions;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTree;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.io.*;
import java.net.URISyntaxException;
import java.util.Collection;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;
import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED;

/**
 * this panel shows a live view of rusEFI firmware C/C++ code
 * @see LiveDataParserSandbox
 */
public class LiveDataParserPanel {
    private static final Logging log = getLogging(LiveDataParserPanel.class);

    static {
//        log.configureDebugEnabled(true);
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private ParseResult parseResult = ParseResult.VOID;
    private final UIContext uiContext;
    private final JTextPane text = new JTextPane() {
        @Override
        public void paint(Graphics g) {
            super.paint(g);
            BinaryProtocol binaryProtocol = uiContext.getBinaryProtocol();
            if (binaryProtocol == null)
                return;
            BinaryProtocolState bps = binaryProtocol.getBinaryProtocolState();
            if (bps == null)
                return;
            ConfigurationImage ci = bps.getConfigurationImage();
            if (ci == null)
                return;

            g.setColor(Color.red);

            for (Token setting : parseResult.getConfigTokens()) {
                // todo: something like binaryProtocol.getIniFile().getFieldsInUiOrder().values()?
                Field field = findFieldOrNull(null, "", setting.getText());
                if (field == null)
                    continue;
                if (field.getType().isString())
                    continue;
                Number value = field.getValue(ci);
                Rectangle r;
                try {
                    r = getUI().modelToView(text, setting.getStopIndex());
                } catch (BadLocationException e) {
                    throw new IllegalStateException(e);
                }
                if (r == null)
                    return; // when would this happen?
                g.drawString(value.toString(), r.x, r.y);
            }
        }
    };
    private final VariableValueSource valueSource;
    private final String fileName;
    private String sourceCode;

    public LiveDataParserPanel(UIContext uiContext, VariableValueSource valueSource, String fileName) {
        this.uiContext = uiContext;
        this.valueSource = valueSource;
        this.fileName = fileName;

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

    /**
     * Finds field by name, ignoring case
     */
    public static Field findFieldOrNull(Collection<Field> values, String instancePrefix, String fieldName) {
        Objects.requireNonNull(fieldName);
        for (Field f : values) {
            if (fieldName.equalsIgnoreCase(f.getName()))
                return f;
        }
        // 2nd pass - let's try to find field with prefix if it was not found without prefix
        if (!instancePrefix.isEmpty()) {
            fieldName = instancePrefix + "_" + fieldName;
            for (Field f : values) {
                if (fieldName.equalsIgnoreCase(f.getName()))
                    return f;
            }
        }
        return null;
    }

    public ParseResult getParseResult() {
        return parseResult;
    }

    public JTextPane getText() {
        return text;
    }

    @NotNull
    public static String getContent(Class<?> clazz, String fileName) throws IOException, URISyntaxException {
        String contentOrNull = getContentOrNull(clazz, fileName);
        if (contentOrNull == null)
            return fileName + " getResourceAsStream not found";
        return contentOrNull;
    }

    @Nullable
    public static String getContentOrNull(Class<?> clazz, String fileName) throws IOException {
        InputStream cpp = clazz.getResourceAsStream("/c_sources/" + fileName);
        if (cpp == null)
            return null;
        String line;

        StringBuilder result = new StringBuilder();
        try (BufferedReader br = new BufferedReader(new InputStreamReader(cpp))) {
            while ((line = br.readLine()) != null) {
                result.append(line).append("\n");
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

    public JPanel getContent() {
        return content;
    }

    public void refresh() {
        ParseTree tree = getParseTree(sourceCode);

        StyleContext sc = StyleContext.getDefaultStyleContext();

        SimpleAttributeSet attributes = new SimpleAttributeSet();
        DefaultStyledDocument styledDocument = new DefaultStyledDocument();
        try {
            styledDocument.insertString(0, sourceCode, attributes);
        } catch (BadLocationException e) {
            throw new IllegalStateException(e);
        }

        AttributeSet oldSet = styledDocument.getCharacterElement(0).getAttributes();
        styledDocument.setCharacterAttributes(0, sourceCode.length(), sc.getEmptySet(), true);

        // todo: technically we do not need to do the complete re-compile on fresh data arrival just repaint!
        // todo: split compilation and painting/repainting
        try {
            parseResult = CodeWalkthrough.applyVariables(valueSource, sourceCode, new SourceCodePainter() {
                @Override
                public void paintBackground(Color color, Range range) {
                    AttributeSet s = sc.addAttribute(oldSet, StyleConstants.Background, color);
                    styledDocument.setCharacterAttributes(range.getStart(), range.getLength(), s, false);
                }

                @Override
                public void paintForeground(Color color, Range range) {
                    AttributeSet s = sc.addAttribute(oldSet, StyleConstants.Foreground, color);
                    styledDocument.setCharacterAttributes(range.getStart(), range.getLength(), s, false);
                }
            }, tree);
        } catch (RuntimeException e) {
            throw new IllegalStateException("While " + fileName, e);
        }

        text.setDocument(styledDocument);
    }

    @NotNull
    public static LiveDataParserPanel createLiveDataParserPanel(UIContext uiContext, final live_data_e live_data_e, final Field[] values, String fileName) {
        AtomicReference<byte[]> reference = new AtomicReference<>();

        LiveDataParserPanel livePanel = new LiveDataParserPanel(uiContext, name -> {
            byte[] bytes = reference.get();
            if (bytes == null)
                return null;
            Field f = findFieldOrNull(null, "", name);
            if (f == null) {
                //log.error("BAD condition, should be variable: " + name);
                return null;
            }
            double number = f.getValue(new ConfigurationImage(bytes));
            if (log.debugEnabled()) {
                log.debug("getValue(" + name + "): " + number);
            }
            // convert Number to Boolean
            return new VariableValueSource.VariableState(f, number);
        }, fileName);
        RefreshActions refreshAction = new RefreshActions() {
            @Override
            public void refresh(byte[] response) {
                if (log.debugEnabled())
                    log.debug("Got data " + response.length + " bytes");
                reference.set(response);
                livePanel.refresh();
            }
        };

        LiveDocsRegistry.INSTANCE.register(new LiveDocHolder(live_data_e, refreshAction) {
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
