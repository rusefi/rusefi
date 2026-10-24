package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;

import javax.swing.*;
import java.awt.*;
import java.awt.event.HierarchyEvent;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Wizard step that edits one or more INI fields generically.
 * Supports string/scalar fields (JTextField) and enum fields (JComboBox).
 * Arrays are not supported.
 * <p>
 * Field-specific behavior (prefill, validation, input filters) lives in
 * {@link WizardFieldPolicy} implementations — this class stays field-agnostic.
 */
public class GenericFieldsPanel extends AbstractWizardStep {

    private final UIContext uiContext;
    private final String description;
    private final List<String> fieldNames;

    private final JPanel content = new JPanel(new BorderLayout());
    private final Map<String, JComponent> editors = new LinkedHashMap<>();
    private final Map<String, JLabel> errorLabels = new LinkedHashMap<>();
    private JButton saveButton;
    private final JLabel connectionStatusLabel = new JLabel(" ");

    private final ConnectionStatusLogic.Listener connectionStatusListener = isConnected ->
        SwingUtilities.invokeLater(() -> onConnectionStatusChanged(isConnected));

    public GenericFieldsPanel(UIContext uiContext, String title, String description,
                              List<String> fieldNames, String wizardFlagFieldName) {
        super(title, wizardFlagFieldName);
        this.uiContext = uiContext;
        this.description = description;
        this.fieldNames = fieldNames;

        buildUi();
        loadValues();
    }

    private void buildUi() {
        JPanel card = new JPanel(new BorderLayout(0, WizardStyle.GAP));
        JLabel titleLabel = WizardStyle.cardTitle(getTitle());
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
        card.add(titleLabel, BorderLayout.NORTH);

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
            WizardFieldPolicy.forField(fieldName).decorate(editor, this::updateValidity);
            scale(editor, 1.3f);
            editors.put(fieldName, editor);
            form.add(editor, gbc);
            row++;

            gbc.gridx = 1;
            gbc.gridy = row++;
            JLabel errorLabel = new JLabel(" ");
            WizardStyle.styleError(errorLabel);
            errorLabels.put(fieldName, errorLabel);
            form.add(errorLabel, gbc);
        }

        card.add(form, BorderLayout.CENTER);

        saveButton = new JButton("Save and Continue");
        styleContinueAction(saveButton);
        saveButton.addActionListener(e -> onSave());

        connectionStatusLabel.setHorizontalAlignment(SwingConstants.CENTER);
        WizardStyle.styleError(connectionStatusLabel);

        card.add(connectionStatusLabel, BorderLayout.SOUTH);
        WizardStyle.styleCard(card, 24, 28);
        content.add(WizardStyle.cardPage(card), BorderLayout.CENTER);

        content.addHierarchyListener(e -> {
            if ((e.getChangeFlags() & HierarchyEvent.SHOWING_CHANGED) == 0) {
                return;
            }
            if (content.isShowing()) {
                ConnectionStatusLogic.INSTANCE.addAndFireListener(connectionStatusListener);
            } else {
                ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
            }
        });
    }

    private static JComponent buildEditorFor(IniField field) {
        if (field instanceof EnumIniField) {
            JComboBox<String> combo = new JComboBox<>();
            combo.setEditable(false);
            for (String v : ((EnumIniField) field).getDisplayValues()) {
                if (v == null || v.isEmpty()) continue;
                if (INVALID_SENTINEL.equalsIgnoreCase(v)) continue;
                combo.addItem(v);
            }
            combo.setSelectedIndex(-1); // blank until loadValues() or user picks
            return combo;
        }
        return new JTextField(20);
    }

    private void onConnectionStatusChanged(boolean isConnected) {
        if (!isConnected) {
            connectionStatusLabel.setText("ECU disconnected — reconnect to save");
            if (saveButton != null) {
                saveButton.setEnabled(false);
            }
        } else {
            connectionStatusLabel.setText(" ");
            updateValidity();
        }
    }

    private void updateValidity() {
        boolean ok = ConnectionStatusLogic.INSTANCE.isConnected();
        for (Map.Entry<String, JComponent> entry : editors.entrySet()) {
            String name = entry.getKey();
            String error = WizardFieldPolicy.forField(name).errorMessage(entry.getValue());
            JLabel errorLabel = errorLabels.get(name);
            if (errorLabel != null) errorLabel.setText(error == null ? " " : error);
            if (error != null) ok = false;
        }
        if (saveButton != null) saveButton.setEnabled(ok);
    }

    private void loadValues() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return;

        for (Map.Entry<String, JComponent> entry : editors.entrySet()) {
            IniField field = cfg.ini.findIniField(entry.getKey()).orElse(null);
            if (field == null) continue;
            String unquoted = readValue(field, cfg.image);
            JComponent editor = entry.getValue();

            if (editor instanceof JComboBox) {
                @SuppressWarnings("unchecked")
                JComboBox<String> combo = (JComboBox<String>) editor;
                // INVALID is a sentinel used by external tooling for "no valid value" — render blank.
                if (unquoted == null || INVALID_SENTINEL.equalsIgnoreCase(unquoted) || unquoted.isEmpty()) {
                    combo.setSelectedIndex(-1);
                } else {
                    combo.setSelectedItem(unquoted);
                }
            } else if (editor instanceof JTextField) {
                String text = WizardFieldPolicy.forField(entry.getKey()).transformLoadedValue(unquoted);
                ((JTextField) editor).setText(text == null ? "" : text);
            }
        }
        updateValidity();
    }

    private void onSave() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            onConnectionStatusChanged(false);
            return;
        }

        ConfigurationImage modified = cfg.image.clone();
        for (Map.Entry<String, JComponent> entry : editors.entrySet()) {
            String name = entry.getKey();
            IniField field = cfg.ini.findIniField(name).orElse(null);
            if (field == null) continue;
            JComponent editor = entry.getValue();

            if (editor instanceof JComboBox) {
                Object selected = ((JComboBox<?>) editor).getSelectedItem();
                if (selected == null) continue; // leave INVALID/blank untouched
                ConfigurationImageGetterSetter.setValue2(field, modified, name, selected.toString());
            } else if (editor instanceof JTextField) {
                String text = WizardFieldPolicy.forField(name).transformSavedValue(((JTextField) editor).getText());
                ConfigurationImageGetterSetter.setValue2(field, modified, name, text);
            }
        }
        fireCompleted(new WizardStepResult(modified));
    }

    /**
     * Returns true when any named field reads back as blank, empty, or the INVALID sentinel.
     * Useful for "auto-launch if needs attention" gating.
     */
    public static boolean anyFieldEmpty(UIContext uiContext, List<String> fieldNames) {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) return false;
        return anyFieldEmpty(cfg.ini, cfg.image, fieldNames);
    }

    /** Pure-data variant for testing / reuse outside a UIContext. */
    static boolean anyFieldEmpty(IniFileModel ini, ConfigurationImage image, List<String> fieldNames) {
        for (String name : fieldNames) {
            IniField field = ini.findIniField(name).orElse(null);
            if (field == null) continue;
            String value = readValue(field, image);
            if (value == null) return true;
            String stripped = value.trim();
            if (stripped.isEmpty()) return true;
            if (INVALID_SENTINEL.equalsIgnoreCase(stripped)) return true;
        }
        return false;
    }

    @Override
    public JComponent getPanel() { return content; }

    @Override
    public JButton getContinueButton() { return saveButton; }

    @Override
    public void onShow() {
        loadValues();
    }
}
