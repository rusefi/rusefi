package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicBoolean;

/** Acknowledges a firmware eligibility error before returning to the previous screen. */
public final class FirmwareUpdateBlockedPanel extends JPanel {
    private static final int CARD_WIDTH = 1100;

    private final JButton backButton = new JButton("Back");
    private final JTextArea messageDetails;

    public FirmwareUpdateBlockedPanel(String message, Runnable close) {
        super(new BorderLayout());
        Objects.requireNonNull(message, "message");
        Objects.requireNonNull(close, "close");

        setBackground(WizardStyle.background());
        setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));

        JPanel card = new JPanel(new BorderLayout(0, WizardStyle.LARGE_GAP));
        card.setBackground(WizardStyle.surface());
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(26, 30, 26, 30)));
        card.setPreferredSize(new Dimension(CARD_WIDTH, 580));

        JPanel headings = new JPanel();
        headings.setOpaque(false);
        headings.setLayout(new BoxLayout(headings, BoxLayout.Y_AXIS));
        JLabel heading = new JLabel("Firmware update blocked");
        AbstractWizardStep.styleTitle(heading);
        heading.setAlignmentX(Component.LEFT_ALIGNMENT);
        headings.add(heading);
        headings.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));

        JLabel warning = new JLabel("Firmware update cannot continue", UIManager.getIcon("OptionPane.errorIcon"),
            SwingConstants.LEFT);
        warning.setFont(warning.getFont().deriveFont(Font.BOLD, warning.getFont().getSize() * 1.3f));
        warning.setAlignmentX(Component.LEFT_ALIGNMENT);
        warning.getAccessibleContext().setAccessibleName("Firmware update error");
        headings.add(warning);
        card.add(headings, BorderLayout.NORTH);

        messageDetails = new JTextArea(message);
        messageDetails.setEditable(false);
        messageDetails.setLineWrap(true);
        messageDetails.setWrapStyleWord(true);
        messageDetails.setFont(UIManager.getFont("Label.font").deriveFont(
            UIManager.getFont("Label.font").getSize() * 1.1f));
        messageDetails.setBackground(WizardStyle.surface());
        messageDetails.getAccessibleContext().setAccessibleName("Firmware update eligibility details");

        JScrollPane messageScrollPane = new JScrollPane(messageDetails);
        messageScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        messageScrollPane.getVerticalScrollBar().setUnitIncrement(16);
        card.add(messageScrollPane, BorderLayout.CENTER);

        AtomicBoolean closed = new AtomicBoolean();
        Runnable closeOnce = () -> {
            if (closed.compareAndSet(false, true)) {
                close.run();
            }
        };
        AbstractWizardStep.stylePrimaryAction(backButton);
        backButton.addActionListener(e -> closeOnce.run());
        backButton.getAccessibleContext().setAccessibleDescription("Return to the previous screen");

        JPanel actions = new JPanel(new BorderLayout());
        actions.setOpaque(false);
        actions.add(backButton, BorderLayout.WEST);
        card.add(actions, BorderLayout.SOUTH);

        JPanel center = new JPanel(new GridBagLayout());
        center.setOpaque(false);
        center.add(card);
        add(center, BorderLayout.CENTER);

        registerKeyboardAction(e -> closeOnce.run(), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
        SwingUtilities.invokeLater(backButton::requestFocusInWindow);
    }

    JButton backButtonForTests() {
        return backButton;
    }

    JTextArea messageDetailsForTests() {
        return messageDetails;
    }
}
