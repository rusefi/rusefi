package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.ui.widgets.tune.CustomIndicator.Tone;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.*;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * "New indicator…" / "Edit indicator…" of the indicator strip: a condition on an output channel,
 * a label and a colour for each state, and a live preview chip.
 * <p>
 * The condition is one of: the channel is on (non-zero), the channel compared with a number, or a
 * free expression over any channels, evaluated like the INI indicators. An edited indicator opens in
 * the simplest of these that reads its expression back exactly.
 */
public final class CustomIndicatorDialog {
    static final String NEW_TITLE = "New Indicator";
    static final String EDIT_TITLE = "Edit Indicator";
    static final int MAX_LABEL = CustomIndicator.MAX_LABEL;
    private static final Pattern IDENTIFIER = Pattern.compile("[A-Za-z_][A-Za-z0-9_]*");
    private static final Pattern THRESHOLD = Pattern.compile(
        "(" + IDENTIFIER.pattern() + ")\\s*(>=|<=|==|!=|>|<)\\s*(-?[0-9]+(?:\\.[0-9]+)?)");

    private enum Mode {
        IS_ON, THRESHOLD, EXPRESSION
    }

    enum Operator {
        ABOVE(">", ">"),
        AT_LEAST("≥", ">="),
        BELOW("<", "<"),
        AT_MOST("≤", "<="),
        EQUAL("=", "=="),
        NOT_EQUAL("≠", "!=");

        private final String symbol;
        final String expression;

        Operator(String symbol, String expression) {
            this.symbol = symbol;
            this.expression = expression;
        }

        @Override
        public String toString() {
            return symbol;
        }

        static Operator of(String expression) {
            for (Operator operator : values()) {
                if (operator.expression.equals(expression)) {
                    return operator;
                }
            }
            throw new IllegalArgumentException(expression);
        }
    }

