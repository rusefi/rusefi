package com.rusefi.ui.wizard;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.util.ScrollablePanel;

import javax.swing.*;
import java.awt.*;

public class MapSensorTypePanel extends AbstractWizardStep {
    static final int CARD_WIDTH = 760;
    static final String ADC_FIELD = "map_sensor_hwChannel";
    static final String TYPE_FIELD = "map_sensor_type";
    static final String LOW_VOLTAGE_FIELD = "mapLowValueVoltage";
    static final String LOW_PRESSURE_FIELD = "map_sensor_lowValue";
    static final String HIGH_VOLTAGE_FIELD = "mapHighValueVoltage";
    static final String HIGH_PRESSURE_FIELD = "map_sensor_highValue";
    static final String RAW_MAP_CHANNEL = "rawMap";
    static final String MAP_CHANNEL = "MapPressure";
    static final String MAP_VALID_CHANNEL = "isMapValid";

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
    private final JComboBox<String> adcCombo = new JComboBox<>();
    private final JComboBox<String> typeCombo = new JComboBox<>();
    private final JTextField lowVoltageField = new JTextField(8);
    private final JTextField lowPressureField = new JTextField(8);
    private final JTextField highVoltageField = new JTextField(8);
    private final JTextField highPressureField = new JTextField(8);
    private final JPanel customCalibration = new JPanel(new BorderLayout(0, 10));
    private final JLabel mapValidity = new JLabel("BAD Map Input");
    private final JLabel voltageValue = new JLabel("-- V");
    private final JLabel mapValue = new JLabel("-- kPa");
    private final JLabel errorLabel = new JLabel(" ");
    private final JButton nextButton = new JButton("Next");

    private SensorCentral.ListenerToken voltageListener;
    private SensorCentral.ListenerToken mapListener;
    private SensorCentral.ListenerToken mapValidityListener;

    public MapSensorTypePanel(UIContext uiContext) {
        this(uiContext, SensorCentral.getInstance());
    }

    MapSensorTypePanel(UIContext uiContext, ISensorCentral sensorCentral) {
        super("Manifold Absolute Pressure Sensor (MAP)", "wizardMapSensorType");
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

        JLabel question = new JLabel("MAP SENSOR SETUP");
        question.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        card.add(question, gbc);

        gbc.gridy++;
        JLabel title = new JLabel(getTitle());
        scale(title, 1.5f);
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        card.add(title, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 18, 0);
        card.add(new JLabel("Choose the analog input and the MAP sensor model installed on the engine."), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(sectionLabel("ANALOG INPUT (PIN)"), gbc);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 16, 0);
        card.add(adcCombo, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(sectionLabel("SENSOR MODEL"), gbc);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 16, 0);
        card.add(typeCombo, gbc);

        JPanel calibrationPoints = new JPanel(new GridLayout(1, 2, 28, 0));
        calibrationPoints.add(calibrationPoint("LOW POINT", lowVoltageField, lowPressureField));
        calibrationPoints.add(calibrationPoint("HIGH POINT", highVoltageField, highPressureField));
        customCalibration.add(calibrationPoints, BorderLayout.CENTER);
        mapValidity.setOpaque(true);
        mapValidity.setHorizontalAlignment(SwingConstants.CENTER);
        mapValidity.setBorder(BorderFactory.createEmptyBorder(5, 8, 5, 8));
        customCalibration.add(mapValidity, BorderLayout.SOUTH);
        updateMapValidity(Double.NaN);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 14, 0);
        card.add(customCalibration, gbc);

        gbc.gridy++;
        card.add(buildReadouts(), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 8, 0);
        errorLabel.setForeground(Color.RED);
        card.add(errorLabel, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 0, 0);
        JPanel actions = new JPanel(new BorderLayout());
        nextButton.setFont(nextButton.getFont().deriveFont(Font.BOLD));
        actions.add(nextButton, BorderLayout.EAST);
        card.add(actions, gbc);

        JPanel wrapper = new ScrollablePanel();
        wrapper.setLayout(new BoxLayout(wrapper, BoxLayout.Y_AXIS));
        wrapper.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        card.setAlignmentX(Component.CENTER_ALIGNMENT);
        wrapper.add(card);
        content.add(new JScrollPane(wrapper), BorderLayout.CENTER);

