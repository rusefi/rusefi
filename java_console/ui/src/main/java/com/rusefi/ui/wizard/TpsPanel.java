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
import java.util.Locale;

public class TpsPanel extends AbstractWizardStep {
    static final int CARD_WIDTH = 760;
    static final String ADC_FIELD = "tps1_1AdcChannel";
    static final String CLOSED_FIELD = "tpsMin";
    static final String WOT_FIELD = "tpsMax";
    static final String RAW_TPS_CHANNEL = "rawTps1Primary";
    static final String TPS_CHANNEL = "TPSValue";
    private static final double MINIMUM_SPAN = 0.1;

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
    private final JTextField closedField = new JTextField(10);
    private final JTextField wotField = new JTextField(10);
    private final JButton grabClosedButton = new JButton("Grab");
    private final JButton grabWotButton = new JButton("Grab");
    private final JLabel voltageValue = new JLabel("-- V");
    private final JLabel tpsValue = new JLabel("-- %");
    private final JLabel errorLabel = new JLabel(" ");
    private final JButton nextButton = new JButton("Next");

    private SensorCentral.ListenerToken voltageListener;
    private SensorCentral.ListenerToken tpsListener;

    public TpsPanel(UIContext uiContext) {
        this(uiContext, SensorCentral.getInstance());
    }

    TpsPanel(UIContext uiContext, ISensorCentral sensorCentral) {
        super("Throttle Position Sensor (TPS)", "wizardTps");
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

        JLabel question = new JLabel("TPS SENSOR SETUP");
        question.setForeground(uiColor("Label.disabledForeground", Color.GRAY));
        card.add(question, gbc);

        gbc.gridy++;
        JLabel title = new JLabel(getTitle());
        scale(title, 1.5f);
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        card.add(title, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 18, 0);
        card.add(new JLabel("Choose the analog input and enter the closed and fully open throttle voltages."), gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 5, 0);
        card.add(sectionLabel("ANALOG INPUT (PIN)"), gbc);
        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 16, 0);
        card.add(adcCombo, gbc);

        gbc.gridy++;
        gbc.insets = new Insets(0, 0, 14, 0);
        card.add(buildEndpointFields(), gbc);

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

