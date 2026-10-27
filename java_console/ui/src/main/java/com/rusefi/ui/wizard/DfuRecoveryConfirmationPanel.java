package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;

/** Full-screen recovery consent; completing or hiding the panel invalidates its actions. */
public final class DfuRecoveryConfirmationPanel extends JPanel {
    private final JButton cancelButton = new JButton("Cancel");
    private final JButton confirmButton = new JButton("Recover ECU");
    private final JTextArea messageDetails;
    private final AtomicBoolean completed = new AtomicBoolean();

    public DfuRecoveryConfirmationPanel(String message, Runnable confirm, Runnable cancel) {
        super(new BorderLayout());
        Objects.requireNonNull(message, "message");
        Objects.requireNonNull(confirm, "confirm");
        Objects.requireNonNull(cancel, "cancel");
        setBackground(WizardStyle.background());
        setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));

        JPanel card = new JPanel(new BorderLayout(0, WizardStyle.LARGE_GAP));
        card.setBackground(WizardStyle.surface());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(26, 30, 26, 30)));
        card.setPreferredSize(new Dimension(1100, 580));

        JPanel headings = new JPanel();
        headings.setOpaque(false);
        headings.setLayout(new BoxLayout(headings, BoxLayout.Y_AXIS));
        JLabel heading = new JLabel("Recover ECU");
        AbstractWizardStep.styleTitle(heading);
        heading.setAlignmentX(Component.LEFT_ALIGNMENT);
        headings.add(heading);
        headings.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));
        JLabel warning = new JLabel("Recovery may erase firmware and settings",
            UIManager.getIcon("OptionPane.warningIcon"), SwingConstants.LEFT);
        warning.setFont(warning.getFont().deriveFont(Font.BOLD, warning.getFont().getSize() * 1.3f));
        warning.setAlignmentX(Component.LEFT_ALIGNMENT);
        headings.add(warning);
        card.add(headings, BorderLayout.NORTH);

        messageDetails = new JTextArea(message);
        messageDetails.setEditable(false);
        messageDetails.setLineWrap(true);
        messageDetails.setWrapStyleWord(true);
        messageDetails.setFont(UIManager.getFont("Label.font").deriveFont(
            UIManager.getFont("Label.font").getSize() * 1.1f));
        messageDetails.setBackground(WizardStyle.surface());
        messageDetails.getAccessibleContext().setAccessibleName("Recovery confirmation details");
        JScrollPane details = new JScrollPane(messageDetails);
        details.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        details.getVerticalScrollBar().setUnitIncrement(16);
        card.add(details, BorderLayout.CENTER);

        AbstractWizardStep.styleNegativeAction(cancelButton);
        AbstractWizardStep.stylePrimaryAction(confirmButton);
        cancelButton.addActionListener(e -> complete(cancel));
        confirmButton.addActionListener(e -> complete(confirm));
        JPanel actions = new JPanel(new BorderLayout());
        actions.setOpaque(false);
        actions.add(cancelButton, BorderLayout.WEST);
        actions.add(confirmButton, BorderLayout.EAST);
        card.add(actions, BorderLayout.SOUTH);

        JPanel center = new JPanel(new GridBagLayout());
        center.setOpaque(false);
        center.add(card);
        add(center, BorderLayout.CENTER);

        registerKeyboardAction(e -> complete(cancel), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
        setFocusCycleRoot(true);
        setFocusTraversalPolicy(new LayoutFocusTraversalPolicy() {
            @Override
            public Component getDefaultComponent(Container focusCycleRoot) {
                return cancelButton;
            }
        });
        SwingUtilities.invokeLater(cancelButton::requestFocusInWindow);
        addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) != 0 && !isShowing()) {
                invalidateActions();
            }
        });
    }

    private void complete(Runnable action) {
        if (completed.compareAndSet(false, true)) {
            disableActions();
            action.run();
        }
    }

    private void invalidateActions() {
        completed.set(true);
        disableActions();
    }

    private void disableActions() {
        cancelButton.setEnabled(false);
        confirmButton.setEnabled(false);
    }

    @Override
    public void setVisible(boolean visible) {
        if (!visible && completed != null) {
            invalidateActions();
        }
        super.setVisible(visible);
    }

    @Override
    public void removeNotify() {
        invalidateActions();
        super.removeNotify();
    }

    JButton cancelButtonForTests() {
        return cancelButton;
    }

    JButton confirmButtonForTests() {
        return confirmButton;
    }

    JTextArea messageDetailsForTests() {
        return messageDetails;
    }
}
