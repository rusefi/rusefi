package com.rusefi.ui.wizard;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.function.IntConsumer;

public final class FirmwareRollbackPanel extends JPanel {
    private static final int CARD_WIDTH = 760;
    private final JToggleButton[] choices;

    public FirmwareRollbackPanel(boolean currentKnown, String[] builds, IntConsumer confirm, Runnable cancel) {
        super(new BorderLayout());
        setBorder(BorderFactory.createEmptyBorder(
            WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP));

        JLabel heading = new JLabel("Rollback Firmware");
        AbstractWizardStep.styleTitle(heading);

        JPanel header = new JPanel(new BorderLayout());
        header.add(heading, BorderLayout.WEST);
        add(header, BorderLayout.NORTH);

        JPanel card = new JPanel();
        card.setLayout(new BoxLayout(card, BoxLayout.Y_AXIS));
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(WizardStyle.border()),
            BorderFactory.createEmptyBorder(22, 26, 22, 26)));
        card.setPreferredSize(new Dimension(CARD_WIDTH, 520));
        card.setMaximumSize(new Dimension(CARD_WIDTH, 520));

        JLabel prompt = new JLabel("Double-click a previous LTS firmware build");
        AbstractWizardStep.styleTitle(prompt);
        prompt.setAlignmentX(Component.CENTER_ALIGNMENT);
        card.add(prompt);

        JLabel explanation = new JLabel("<html><center>"
            + (currentKnown
                ? "Choose an older build to install."
                : "The current build is unknown. Explicitly choose the firmware to install.")
            + "<br>The current tune will be backed up. If the calibration layout changed, defaults will be used."
            + "</center></html>");
        explanation.setHorizontalAlignment(SwingConstants.CENTER);
        explanation.setForeground(WizardStyle.muted());
        explanation.setAlignmentX(Component.CENTER_ALIGNMENT);
        card.add(explanation);
        card.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));

        JPanel buildButtons = new JPanel();
        buildButtons.setLayout(new BoxLayout(buildButtons, BoxLayout.Y_AXIS));
        ButtonGroup group = new ButtonGroup();
        choices = new JToggleButton[builds.length];
        for (int i = 0; i < builds.length; i++) {
            int index = i;
            JToggleButton choice = new JToggleButton(builds[i]);
            AbstractWizardStep.styleButton(choice);
            choice.setFont(choice.getFont().deriveFont(choice.getFont().getSize() * 1.2f));
            choice.setMargin(new Insets(14, 24, 14, 24));
            choice.setAlignmentX(Component.CENTER_ALIGNMENT);
            choice.setMaximumSize(new Dimension(Integer.MAX_VALUE, choice.getPreferredSize().height));
            choice.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent event) {
                    if (event.getClickCount() == 2 && SwingUtilities.isLeftMouseButton(event)) {
                        confirm.accept(index);
                    }
                }
            });
            group.add(choice);
            buildButtons.add(choice);
            if (i + 1 < builds.length) {
                buildButtons.add(Box.createVerticalStrut(WizardStyle.GAP));
            }
            choices[i] = choice;
        }

        JScrollPane scrollPane = new JScrollPane(buildButtons);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());
        scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.getVerticalScrollBar().setUnitIncrement(16);
        scrollPane.setAlignmentX(Component.CENTER_ALIGNMENT);
        card.add(scrollPane);
        card.add(Box.createVerticalStrut(WizardStyle.LARGE_GAP));

        JButton cancelButton = new JButton("Cancel");
        AbstractWizardStep.styleButton(cancelButton);
        cancelButton.addActionListener(e -> cancel.run());

        JPanel actions = new JPanel(new BorderLayout(WizardStyle.GAP, 0));
        actions.setAlignmentX(Component.CENTER_ALIGNMENT);
        actions.setMaximumSize(new Dimension(Integer.MAX_VALUE, cancelButton.getPreferredSize().height));
        actions.add(cancelButton, BorderLayout.WEST);
        card.add(actions);

        JPanel center = new JPanel(new GridBagLayout());
        center.add(card);
        add(center, BorderLayout.CENTER);

        registerKeyboardAction(e -> cancel.run(), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
    }

    JToggleButton choiceForTests(int index) {
        return choices[index];
    }
}
