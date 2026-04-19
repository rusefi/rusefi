package com.rusefi.ui.lua;

import com.opensr5.ini.field.StringIniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.ShortcutsHelper;
import com.rusefi.io.LinkManager;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.UiUtils;
import org.jetbrains.annotations.NotNull;
import org.fife.ui.autocomplete.AutoCompletion;
import org.fife.ui.autocomplete.BasicCompletion;
import org.fife.ui.autocomplete.CompletionProvider;
import org.fife.ui.autocomplete.DefaultCompletionProvider;
import org.fife.ui.rsyntaxtextarea.RSyntaxTextArea;
import org.fife.ui.rsyntaxtextarea.SyntaxConstants;
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
        Font current = textArea.getFont();
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, current != null ? current.getSize() : 12));
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
                int offset = textArea.getCaretPosition();
                try {
                    int line = textArea.getLineOfOffset(offset);
                    locationLabel.setText(Integer.toString(line + 1));
                } catch (BadLocationException ignored) {
                }
            }
        });

        // Install autocomplete
        installDefaultCompletions(completionProvider);
        AutoCompletion ac = new AutoCompletion(completionProvider);
        ac.setAutoActivationEnabled(true);
        ac.setAutoActivationDelay(250);
        ac.install(textArea);

        RTextScrollPane textAreaScroll = new RTextScrollPane(textArea, true);
        area.add(textAreaScroll, BorderLayout.CENTER);
        JPanel bottomArea = new JPanel(new BorderLayout());
        area.add(bottomArea, BorderLayout.SOUTH);
        bottomArea.add(locationLabel, BorderLayout.WEST);
        bottomArea.add(sizeLabel, BorderLayout.EAST);

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
        int limit = field.getSize();
        sizeLabel.setText(textArea.getText().length() + "/" + limit);
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
