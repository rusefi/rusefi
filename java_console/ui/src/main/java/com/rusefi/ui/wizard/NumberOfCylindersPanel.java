package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.SegmentedControl;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

public class NumberOfCylindersPanel extends AbstractWizardStep {
    private static final String TWO_STROKE_FIELD = "twoStroke";
    private static final String FOUR_STROKE_VALUE = "Four Stroke";
    private static final String TWO_STROKE_VALUE = "Two Stroke";

    private final JLayeredPane layeredPane = new JLayeredPane();
    private final JPanel mainPanel = new JPanel(new GridBagLayout());
    private final JPanel overlayPanel = new JPanel() {
        @Override
        protected void paintComponent(Graphics g) {
            g.setColor(new Color(0, 0, 0, 150));
            g.fillRect(0, 0, getWidth(), getHeight());
            super.paintComponent(g);
        }
    };

    private final UIContext uiContext;

    public NumberOfCylindersPanel(UIContext uiContext) {
        super("Number of Cylinders", "wizardNumberOfCylinders");
        this.uiContext = uiContext;
        layeredPane.add(mainPanel, JLayeredPane.DEFAULT_LAYER);
        layeredPane.add(overlayPanel, JLayeredPane.PALETTE_LAYER);
        overlayPanel.setVisible(false);
        overlayPanel.setOpaque(false);
        overlayPanel.setLayout(new GridBagLayout());

        overlayPanel.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mouseClicked(java.awt.event.MouseEvent e) {
                overlayPanel.setVisible(false);
            }
        });

        JLabel messageLabel = new JLabel();
        messageLabel.setForeground(Color.WHITE);
        scale(messageLabel, 2);
        overlayPanel.add(messageLabel);

        layeredPane.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                mainPanel.setBounds(0, 0, layeredPane.getWidth(), layeredPane.getHeight());
                updateOverlayBounds();
            }
        });

        JPanel card = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(WizardStyle.sectionLabel("CYLINDER SETUP"), gbc);

        gbc.gridy++;
        card.add(WizardStyle.cardTitle("How many cylinders?"), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 22, 0);
        card.add(WizardStyle.description("Pick the engine cycle, then click the number of cylinders."), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 6, 0);
        card.add(WizardStyle.sectionLabel("ENGINE CYCLE"), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 20, 0);
        gbc.fill = GridBagConstraints.NONE;
        JToggleButton fourStroke = SegmentedControl.segment("4 Stroke");
        JToggleButton twoStroke = SegmentedControl.segment("2 Stroke");
        fourStroke.setSelected(true);
        ButtonGroup strokeGroup = new ButtonGroup();
        strokeGroup.add(fourStroke);
        strokeGroup.add(twoStroke);
        card.add(new SegmentedControl(fourStroke, twoStroke), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 6, 0);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        card.add(WizardStyle.sectionLabel("CYLINDER COUNT"), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 0, 0);
        JPanel buttonsPanel = new JPanel(new GridLayout(3, 4, 10, 10));
        int[] options = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16};

        for (int option : options) {
            JButton button = new JButton(String.valueOf(option));
            scale(button, 1.5f);
            buttonsPanel.add(button);

            boolean isGotcha = option == 7 || option == 9 || option == 16;
            if (isGotcha) {
                styleUnsupportedChoice(button, "Not supported by the setup wizard");
                button.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        messageLabel.setText("<html><center>Oh, " + option +
                            "? If you <i>really</i> have an engine like that,<br>please give us a call!</center></html>");
                        overlayPanel.setVisible(true);
                        updateOverlayBounds();
                    }
                });
            } else {
                styleChoiceAction(button);
                button.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        WizardConfig cfg = WizardConfig.snapshot(uiContext);
                        if (cfg == null) {
                            return;
                        }
                        WizardStepResult result = createResult(cfg.ini, cfg.image, option, twoStroke.isSelected());
                        if (result == null) {
                            return;
                        }
                        fireCompleted(result);
                    }
                });
            }
        }

        card.add(buttonsPanel, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(10, 0, 0, 0);
        card.add(WizardStyle.description("Greyed-out counts are not supported by the setup wizard."), gbc);
        WizardStyle.styleCard(card, 24, 28);
        mainPanel.add(card);
    }

    static WizardStepResult createResult(IniFileModel ini, ConfigurationImage image, int cylinders, boolean twoStroke) {
        IniField field = ini.findIniField(TWO_STROKE_FIELD).orElse(null);
        if (field == null) {
            return null;
        }

        ConfigurationImage modified = image.clone();
        ConfigurationImageGetterSetter.setValue2(field, modified, TWO_STROKE_FIELD,
            twoStroke ? TWO_STROKE_VALUE : FOUR_STROKE_VALUE);
        return new WizardStepResult("cylindersCount", String.valueOf(cylinders), modified);
    }

    private void updateOverlayBounds() {
        int w = (int) (layeredPane.getWidth() * 0.8);
        int h = (int) (layeredPane.getHeight() * 0.8);
        int x = (layeredPane.getWidth() - w) / 2;
        int y = (layeredPane.getHeight() - h) / 2;
        overlayPanel.setBounds(x, y, w, h);
    }

    @Override
    public String getFooterHint() {
        return "Click the number of cylinders to save it and continue.";
    }

    @Override
    public JComponent getPanel() {
        return layeredPane;
    }
}
