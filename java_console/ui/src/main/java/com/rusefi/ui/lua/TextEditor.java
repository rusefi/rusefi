package com.rusefi.ui.lua;

import com.rusefi.config.generated.Fields;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AbstractDocument;
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
public class TextEditor {
    private final JPanel area = new JPanel(new BorderLayout());
    private final JTextArea textArea = new JTextArea();
    private final JLabel sizeLabel = new JLabel();

    public TextEditor() {
        textArea.setTabSize(2);

        AbstractDocument pDoc = (AbstractDocument) textArea.getDocument();
        pDoc.setDocumentFilter(new DocumentSizeFilter(Fields.LUA_SCRIPT_SIZE));
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

        area.add(textArea, BorderLayout.CENTER);
        JPanel bottomArea = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        area.add(bottomArea, BorderLayout.SOUTH);
        bottomArea.add(sizeLabel);

        installUndoRedoKeystrokes();
    }

    private void updateSize() {
        sizeLabel.setText(textArea.getText().length() + "/" + Fields.LUA_SCRIPT_SIZE);
    }

    private void installUndoRedoKeystrokes() {
        KeyStroke undoKeyStroke = KeyStroke.getKeyStroke(
                KeyEvent.VK_Z, InputEvent.CTRL_MASK);
        KeyStroke redoKeyStroke = KeyStroke.getKeyStroke(
                KeyEvent.VK_Y, InputEvent.CTRL_MASK);

        UndoManager undoManager = new UndoManager();

        Document document = textArea.getDocument();
        document.addUndoableEditListener(e -> undoManager.addEdit(e.getEdit()));

        // Map undo action
        textArea.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(undoKeyStroke, "undoKeyStroke");
        textArea.getActionMap().put("undoKeyStroke", new AbstractAction() {
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
        textArea.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW)
                .put(redoKeyStroke, "redoKeyStroke");
        textArea.getActionMap().put("redoKeyStroke", new AbstractAction() {
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
        textArea.setText(text);
    }

    public String getText() {
        return textArea.getText();
    }
}
