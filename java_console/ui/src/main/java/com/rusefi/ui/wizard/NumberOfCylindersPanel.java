package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;
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

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.insets = new Insets(20, 20, 20, 20);
        gbc.anchor = GridBagConstraints.CENTER;

        JLabel label = new JLabel("How many cylinders?");
        styleTitle(label);
        mainPanel.add(label, gbc);

        gbc.gridy++;
        JToggleButton fourStroke = new JToggleButton("4 Stroke", true);
        JToggleButton twoStroke = new JToggleButton("2 Stroke");
        styleButton(fourStroke);
        styleButton(twoStroke);
        scale(fourStroke, 1.2f);
        scale(twoStroke, 1.2f);
        fourStroke.setMargin(new Insets(9, 18, 9, 18));
        twoStroke.setMargin(new Insets(9, 18, 9, 18));
        ButtonGroup strokeGroup = new ButtonGroup();
        strokeGroup.add(fourStroke);
        strokeGroup.add(twoStroke);
        JPanel strokePanel = new JPanel(new GridLayout(1, 2, 10, 0));
        strokePanel.add(fourStroke);
        strokePanel.add(twoStroke);
        mainPanel.add(strokePanel, gbc);

        gbc.gridy++;
        JPanel buttonsPanel = new JPanel(new GridLayout(3, 4, 10, 10));
        int[] options = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 16};

        for (int option : options) {
            JButton button = new JButton(String.valueOf(option));
            scale(button, 1.5f);
            stylePrimaryAction(button);
            buttonsPanel.add(button);

            boolean isGotcha = option == 7 || option == 9 || option == 16;
            if (isGotcha) {
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

        mainPanel.add(buttonsPanel, gbc);
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
    public JComponent getPanel() {
        return layeredPane;
    }
}
