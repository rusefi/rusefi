package com.rusefi.ui.lua;

import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.ShortcutsHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import com.rusefi.ui.widgets.ConsoleFonts;
import org.jetbrains.annotations.NotNull;
import org.fife.ui.autocomplete.AutoCompletion;
import org.fife.ui.autocomplete.BasicCompletion;
import org.fife.ui.autocomplete.CompletionProvider;
import org.fife.ui.autocomplete.DefaultCompletionProvider;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.fife.ui.rsyntaxtextarea.SyntaxConstants;
import org.fife.ui.rtextarea.Gutter;
import org.fife.ui.rtextarea.RTextScrollPane;

import javax.swing.*;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AbstractDocument;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.UndoManager;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

/**
 * open question how much of text editor we want
 * <p>
 * todo: Find text feature?
 */
public class LuaTextEditor {
    /** Share of the ECU's script space from which the size turns amber. */
    static final double NEAR_LIMIT = 0.9;
    private static final Color CURRENT_LINE = new Color(0xF2F5FA);
    /** Console.Status.pending.foreground of the light theme, for when no theme is installed. */
    private static final Color NEAR_LIMIT_FALLBACK = new Color(0xB45309);
    private final JPanel area = new JPanel(new BorderLayout());
    private final RSyntaxTextArea textArea = new RSyntaxTextArea();
    private final JLabel sizeLabel = new JLabel();
    private final JLabel locationLabel = new JLabel();
    private final UIContext context;
    private final DefaultCompletionProvider completionProvider = new DefaultCompletionProvider();

    public LuaTextEditor(UIContext context) {
        this.context = context;
        // Configure RSyntaxTextArea editor
        textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_LUA);
        textArea.setCodeFoldingEnabled(true);
        textArea.setAutoIndentEnabled(true);
        textArea.setBracketMatchingEnabled(true);
        textArea.setTabSize(2);
        textArea.setToolTipText("Edit Lua script; Ctrl+Z to undo, Ctrl+Y to redo, Ctrl+Space for completions");
        // the console's monospaced font; the logical Monospaced font is Courier New on Windows
        textArea.setFont(ConsoleFonts.mono(Font.PLAIN, ConsoleFonts.baseSize()));
        textArea.setCurrentLineHighlightColor(CURRENT_LINE);
		textArea.requestFocusInWindow();

