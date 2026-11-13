package com.rusefi.ui.wizard;

import javax.swing.*;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.WrapLayout;
import java.awt.*;
import java.util.function.Consumer;

/** Explicit confirmation for a user-requested firmware branch change. */
public final class FirmwareBranchSwitchPanel extends JPanel {
    private final JButton limaDevButton = new JButton("Switch to Lima-dev");
    private final JButton developmentButton = new JButton("Switch to dev");
    private final JButton cancelButton = new JButton("Cancel");

    public FirmwareBranchSwitchPanel(String sourceBranch, Consumer<String> confirm, Runnable cancel) {
        super(new BorderLayout());

        JTextArea warning = new JTextArea("Development firmware is unstable.\nSettings may need migration.\n"
            + ("Lima-Dev".equals(sourceBranch) ? "This updater cannot return to Lima-Dev."
                : "This updater cannot return to stable firmware."));
        JPanel choices = new JPanel(new WrapLayout(FlowLayout.LEFT, WizardStyle.GAP, 0));
        JPanel card = new JPanel() {
            @Override
            public Dimension getPreferredSize() {
                Container viewport = SwingUtilities.getAncestorOfClass(JViewport.class, this);
                int pageWidth = viewport != null && viewport.getWidth() > 0
                    ? viewport.getWidth() : FirmwareBranchSwitchPanel.this.getWidth();
                int width = Math.min(700, Math.max(0, pageWidth - 48));
                Insets padding = getInsets();
                choices.setSize(Math.max(0, width - padding.left - padding.right), choices.getHeight());
                Dimension warningSize = warning.getSize();
                try {
                    if (warning.getParent() != null) {
                        Insets warningPadding = warning.getParent().getInsets();
                        // Let Swing lay out all wrapped lines before measuring their preferred height.
                        warning.setSize(Math.max(1, width - padding.left - padding.right
                            - warningPadding.left - warningPadding.right), Short.MAX_VALUE);
                    }
                    ((BoxLayout) getLayout()).invalidateLayout(this);
                    Dimension preferred = super.getPreferredSize();
                    return new Dimension(width, preferred.height);
                } finally {
                    warning.setSize(warningSize);
                }
            }

            @Override
            public Dimension getMaximumSize() {
                return new Dimension(700, getPreferredSize().height);
            }
        };
        card.setLayout(new BoxLayout(card, BoxLayout.Y_AXIS));
        JLabel title = WizardStyle.cardTitle("Switch firmware branch");
        JLabel currentBranch = WizardStyle.description("Current ECU branch: " + sourceBranch);
        for (JComponent header : new JComponent[]{title, currentBranch}) {
            header.setAlignmentX(Component.LEFT_ALIGNMENT);
            card.add(header);
            card.add(Box.createVerticalStrut(WizardStyle.GAP));
        }

        warning.setEditable(false);
        warning.setFocusable(false);
        warning.setLineWrap(true);
        warning.setWrapStyleWord(true);
        warning.setOpaque(false);
        warning.setFont(UIManager.getFont("Label.font").deriveFont(Font.BOLD));
        warning.setForeground(WizardStyle.text());
        warning.setColumns(48);
        JPanel warningSection = new JPanel(new BorderLayout());
        warningSection.add(warning, BorderLayout.CENTER);
        WizardStyle.styleSection(warningSection, WizardStyle.GAP, WizardStyle.GAP);
        warningSection.setAlignmentX(Component.LEFT_ALIGNMENT);
        card.add(warningSection);
        card.add(Box.createVerticalStrut(WizardStyle.GAP));

        AbstractWizardStep.styleChoiceAction(limaDevButton);
        AbstractWizardStep.styleChoiceAction(developmentButton);
        choices.setAlignmentX(Component.LEFT_ALIGNMENT);
        developmentButton.addActionListener(e -> confirm.accept("development"));
        if (!"Lima-Dev".equals(sourceBranch)) {
            limaDevButton.addActionListener(e -> confirm.accept("lts-26lima-dev"));
            addChoice(choices, limaDevButton, "Development builds for LTS Lima.");
        }
        addChoice(choices, developmentButton, "Latest development builds.");
        card.add(choices);

        AbstractWizardStep.styleFooterAction(cancelButton);
        cancelButton.addActionListener(e -> cancel.run());
        JPanel footer = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        footer.setAlignmentX(Component.LEFT_ALIGNMENT);
        footer.add(cancelButton);
        card.add(Box.createVerticalStrut(WizardStyle.GAP));
        card.add(footer);

        WizardStyle.styleCard(card, WizardStyle.LARGE_GAP, WizardStyle.LARGE_GAP);
        Box row = new Box(BoxLayout.X_AXIS) {
            @Override
            public Dimension getMaximumSize() {
                return new Dimension(Integer.MAX_VALUE, card.getPreferredSize().height);
            }
        };
        row.add(Box.createHorizontalGlue());
        row.add(card);
        row.add(Box.createHorizontalGlue());
        JPanel page = new ScrollablePanel() {
            @Override
            public boolean getScrollableTracksViewportHeight() {
                return getParent() instanceof JViewport && getPreferredSize().height <= getParent().getHeight();
            }
        };
        page.setLayout(new BoxLayout(page, BoxLayout.Y_AXIS));
        page.setBorder(BorderFactory.createEmptyBorder(24, 24, 24, 24));
        page.add(Box.createVerticalGlue());
        page.add(row);
        page.add(Box.createVerticalGlue());
        add(WizardStyle.pageScrollPane(page), BorderLayout.CENTER);
        registerKeyboardAction(e -> cancel.run(), KeyStroke.getKeyStroke("ESCAPE"),
            JComponent.WHEN_IN_FOCUSED_WINDOW);
    }

    private static void addChoice(JPanel choices, JButton button, String description) {
        JPanel choice = new JPanel(new BorderLayout(0, 4));
        JPanel actionRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        actionRow.add(button);
        choice.add(actionRow, BorderLayout.NORTH);
        JLabel detail = WizardStyle.description(description);
        detail.setAlignmentX(Component.LEFT_ALIGNMENT);
        detail.setBorder(BorderFactory.createEmptyBorder(4, 4, WizardStyle.GAP, 0));
        choice.add(detail, BorderLayout.CENTER);
        choices.add(choice);
    }

    public void clickLimaDevForTests() { limaDevButton.doClick(); }
    public void clickDevelopmentForTests() { developmentButton.doClick(); }
    public void clickCancelForTests() { cancelButton.doClick(); }
}
