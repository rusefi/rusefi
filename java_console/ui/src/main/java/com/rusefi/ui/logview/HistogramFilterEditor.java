package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.util.WrapLayout;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSpinner;
import javax.swing.JTextField;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The fields of a new or edited histogram filter, with a live count of the records it would hide. Quick filters
 * fill the form for the usual cases (warm-up, throttle transients, engine off, fuel cut, a saturated sensor),
 * offered when the log has their signal. The signal list narrows as you type, lists the histogram's signals and
 * the usual filter signals first, leaves out signals that never change, and shows each one's unit and range in
 * this log, which is also shown next to the value.
 */
final class HistogramFilterEditor extends JPanel {
    /** Signals people usually filter on, listed after the histogram's own. */
    static final String[] SUGGESTED = {"TPS", "CLT", "RPM", "MAP", "dfcoActive", "Vehicle Speed", "IAT",
            "Intake Air IAT"};
    /** The form's width; the quick filters wrap at it. */
    static final int FORM_WIDTH = 500;

    /** A one-click starting point: a filter and the words on its button. */
    static final class QuickFilter {
        final String title;
        final HistogramFilter filter;

        QuickFilter(String title, HistogramFilter filter) {
            this.title = title;
            this.filter = filter;
        }
    }

    private final CsvLogReader log;
    final JTextField search = new JTextField();
    private final DefaultListModel<Integer> signals = new DefaultListModel<>();
    final JList<Integer> signalList = new JList<>(signals);
    private final List<Integer> ordered;
    private final Set<Integer> suggested;
    final List<QuickFilter> quickFilters;
    /** The conditions on one signal: a box filter comes from a box on the scatter plot, not from this dialog. */
    final JComboBox<HistogramFilter.Condition> condition = new JComboBox<>(java.util.Arrays.stream(
            HistogramFilter.Condition.values()).filter(c -> !c.isBox()).toArray(HistogramFilter.Condition[]::new));
    final JTextField value = new JTextField(7);
    final JTextField value2 = new JTextField(7);
    private final JLabel and = ChartToolbar.muted(new JLabel("and"));
    private final JLabel unit = ChartToolbar.muted(new JLabel());
    final JLabel range = ChartToolbar.muted(new JLabel(" "));
    final JCheckBox hold = new JCheckBox("Also hide the next");
    final JSpinner holdMs = new JSpinner(new SpinnerNumberModel(300, 10, 10_000, 50));
    final JLabel preview = ChartToolbar.muted(new JLabel(" "));
    /** The filter's signal when the log does not have it (editing a filter made on another log). */
    private final String missingSignal;