    final JTextField name = new JTextField(22);
    final JTextField search = new JTextField(22);
    final DefaultListModel<String> channelModel = new DefaultListModel<>();
    final JList<String> channelList = new JList<>(channelModel);
    final JRadioButton isOn = new JRadioButton("Is on");
    final JRadioButton threshold = new JRadioButton("Compare with a value");
    final JRadioButton advanced = new JRadioButton("Expression");
    final JComboBox<Operator> operator = new JComboBox<>(Operator.values());
    final JTextField value = new JTextField(8);
    final JLabel units = new JLabel();
    final JTextField expression = new JTextField(22);
    final JTextField offLabel = new JTextField(16);
    final JTextField onLabel = new JTextField(16);
    final JComboBox<Tone> offTone = new JComboBox<>(Tone.values());
    final JComboBox<Tone> onTone = new JComboBox<>(Tone.values());
    private final JPanel thresholdRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 0));
    /** The mode's own fields: nothing, the comparison or the expression; as tall as the tallest, so switching never resizes. */
    private final JPanel conditionDetail = new JPanel(new CardLayout());
    /*
     * The option pane does not grow once shown, and a GridBagLayout short of its preferred size shrinks
     * every row to its minimum (the channel list to one cut row). So nothing here may change the content's
     * preferred size: the preview keeps the chip's height and takes the column's width.
     */
    private final JPanel previewHolder = new JPanel(new BorderLayout()) {
        @Override
        public Dimension getPreferredSize() {
            return new Dimension(0, previewHeight);
        }

        @Override
        public Dimension getMinimumSize() {
            return getPreferredSize();
        }
    };
    private int previewHeight;
    private final JPanel content = new JPanel(new GridBagLayout());

    @Nullable
    private final IniFileModel ini;
    /** Every channel name, sorted; the list shows those matching the search. */
    private final List<String> channels;
    private final Map<String, String> channelUnits = new HashMap<>();
    private final String id;
    private final boolean editing;
    private IndicatorPanel preview;
    private boolean active;
    /** Whether the last channel mode was the comparison, for Expression to start from. */
    private boolean thresholdWasChosen;

    public CustomIndicatorDialog(@Nullable IniFileModel ini, @Nullable CustomIndicator existing) {
        this.ini = ini;
        editing = existing != null;
        id = existing != null ? existing.getId() : CustomIndicator.newId();
        channels = channelsOf(ini, channelUnits);

        search.putClientProperty("JTextField.placeholderText", "Search channels");
        search.getDocument().addDocumentListener(onChange(this::filterChannels));
        channelList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        channelList.setVisibleRowCount(7);
        channelList.setCellRenderer(new DefaultListCellRenderer() {
            @Override
            public Component getListCellRendererComponent(JList<?> list, Object item, int index, boolean selected, boolean focus) {
                String unit = channelUnits.get(item);
                return super.getListCellRendererComponent(list,
                    unit == null || unit.isEmpty() ? item : item + "  (" + unit + ")", index, selected, focus);
            }
        });
        channelList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                updateUnits();
                updatePreview();
            }
        });
        // in Expression mode a double-click adds the channel where the cursor is
        channelList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2 && advanced.isSelected() && channelList.getSelectedValue() != null) {
                    expression.replaceSelection(channelList.getSelectedValue());
                    expression.requestFocusInWindow();
                }
            }
        });
        filterChannels();

        ButtonGroup modes = new ButtonGroup();
        for (JRadioButton mode : new JRadioButton[]{isOn, threshold, advanced}) {
            modes.add(mode);
            mode.addActionListener(e -> updateMode());
        }
        isOn.setToolTipText("On while the channel is not zero, e.g. a fan or relay state");
        threshold.setToolTipText("On while the channel compares true with a number, e.g. above 100");
        advanced.setToolTipText("Any condition, e.g. CLT > 100 && RPM < 1000; double-click a channel to add it");
        operator.addActionListener(e -> updatePreview());
        value.getDocument().addDocumentListener(onChange(this::updatePreview));
        expression.getDocument().addDocumentListener(onChange(this::updatePreview));
        name.putClientProperty("JTextField.placeholderText", "Optional, e.g. Coolant overheat");
        name.getDocument().addDocumentListener(onChange(this::updatePreview));
        name.setToolTipText("What the indicator is called; the chip shows it unless you type labels below");
        for (JTextField label : new JTextField[]{offLabel, onLabel}) {
            label.getDocument().addDocumentListener(onChange(this::updatePreview));
        }
        offLabel.putClientProperty("JTextField.placeholderText", "Same as on");
        onLabel.putClientProperty("JTextField.placeholderText", "The name, or else the condition");
        for (JComboBox<Tone> tone : Arrays.asList(offTone, onTone)) {
            tone.setRenderer(new ToneRenderer());
            tone.addActionListener(e -> updatePreview());
        }

        if (existing != null) {
            load(existing);
        } else {
            isOn.setSelected(true);
            offTone.setSelectedItem(Tone.NEUTRAL);
            onTone.setSelectedItem(Tone.RED);
        }
        layOut();
        updateMode();
    }

    private static List<String> channelsOf(@Nullable IniFileModel ini, Map<String, String> units) {
        Set<String> names = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);
        if (ini != null) {
            for (Map.Entry<String, IniField> channel : ini.getAllOutputChannels().entrySet()) {
                names.add(channel.getKey());
                if (channel.getValue() instanceof ScalarIniField) {
                    units.put(channel.getKey(), ((ScalarIniField) channel.getValue()).getUnits());
                }
            }
            Map<String, String> expressions = ini.getExpressionOutputChannels();
            if (expressions != null) {
                names.addAll(expressions.keySet());
            }
        }
        return new ArrayList<>(names);
    }

    /** Opens {@code indicator} in the simplest mode that writes its expression back unchanged. */
    private void load(CustomIndicator indicator) {
        String condition = indicator.getExpression();
        Matcher compare = THRESHOLD.matcher(condition);
        if (IDENTIFIER.matcher(condition).matches() && channels.contains(condition)) {
            isOn.setSelected(true);
            selectChannel(condition);
        } else if (compare.matches() && channels.contains(compare.group(1))) {
            threshold.setSelected(true);
            selectChannel(compare.group(1));
            operator.setSelectedItem(Operator.of(compare.group(2)));
            value.setText(compare.group(3));
        } else {
            advanced.setSelected(true);
            expression.setText(condition);
        }
        name.setText(indicator.getName());
        // labels that are the default stay empty, so they keep following the name and condition
        boolean onIsDefault = indicator.isDefaultLabel(indicator.getOnLabel());
        onLabel.setText(onIsDefault ? "" : indicator.getOnLabel());
        offLabel.setText(indicator.getOffLabel().equals(indicator.getOnLabel())
            || (onIsDefault && indicator.isDefaultLabel(indicator.getOffLabel())) ? "" : indicator.getOffLabel());
        offTone.setSelectedItem(indicator.getOffTone());
        onTone.setSelectedItem(indicator.getOnTone());
    }

    void selectChannel(String channel) {
        search.setText("");
        channelList.setSelectedValue(channel, true);
    }

    private void filterChannels() {
        String selected = channelList.getSelectedValue();
        String query = search.getText().trim().toLowerCase(Locale.ROOT);
        channelModel.clear();
        for (String channel : channels) {
            if (query.isEmpty() || channel.toLowerCase(Locale.ROOT).contains(query)) {
                channelModel.addElement(channel);
            }
        }
        if (selected != null && channelModel.contains(selected)) {
            channelList.setSelectedValue(selected, true);
        } else if (!query.isEmpty() && !channelModel.isEmpty()) {
            channelList.setSelectedIndex(0);
        }
    }

    private void layOut() {
        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(4, 4, 4, 4);
        c.anchor = GridBagConstraints.NORTHWEST;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridy = 0;
        addRow(c, "Name", name);
        c.gridy++;
        addRow(c, "Channel", search);
        c.gridy++;
        c.gridx = 1;
        JScrollPane channelScroll = new JScrollPane(channelList);
        channelScroll.setMinimumSize(channelScroll.getPreferredSize());
        content.add(channelScroll, c);

        JPanel modes = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        for (JRadioButton mode : new JRadioButton[]{isOn, threshold, advanced}) {
            modes.add(mode);
        }
        c.gridy++;
        addRow(c, "Condition", modes);
        thresholdRow.add(operator);
        thresholdRow.add(value);
        thresholdRow.add(units);
        conditionDetail.add(new JPanel(), Mode.IS_ON.name());
        conditionDetail.add(thresholdRow, Mode.THRESHOLD.name());
        JPanel expressionCard = new JPanel(new BorderLayout());
        expressionCard.add(expression, BorderLayout.NORTH);
        conditionDetail.add(expressionCard, Mode.EXPRESSION.name());
        c.gridy++;
        c.gridx = 1;
        content.add(conditionDetail, c);

        c.gridy++;
        addRow(c, "Off", stateRow(offLabel, offTone));
        c.gridy++;
        addRow(c, "On", stateRow(onLabel, onTone));
        c.gridy++;
        previewHolder.setOpaque(false);
        previewHeight = chipHeight();
        addRow(c, "Preview", previewHolder);
    }

    private static JPanel stateRow(JTextField label, JComboBox<Tone> tone) {
        JPanel row = new JPanel(new BorderLayout(6, 0));
        row.add(label, BorderLayout.CENTER);
        row.add(tone, BorderLayout.EAST);
        return row;
    }

    private void addRow(GridBagConstraints c, String label, JComponent field) {
        c.gridx = 0;
        c.weightx = 0;
        content.add(new JLabel(label), c);
        c.gridx = 1;
        c.weightx = 1;
        content.add(field, c);
    }

    private void updateMode() {
        Mode mode = advanced.isSelected() ? Mode.EXPRESSION : threshold.isSelected() ? Mode.THRESHOLD : Mode.IS_ON;
        ((CardLayout) conditionDetail.getLayout()).show(conditionDetail, mode.name());
        if (advanced.isSelected()) {
            if (expression.getText().trim().isEmpty()) {
                // start from the condition the previous mode described
                expression.setText(channelCondition(thresholdWasChosen));
            }
        } else {
            thresholdWasChosen = threshold.isSelected();
        }
        content.revalidate();
        updatePreview();
    }

    private void updateUnits() {
        String unit = channelUnits.get(channelList.getSelectedValue());
        units.setText(unit == null ? "" : unit);
    }

    /** The condition the fields describe, valid or not; empty when nothing is chosen yet. */
    String conditionFromFields() {
        return advanced.isSelected() ? expression.getText().trim() : channelCondition(threshold.isSelected());
    }

    /** The chosen channel alone, or compared with the value; empty without a channel. */
    private String channelCondition(boolean compare) {
        String channel = channelList.getSelectedValue();
        if (channel == null) {
            return "";
        }
        if (!compare) {
            return channel;
        }
        return channel + " " + ((Operator) operator.getSelectedItem()).expression + " " + value.getText().trim();
    }


    /** The indicator the dialog describes, or the reason it cannot be made. */
    static final class Parsed {
        @Nullable
        final CustomIndicator indicator;
        @Nullable
        final String error;

        Parsed(@Nullable CustomIndicator indicator, @Nullable String error) {
            this.indicator = indicator;
            this.error = error;
        }
    }

    Parsed parse() {
        if (!advanced.isSelected() && channelList.getSelectedValue() == null) {
            return new Parsed(null, "Choose a channel.");
        }
        if (threshold.isSelected()) {
            try {
                if (!Double.isFinite(Double.parseDouble(value.getText().trim()))) {
                    throw new NumberFormatException();
                }
            } catch (NumberFormatException e) {
                return new Parsed(null, "Enter a number to compare the channel with.");
            }
        }
        String condition = conditionFromFields();
        if (condition.isEmpty()) {
            return new Parsed(null, "Enter a condition, e.g. CLT > 100.");
        }
        if (condition.contains("{") || condition.contains("}")) {
            return new Parsed(null, "The condition cannot contain { or }.");
        }
        Set<String> names = ExpressionEvaluator.extractVariables(condition);
        if (names.isEmpty()) {
            return new Parsed(null, "The condition must use at least one channel.");
        }
        if (ini != null) {
            List<String> unknown = new ArrayList<>();
            for (String name : names) {
                if (!channels.contains(name)) {
                    unknown.add(name);
                }
            }
            if (!unknown.isEmpty()) {
                return new Parsed(null, "Unknown channel" + (unknown.size() == 1 ? ": " : "s: ") + String.join(", ", unknown) + ".");
            }
        }
        Map<String, Double> zeros = new HashMap<>();
        for (String name : names) {
            zeros.put(name, 0.0);
        }
        if (ExpressionEvaluator.evaluateBooleanExpression(condition, zeros) == null) {
            return new Parsed(null, "The condition is not a valid expression.");
        }
        String on = onLabel.getText().trim();
        String off = offLabel.getText().trim();
        if (on.isEmpty()) {
            on = CustomIndicator.defaultLabel(name.getText(), condition);
        }
        if (off.isEmpty()) {
            off = on;
        }
        for (String label : new String[]{on, off}) {
            if (label.contains("{") || label.contains("}")) {
                return new Parsed(null, "Labels cannot contain { or }.");
            }
            if (label.length() > MAX_LABEL) {
                return new Parsed(null, "Labels can be at most " + MAX_LABEL + " characters.");
            }
        }
        if (name.getText().trim().length() > MAX_LABEL) {
            return new Parsed(null, "A name can be at most " + MAX_LABEL + " characters.");
        }
        return new Parsed(new CustomIndicator(id, condition, off, on,
            (Tone) offTone.getSelectedItem(), (Tone) onTone.getSelectedItem(), name.getText()), null);
    }

    /** Shows the indicator as the strip would, live, or nothing while the fields are not valid yet. */
    private void updatePreview() {
        if (preview != null) {
            preview.destroy();
            preview = null;
        }
        previewHolder.removeAll();
        Parsed parsed = parse();
        if (parsed.indicator != null) {
            preview = new IndicatorPanel(Collections.singletonList(parsed.indicator.toModel()), ini, 0);
            preview.getPanel().setOpaque(false);
            preview.setActive(active);
            previewHolder.add(preview.getPanel(), BorderLayout.WEST);
        } else {
            JLabel hint = new JLabel(parsed.error);
            hint.setToolTipText(parsed.error);
            hint.setForeground(UIManager.getColor("Label.disabledForeground"));
            previewHolder.add(hint, BorderLayout.WEST);
        }
        previewHolder.revalidate();
        previewHolder.repaint();
    }

    /** The height of the indicator strip around one chip, measured once. */
    private int chipHeight() {
        IndicatorPanel sample = new IndicatorPanel(Collections.singletonList(
            CustomIndicator.create("x", "Off", "On", Tone.NEUTRAL, Tone.RED).toModel()), ini, 0);
        int height = sample.getPanel().getPreferredSize().height;
        sample.destroy();
        return height;
    }

    // Package-private so tests read the preview.
    @Nullable
    IndicatorPanel getPreview() {
        return preview;
    }

    JPanel getContent() {
        return content;
    }

    /** Shows the dialog until the entry is valid or cancelled; {@code null} when cancelled. */
    @Nullable
    public CustomIndicator show(Component parent) {
        // the preview follows the live channel only while the dialog is open
        active = true;
        if (preview != null) {
            preview.setActive(true);
        }
        try {
            while (true) {
                int choice = JOptionPane.showConfirmDialog(parent, content, editing ? EDIT_TITLE : NEW_TITLE,
                    JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
                if (choice != JOptionPane.OK_OPTION) {
                    return null;
                }
                Parsed parsed = parse();
                if (parsed.indicator != null) {
                    return parsed.indicator;
                }
                JOptionPane.showMessageDialog(parent, parsed.error, editing ? EDIT_TITLE : NEW_TITLE,
                    JOptionPane.ERROR_MESSAGE);
            }
        } finally {
            active = false;
            if (preview != null) {
                preview.destroy();
                preview = null;
            }
        }
    }

    private static DocumentListener onChange(Runnable action) {
        return new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                action.run();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                action.run();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                action.run();
            }
        };
    }

    /** A tone in the colour picker: its name on a swatch of the chip colour. */
    private static final class ToneRenderer extends DefaultListCellRenderer {
        @Override
        public Component getListCellRendererComponent(JList<?> list, Object item, int index, boolean selected, boolean focus) {
            super.getListCellRendererComponent(list, item, index, selected, focus);
            if (item instanceof Tone) {
                Tone tone = (Tone) item;
                Color[] colors = TuningStyle.indicatorColors(tone.iniBackground, tone.iniForeground);
                setIcon(new Icon() {
                    @Override
                    public void paintIcon(Component c, Graphics g, int x, int y) {
                        Graphics2D g2 = (Graphics2D) g.create();
                        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                        g2.setColor(colors[0]);
                        g2.fillRoundRect(x, y, getIconWidth(), getIconHeight(), 6, 6);
                        g2.setColor(colors[1]);
                        g2.drawRoundRect(x, y, getIconWidth() - 1, getIconHeight() - 1, 6, 6);
                        g2.dispose();
                    }

                    @Override
                    public int getIconWidth() {
                        return 18;
                    }

                    @Override
                    public int getIconHeight() {
                        return 12;
                    }
                });
            }
            return this;
        }
    }
}