        AbstractDocument pDoc = (AbstractDocument) textArea.getDocument();
        pDoc.setDocumentFilter(new DocumentSizeFilter(context));
        pDoc.addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                updateSize();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                updateSize();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                updateSize();
            }
        });

        textArea.addCaretListener(new CaretListener() {
            @Override
            public void caretUpdate(CaretEvent e) {
                updateLocation();
            }
        });

        // Install autocomplete
        installDefaultCompletions(completionProvider);
        AutoCompletion ac = new AutoCompletion(completionProvider);
        ac.setAutoActivationEnabled(true);
        ac.setAutoActivationDelay(250);
        ac.install(textArea);

        RTextScrollPane textAreaScroll = new RTextScrollPane(textArea, true);
        textAreaScroll.setBorder(BorderFactory.createEmptyBorder());
        Gutter gutter = textAreaScroll.getGutter();
        gutter.setLineNumberFont(textArea.getFont());
        gutter.setLineNumberColor(muted());
        gutter.setBorderColor(separator());
        area.add(textAreaScroll, BorderLayout.CENTER);

        // status bar: caret position and script size, muted like the console's other status text
        JPanel bottomArea = new JPanel(new BorderLayout()) {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                g.setColor(separator());
                g.drawLine(0, 0, getWidth(), 0);
            }
        };
        bottomArea.setBorder(BorderFactory.createEmptyBorder(4, 12, 4, 12));
        locationLabel.setForeground(muted());
        sizeLabel.setForeground(muted());
        sizeLabel.setToolTipText("Script size and the most the ECU can store");
        area.add(bottomArea, BorderLayout.SOUTH);
        bottomArea.add(locationLabel, BorderLayout.WEST);
        bottomArea.add(sizeLabel, BorderLayout.EAST);
        updateLocation();

        installUndoRedoKeystrokes();
        UiUtils.installPopupMenu(createPopupMenu(), textArea);
    }

    @NotNull
    private JPopupMenu createPopupMenu() {
        JPopupMenu menu = new JPopupMenu();
        menu.add(UiUtils.createCopyMenu(textArea));
        return menu;
    }

    private void updateSize() {
        LinkManager linkManager = context.getLinkManager();
        BinaryProtocol bp = linkManager.getBinaryProtocol();
        if (bp == null)
            return;
        StringIniField field = LuaScriptPanel.getLuaScriptField(bp);
        // field is null when the active .ini has no luaScript field yet — e.g. during config
        // migration before the ini is wired up. Match the bp==null guard above instead of NPEing
        // on every document change (see #9709).
        if (field == null)
            return;
        showSize(textArea.getText().length(), field.getSize());
    }

    // Package-private so tests check the status text users see.
    void showSize(int length, int limit) {
        sizeLabel.setText(String.format("%d / %d bytes", length, limit));
        // amber once the script is close to what the ECU can store
        Color warning = UIManager.getColor("Console.Status.pending.foreground");
        sizeLabel.setForeground(length >= limit * NEAR_LIMIT ? (warning != null ? warning : NEAR_LIMIT_FALLBACK) : muted());
    }

    private void updateLocation() {
        int offset = textArea.getCaretPosition();
        try {
            int line = textArea.getLineOfOffset(offset);
            int column = offset - textArea.getLineStartOffset(line);
            locationLabel.setText("Ln " + (line + 1) + ", Col " + (column + 1));
        } catch (BadLocationException ignored) {
        }
    }

    JLabel getLocationLabel() {
        return locationLabel;
    }

    JLabel getSizeLabel() {
        return sizeLabel;
    }

    private static Color muted() {
        Color c = UIManager.getColor("Label.disabledForeground");
        return c != null ? c : Color.GRAY;
    }

    private static Color separator() {
        Color c = UIManager.getColor("Separator.foreground");
        return c != null ? c : Color.LIGHT_GRAY;
    }

    private void installUndoRedoKeystrokes() {
        KeyStroke undoKeyStroke = KeyStroke.getKeyStroke(
                KeyEvent.VK_Z, InputEvent.CTRL_DOWN_MASK);
        KeyStroke redoKeyStroke = KeyStroke.getKeyStroke(
                KeyEvent.VK_Y, InputEvent.CTRL_DOWN_MASK);

        UndoManager undoManager = new UndoManager();

        Document document = textArea.getDocument();
        document.addUndoableEditListener(e -> undoManager.addEdit(e.getEdit()));

        // Map undo action
        ShortcutsHelper.installKeyAction(undoKeyStroke, "undoKeyStroke", textArea, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    undoManager.undo();
                } catch (CannotUndoException cue) {
                    // ignored
                }
            }
        });
        // Map redo action
        ShortcutsHelper.installKeyAction(redoKeyStroke, "redoKeyStroke", textArea, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    undoManager.redo();
                } catch (CannotRedoException cre) {
                    // ignored
                }
            }
        });
    }

    public JComponent getControl() {
        return area;
    }

    public void setText(String text) {
        // note that text would be silently not-changed if it's too large to fit into control - see DocumentSizeFilter
        textArea.setText(text);
    }

    public String getText() {
        return textArea.getText();
    }

    public CompletionProvider getCompletionProvider() {
        return completionProvider;
    }

    private static void installDefaultCompletions(DefaultCompletionProvider provider) {
        // Basic Lua keywords - extend as needed
        String[] keywords = new String[]{
            "and","break","do","else","elseif","end","false","for","function","goto","if",
            "in","local","nil","not","or","repeat","return","then","true","until","while"
        };
        for (String kw : keywords) {
            provider.addCompletion(new BasicCompletion(provider, kw));
        }
        // Common global functions (subset)
        String[] funcs = new String[]{
            "assert","collectgarbage","dofile","error","getmetatable","ipairs","load","loadfile","next",
            "pairs","pcall","print","rawequal","rawget","rawlen","rawset","require","select","setmetatable",
            "tonumber","tostring","type","xpcall"
        };
        for (String f : funcs) {
            provider.addCompletion(new BasicCompletion(provider, f));
        }
    }

}
