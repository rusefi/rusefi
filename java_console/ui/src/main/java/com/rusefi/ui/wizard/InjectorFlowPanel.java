package com.rusefi.ui.wizard;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;

import static com.devexperts.logging.Logging.getLogging;

public class InjectorFlowPanel extends AbstractWizardStep {
    private static final Logging log = getLogging(InjectorFlowPanel.class);

    private static final String HARDCODED_FIELD = "hardcoded_injectors";
    private static final String FLOW_FIELD = "injector_flow";
    private static final String UNITS_FIELD = "injectorFlowAsMassFlow";
    private static final String UNITS_CC_MIN = "cc/min";

    private final UIContext uiContext;
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel body = new JPanel(new GridBagLayout());

    private JComboBox<String> presetCombo;
    private JTextField flowTextField;

    public InjectorFlowPanel(UIContext uiContext) {
        super("Injector Flow", "wizardInjectorFlow");
        this.uiContext = uiContext;

        JLabel title = new JLabel(getTitle());
        title.setHorizontalAlignment(SwingConstants.CENTER);
        title.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 15));
        scale(title, 2.5f);
        content.add(title, BorderLayout.NORTH);

        content.add(body, BorderLayout.CENTER);

        JButton continueButton = new JButton("Continue");
        scale(continueButton, 1.5f);
        continueButton.addActionListener(e -> onContinue());
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 15));
        buttonPanel.add(continueButton);
        content.add(buttonPanel, BorderLayout.SOUTH);

        rebuildBody();
    }

    /**
     * Rebuilds the center panel against the current INI — picks hardcoded-preset dropdown or cc/min text input.
     */
    private void rebuildBody() {
        body.removeAll();
        presetCombo = null;
        flowTextField = null;

        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        IniField hardcoded = ini != null ? ini.findIniField(HARDCODED_FIELD).orElse(null) : null;

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.anchor = GridBagConstraints.CENTER;

        if (hardcoded instanceof EnumIniField) {
            buildHardcodedMode((EnumIniField) hardcoded, gbc);
        } else {
            buildFlowMode(gbc);
        }

        body.revalidate();
        body.repaint();
    }

    private void buildHardcodedMode(EnumIniField hardcoded, GridBagConstraints gbc) {
        JLabel prompt = new JLabel("Select your injector:");
        scale(prompt, 1.5f);
        gbc.gridy = 0;
        body.add(prompt, gbc);

        presetCombo = new JComboBox<>();
        for (String v : hardcoded.getEnums().values()) {
            if (v == null || v.isEmpty()) continue;
            if (INVALID_SENTINEL.equalsIgnoreCase(v)) continue;
            presetCombo.addItem(v);
        }
        presetCombo.setSelectedIndex(-1);
        scale(presetCombo, 1.5f);
        gbc.gridy = 1;
        body.add(presetCombo, gbc);

        // Preselect the current stored value if it's real (not INVALID/blank)
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg != null) {
            String current = ConfigurationImageGetterSetter.getStringValue(hardcoded, cfg.image);
            if (current != null) {
                String unquoted = stripQuotes(current).trim();
                if (!unquoted.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(unquoted)) {
                    presetCombo.setSelectedItem(unquoted);
                }
            }
        }
    }

    private void buildFlowMode(GridBagConstraints gbc) {
        JLabel prompt = new JLabel("Injector flow (cc/min):");
        scale(prompt, 1.5f);
        gbc.gridy = 0;
        body.add(prompt, gbc);

        flowTextField = new JTextField(10);
        scale(flowTextField, 1.5f);
        gbc.gridy = 1;
        body.add(flowTextField, gbc);

        // Prefill with the current value if present
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg != null) {
            IniField flowField = cfg.ini.findIniField(FLOW_FIELD).orElse(null);
            if (flowField != null) {
                String value = ConfigurationImageGetterSetter.getStringValue(flowField, cfg.image);
                if (value != null) flowTextField.setText(value);
            }
        }
    }

    private void onContinue() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return;

        ConfigurationImage modified = cfg.image.clone();

        if (presetCombo != null) {
            Object selected = presetCombo.getSelectedItem();
            if (selected == null) return; // user hasn't picked; don't advance
            cfg.ini.findIniField(HARDCODED_FIELD).ifPresent(
                hardcoded -> ConfigurationImageGetterSetter.setValue2(hardcoded, modified, HARDCODED_FIELD, selected.toString())
            );

            // TODO: invoke cmd_preset_injector_preset
            log.info("TODO: invoke cmd_preset_injector_preset for injector " + selected);
        } else if (flowTextField != null) {
            String flowText = flowTextField.getText().trim();
            if (flowText.isEmpty()) return;
            IniField flowField = cfg.ini.findIniField(FLOW_FIELD).orElse(null);
            if (flowField != null) {
                ConfigurationImageGetterSetter.setValue2(flowField, modified, FLOW_FIELD, flowText);
            } else {
                log.warn("InjectorFlowPanel: " + FLOW_FIELD + " missing from INI");
            }
            cfg.ini.findIniField(UNITS_FIELD).ifPresent(
                unitsField -> ConfigurationImageGetterSetter.setValue2(unitsField, modified, UNITS_FIELD, UNITS_CC_MIN)
            );
        }

        fireCompleted(new WizardStepResult(modified));
    }

    @Override
    public JComponent getPanel() {
        return content;
    }

    @Override
    public void onShow() {
        rebuildBody();
    }
}
