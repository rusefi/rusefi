package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.ScrollablePanel;

import javax.swing.*;
import java.awt.*;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class OutputAssignmentPanel extends AbstractWizardStep {
    enum OutputType {
        IGNITION("Ignition Outputs", "IGNITION OUTPUT SETUP", "ignitionPins", "wizardIgnitionOutputs",
            VariableRegistryValues.ts_command_e_TS_IGNITION_CATEGORY,
            "Ignition tests produce hazardous voltage. Keep clear of coils and exposed conductors."),
        INJECTOR("Injector Outputs", "INJECTOR OUTPUT SETUP", "injectionPins", "wizardInjectorOutputs",
            VariableRegistryValues.ts_command_e_TS_INJECTOR_CATEGORY,
            "Injector tests can release fuel. Ensure the fuel system is safe before continuing.");

        final String title;
        final String heading;
        final String fieldPrefix;
        final String flag;
        final int commandCategory;
        final String safetyWarning;

        OutputType(String title, String heading, String fieldPrefix, String flag,
                   int commandCategory, String safetyWarning) {
            this.title = title;
            this.heading = heading;
            this.fieldPrefix = fieldPrefix;
            this.flag = flag;
            this.commandCategory = commandCategory;
            this.safetyWarning = safetyWarning;
        }
    }

    private static final int CARD_WIDTH = 760;

    private final UIContext uiContext;
    private final OutputType type;
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel rows = new JPanel(new GridBagLayout());
    private final Map<String, JComboBox<String>> editors = new LinkedHashMap<>();
    private final List<JButton> testButtons = new ArrayList<>();
    private final JButton saveButton = new JButton("Save and Continue");
    private final JLabel statusLabel = new JLabel(" ");
    private boolean busy;

    private final ConnectionStatusLogic.Listener connectionStatusListener = connected ->
        SwingUtilities.invokeLater(this::updateActions);

    public OutputAssignmentPanel(UIContext uiContext, OutputType type) {
        super(type.title, type.flag);
        this.uiContext = uiContext;
        this.type = type;
        buildUi();
        rebuildRows();
    }

    private void buildUi() {
        JPanel card = new JPanel(new BorderLayout(0, 12)) {
            @Override
            public Dimension getPreferredSize() {
                Dimension preferred = super.getPreferredSize();
                preferred.width = CARD_WIDTH;
                return preferred;
            }
        };
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(uiColor("Separator.foreground", Color.GRAY)),
            BorderFactory.createEmptyBorder(22, 26, 22, 26)));

        JPanel header = new JPanel();
        header.setLayout(new BoxLayout(header, BoxLayout.Y_AXIS));
        JLabel heading = new JLabel(type.heading);
        heading.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        heading.setAlignmentX(Component.LEFT_ALIGNMENT);
        header.add(heading);
        header.add(Box.createVerticalStrut(5));

        JLabel title = new JLabel(getTitle());
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() * 1.5f));
        title.setAlignmentX(Component.LEFT_ALIGNMENT);
        header.add(title);
        header.add(Box.createVerticalStrut(5));

        JLabel guidance = new JLabel("Assign each cylinder output. Test applies all visible assignments first.");
        guidance.setAlignmentX(Component.LEFT_ALIGNMENT);
        header.add(guidance);
        card.add(header, BorderLayout.NORTH);

        card.add(rows, BorderLayout.CENTER);

        saveButton.setFont(saveButton.getFont().deriveFont(Font.BOLD));
        saveButton.addActionListener(e -> saveAndContinue());
        statusLabel.setForeground(Color.RED);

        JPanel footer = new JPanel(new BorderLayout());
        footer.add(statusLabel, BorderLayout.CENTER);
        footer.add(saveButton, BorderLayout.EAST);
        card.add(footer, BorderLayout.SOUTH);

        JPanel wrapper = new ScrollablePanel();
        wrapper.setLayout(new BoxLayout(wrapper, BoxLayout.Y_AXIS));
        wrapper.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        card.setAlignmentX(Component.CENTER_ALIGNMENT);
        wrapper.add(card);
        content.add(new JScrollPane(wrapper), BorderLayout.CENTER);
    }

    private void rebuildRows() {
        rows.removeAll();
        editors.clear();
        testButtons.clear();

        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            statusLabel.setText("Configuration is not available.");
            updateActions();
            return;
        }

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.insets = new Insets(4, 4, 4, 8);
        gbc.anchor = GridBagConstraints.WEST;

        gbc.gridx = 0;
        rows.add(sectionLabel("CYLINDER"), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        rows.add(sectionLabel("OUTPUT PIN"), gbc);
        gbc.gridx = 2;
        gbc.weightx = 0;
        gbc.fill = GridBagConstraints.NONE;
        rows.add(sectionLabel("BENCH TEST"), gbc);

        int cylinderCount = readCylinderCount(cfg);
        boolean missingField = false;
        for (int index = 1; index <= cylinderCount; index++) {
            String fieldName = type.fieldPrefix + index;
            IniField field = cfg.ini.findIniField(fieldName).orElse(null);
            JComboBox<String> selector = new JComboBox<>();
            if (field instanceof EnumIniField) {
                populateSelector(selector, (EnumIniField) field, cfg.image);
            } else {
                selector.setEnabled(false);
                missingField = true;
            }
            selector.addActionListener(e -> updateActions());
            editors.put(fieldName, selector);

            JButton testButton = new JButton("Test " + index);
            styleButton(testButton);
            int outputIndex = index;
            testButton.addActionListener(e -> confirmAndTest(outputIndex));
            testButtons.add(testButton);

            gbc.gridy = index;
            gbc.gridx = 0;
            gbc.weightx = 0;
            gbc.fill = GridBagConstraints.NONE;
            rows.add(new JLabel(Integer.toString(index)), gbc);
            gbc.gridx = 1;
            gbc.weightx = 1;
            gbc.fill = GridBagConstraints.HORIZONTAL;
            rows.add(selector, gbc);
            gbc.gridx = 2;
            gbc.weightx = 0;
            gbc.fill = GridBagConstraints.NONE;
            rows.add(testButton, gbc);
        }

        statusLabel.setText(missingField ? "One or more output fields are unavailable." : " ");
        rows.revalidate();
        rows.repaint();
        updateActions();
    }

    private void populateSelector(JComboBox<String> selector, EnumIniField field, ConfigurationImage image) {
        for (String value : field.getEnums().values()) {
            if (value == null || value.isEmpty() || INVALID_SENTINEL.equalsIgnoreCase(value)) {
                continue;
            }
            selector.addItem(value);
        }

        String current = readValue(field, image);
        if (current == null || INVALID_SENTINEL.equalsIgnoreCase(current)) {
            selector.setSelectedIndex(-1);
        } else {
            selector.setSelectedItem(current);
            if (!current.equals(selector.getSelectedItem())) {
                selector.setSelectedIndex(-1);
            }
        }
    }

    private int readCylinderCount(WizardConfig cfg) {
        IniField field = cfg.ini.findIniField("cylindersCount").orElse(null);
        if (field == null) {
            return 1;
        }
        try {
            int value = (int) Double.parseDouble(ConfigurationImageGetterSetter.getStringValue(field, cfg.image));
            return Math.max(1, Math.min(value, VariableRegistryValues.MAX_CYLINDER_COUNT));
        } catch (RuntimeException e) {
            return 1;
        }
    }

    private ConfigurationImage buildModifiedImage() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null || !allSelected()) {
            return null;
        }

        ConfigurationImage modified = cfg.image.clone();
        for (Map.Entry<String, JComboBox<String>> entry : editors.entrySet()) {
            IniField field = cfg.ini.findIniField(entry.getKey()).orElse(null);
            Object selected = entry.getValue().getSelectedItem();
            if (!(field instanceof EnumIniField) || selected == null) {
                return null;
            }
            ConfigurationImageGetterSetter.setValue2(field, modified, entry.getKey(), selected.toString());
        }
        return modified;
    }

    private void saveAndContinue() {
        ConfigurationImage modified = buildModifiedImage();
        if (modified != null) {
            fireCompleted(new WizardStepResult(modified));
        }
    }

    private void confirmAndTest(int outputIndex) {
        int result = JOptionPane.showConfirmDialog(content,
            type.safetyWarning + "\n\nThe engine must be stopped. Test output " + outputIndex + "?",
            "Confirm Bench Test", JOptionPane.OK_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE);
        if (result != JOptionPane.OK_OPTION) {
            return;
        }

        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        ConfigurationImage modified = buildModifiedImage();
        if (cfg == null || modified == null) {
            statusLabel.setText("Select every output before testing.");
            return;
        }

        busy = true;
        statusLabel.setForeground(uiColor("Label.foreground", Color.BLACK));
        statusLabel.setText("Applying assignments and testing output " + outputIndex + "...");
        updateActions();

        BinaryProtocol bp = cfg.bp;
        uiContext.getLinkManager().submit(() -> {
            boolean completed = applyAndTest(bp, modified, type.commandCategory, outputIndex,
                type.title + " bench test " + outputIndex);
            SwingUtilities.invokeLater(() -> finishTest(outputIndex, completed));
        });
    }

    private void finishTest(int outputIndex, boolean success) {
        busy = false;
        statusLabel.setForeground(success ? new Color(0, 140, 0) : Color.RED);
        statusLabel.setText(success
            ? "Test command sent for output " + outputIndex + "."
            : "Bench test command failed.");
        updateActions();
    }

    static byte[] buildTestPayload(int category, int outputIndex) {
        if (outputIndex < 1 || outputIndex > VariableRegistryValues.MAX_CYLINDER_COUNT) {
            throw new IllegalArgumentException("Invalid output index " + outputIndex);
        }
        return ByteBuffer.allocate(4)
            .putShort((short) category)
            .putShort((short) outputIndex)
            .array();
    }

    static boolean applyAndTest(BinaryProtocol bp, ConfigurationImage image, int category,
                                int outputIndex, String description) {
        try {
            bp.uploadChanges(image);
            byte[] response = bp.executeCommand(Integration.TS_IO_TEST_COMMAND,
                buildTestPayload(category, outputIndex), description);
            return response != null && response.length > 0 &&
                response[0] == (byte) Integration.TS_RESPONSE_OK;
        } catch (RuntimeException ignored) {
            return false;
        }
    }

    private boolean allSelected() {
        return !editors.isEmpty() && editors.values().stream().allMatch(editor -> editor.getSelectedItem() != null);
    }

    private void updateActions() {
        boolean enabled = !busy && ConnectionStatusLogic.INSTANCE.isConnected() && allSelected();
        saveButton.setEnabled(enabled);
        for (JButton button : testButtons) {
            button.setEnabled(enabled);
        }
    }

    private JLabel sectionLabel(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        return label;
    }

    private static Color uiColor(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color == null ? fallback : color;
    }

    @Override
    public void onShow() {
        rebuildRows();
        ConnectionStatusLogic.INSTANCE.addAndFireListener(connectionStatusListener);
    }

    @Override
    public void onHide() {
        ConnectionStatusLogic.INSTANCE.removeListener(connectionStatusListener);
    }

    @Override
    public JComponent getPanel() {
        return content;
    }

    int getRowCountForTests() {
        return editors.size();
    }

    JComboBox<String> getEditorForTests(int oneBasedIndex) {
        return editors.get(type.fieldPrefix + oneBasedIndex);
    }

    ConfigurationImage buildModifiedImageForTests() {
        return buildModifiedImage();
    }

    boolean areActionsEnabledForTests() {
        return saveButton.isEnabled() && testButtons.stream().allMatch(AbstractButton::isEnabled);
    }
}
