package com.rusefi.ui.wizard;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

/** Full-size manual DFU selection, hosted by the same card as the other firmware wizards. */
public final class DfuBoardSelectionPanel extends JPanel {
    private final JPanel card = new JPanel(new BorderLayout(WizardStyle.GAP, WizardStyle.LARGE_GAP));
    private final JTextField search = new JTextField();
    private final DefaultListModel<String> model = new DefaultListModel<>();
    private final JList<String> boards = new JList<>(model);
    private final JLabel status = new JLabel("Loading board list...");
    private final JButton cancel = new JButton("Cancel");
    private final JButton confirm = new JButton("Download and flash");
    private final List<String> targets = new ArrayList<>();
    private final Consumer<String> completion;
    private boolean completed;

    public DfuBoardSelectionPanel(Consumer<String> completion) {
        super(null);
        this.completion = completion;
        setBackground(WizardStyle.background());
        card.setBackground(WizardStyle.surface());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(26, 30, 26, 30)));
        Font bodyFont = UIManager.getFont("Label.font").deriveFont(
            Math.max(18f, UIManager.getFont("Label.font").getSize2D() * 1.15f));
        JPanel heading = new JPanel(new BorderLayout(0, WizardStyle.GAP));
        heading.setOpaque(false);
        JLabel title = new JLabel("Pick board - Manual DFU Update");
        AbstractWizardStep.styleTitle(title);
        heading.add(title, BorderLayout.NORTH);
        JTextArea explanation = new JTextArea("DFU mode cannot identify or verify the board model. "
            + "Select the board connected in DFU mode, then choose Download and flash to update its firmware.");
        explanation.setEditable(false);
        explanation.setOpaque(false);
        explanation.setLineWrap(true);
        explanation.setWrapStyleWord(true);
        explanation.setRows(3);
        explanation.setFont(bodyFont);
        heading.add(explanation, BorderLayout.CENTER);
        card.add(heading, BorderLayout.NORTH);

        JPanel selection = new JPanel(new BorderLayout(0, WizardStyle.GAP));
        selection.setOpaque(false);
        JPanel searchRow = new JPanel(new BorderLayout(WizardStyle.GAP, 0));
        searchRow.setOpaque(false);
        JLabel searchLabel = new JLabel("Find board:");
        searchLabel.setFont(bodyFont);
        search.setFont(bodyFont);
        status.setFont(bodyFont);
        searchLabel.setLabelFor(search);
        searchRow.add(searchLabel, BorderLayout.WEST);
        searchRow.add(search, BorderLayout.CENTER);
        search.setEnabled(false);
        selection.add(searchRow, BorderLayout.NORTH);
        boards.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        boards.setFont(bodyFont.deriveFont(bodyFont.getSize2D() * 1.15f));
        boards.setFixedCellHeight(boards.getFontMetrics(boards.getFont()).getHeight() + 14);
        boards.getAccessibleContext().setAccessibleName("DFU board model");
        selection.add(new JScrollPane(boards), BorderLayout.CENTER);
        selection.add(status, BorderLayout.SOUTH);
        card.add(selection, BorderLayout.CENTER);

        AbstractWizardStep.styleButton(cancel);
        AbstractWizardStep.stylePrimaryAction(confirm);
        cancel.setFont(bodyFont);
        confirm.setFont(bodyFont.deriveFont(Font.BOLD));
        confirm.setEnabled(false);
        cancel.addActionListener(e -> finish(null));
        confirm.addActionListener(e -> {
            if (boards.getSelectedValue() != null) {
                finish(boards.getSelectedValue());
            }
        });
        boards.addListSelectionListener(e -> confirm.setEnabled(!completed && boards.getSelectedValue() != null));
        search.getDocument().addDocumentListener(new DocumentListener() {
            public void insertUpdate(DocumentEvent e) { filter(); }
            public void removeUpdate(DocumentEvent e) { filter(); }
            public void changedUpdate(DocumentEvent e) { filter(); }
        });
        JPanel actions = new JPanel(new BorderLayout());
        actions.setOpaque(false);
        actions.add(cancel, BorderLayout.WEST);
        actions.add(confirm, BorderLayout.EAST);
        card.add(actions, BorderLayout.SOUTH);
        add(card);
        registerKeyboardAction(e -> finish(null), KeyStroke.getKeyStroke("ESCAPE"), WHEN_IN_FOCUSED_WINDOW);
        SwingUtilities.invokeLater(() -> {
            if (!completed) {
                cancel.requestFocusInWindow();
            }
        });
    }

    @Override
    public void doLayout() {
        // Size against the actual host view, independent of BoxLayout alignment and UI scaling.
        int width = (int) Math.round(getWidth() * 0.7);
        int height = (int) Math.round(getHeight() * 0.7);
        card.setBounds((getWidth() - width) / 2, (getHeight() - height) / 2, width, height);
    }

    public void setBoards(List<String> targets, String suggested) {
        if (completed) {
            return;
        }
        this.targets.clear();
        this.targets.addAll(targets);
        search.setEnabled(true);
        filter();
        if (targets.contains(suggested)) {
            boards.setSelectedValue(suggested, true);
            status.setText("Previously used board suggested. Confirm it matches the connected board.");
        }
        search.requestFocusInWindow();
    }

    public void showLoadError(String message) {
        if (!completed) {
            status.setText("Could not load boards: " + message);
            confirm.setEnabled(false);
        }
    }

    private void filter() {
        String previous = boards.getSelectedValue();
        String query = search.getText().trim().toLowerCase(java.util.Locale.ROOT);
        model.clear();
        for (String target : targets) {
            if (target.toLowerCase(java.util.Locale.ROOT).contains(query)) {
                model.addElement(target);
            }
        }
        // JList.setSelectedValue clears selection when the previous target is filtered out.
        boards.setSelectedValue(previous, true);
        status.setText(model.isEmpty() ? "No boards match your search." : "Select the board to flash.");
    }

    private void finish(String selected) {
        if (!completed) {
            completed = true;
            confirm.setEnabled(false);
            cancel.setEnabled(false);
            completion.accept(selected);
        }
    }

    JTextField searchForTests() { return search; }
    JList<String> boardsForTests() { return boards; }
    JButton confirmForTests() { return confirm; }
    JButton cancelForTests() { return cancel; }
}