    /**
     * @param axes the histogram's X, Y and Z, listed first; Z also offers a saturation quick filter
     */
    HistogramFilterEditor(CsvLogReader log, HistogramFilter initial, int... axes) {
        super(new GridBagLayout());
        this.log = log;
        missingSignal = initial != null && initial.field(log) < 0 ? initial.signal : null;
        suggested = suggested(log, axes);
        ordered = ordered(log, suggested, initial);
        quickFilters = quickFilters(log, axes.length > 2 ? axes[2] : -1);
        setBorder(BorderFactory.createEmptyBorder(4, 0, 4, 0));

        int row = 0;
        if (!quickFilters.isEmpty() && initial == null) {
            add(ChartToolbar.muted(new JLabel("Quick filters")), 0, row++, 2);
            JPanel quick = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 2));
            for (QuickFilter quickFilter : quickFilters) {
                JButton button = new JButton(quickFilter.title);
                int hides = quickFilter.filter.count(log);
                // e.g. Warm-up on a log recorded hot: offered, but there is nothing to hide
                button.setEnabled(hides > 0);
                button.setToolTipText(hides > 0
                        ? String.format(Locale.ROOT, "%s: hides %,d records. Fills in the form; OK adds it",
                        quickFilter.filter, hides)
                        : quickFilter.filter + ": hides nothing in this log");
                button.addActionListener(e -> fill(quickFilter.filter));
                quick.add(button);
                quick.add(javax.swing.Box.createHorizontalStrut(6));
            }
            // wrap at the dialog's width: lay the buttons out at that width once to learn the height
            quick.setSize(FORM_WIDTH, 1);
            quick.setPreferredSize(new Dimension(FORM_WIDTH, quick.getPreferredSize().height));
            add(quick, 0, row++, 2);
        }
        add(ChartToolbar.muted(new JLabel("Hide records where")), 0, row++, 2);

        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search signals");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent e) { refilter(); }
            @Override public void removeUpdate(DocumentEvent e) { refilter(); }
            @Override public void changedUpdate(DocumentEvent e) { refilter(); }
        });
        // Down moves from the search box into the list, Enter there picks the first match
        search.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent event) {
                if (event.getKeyCode() == KeyEvent.VK_DOWN && !signals.isEmpty()) {
                    signalList.requestFocusInWindow();
                    if (signalList.getSelectedIndex() < 0) {
                        signalList.setSelectedIndex(0);
                    }
                } else if (event.getKeyCode() == KeyEvent.VK_ENTER && !signals.isEmpty()
                        && signalList.getSelectedIndex() < 0) {
                    signalList.setSelectedIndex(0);
                    event.consume();
                }
            }
        });
        signalList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        signalList.setCellRenderer(new SignalRenderer());
        signalList.setFixedCellHeight(26);
        signalList.setVisibleRowCount(7);
        signalList.addListSelectionListener(e -> {
            if (!e.getValueIsAdjusting()) {
                update();
            }
        });
        JScrollPane listScroll = new JScrollPane(signalList);
        listScroll.setPreferredSize(new Dimension(FORM_WIDTH - 60, 26 * 7 + 4));
        JPanel signalBox = new JPanel(new BorderLayout(0, 4));
        signalBox.add(search, BorderLayout.NORTH);
        signalBox.add(listScroll, BorderLayout.CENTER);
        add(ChartToolbar.muted(new JLabel("Signal")), 0, row, 1);
        add(signalBox, 1, row++, 1);

        add(ChartToolbar.muted(new JLabel("Is")), 0, row, 1);
        add(condition, 1, row++, 1);
        JPanel values = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        values.add(value);
        values.add(and);
        values.add(value2);
        values.add(unit);
        add(ChartToolbar.muted(new JLabel("Value")), 0, row, 1);
        add(values, 1, row++, 1);
        add(new JLabel(), 0, row, 1);
        add(range, 1, row++, 1);
        JPanel holdRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 6, 0));
        holdRow.add(hold);
        holdRow.add(holdMs);
        holdRow.add(new JLabel("ms after each match"));
        add(holdRow, 0, row++, 2);
        add(preview, 0, row, 2);

        refilter();
        if (initial != null) {
            fill(initial);
        }
        condition.addActionListener(e -> update());
        hold.addActionListener(e -> update());
        holdMs.addChangeListener(e -> update());
        DocumentListener typing = new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent e) { update(); }
            @Override public void removeUpdate(DocumentEvent e) { update(); }
            @Override public void changedUpdate(DocumentEvent e) { update(); }
        };
        value.getDocument().addDocumentListener(typing);
        value2.getDocument().addDocumentListener(typing);
        update();
    }

    /** The histogram's signals, then the usual filter signals the log has. */
    private static Set<Integer> suggested(CsvLogReader log, int[] axes) {
        Set<Integer> fields = new LinkedHashSet<>();
        for (int axis : axes) {
            if (axis > 0) {
                fields.add(axis);
            }
        }
        for (String name : SUGGESTED) {
            int field = HistogramSignals.byName(log, name);
            if (field > 0 && !log.isConstant(field)) {
                fields.add(field);
            }
        }
        return fields;
    }

    /** Suggested first, then every other signal that changes, by name. */
    private static List<Integer> ordered(CsvLogReader log, Set<Integer> suggested, HistogramFilter initial) {
        List<Integer> fields = new ArrayList<>(suggested);
        List<Integer> rest = new ArrayList<>();
        int initialField = initial == null ? -1 : initial.field(log);
        for (int field = 1; field < log.fieldCount(); field++) {
            // a signal that never changes can only hide all records or none
            if (!fields.contains(field) && (!log.isConstant(field) || field == initialField)) {
                rest.add(field);
            }
        }
        rest.sort((a, b) -> log.name(a).compareToIgnoreCase(log.name(b)));
        fields.addAll(rest);
        return fields;
    }

    /** The usual filters, each offered when the log has its signal. */
    static List<QuickFilter> quickFilters(CsvLogReader log, int zField) {
        List<QuickFilter> result = new ArrayList<>();
        if (zField > 0) {
            HistogramSignals.Saturation saturation = HistogramSignals.saturation(log, zField);
            if (saturation != null) {
                result.add(new QuickFilter("Saturated " + log.name(zField), saturation.filter(log.name(zField))));
            }
        }
        add(result, log, "Warm-up", new HistogramFilter("CLT", HistogramFilter.Condition.AT_MOST, 70, Double.NaN, 0,
                true));
        add(result, log, "Throttle transients", new HistogramFilter("TPS", HistogramFilter.Condition.CHANGING, 50,
                Double.NaN, 0.3, true));
        add(result, log, "Engine off / cranking", new HistogramFilter("RPM", HistogramFilter.Condition.AT_MOST, 500,
                Double.NaN, 0, true));
        add(result, log, "Fuel cut", new HistogramFilter("dfcoActive", HistogramFilter.Condition.AT_LEAST, 1,
                Double.NaN, 0.5, true));
        return result;
    }

    private static void add(List<QuickFilter> result, CsvLogReader log, String title, HistogramFilter filter) {
        int field = filter.field(log);
        if (field > 0 && !log.isConstant(field)) {
            result.add(new QuickFilter(title, filter));
        }
    }

    private void refilter() {
        Integer selected = signalList.getSelectedValue();
        String needle = search.getText().trim().toLowerCase(Locale.ROOT);
        signals.clear();
        for (int field : ordered) {
            if (needle.isEmpty() || log.name(field).toLowerCase(Locale.ROOT).contains(needle)) {
                signals.addElement(field);
            }
        }
        if (selected != null && signals.contains(selected)) {
            signalList.setSelectedValue(selected, true);
        }
    }

    /** Selects a signal by name, clearing the search if it hides it; false when the list lacks it. */
    boolean selectSignal(String name) {
        int field = HistogramSignals.byName(log, name);
        if (field < 0 || !ordered.contains(field)) {
            return false;
        }
        if (!signals.contains(field)) {
            search.setText("");
        }
        signalList.setSelectedValue(field, true);
        return true;
    }

    /** Puts a filter's settings in the form, e.g. from a quick filter or the filter being edited. */
    void fill(HistogramFilter filter) {
        selectSignal(filter.signal);
        condition.setSelectedItem(filter.condition);
        value.setText(HistogramFilter.number(filter.value));
        value2.setText(filter.condition.needsSecondValue() ? HistogramFilter.number(filter.value2) : "");
        hold.setSelected(filter.holdSeconds > 0);
        if (filter.holdSeconds > 0) {
            holdMs.setValue((int) Math.round(filter.holdSeconds * 1000));
        }
        update();
    }

    private void add(Component component, int column, int row, int width) {
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = column;
        c.gridy = row;
        c.gridwidth = width;
        c.anchor = GridBagConstraints.NORTHWEST;
        c.fill = column == 1 || width == 2 ? GridBagConstraints.HORIZONTAL : GridBagConstraints.NONE;
        c.insets = new Insets(4, column == 1 ? 10 : 0, 4, 0);
        add(component, c);
    }

    private HistogramFilter.Condition selectedCondition() {
        return (HistogramFilter.Condition) condition.getSelectedItem();
    }

    /** The chosen signal's name, or the missing one of the filter being edited, or null. */
    private String signalName() {
        Integer field = signalList.getSelectedValue();
        return field != null ? log.name(field) : missingSignal;
    }

    private void update() {
        boolean second = selectedCondition().needsSecondValue();
        and.setVisible(second);
        value2.setVisible(second);
        Integer field = signalList.getSelectedValue();
        String unitText = field == null ? "" : log.unit(field);
        boolean rate = selectedCondition() == HistogramFilter.Condition.CHANGING;
        unit.setText(rate ? (unitText.isEmpty() ? "per second" : unitText + " per second") : unitText);
        range.setText(field == null ? (missingSignal != null ? missingSignal + " is not in this log" : " ")
                : String.format(Locale.ROOT, "In this log: %s to %s%s", ValueFormat.format(log, field, log.minimum(field)),
                ValueFormat.format(log, field, log.maximum(field)), unitText.isEmpty() ? "" : " " + unitText));
        value.setToolTipText(rate ? "Change per second, in the signal's unit" : null);
        holdMs.setEnabled(hold.isSelected());
        HistogramFilter filter = filter();
        if (signalName() == null) {
            preview.setText("Pick a signal");
        } else if (filter == null) {
            preview.setText("Enter a number");
        } else {
            int hidden = filter.count(log);
            preview.setText(String.format(Locale.ROOT, "Hides %,d records (%.1f%% of the log)", hidden,
                    100.0 * hidden / Math.max(1, log.sampleCount())));
        }
        revalidate();
    }

    /** The filter as entered, enabled; null without a signal or while a value is not a number. */
    HistogramFilter filter() {
        String signal = signalName();
        if (signal == null) {
            return null;
        }
        try {
            double first = Double.parseDouble(value.getText().trim().replace(',', '.'));
            double second = selectedCondition().needsSecondValue()
                    ? Double.parseDouble(value2.getText().trim().replace(',', '.')) : Double.NaN;
            double holdSeconds = hold.isSelected() ? ((Integer) holdMs.getValue()) / 1000.0 : 0;
            return new HistogramFilter(signal, selectedCondition(), first, second, holdSeconds, true);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    /** A signal: name on the left, "suggested" for the first ones, unit and range in this log on the right. */
    private final class SignalRenderer extends JPanel implements ListCellRenderer<Integer> {
        private final JLabel name = new JLabel();
        private final JLabel details = new JLabel();

        SignalRenderer() {
            super(new BorderLayout(12, 0));
            setBorder(BorderFactory.createEmptyBorder(0, 8, 0, 8));
            add(name, BorderLayout.CENTER);
            add(details, BorderLayout.EAST);
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends Integer> list, Integer field, int index,
                                                      boolean selected, boolean focused) {
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            setBackground(selected ? list.getSelectionBackground() : list.getBackground());
            name.setText(log.name(field));
            name.setFont(list.getFont().deriveFont(suggested.contains(field) ? Font.BOLD : Font.PLAIN));
            name.setForeground(selected ? list.getSelectionForeground() : list.getForeground());
            String unitText = log.unit(field);
            details.setText(ValueFormat.format(log, field, log.minimum(field)) + "–"
                    + ValueFormat.format(log, field, log.maximum(field)) + (unitText.isEmpty() ? "" : " " + unitText));
            details.setFont(list.getFont().deriveFont(list.getFont().getSize2D() - 1));
            details.setForeground(selected ? list.getSelectionForeground() : muted);
            return this;
        }
    }
}
