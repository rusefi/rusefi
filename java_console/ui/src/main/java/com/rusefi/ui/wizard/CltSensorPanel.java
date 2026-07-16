package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.ScrollablePanel;

import javax.swing.*;
import java.awt.*;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

public class CltSensorPanel extends AbstractWizardStep {
    static final int CARD_WIDTH = 760;
    static final String ADC_FIELD = "clt_adcChannel";
    static final String PULLUP_FIELD = "clt_bias_resistor";
    static final String LINEAR_FIELD = "useLinearCltSensor";
    static final String DIALOG_KEY = "clt_thermistor";
    static final String RAW_CLT_CHANNEL = "rawClt";
    static final String CLT_CHANNEL = "coolantTemperature";
    private static final String[] TEMP_FIELDS = {"clt_tempC_1", "clt_tempC_2", "clt_tempC_3"};
    private static final String[] RESISTANCE_FIELDS = {"clt_resistance_1", "clt_resistance_2", "clt_resistance_3"};

    private final UIContext uiContext;
    private final ISensorCentral sensorCentral;
    private final JPanel content = new JPanel(new BorderLayout());
    private final JPanel card = new JPanel(new GridBagLayout()) {
        @Override
        public Dimension getPreferredSize() {
            Dimension preferred = super.getPreferredSize();
            preferred.width = CARD_WIDTH;
            return preferred;
        }

        @Override
        public Dimension getMaximumSize() {
            Dimension preferred = super.getPreferredSize();
            return new Dimension(CARD_WIDTH, preferred.height);
        }
    };
    private final JPanel calibrationCards = new JPanel(new CardLayout());
    private final JComboBox<String> adcCombo = new JComboBox<>();
    private final JComboBox<DialogModel.SettingOption> presetCombo = new JComboBox<>();
    private final JTextField[] temperatureFields = new JTextField[3];
    private final JTextField[] resistanceFields = new JTextField[3];
    private final JTextField pullupField = new JTextField(10);
    private final JComboBox<String> linearCombo = new JComboBox<>();
    private final JToggleButton presetMode = new JToggleButton("Preset");
    private final JToggleButton customMode = new JToggleButton("Custom");
    private final JLabel voltageValue = new JLabel("-- V");
    private final JLabel cltValue = new JLabel("--");
    private final JLabel errorLabel = new JLabel(" ");
    private final JButton nextButton = new JButton("Next");
    private final JPanel optionalPanel = new JPanel(new GridBagLayout());

    private ConfigurationImage workingImage;
    private DialogModel.SettingSelector selector;
    private SensorCentral.ListenerToken voltageListener;
    private SensorCentral.ListenerToken cltListener;
    private boolean loading;
    private boolean pullupVisible;
    private boolean linearVisible;

    public CltSensorPanel(UIContext uiContext) {
        this(uiContext, SensorCentral.getInstance());
    }

    CltSensorPanel(UIContext uiContext, ISensorCentral sensorCentral) {
        super("Coolant Temperature Sensor (CLT)", "wizardCltSensor");
        this.uiContext = uiContext;
        this.sensorCentral = sensorCentral;
        buildUi();
        loadValues();
    }