        grabClosedButton.addActionListener(e -> captureCurrentVoltage(closedField));
        grabWotButton.addActionListener(e -> captureCurrentVoltage(wotField));
        nextButton.addActionListener(e -> saveAndContinue());
    }

    private JPanel buildEndpointFields() {
        JPanel panel = new JPanel(new GridLayout(1, 2, 28, 0));
        panel.add(labeledField("CLOSED VOLTAGE", closedField, grabClosedButton));
        panel.add(labeledField("OPEN VOLTAGE (WOT)", wotField, grabWotButton));
        return panel;
    }

    private JPanel labeledField(String title, JTextField field, JButton grabButton) {
        JPanel panel = new JPanel(new BorderLayout(0, 5));
        panel.add(sectionLabel(title), BorderLayout.NORTH);
        JPanel input = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
        input.add(field);
        input.add(new JLabel("V"));
        input.add(grabButton);
        panel.add(input, BorderLayout.CENTER);
        return panel;
    }

    private void captureCurrentVoltage(JTextField field) {
        double voltage = sensorCentral.getValue(RAW_TPS_CHANNEL);
        if (!Double.isFinite(voltage) || voltage < 0 || voltage > 5) {
            errorLabel.setText("TPS voltage is not available.");
            return;
        }
        field.setText(String.format(Locale.ROOT, "%.3f", voltage));
        errorLabel.setText(" ");
    }

    private JPanel buildReadouts() {
        JPanel panel = new JPanel(new GridLayout(1, 2, 12, 0));
        panel.add(readout("VOLTAGE", voltageValue));
        panel.add(readout("TPS", tpsValue));
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
        IniField closed = cfg.ini.findIniField(CLOSED_FIELD).orElse(null);
        IniField wot = cfg.ini.findIniField(WOT_FIELD).orElse(null);
        if (!(adc instanceof EnumIniField) || closed == null || wot == null) {
            setUnsupported("TPS calibration is not supported by this firmware.");
            return;
        }

        adcCombo.removeAllItems();
        for (String value : ((EnumIniField) adc).getEnums().values()) {
            if (value != null && !value.isEmpty() && !INVALID_SENTINEL.equalsIgnoreCase(value)) {
                adcCombo.addItem(value);
            }
        }
        String selectedAdc = readEnum((EnumIniField) adc, cfg.image);
        if (selectedAdc == null) {
            adcCombo.setSelectedIndex(-1);
        } else {
            adcCombo.setSelectedItem(selectedAdc);
        }
        closedField.setText(read(closed, cfg.image));
        wotField.setText(read(wot, cfg.image));
        adcCombo.setEnabled(true);
        closedField.setEnabled(true);
        wotField.setEnabled(true);
        grabClosedButton.setEnabled(true);
        grabWotButton.setEnabled(true);
        nextButton.setEnabled(true);
        errorLabel.setText(selectedAdc == null
            ? "The stored TPS input is not available on this board. Select a new input."
            : " ");
    }

    private void setUnsupported(String message) {
        errorLabel.setText(message);
        adcCombo.setEnabled(false);
        closedField.setEnabled(false);
        wotField.setEnabled(false);
        grabClosedButton.setEnabled(false);
        grabWotButton.setEnabled(false);
        nextButton.setEnabled(false);
    }

    private void saveAndContinue() {
        WizardConfig cfg = WizardConfig.snapshot(uiContext);
        if (cfg == null) {
            setUnsupported("Configuration is not available.");
            return;
        }
        String adc = (String) adcCombo.getSelectedItem();
        String closed = closedField.getText().trim();
        String wot = wotField.getText().trim();
        String error = validate(adc, closed, wot);
        if (error != null) {
            errorLabel.setText(error);
            return;
        }

        ConfigurationImage modified = cfg.image.clone();
        setValue(cfg, modified, ADC_FIELD, adc);
        setValue(cfg, modified, CLOSED_FIELD, closed);
        setValue(cfg, modified, WOT_FIELD, wot);
        fireCompleted(new WizardStepResult(modified));
    }

    static String validate(String adc, String closedText, String wotText) {
        if (adc == null || adc.trim().isEmpty() || "NONE".equalsIgnoreCase(adc.trim())
            || "DISABLED".equalsIgnoreCase(adc.trim())) {
            return "Select a TPS analog input.";
        }
        double closed = finite(closedText);
        double wot = finite(wotText);
        if (!Double.isFinite(closed) || !Double.isFinite(wot)) {
            return "Enter valid Closed and WOT voltages.";
        }
        if (closed < 0 || closed > 5 || wot < 0 || wot > 5) {
            return "TPS voltages must be between 0 and 5 V.";
        }
        if (Math.abs(wot - closed) + 1e-9 < MINIMUM_SPAN) {
            return "Closed and WOT voltages must differ by at least 0.1 V.";
        }
        return null;
    }

    private static double finite(String value) {
        try {
            double parsed = Double.parseDouble(value);
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
        return readValue(field, image);
    }

    static String readEnum(EnumIniField field, ConfigurationImage image) {
        return readValue(field, image);
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
        voltageListener = sensorCentral.addListener(RAW_TPS_CHANNEL,
            value -> updateReadout(voltageValue, value, 3, "V"));
        tpsListener = sensorCentral.addListener(TPS_CHANNEL,
            value -> updateReadout(tpsValue, value, 1, "%"));
        updateReadout(voltageValue, sensorCentral.getValue(RAW_TPS_CHANNEL), 3, "V");
        updateReadout(tpsValue, sensorCentral.getValue(TPS_CHANNEL), 1, "%");
    }

    private void stopLiveUpdates() {
        if (voltageListener != null) {
            voltageListener.remove();
            voltageListener = null;
        }
        if (tpsListener != null) {
            tpsListener.remove();
            tpsListener = null;
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
        return voltageListener != null && tpsListener != null;
    }

    JComponent getCardForTests() {
        return card;
    }

    void grabClosedForTests() {
        grabClosedButton.doClick();
    }

    void grabWotForTests() {
        grabWotButton.doClick();
    }

    String getClosedVoltageForTests() {
        return closedField.getText();
    }

    String getWotVoltageForTests() {
        return wotField.getText();
    }
}
