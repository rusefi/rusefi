package com.rusefi.ui.wizard;

import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;

/** Full-screen firmware update consent or a readable programming error with a safe return action. */
public final class FirmwareProgrammingPanel extends JPanel {
    private final JButton backButton;
    @Nullable
    private final JButton confirmButton;
    private final JTextArea messageDetails;
    private final AtomicBoolean completed = new AtomicBoolean();

    public static FirmwareProgrammingPanel updateConfirmation(String message, Runnable confirm, Runnable cancel) {
        return new FirmwareProgrammingPanel("Update ECU firmware", message, "Update ECU firmware", confirm, cancel);
    }

    public static FirmwareProgrammingPanel error(String title, String message, Runnable close) {
        return new FirmwareProgrammingPanel(title, message, null, close, close);
    }

    private FirmwareProgrammingPanel(String title, String message, @Nullable String confirmLabel,
                                     Runnable confirm, Runnable close) {
        super(new BorderLayout());
        Objects.requireNonNull(title, "title");
        Objects.requireNonNull(message, "message");
        Objects.requireNonNull(confirm, "confirm");
        Objects.requireNonNull(close, "close");
        backButton = new JButton(confirmLabel == null ? "Back" : "Cancel");
        confirmButton = confirmLabel == null ? null : new JButton(confirmLabel);
        setBackground(WizardStyle.background());
        setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));

        JPanel card = new JPanel(new BorderLayout(0, WizardStyle.LARGE_GAP));
        card.setBackground(WizardStyle.surface());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(26, 30, 26, 30)));
        card.setPreferredSize(new Dimension(1100, 580));
        JLabel heading = new JLabel(title, UIManager.getIcon(
            confirmLabel == null ? "OptionPane.errorIcon" : "OptionPane.warningIcon"), SwingConstants.LEFT);
        AbstractWizardStep.styleTitle(heading);
        card.add(heading, BorderLayout.NORTH);

        messageDetails = new JTextArea(message);
        messageDetails.setEditable(false);
        messageDetails.setLineWrap(true);
        messageDetails.setWrapStyleWord(true);
        messageDetails.setFont(UIManager.getFont("Label.font").deriveFont(
            UIManager.getFont("Label.font").getSize() * 1.1f));
        messageDetails.setBackground(WizardStyle.surface());
        messageDetails.getAccessibleContext().setAccessibleName("Firmware programming details");
        JScrollPane details = new JScrollPane(messageDetails);
        details.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        details.getVerticalScrollBar().setUnitIncrement(16);
        card.add(details, BorderLayout.CENTER);

        AbstractWizardStep.styleNegativeAction(backButton);
        backButton.addActionListener(e -> complete(close));
        JPanel actions = new JPanel(new BorderLayout());
        actions.setOpaque(false);
        actions.add(backButton, BorderLayout.WEST);
        if (confirmButton != null) {
            AbstractWizardStep.stylePrimaryAction(confirmButton);
            confirmButton.addActionListener(e -> complete(confirm));
            actions.add(confirmButton, BorderLayout.EAST);
        }
        card.add(actions, BorderLayout.SOUTH);
        JPanel center = new JPanel(new GridBagLayout());
        center.setOpaque(false);
        center.add(card);
        add(center, BorderLayout.CENTER);

        registerKeyboardAction(e -> complete(close), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
        setFocusCycleRoot(true);
        setFocusTraversalPolicy(new LayoutFocusTraversalPolicy() {
            @Override
            public Component getDefaultComponent(Container focusCycleRoot) {
                return backButton;
            }
        });
        SwingUtilities.invokeLater(backButton::requestFocusInWindow);
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
        backButton.setEnabled(false);
        if (confirmButton != null) {
            confirmButton.setEnabled(false);
        }
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

    JButton backButtonForTests() {
        return backButton;
    }

    @Nullable
    JButton confirmButtonForTests() {
        return confirmButton;
    }

    JTextArea messageDetailsForTests() {
        return messageDetails;
    }
}
