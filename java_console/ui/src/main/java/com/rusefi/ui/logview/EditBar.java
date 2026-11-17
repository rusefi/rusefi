package com.rusefi.ui.logview;

import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;

/**
 * The strip above the log viewer while the shown log has edits that are not saved: what the last edit did, with
 * Save as… and Undo. [tag:console_theme]
 */
final class EditBar extends ThemedStrip {
    final JLabel titleLabel = new JLabel("Edited log, not saved");
    final JLabel detailLabel = ChartToolbar.muted(new JLabel());
    final JButton saveButton = new JButton("Save as…");
    final JButton undoButton = new JButton("Undo");

    EditBar(Runnable onSave, Runnable onUndo) {
        super(new BorderLayout(12, 0), "Console.Banner.warning.background", false);
        setBorder(BorderFactory.createEmptyBorder(6, 12, 6, 12));
        titleLabel.setForeground(themed("Console.Banner.warning.accent", new Color(0xb7791f)));
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.BOLD));
        saveButton.setToolTipText("Save the edited log to a new .msl file");
        undoButton.setToolTipText("Bring back the data the last edit dropped");
        saveButton.addActionListener(e -> onSave.run());
        undoButton.addActionListener(e -> onUndo.run());

        JPanel text = new JPanel(new FlowLayout(FlowLayout.LEFT, 14, 0));
        text.setOpaque(false);
        text.add(titleLabel);
        text.add(detailLabel);
        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));
        buttons.setOpaque(false);
        buttons.add(new JoinedButtons(saveButton));
        buttons.add(new JoinedButtons(undoButton));
        add(text, BorderLayout.CENTER);
        add(buttons, BorderLayout.EAST);
        setVisible(false);
    }

    /** Shows the strip with the last edit, e.g. "Dropped 0.00–12.34 s (1234 samples)", and how many edits there are. */
    void show(String lastEdit, int edits) {
        detailLabel.setText(edits > 1 ? lastEdit + " · " + edits + " edits" : lastEdit);
        setVisible(true);
    }
}