    private void buildUi() {
        card.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(uiColor("Separator.foreground", Color.GRAY)),
            BorderFactory.createEmptyBorder(22, 26, 22, 26)));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.insets = new Insets(0, 0, 5, 0);

        JLabel question = new JLabel("CLT SENSOR SETUP");
        question.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        card.add(question, gbc);

        gbc.gridy++;
        JLabel title = new JLabel(getTitle());
        scale(title, 1.5f);
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        card.add(title, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 18, 0);
        card.add(new JLabel("Choose the analog input and either a known sensor preset or three calibration points."), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(sectionLabel("ANALOG INPUT (PIN)"), gbc);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 14, 0);
        card.add(adcCombo, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(sectionLabel("CALIBRATION"), gbc);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 12, 0);
        card.add(buildModeButtons(), gbc);

        calibrationCards.add(buildPresetPanel(), "preset");
        calibrationCards.add(buildCustomPanel(), "custom");
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 12, 0);
        card.add(calibrationCards, gbc);

        gbc.gridy++;
        card.add(optionalPanel, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 14, 0);
        card.add(buildReadouts(), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 8, 0);
        errorLabel.setForeground(Color.RED);
        card.add(errorLabel, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 0, 0);
        card.add(buildActions(), gbc);

        JPanel wrapper = new ScrollablePanel();
        wrapper.setLayout(new BoxLayout(wrapper, BoxLayout.Y_AXIS));
        wrapper.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        card.setAlignmentX(Component.CENTER_ALIGNMENT);
        wrapper.add(card);
        content.add(new JScrollPane(wrapper), BorderLayout.CENTER);

        ButtonGroup modes = new ButtonGroup();
        modes.add(presetMode);
        modes.add(customMode);
        presetMode.addActionListener(e -> showMode(false));
        customMode.addActionListener(e -> showMode(true));
        presetCombo.setRenderer(new DefaultListCellRenderer() {
            @Override
            public Component getListCellRendererComponent(JList<?> list, Object value, int index,
                                                           boolean isSelected, boolean cellHasFocus) {
                Object label = value instanceof DialogModel.SettingOption
                    ? ((DialogModel.SettingOption) value).getLabel() : value;
                return super.getListCellRendererComponent(list, label, index, isSelected, cellHasFocus);
            }
        });
        presetCombo.addActionListener(e -> {
            if (!loading && presetMode.isSelected()) {
                applySelectedPreset();
            }
        });
        adcCombo.addActionListener(e -> optionalPanel.setVisible(!isNoInputSelected()));
        nextButton.addActionListener(e -> saveAndContinue());
    }

    private JLabel sectionLabel(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        return label;
    }

    private JPanel buildModeButtons() {
        JPanel panel = new JPanel(new GridLayout(1, 2, 8, 0));
        panel.add(presetMode);
        panel.add(customMode);
        return panel;
    }

    private JPanel buildPresetPanel() {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.insets = new Insets(0, 0, 5, 0);
        panel.add(sectionLabel("SENSOR"), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 0, 0);
        panel.add(presetCombo, gbc);

        gbc.gridy++;
        gbc.weighty = 1;
        gbc.fill = GridBagConstraints.BOTH;
        panel.add(Box.createGlue(), gbc);
        return panel;
    }

    private JPanel buildCustomPanel() {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1;
        gbc.insets = new Insets(3, 0, 3, 10);
        for (int i = 0; i < 3; i++) {
            gbc.gridy = i * 2;
            gbc.gridx = 0;
            panel.add(sectionLabel("TEMPERATURE " + (i + 1)), gbc);
            gbc.gridx = 1;
            panel.add(sectionLabel("RESISTANCE " + (i + 1)), gbc);

            temperatureFields[i] = new JTextField(10);
            resistanceFields[i] = new JTextField(10);
            gbc.gridy++;
            gbc.gridx = 0;
            panel.add(temperatureFields[i], gbc);
            gbc.gridx = 1;
            panel.add(resistanceFields[i], gbc);
        }
        return panel;
    }

    private void rebuildOptionalPanel(IniFileModel ini, DialogModel dialog) {
        optionalPanel.removeAll();
        linearCombo.removeAllItems();
        pullupVisible = dialogHasField(dialog, PULLUP_FIELD);
        linearVisible = dialogHasField(dialog, LINEAR_FIELD);
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1;
        gbc.insets = new Insets(0, 0, 4, 10);
        if (pullupVisible) {
            gbc.gridx = 0;
            optionalPanel.add(sectionLabel("PULLUP RESISTOR"), gbc);
            gbc.gridy = 1;
            optionalPanel.add(pullupField, gbc);
        }
        IniField linear = ini.findIniField(LINEAR_FIELD).orElse(null);
        if (linearVisible && linear instanceof EnumIniField) {
            for (String value : ((EnumIniField) linear).getEnums().values()) {
                if (value != null && !value.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(value)) {
                    linearCombo.addItem(value);
                }
            }
            gbc.gridx = 1;
            gbc.gridy = 0;
            optionalPanel.add(sectionLabel("LINEAR CHARACTERISTIC"), gbc);
            gbc.gridy = 1;
            optionalPanel.add(linearCombo, gbc);
        }
    }

    private JPanel buildReadouts() {
        JPanel panel = new JPanel(new GridLayout(1, 2, 0, 0));
        panel.add(readout("VOLTAGE", voltageValue));
        panel.add(readout("CLT", cltValue));
        return panel;
    }

    private JPanel readout(String title, JLabel value) {
        JPanel panel = new JPanel(new BorderLayout(0, 4));
        panel.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createLineBorder(uiColor("Separator.foreground", Color.GRAY)),
            BorderFactory.createEmptyBorder(10, 12, 10, 12)));
        panel.add(sectionLabel(title), BorderLayout.NORTH);
        value.setFont(value.getFont().deriveFont(Font.BOLD, value.getFont().getSize2D() * 1.3f));
        panel.add(value, BorderLayout.CENTER);
        return panel;
    }

    private JPanel buildActions() {
        JPanel panel = new JPanel(new BorderLayout());
        nextButton.setFont(nextButton.getFont().deriveFont(Font.BOLD));
        panel.add(nextButton, BorderLayout.EAST);
        return panel;
    }

    private void loadValues() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            setUnsupported("Configuration is not available.");
            return;
        }
        DialogModel dialog = cfg.ini.getDialogs().get(DIALOG_KEY);
        selector = dialog == null || dialog.getSettingSelectors().isEmpty()
            ? null : dialog.getSettingSelectors().get(0);
        IniField adc = cfg.ini.findIniField(ADC_FIELD).orElse(null);
        if (!(adc instanceof EnumIniField) || selector == null || !hasCalibrationFields(cfg.ini)) {
            setUnsupported("CLT calibration is not supported by this firmware.");
            return;
        }

        loading = true;
        adcCombo.setEnabled(true);
        presetMode.setEnabled(true);
        customMode.setEnabled(true);
        workingImage = cfg.image.clone();
        adcCombo.removeAllItems();
        for (String value : ((EnumIniField) adc).getEnums().values()) {
            if (value != null && !value.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(value)) {
                adcCombo.addItem(value);
            }
        }
        String selectedAdc = readValue(adc, cfg.image);
        if (selectedAdc == null) {
            adcCombo.setSelectedIndex(-1);
        } else {
            adcCombo.setSelectedItem(selectedAdc);
        }

        presetCombo.removeAllItems();
        for (DialogModel.SettingOption option : selector.getOptions()) {
            presetCombo.addItem(option);
        }
        rebuildOptionalPanel(cfg.ini, dialog);
        loadCustomValues(cfg.ini, cfg.image);

        DialogModel.SettingOption matching = findMatchingPreset(cfg.ini, cfg.image, selector);
        if (matching != null) {
            presetCombo.setSelectedItem(matching);
            presetMode.setSelected(true);
            showMode(false);
        } else {
            customMode.setSelected(true);
            showMode(true);
        }
        optionalPanel.setVisible(!isNoInputSelected());
        errorLabel.setText(selectedAdc == null
            ? "The stored CLT input is not available on this board. Select a new input."
            : " ");
        nextButton.setEnabled(true);
        loading = false;
    }

    private void loadCustomValues(IniFileModel ini, ConfigurationImage image) {
        for (int i = 0; i < 3; i++) {
            temperatureFields[i].setText(read(ini.findIniField(TEMP_FIELDS[i]).orElse(null), image));
            resistanceFields[i].setText(read(ini.findIniField(RESISTANCE_FIELDS[i]).orElse(null), image));
        }
        IniField pullup = ini.findIniField(PULLUP_FIELD).orElse(null);
        if (pullup != null) {
            pullupField.setText(read(pullup, image));
        }
        IniField linear = ini.findIniField(LINEAR_FIELD).orElse(null);
        if (linear != null) {
            linearCombo.setSelectedItem(read(linear, image));
        }
    }

    private void setUnsupported(String message) {
        errorLabel.setText(message);
        nextButton.setEnabled(false);
        adcCombo.setEnabled(false);
        presetMode.setEnabled(false);
        customMode.setEnabled(false);
    }

    private static Color uiColor(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color == null ? fallback : color;
    }

    private void showMode(boolean custom) {
        ((CardLayout) calibrationCards.getLayout()).show(calibrationCards, custom ? "custom" : "preset");
    }

    private void applySelectedPreset() {
        DialogModel.SettingOption option = (DialogModel.SettingOption) presetCombo.getSelectedItem();
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (option == null || cfg == null || workingImage == null) {
            return;
        }
        applyAssignments(cfg.ini, workingImage, option.getAssignments());
        loadCustomValues(cfg.ini, workingImage);
    }

    private void saveAndContinue() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null || workingImage == null) {
            return;
        }
        String adc = (String) adcCombo.getSelectedItem();
        Map<String, String> values = presetMode.isSelected()
            ? selectedPresetAssignments() : customAssignments();
        String error = validate(adc, values, pullupValue());
        if (error != null) {
            errorLabel.setText(error);
            return;
        }

        ConfigurationImage modified = cfg.image.clone();
        setValue(cfg.ini, modified, ADC_FIELD, adc);
        applyAssignments(cfg.ini, modified, values);
        if (pullupVisible) {
            setValue(cfg.ini, modified, PULLUP_FIELD, pullupField.getText().trim());
        }
        if (linearVisible && customMode.isSelected()) {
            setValue(cfg.ini, modified, LINEAR_FIELD, (String) linearCombo.getSelectedItem());
        }
        fireCompleted(new WizardStepResult(modified));
    }

    private Map<String, String> selectedPresetAssignments() {
        DialogModel.SettingOption option = (DialogModel.SettingOption) presetCombo.getSelectedItem();
        return option == null ? new LinkedHashMap<>() : option.getAssignments();
    }

    private Map<String, String> customAssignments() {
        Map<String, String> values = new LinkedHashMap<>();
        for (int i = 0; i < 3; i++) {
            values.put(TEMP_FIELDS[i], temperatureFields[i].getText().trim());
            values.put(RESISTANCE_FIELDS[i], resistanceFields[i].getText().trim());
        }
        return values;
    }

    private String pullupValue() {
        return pullupVisible ? pullupField.getText().trim() : null;
    }

    static String validate(String adc, Map<String, String> values, String pullup) {
        if (adc == null || adc.trim().isEmpty() || "NONE".equalsIgnoreCase(adc.trim())) {
            return "Select a CLT analog input.";
        }
        double previous = Double.NEGATIVE_INFINITY;
        Set<Double> resistances = new HashSet<>();
        double[] temperatures = new double[3];
        double[] resistanceValues = new double[3];
        for (int i = 0; i < 3; i++) {
            double temperature = finite(values.get(TEMP_FIELDS[i]));
            double resistance = finite(values.get(RESISTANCE_FIELDS[i]));
            if (!Double.isFinite(temperature) || !Double.isFinite(resistance)) {
                return "Enter valid temperature and resistance values.";
            }
            if (temperature <= previous) {
                return "Temperatures must increase from point 1 to point 3.";
            }
            if (temperature < -55 || temperature > 200) {
                return "Temperatures must be between -55 and 200 °C.";
            }
            if (resistance <= 0 || resistance > 1_000_000) {
                return "Resistance values must be between 0 and 1,000,000 ohms.";
            }
            if (!resistances.add(resistance)) {
                return "Resistance values must be different at each point.";
            }
            temperatures[i] = temperature;
            resistanceValues[i] = resistance;
            previous = temperature;
        }
        if (!hasValidThermistorCurve(temperatures, resistanceValues)) {
            return "Calibration points do not produce a valid thermistor curve.";
        }
        if (pullup != null) {
            double pullupResistance = finite(pullup);
            if (!Double.isFinite(pullupResistance) || pullupResistance <= 0 || pullupResistance > 200_000) {
                return "Pullup resistance must be between 0 and 200,000 ohms.";
            }
        }
        return null;
    }

    private static boolean hasValidThermistorCurve(double[] temperatures, double[] resistances) {
        double l1 = Math.log(resistances[0]);
        double l2 = Math.log(resistances[1]);
        double l3 = Math.log(resistances[2]);
        double y1 = 1 / (temperatures[0] + 273.15);
        double y2 = 1 / (temperatures[1] + 273.15);
        double y3 = 1 / (temperatures[2] + 273.15);
        double u2 = (y2 - y1) / (l2 - l1);
        double u3 = (y3 - y1) / (l3 - l1);
        double c = ((u3 - u2) / (l3 - l2)) / (l1 + l2 + l3);
        double b = u2 - c * (l1 * l1 + l1 * l2 + l2 * l2);
        double a = y1 - (b + l1 * l1 * c) * l1;
        double at10Percent = thermistorTemperature(
            resistances[0] + 0.1 * (resistances[1] - resistances[0]), a, b, c);
        double at90Percent = thermistorTemperature(
            resistances[1] + 0.9 * (resistances[2] - resistances[1]), a, b, c);
        return Double.isFinite(at10Percent) && Double.isFinite(at90Percent)
            && at10Percent >= temperatures[0] && at90Percent <= temperatures[2];
    }

    private static double thermistorTemperature(double resistance, double a, double b, double c) {
        double logResistance = Math.log(resistance);
        return 1 / (a + b * logResistance + c * logResistance * logResistance * logResistance) - 273.15;
    }

    private static double finite(String value) {
        try {
            double parsed = Double.parseDouble(value);
            return Double.isFinite(parsed) ? parsed : Double.NaN;
        } catch (RuntimeException e) {
            return Double.NaN;
        }
    }

    static void applyAssignments(IniFileModel ini, ConfigurationImage image, Map<String, String> assignments) {
        for (Map.Entry<String, String> assignment : assignments.entrySet()) {
            setValue(ini, image, assignment.getKey(), assignment.getValue());
        }
    }

    private static void setValue(IniFileModel ini, ConfigurationImage image, String name, String value) {
        IniField field = ini.findIniField(name).orElse(null);
        if (field == null) {
            throw new IllegalArgumentException("Missing INI field " + name);
        }
        if (field instanceof EnumIniField && value.matches("-?\\d+")) {
            image.setBitValue((EnumIniField) field, Integer.parseInt(value));
        } else {
            ConfigurationImageGetterSetter.setValue2(field, image, name, value);
        }
    }

    private static DialogModel.SettingOption findMatchingPreset(IniFileModel ini, ConfigurationImage image,
                                                                 DialogModel.SettingSelector selector) {
        for (DialogModel.SettingOption option : selector.getOptions()) {
            boolean matches = true;
            for (Map.Entry<String, String> assignment : option.getAssignments().entrySet()) {
                IniField field = ini.findIniField(assignment.getKey()).orElse(null);
                if (field == null || !sameValue(field, image, assignment.getValue())) {
                    matches = false;
                    break;
                }
            }
            if (matches) {
                return option;
            }
        }
        return null;
    }

    private static boolean sameValue(IniField field, ConfigurationImage image, String expected) {
        if (field instanceof EnumIniField && expected.matches("-?\\d+")) {
            EnumIniField enumField = (EnumIniField) field;
            int raw = (int) enumField.getType().readRawValue(image.getByteBuffer(enumField));
            int ordinal = ConfigurationImage.getBitRange(raw, enumField.getBitPosition(), enumField.getBitSize0() + 1);
            return ordinal == Integer.parseInt(expected);
        }
        String actual = read(field, image);
        double expectedNumber = finite(expected);
        double actualNumber = finite(actual);
        return Double.isFinite(expectedNumber) && Double.isFinite(actualNumber)
            ? Double.compare(expectedNumber, actualNumber) == 0 : expected.equals(actual);
    }

    private static String read(IniField field, ConfigurationImage image) {
        if (field == null) return "";
        String value = readValue(field, image);
        return value == null ? "" : value;
    }

    private static boolean hasCalibrationFields(IniFileModel ini) {
        for (int i = 0; i < 3; i++) {
            if (!ini.findIniField(TEMP_FIELDS[i]).isPresent()
                || !ini.findIniField(RESISTANCE_FIELDS[i]).isPresent()) {
                return false;
            }
        }
        return true;
    }

    private static boolean dialogHasField(DialogModel dialog, String fieldName) {
        for (DialogModel.Field field : dialog.getFields()) {
            if (fieldName.equals(field.getKey())) {
                return true;
            }
        }
        return false;
    }

    private boolean isNoInputSelected() {
        Object selected = adcCombo.getSelectedItem();
        return selected == null || "NONE".equalsIgnoreCase(selected.toString());
    }

    private void updateReadout(JLabel label, double value, int precision, String units) {
        SwingUtilities.invokeLater(() -> label.setText(Double.isFinite(value)
            ? String.format("%." + precision + "f %s", value, units) : "-- " + units));
    }

    private void startLiveUpdates() {
        stopLiveUpdates();
        voltageListener = sensorCentral.addListener(RAW_CLT_CHANNEL,
            value -> updateReadout(voltageValue, value, 3, "V"));
        cltListener = sensorCentral.addListener(CLT_CHANNEL,
            value -> updateReadout(cltValue, value, 1, temperatureUnits()));
        updateReadout(voltageValue, sensorCentral.getValue(RAW_CLT_CHANNEL), 3, "V");
        updateReadout(cltValue, sensorCentral.getValue(CLT_CHANNEL), 1, temperatureUnits());
    }

    private String temperatureUnits() {
        if (sensorCentral instanceof SensorCentral) {
            ISensorCentral.ResolvedGaugeLabels labels = ((SensorCentral) sensorCentral).getResolvedLabels("CLTGauge");
            if (labels != null && labels.getUnits() != null) {
                return labels.getUnits();
            }
        }
        return "°C";
    }

    private void stopLiveUpdates() {
        if (voltageListener != null) {
            voltageListener.remove();
            voltageListener = null;
        }
        if (cltListener != null) {
            cltListener.remove();
            cltListener = null;
        }
    }

    @Override
    public void onShow() {
        loadValues();
        startLiveUpdates();
    }

    @Override
    public void onHide() {
        stopLiveUpdates();
    }

    @Override
    public JComponent getPanel() {
        return content;
    }

    boolean isCustomModeVisibleForTests() {
        return customMode.isSelected();
    }

    void selectCustomModeForTests() {
        customMode.doClick();
    }

    boolean areLiveListenersActiveForTests() {
        return voltageListener != null && cltListener != null;
    }

    JComponent getCardForTests() {
        return card;
    }
}
