package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;

public final class EmergencyWipePanel extends JPanel {
    private static final int CARD_WIDTH = 760;
    private final JButton cancelButton = new JButton("Cancel");
    private final JButton eraseButton = new JButton("Erase Internal Flash");

    public EmergencyWipePanel(String message, Runnable erase, Runnable cancel) {
        super(new BorderLayout());
        setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));

        JLabel heading = new JLabel("Emergency Internal Flash Wipe");
        AbstractWizardStep.styleTitle(heading);
        add(heading, BorderLayout.NORTH);

        JPanel card = new JPanel();
        card.setLayout(new BoxLayout(card, BoxLayout.Y_AXIS));
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(26, 30, 26, 30)));
        card.setPreferredSize(new Dimension(CARD_WIDTH, 460));
        card.setMaximumSize(new Dimension(CARD_WIDTH, 460));

        JLabel warning = new JLabel("This action cannot be undone", UIManager.getIcon("OptionPane.warningIcon"),
            SwingConstants.LEFT);
        warning.setFont(warning.getFont().deriveFont(Font.BOLD, warning.getFont().getSize() * 1.3f));
        warning.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(warning);
        card.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));

        JTextArea details = new JTextArea(message);
        details.setEditable(false);
        details.setOpaque(false);
        details.setLineWrap(true);
        details.setWrapStyleWord(true);
        details.setFont(UIManager.getFont("Label.font").deriveFont(UIManager.getFont("Label.font").getSize() * 1.1f));
        details.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(details);
        card.add(Box.createVerticalGlue());
        card.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));

        AbstractWizardStep.styleButton(cancelButton);
        cancelButton.addActionListener(e -> cancel.run());
        AbstractWizardStep.stylePrimaryAction(eraseButton);
        eraseButton.addActionListener(e -> {
            eraseButton.setEnabled(false);
            erase.run();
        });

        JPanel actions = new JPanel(new BorderLayout(WizardStyle.GAP, 0));
        actions.setAlignmentX(Component.LEFT_ALIGNMENT);
        actions.setMaximumSize(new Dimension(Integer.MAX_VALUE, eraseButton.getPreferredSize().height));
        actions.add(cancelButton, BorderLayout.WEST);
        actions.add(eraseButton, BorderLayout.EAST);
        card.add(actions);

        JPanel center = new JPanel(new GridBagLayout());
        center.add(card);
        add(center, BorderLayout.CENTER);

        registerKeyboardAction(e -> cancel.run(), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
        SwingUtilities.invokeLater(cancelButton::requestFocusInWindow);
    }

    JButton cancelButtonForTests() {
        return cancelButton;
    }

    JButton eraseButtonForTests() {
        return eraseButton;
    }
}