        typeCombo.addActionListener(e -> updateCustomCalibrationVisibility());
        nextButton.addActionListener(e -> saveAndContinue());
    }

    private JPanel calibrationPoint(String title, JTextField voltage, JTextField pressure) {
        JPanel panel = new JPanel(new GridLayout(2, 1, 0, 8));
        panel.add(labeledField(title + " VOLTAGE", voltage, "V"));
        panel.add(labeledField(title + " PRESSURE", pressure, "kPa"));
        return panel;
    }

    private JPanel labeledField(String title, JTextField field, String units) {
        JPanel panel = new JPanel(new BorderLayout(0, 5));
        panel.add(sectionLabel(title), BorderLayout.NORTH);
        JPanel input = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        input.add(field);
        input.add(new JLabel(units));
        panel.add(input, BorderLayout.CENTER);
        return panel;
    }

    private JPanel buildReadouts() {
        JPanel panel = new JPanel(new GridLayout(1, 2, 12, 0));
        panel.add(readout("VOLTAGE", voltageValue));
        panel.add(readout("MAP", mapValue));
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

    private JLabel sectionLabel(String text) {
        JLabel label = new JLabel(text);
        label.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        return label;
    }

    private void loadValues() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            setUnsupported("Configuration is not available.");
            return;
        }
        IniField adc = cfg.ini.findIniField(ADC_FIELD).orElse(null);
        IniField type = cfg.ini.findIniField(TYPE_FIELD).orElse(null);
        IniField lowVoltage = cfg.ini.findIniField(LOW_VOLTAGE_FIELD).orElse(null);
        IniField lowPressure = cfg.ini.findIniField(LOW_PRESSURE_FIELD).orElse(null);
        IniField highVoltage = cfg.ini.findIniField(HIGH_VOLTAGE_FIELD).orElse(null);
        IniField highPressure = cfg.ini.findIniField(HIGH_PRESSURE_FIELD).orElse(null);
        if (!(adc instanceof EnumIniField) || !(type instanceof EnumIniField)
            || lowVoltage == null || lowPressure == null || highVoltage == null || highPressure == null) {
            setUnsupported("MAP calibration is not supported by this firmware.");
            return;
        }

        loadEnum(adcCombo, (EnumIniField) adc);
        loadEnum(typeCombo, (EnumIniField) type);
        String selectedAdc = readValue(adc, cfg.image);
        String selectedType = readValue(type, cfg.image);
        select(adcCombo, selectedAdc);
        select(typeCombo, selectedType);
        lowVoltageField.setText(read(lowVoltage, cfg.image));
        lowPressureField.setText(read(lowPressure, cfg.image));
        highVoltageField.setText(read(highVoltage, cfg.image));
        highPressureField.setText(read(highPressure, cfg.image));

        setControlsEnabled(true);
        errorLabel.setText(selectedAdc == null || selectedType == null
            ? "The stored MAP configuration is not available on this board. Select new values."
            : " ");
        updateCustomCalibrationVisibility();
    }

    private static void loadEnum(JComboBox<String> combo, EnumIniField field) {
        combo.removeAllItems();
        for (String value : field.getEnums().values()) {
            if (value != null && !value.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(value)) {
                combo.addItem(value);
            }
        }
    }

    private static void select(JComboBox<String> combo, String value) {
        if (value == null) {
            combo.setSelectedIndex(-1);
        } else {
            combo.setSelectedItem(value);
        }
    }

    private void updateCustomCalibrationVisibility() {
        customCalibration.setVisible("Custom".equalsIgnoreCase((String) typeCombo.getSelectedItem()));
        card.revalidate();
    }

    private void setUnsupported(String message) {
        errorLabel.setText(message);
        setControlsEnabled(false);
    }

    private void setControlsEnabled(boolean enabled) {
        adcCombo.setEnabled(enabled);
        typeCombo.setEnabled(enabled);
        lowVoltageField.setEnabled(enabled);
        lowPressureField.setEnabled(enabled);
        highVoltageField.setEnabled(enabled);
        highPressureField.setEnabled(enabled);
        nextButton.setEnabled(enabled);
    }

    private void saveAndContinue() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            setUnsupported("Configuration is not available.");
            return;
        }
        String adc = (String) adcCombo.getSelectedItem();
        String type = (String) typeCombo.getSelectedItem();
        String error = validate(adc, type, lowVoltageField.getText(), lowPressureField.getText(),
            highVoltageField.getText(), highPressureField.getText());
        if (error != null) {
            errorLabel.setText(error);
            return;
        }

        ConfigurationImage modified = cfg.image.clone();
        setValue(cfg, modified, ADC_FIELD, adc);
        setValue(cfg, modified, TYPE_FIELD, type);
        if ("Custom".equalsIgnoreCase(type)) {
            setValue(cfg, modified, LOW_VOLTAGE_FIELD, lowVoltageField.getText().trim());
            setValue(cfg, modified, LOW_PRESSURE_FIELD, lowPressureField.getText().trim());
            setValue(cfg, modified, HIGH_VOLTAGE_FIELD, highVoltageField.getText().trim());
            setValue(cfg, modified, HIGH_PRESSURE_FIELD, highPressureField.getText().trim());
        }
        fireCompleted(new WizardStepResult(modified));
    }

    static String validate(String adc, String type, String lowVoltageText, String lowPressureText,
                           String highVoltageText, String highPressureText) {
        if (adc == null || adc.trim().isEmpty() || "NONE".equalsIgnoreCase(adc.trim())
            || "DISABLED".equalsIgnoreCase(adc.trim())) {
            return "Select a MAP analog input.";
        }
        if (type == null || type.trim().isEmpty()) {
            return "Select a MAP sensor model.";
        }
        if (!"Custom".equalsIgnoreCase(type)) {
            return null;
        }

        double lowVoltage = finite(lowVoltageText);
        double lowPressure = finite(lowPressureText);
        double highVoltage = finite(highVoltageText);
        double highPressure = finite(highPressureText);
        if (!Double.isFinite(lowVoltage) || !Double.isFinite(lowPressure)
            || !Double.isFinite(highVoltage) || !Double.isFinite(highPressure)) {
            return "Enter valid voltage and pressure values.";
        }
        if (lowVoltage < 0 || lowVoltage > 5 || highVoltage < 0 || highVoltage > 5) {
            return "MAP voltages must be between 0 and 5 V.";
        }
        if (Double.compare(lowVoltage, highVoltage) == 0 || Double.compare(lowPressure, highPressure) == 0) {
            return "Low and high calibration points must be different.";
        }
        return null;
    }

    private static double finite(String value) {
        try {
            double parsed = Double.parseDouble(value.trim());
            return Double.isFinite(parsed) ? parsed : Double.NaN;
        } catch (RuntimeException e) {
            return Double.NaN;
        }
    }

    private static void setValue(WizardConfig cfg, ConfigurationImage image, String name, String value) {
        IniField field = cfg.ini.findIniField(name).orElseThrow(
            () -> new IllegalArgumentException("Missing INI field " + name));
        ConfigurationImageGetterSetter.setValue2(field, image, name, value);
    }

    private static String read(IniField field, ConfigurationImage image) {
        String value = readValue(field, image);
        return value == null ? "" : value;
    }

    private static Color uiColor(String key, Color fallback) {
        Color color = UIManager.getColor(key);
        return color == null ? fallback : color;
    }

    private void updateReadout(JLabel label, double value, int precision, String units) {
        SwingUtilities.invokeLater(() -> label.setText(Double.isFinite(value)
            ? String.format("%." + precision + "f %s", value, units) : "-- " + units));
    }

    private void startLiveUpdates() {
        stopLiveUpdates();
        voltageListener = sensorCentral.addListener(RAW_MAP_CHANNEL,
            value -> updateReadout(voltageValue, value, 3, "V"));
        mapListener = sensorCentral.addListener(MAP_CHANNEL,
            value -> updateReadout(mapValue, value, 1, pressureUnits()));
        mapValidityListener = sensorCentral.addListener(MAP_VALID_CHANNEL, this::updateMapValidity);
        updateReadout(voltageValue, sensorCentral.getValue(RAW_MAP_CHANNEL), 3, "V");
        updateReadout(mapValue, sensorCentral.getValue(MAP_CHANNEL), 1, pressureUnits());
        updateMapValidity(sensorCentral.getValue(MAP_VALID_CHANNEL));
    }

    private void updateMapValidity(double value) {
        SwingUtilities.invokeLater(() -> {
            boolean valid = Double.isFinite(value) && value != 0;
            mapValidity.setText(valid ? "Good MAP Input" : "BAD Map Input");
            mapValidity.setBackground(valid ? Color.GREEN : Color.RED);
            mapValidity.setForeground(Color.BLACK);
        });
    }

    private String pressureUnits() {
        if (sensorCentral instanceof SensorCentral) {
            ISensorCentral.ResolvedGaugeLabels labels = ((SensorCentral) sensorCentral).getResolvedLabels("MAPGauge");
            if (labels != null && labels.getUnits() != null) {
                return labels.getUnits();
            }
        }
        return "kPa";
    }

    private void stopLiveUpdates() {
        if (voltageListener != null) {
            voltageListener.remove();
            voltageListener = null;
        }
        if (mapListener != null) {
            mapListener.remove();
            mapListener = null;
        }
        if (mapValidityListener != null) {
            mapValidityListener.remove();
            mapValidityListener = null;
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

    boolean areLiveListenersActiveForTests() {
        return voltageListener != null && mapListener != null && mapValidityListener != null;
    }

    JComponent getCardForTests() {
        return card;
    }

    boolean isCustomCalibrationVisibleForTests() {
        return customCalibration.isVisible();
    }

    void selectSensorTypeForTests(String type) {
        typeCombo.setSelectedItem(type);
    }

    String getMapValidityTextForTests() {
        return mapValidity.getText();
    }
}
