package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

/**
 * Wizard step that edits one or more INI fields generically.
 * Supports string/scalar fields (JTextField) and enum fields (JComboBox).
 * Arrays are not supported.
 */
public class GenericFieldsPanel implements WizardStep {
    private static final String INVALID_SENTINEL = "INVALID";

    private final UIContext uiContext;
    private final String title;
    private final String description;
    private final List<String> fieldNames;
    private final String wizardFlagFieldName; // may be null for one-off auto-launched panels

    private final JPanel content = new JPanel(new BorderLayout());
    private final Map<String, JComponent> editors = new LinkedHashMap<>();

    private Consumer<WizardStepResult> onStepCompleted;

    public GenericFieldsPanel(UIContext uiContext, String title, String description,
                              List<String> fieldNames, String wizardFlagFieldName) {
        this.uiContext = uiContext;
        this.title = title;
        this.description = description;
        this.fieldNames = fieldNames;
        this.wizardFlagFieldName = wizardFlagFieldName;

        buildUi();
        loadValues();
    }

    private void buildUi() {
        JLabel titleLabel = new JLabel(title);
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
        titleLabel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        scale(titleLabel, 2f);
        content.add(titleLabel, BorderLayout.NORTH);

        IniFileModel ini = uiContext.iniFileState.getIniFileModel();

        JPanel form = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.anchor = GridBagConstraints.WEST;
        int row = 0;

        if (description != null && !description.isEmpty()) {
            gbc.gridx = 0;
            gbc.gridy = row++;
            gbc.gridwidth = 2;
            gbc.anchor = GridBagConstraints.CENTER;
            JLabel desc = new JLabel("<html><center>" + description + "</center></html>");
            scale(desc, 1.2f);
            form.add(desc, gbc);
            gbc.gridwidth = 1;
            gbc.anchor = GridBagConstraints.WEST;
        }

        for (String fieldName : fieldNames) {
            gbc.gridx = 0;
            gbc.gridy = row;
            JLabel label = new JLabel(fieldName + ":");
            scale(label, 1.3f);
            form.add(label, gbc);

            gbc.gridx = 1;
            IniField field = ini != null ? ini.findIniField(fieldName).orElse(null) : null;
            JComponent editor = buildEditorFor(field);
            scale(editor, 1.3f);
            editors.put(fieldName, editor);
            form.add(editor, gbc);
            row++;
        }

        JPanel centerWrapper = new JPanel(new GridBagLayout());
        centerWrapper.add(form);
        content.add(centerWrapper, BorderLayout.CENTER);

        JButton saveButton = new JButton("Save and Continue");
        scale(saveButton, 1.5f);
        saveButton.addActionListener(e -> onSave());

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        buttonPanel.add(saveButton);
        content.add(buttonPanel, BorderLayout.SOUTH);
    }

    private static JComponent buildEditorFor(IniField field) {
        if (field instanceof EnumIniField) {
            JComboBox<String> combo = new JComboBox<>();
            combo.setEditable(false);
            for (String v : ((EnumIniField) field).getEnums().values()) {
                if (v == null || v.isEmpty()) continue;
                if (INVALID_SENTINEL.equalsIgnoreCase(v)) continue;
                combo.addItem(v);
            }
            combo.setSelectedIndex(-1); // blank until loadValues() or user picks
            return combo;
        }
        return new JTextField(20);
    }

    private void loadValues() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (bp == null || ini == null) return;
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) return;

        for (Map.Entry<String, JComponent> entry : editors.entrySet()) {
            IniField field = ini.findIniField(entry.getKey()).orElse(null);
            if (field == null) continue;
            String value = ConfigurationImageGetterSetter.getStringValue(field, image);
            if (value == null) continue;
            String unquoted = stripQuotes(value);
            JComponent editor = entry.getValue();

            if (editor instanceof JComboBox) {
                @SuppressWarnings("unchecked")
                JComboBox<String> combo = (JComboBox<String>) editor;
                // INVALID is a sentinel used by external tooling for "no valid value" — render blank.
                if (INVALID_SENTINEL.equalsIgnoreCase(unquoted) || unquoted.isEmpty()) {
                    combo.setSelectedIndex(-1);
                } else {
                    combo.setSelectedItem(unquoted);
                }
            } else if (editor instanceof JTextField) {
                ((JTextField) editor).setText(unquoted);
            }
        }
    }

    private void onSave() {
        if (onStepCompleted == null) return;
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (bp == null || ini == null) return;
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) return;

        ConfigurationImage modified = image.clone();
        for (Map.Entry<String, JComponent> entry : editors.entrySet()) {
            String name = entry.getKey();
            IniField field = ini.findIniField(name).orElse(null);
            if (field == null) continue;
            JComponent editor = entry.getValue();

            if (editor instanceof JComboBox) {
                Object selected = ((JComboBox<?>) editor).getSelectedItem();
                if (selected == null) continue; // leave INVALID/blank untouched
                ConfigurationImageGetterSetter.setValue2(field, modified, name, selected.toString());
            } else if (editor instanceof JTextField) {
                ConfigurationImageGetterSetter.setValue2(field, modified, name, ((JTextField) editor).getText());
            }
        }
        onStepCompleted.accept(new WizardStepResult(modified));
    }

    /**
     * Returns true when any named field reads back as blank, empty, or the INVALID sentinel.
     * Useful for "auto-launch if needs attention" gating.
     */
    public static boolean anyFieldEmpty(UIContext uiContext, List<String> fieldNames) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (bp == null || ini == null) return false;
        ConfigurationImage image = bp.getControllerConfiguration();
        if (image == null) return false;
        return anyFieldEmpty(ini, image, fieldNames);
    }

    /** Pure-data variant for testing / reuse outside a UIContext. */
    static boolean anyFieldEmpty(IniFileModel ini, ConfigurationImage image, List<String> fieldNames) {
        for (String name : fieldNames) {
            IniField field = ini.findIniField(name).orElse(null);
            if (field == null) continue;
            String value = ConfigurationImageGetterSetter.getStringValue(field, image);
            if (value == null) return true;
            String stripped = stripQuotes(value).trim();
            if (stripped.isEmpty()) return true;
            if (INVALID_SENTINEL.equalsIgnoreCase(stripped)) return true;
        }
        return false;
    }

    private static String stripQuotes(String value) {
        if (value.length() >= 2 && value.startsWith("\"") && value.endsWith("\"")) {
            return value.substring(1, value.length() - 1);
        }
        return value;
    }

    @Override
    public String getTitle() { return title; }

    @Override
    public JComponent getPanel() { return content; }

    @Override
    public String getWizardFlagFieldName() { return wizardFlagFieldName; }

    @Override
    public void setOnStepCompleted(Consumer<WizardStepResult> callback) {
        this.onStepCompleted = callback;
    }

    @Override
    public void onShow() {
        loadValues();
    }

    private void scale(JComponent c, float factor) {
        Font f = c.getFont();
        c.setFont(f.deriveFont(f.getSize() * factor));
    }
}
