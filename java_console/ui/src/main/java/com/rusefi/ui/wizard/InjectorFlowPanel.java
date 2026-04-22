package com.rusefi.ui.wizard;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.nio.ByteBuffer;

import static com.devexperts.logging.Logging.getLogging;

public class InjectorFlowPanel extends AbstractWizardStep {
    private static final Logging log = getLogging(InjectorFlowPanel.class);

    private static final String HARDCODED_FIELD = "hardcoded_injectors";
    private static final String FLOW_FIELD = "injector_flow";
    private static final String UNITS_FIELD = "injectorFlowAsMassFlow";
    private static final String UNITS_CC_MIN = "cc/min";

    /**
     * cmd_preset_injector_preset payload: TS_BOARD_ACTION (u16 BE) + BOARD_ACTION_INJECTOR_PRESET (u16 BE).
     * Sent via {@link BinaryProtocol#executeCommand} with opcode TS_IO_TEST_COMMAND; the ECU reads the
     * target injector from the {@code hardcoded_injectors} config field, so upload must precede this.
     * Do not mutate: shared immutable payload.
     */
    private static final byte[] CMD_PRESET_INJECTOR_PAYLOAD = ByteBuffer.allocate(4)
        .putShort((short) VariableRegistryValues.ts_command_e_TS_BOARD_ACTION)
        .putShort((short) VariableRegistryValues.BOARD_ACTION_INJECTOR_PRESET)
        .array();

    private final UIContext uiContext;
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel body = new JPanel(new GridBagLayout());

    private JList<String> presetList;
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
        presetList = null;
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

        DefaultListModel<String> model = new DefaultListModel<>();
        for (String v : hardcoded.getEnums().values()) {
            if (v == null || v.isEmpty()) continue;
            if (INVALID_SENTINEL.equalsIgnoreCase(v)) continue;
            model.addElement(v);
        }

        presetList = new JList<>(model);
        presetList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        presetList.setVisibleRowCount(Math.min(12, Math.max(5, model.size())));
        scale(presetList, 1.5f);
        presetList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2 && SwingUtilities.isLeftMouseButton(e)) {
                    int index = presetList.locationToIndex(e.getPoint());
                    if (index >= 0 && presetList.getCellBounds(index, index).contains(e.getPoint())) {
                        presetList.setSelectedIndex(index);
                        onContinue();
                    }
                }
            }
        });

        JScrollPane scroll = new JScrollPane(presetList);
        gbc.gridy = 1;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        body.add(scroll, gbc);
        gbc.fill = GridBagConstraints.NONE;
        gbc.weightx = 0;
        gbc.weighty = 0;

        // Preselect the current stored value if it's real (not INVALID/blank)
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg != null) {
            String current = ConfigurationImageGetterSetter.getStringValue(hardcoded, cfg.image);
            if (current != null) {
                String unquoted = stripQuotes(current).trim();
                if (!unquoted.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(unquoted)) {
                    presetList.setSelectedValue(unquoted, true);
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

        if (presetList != null) {
            String selected = presetList.getSelectedValue();
            if (selected == null) return; // user hasn't picked; don't advance
            cfg.ini.findIniField(HARDCODED_FIELD).ifPresent(
                hardcoded -> ConfigurationImageGetterSetter.setValue2(hardcoded, modified, HARDCODED_FIELD, selected)
            );

            // Queue the preset-apply command after fireCompleted's upload+burn so the ECU reads
            // the just-written hardcoded_injectors value. The preset handler mutates ECU config,
            // so we re-read the image afterward to avoid a later wizard save clobbering it.
            BinaryProtocol bp = cfg.bp;
            fireCompleted(new WizardStepResult(modified));
            uiContext.getLinkManager().submit(() -> sendPresetAndReload(bp, selected));
            return;
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

    private void sendPresetAndReload(BinaryProtocol bp, String injectorName) {
        byte[] response = bp.executeCommand(
            Integration.TS_IO_TEST_COMMAND,
            CMD_PRESET_INJECTOR_PAYLOAD,
            "preset injector " + injectorName);

        if (response == null) {
            log.error("cmd_preset_injector_preset failed for " + injectorName);
            return;
        }

        log.info("cmd_preset_injector_preset sent for " + injectorName + ", re-reading config");

        IniFileModel iniFile = bp.getIniFileNullable();
        if (iniFile == null) {
            return;
        }
        ConfigurationImageMetaVersion0_0 meta = ConfigurationImageMetaVersion0_0.getMeta(iniFile);
        ConfigurationImageWithMeta imageWithMeta = bp.readFullImageFromController(meta);
        if (imageWithMeta.isEmpty()) {
            log.warn("readFullImageFromController returned empty; config not refreshed");
            return;
        }
        bp.setConfigurationImage(imageWithMeta.getConfigurationImage());
        log.info("Config refreshed after preset injector");
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
