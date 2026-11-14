package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.tune.TuningStyle;

import javax.swing.AbstractCellEditor;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListCellRenderer;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JToggleButton;
import javax.swing.ListSelectionModel;
import javax.swing.RowFilter;
import javax.swing.UIManager;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableRowSorter;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.function.BiConsumer;

/**
 * The Pinout tab's Functions view: every pin setting of the tune in one list, like an I/O resources sheet, with
 * the pin it uses. The Pin column picks from this board's pins that the setting can choose (or Not set), so a
 * harness change can be redone in one place instead of in each Tuning page. Choosing a pin another setting uses
 * gives it to this one ({@link PinAssignments#setPin}). A double-click on a setting opens it in Tuning
 * [tag:console_theme].
 */
final class PinoutFunctionsPage extends JPanel {
    /** The table filter: every setting, those with a pin, those without. */
    enum Filter {ALL, ASSIGNED, NOT_SET}

    static final String[] COLUMNS = {"Setting", "Tuning page", "Pin", "Pin function"};
    static final int SETTING_COLUMN = 0;
    static final int PIN_COLUMN = 2;

    /** A board pin a setting can choose, as the Pin column shows it; {@link #NOT_SET} for none. */
    static final class PinChoice {
        static final PinChoice NOT_SET = new PinChoice(null, null, null);

        /** The pin; null for {@link #NOT_SET} and for a choice this board's pinout does not have. */
        final PinoutPin pin;
        final String connector;
        /** The tune's choice ({@code ts_name}); null for {@link #NOT_SET}. */
        final String tsName;

        PinChoice(PinoutPin pin, String connector, String tsName) {
            this.pin = pin;
            this.connector = connector;
            this.tsName = tsName;
        }

        /** "B6", or the tune's choice when the pinout does not list it; empty when not set. */
        String name() {
            return pin != null ? pin.name : tsName == null ? "" : tsName;
        }

        @Override
        public String toString() {
            return tsName == null ? "Not set" : name();
        }
    }

    final JTable table;
    final Model model = new Model();
    final Map<Filter, JToggleButton> filterButtons = new EnumMap<>(Filter.class);
    final JLabel summary = new JLabel();
    private final TableRowSorter<Model> sorter = new TableRowSorter<>(model);
    private final JLabel hint = new JLabel();
    /** The board's pins the tune can set, by {@code ts_name}, in connector order. */
    private final Map<String, PinChoice> boardPins = new LinkedHashMap<>();
    /** Sets a setting's pin (null for none); null while the tune cannot be edited from here. */
    private BiConsumer<String, String> pinSetter;
    /** Opens a setting in Tuning: dialog key, field key. */
    private BiConsumer<String, String> openInTuning;
    private IniFileModel ini;
    private Filter filter = Filter.ALL;
    private String query = "";

    PinoutFunctionsPage() {
        super(new BorderLayout());
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        table = new JTable(model) {
            @Override
            public TableCellEditor getCellEditor(int row, int column) {
                return convertColumnIndexToModel(column) == PIN_COLUMN ? pinEditor : super.getCellEditor(row, column);
            }
        };
        add(createCard(), BorderLayout.CENTER);
        updateHint();
    }

    // ---- building

    private JComponent createCard() {
        table.setRowSorter(sorter);
        sorter.setRowFilter(new RowFilter<Model, Integer>() {
            @Override
            public boolean include(Entry<? extends Model, ? extends Integer> entry) {
                return isShown(model.rows.get(entry.getIdentifier()));
            }
        });
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.setRowHeight(30);
        table.setShowGrid(false);
        table.setShowHorizontalLines(true);
        table.setGridColor(PinoutDrawing.themed("Console.Card.borderColor", table.getGridColor()));
        table.setIntercellSpacing(new Dimension(0, 1));
        table.setFillsViewportHeight(true);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);
        int[] widths = {300, 220, 200, 360};
        for (int col = 0; col < widths.length; col++) {
            table.getColumnModel().getColumn(col).setPreferredWidth(widths[col]);
        }
        table.setDefaultRenderer(Object.class, new CellRenderer());
        table.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int row = table.rowAtPoint(e.getPoint());
                int column = table.columnAtPoint(e.getPoint());
                if (row < 0 || column < 0 || table.convertColumnIndexToModel(column) == PIN_COLUMN) {
                    return;
                }
                if (e.getClickCount() == 2) {
                    open(model.rows.get(table.convertRowIndexToModel(row)));
                }
            }
        });

        ButtonGroup group = new ButtonGroup();
        List<JToggleButton> segments = new ArrayList<>();
        String[] tips = {"Every pin setting of the tune", "Settings with a pin", "Settings without a pin"};
        for (Filter f : Filter.values()) {
            JToggleButton button = SegmentedControl.segment("");
            button.setToolTipText(tips[f.ordinal()]);
            button.addActionListener(e -> setFilter(f));
            group.add(button);
            filterButtons.put(f, button);
            segments.add(button);
        }
        filterButtons.get(Filter.ALL).setSelected(true);
        JLabel heading = new JLabel("Functions");
        heading.setFont(heading.getFont().deriveFont(Font.BOLD));
        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP + 4, 0));
        left.setOpaque(false);
        left.add(heading);
        left.add(new SegmentedControl(segments.toArray(new JToggleButton[0])));
        PinAssignPanel.muted(summary);
        left.add(summary);
        PinAssignPanel.muted(hint);
        JPanel header = new JPanel(new BorderLayout());
        header.setOpaque(false);
        header.setBorder(BorderFactory.createEmptyBorder(2, 2, 6, 8));
        header.add(left, BorderLayout.WEST);
        header.add(hint, BorderLayout.EAST);

        JScrollPane scroll = new JScrollPane(table);
        scroll.setBorder(BorderFactory.createEmptyBorder());
        JPanel card = new JPanel(new BorderLayout());
        TuningStyle.styleCard(card, 8, 10);
        card.add(header, BorderLayout.NORTH);
        card.add(scroll, BorderLayout.CENTER);
        return card;
    }

    // ---- content

    /** The board's connectors: their pins the tune can set become the Pin column's choices. */
    void setBoard(List<PinoutConnectorPage> connectors) {
        boardPins.clear();
        for (PinoutConnectorPage page : connectors) {
            for (PinoutPin pin : page.pins) {
                if (!pin.tsName.isEmpty()) {
                    boardPins.putIfAbsent(pin.tsName, new PinChoice(pin, page.title, pin.tsName));
                }
            }
        }
        model.fireTableDataChanged();
    }

    /** Lists the tune's pin settings; keeps the selected setting selected. */
    void setTune(IniFileModel ini, ConfigurationImage image) {
        if (table.isEditing()) {
            table.getCellEditor().cancelCellEditing();
        }
        String selectedKey = selected() == null ? null : selected().fieldKey;
        this.ini = ini;
        model.rows = ini == null || image == null ? Collections.emptyList() : PinAssignments.settings(ini, image);
        model.fireTableDataChanged();
        refreshCounts();
        if (selectedKey != null) {
            select(selectedKey);
        }
    }

    /** Lets the Pin column change the tune; null makes the view read-only. Args: field key, ts_name or null. */
    void setPinSetter(BiConsumer<String, String> pinSetter) {
        this.pinSetter = pinSetter;
        updateHint();
    }

    void setOpenInTuning(BiConsumer<String, String> openInTuning) {
        this.openInTuning = openInTuning;
    }

    /** The tab's search; returns how many settings match it. */
    int setQuery(String query) {
        this.query = query == null ? "" : query;
        refreshCounts();
        int count = 0;
        for (PinAssignments.Candidate row : model.rows) {
            if (matches(row)) {
                count++;
            }
        }
        return count;
    }

    void setFilter(Filter filter) {
        this.filter = filter;
        filterButtons.get(filter).setSelected(true);
        refreshCounts();
    }

    private void refreshCounts() {
        int assigned = 0;
        int shown = 0;
        for (PinAssignments.Candidate row : model.rows) {
            if (row.current != null) {
                assigned++;
            }
            if (isShown(row)) {
                shown++;
            }
        }
        int all = model.rows.size();
        filterButtons.get(Filter.ALL).setText("All  " + all);
        filterButtons.get(Filter.ASSIGNED).setText("Assigned  " + assigned);
        filterButtons.get(Filter.NOT_SET).setText("Not set  " + (all - assigned));
        summary.setText(shown < all ? shown + " of " + all + " shown" : "");
        sorter.sort();
        table.repaint();
    }

    private void updateHint() {
        hint.setText(pinSetter != null ? "Choose a pin in the Pin column · double-click a setting to open it in Tuning"
            : "Double-click a setting to open it in Tuning");
    }

    boolean isShown(PinAssignments.Candidate row) {
        boolean filtered = filter == Filter.ALL || (filter == Filter.ASSIGNED) == (row.current != null);
        return filtered && matches(row);
    }

    private boolean matches(PinAssignments.Candidate row) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        if (needle.isEmpty()) {
            return true;
        }
        PinChoice pin = choiceOf(row);
        String[] texts = {row.label, row.section, row.fieldKey, pin.name(),
            pin.pin == null ? "" : pin.pin.function, pin.connector == null ? "" : pin.connector};
        for (String text : texts) {
            if (text.toLowerCase(Locale.ROOT).contains(needle)) {
                return true;
            }
        }
        return false;
    }

    /** The Pin column's value for a setting. */
    PinChoice choiceOf(PinAssignments.Candidate row) {
        if (row.current == null) {
            return PinChoice.NOT_SET;
        }
        PinChoice board = boardPins.get(row.current);
        return board != null ? board : new PinChoice(null, null, row.current);
    }

    /** "Not set" and the board pins the setting can choose, with the current choice even when not on the board. */
    List<PinChoice> choicesFor(PinAssignments.Candidate row) {
        List<PinChoice> choices = new ArrayList<>();
        choices.add(PinChoice.NOT_SET);
        PinChoice current = choiceOf(row);
        if (current.tsName != null && current.pin == null) {
            choices.add(current);
        }
        for (PinChoice choice : boardPins.values()) {
            if (ini != null && PinAssignments.accepts(ini, row.fieldKey, choice.tsName)) {
                choices.add(choice);
            }
        }
        return choices;
    }

    /** The settings using each pin now, for the pin list's "used by". */
    private Map<String, List<String>> usersByPin() {
        Map<String, List<String>> users = new HashMap<>();
        for (PinAssignments.Candidate row : model.rows) {
            if (row.current != null) {
                users.computeIfAbsent(row.current, k -> new ArrayList<>()).add(row.label);
            }
        }
        return users;
    }

    boolean select(String fieldKey) {
        for (int i = 0; i < model.rows.size(); i++) {
            if (model.rows.get(i).fieldKey.equals(fieldKey)) {
                int view = table.convertRowIndexToView(i);
                if (view < 0) {
                    return false;
                }
                table.setRowSelectionInterval(view, view);
                table.scrollRectToVisible(table.getCellRect(view, 0, true));
                return true;
            }
        }
        return false;
    }

    PinAssignments.Candidate selected() {
        int view = table.getSelectedRow();
        return view < 0 ? null : model.rows.get(table.convertRowIndexToModel(view));
    }

    void open(PinAssignments.Candidate row) {
        if (openInTuning != null) {
            openInTuning.accept(row.dialogKey, row.fieldKey);
        }
    }

    /** Applies a Pin column choice; the list refreshes when the tune change comes back. */
    void choose(PinAssignments.Candidate row, PinChoice choice) {
        if (pinSetter == null || choice == null || java.util.Objects.equals(choice.tsName, row.current)) {
            return;
        }
        pinSetter.accept(row.fieldKey, choice.tsName);
    }

    /** The Setting column: the label, "(off)" while Tuning hides it, "(restart)" when only a restart applies it. */
    static String settingText(PinAssignments.Candidate setting) {
        return setting.label + (setting.hidden ? "  (off)" : "") + (setting.needsRestart ? "  (restart)" : "");
    }

    // ---- table parts

    /** The rows: pin settings in Tuning order; only the Pin column is editable, and only with a pin setter. */
    final class Model extends AbstractTableModel {
        List<PinAssignments.Candidate> rows = Collections.emptyList();

        @Override
        public int getRowCount() {
            return rows.size();
        }

        @Override
        public int getColumnCount() {
            return COLUMNS.length;
        }

        @Override
        public String getColumnName(int column) {
            return COLUMNS[column];
        }

        @Override
        public Object getValueAt(int row, int column) {
            PinAssignments.Candidate setting = rows.get(row);
            PinChoice pin = choiceOf(setting);
            switch (column) {
                case 0:
                    return setting.label;
                case 1:
                    return setting.section;
                case 2:
                    // sorts "Not set" first, then by pin name
                    return pin.name();
                default:
                    return pin.pin == null ? "" : pin.pin.function;
            }
        }

        @Override
        public boolean isCellEditable(int row, int column) {
            return column == PIN_COLUMN && pinSetter != null;
        }

        @Override
        public void setValueAt(Object value, int row, int column) {
            if (value instanceof PinChoice) {
                choose(rows.get(row), (PinChoice) value);
            }
        }
    }

    /** The Pin column's list: refilled for each setting with the pins it can choose. */
    private final TableCellEditor pinEditor = new PinEditor();

    private final class PinEditor extends AbstractCellEditor implements TableCellEditor {
        private final JComboBox<PinChoice> combo = new JComboBox<>();
        private Map<String, List<String>> users = Collections.emptyMap();
        private boolean filling;

        PinEditor() {
            combo.setRenderer(new DefaultListCellRenderer() {
                @Override
                public Component getListCellRendererComponent(JList<?> list, Object value, int index,
                                                              boolean isSelected, boolean cellHasFocus) {
                    super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
                    PinChoice choice = (PinChoice) value;
                    if (choice != null && index >= 0 && choice.pin != null) {
                        List<String> using = users.getOrDefault(choice.tsName, Collections.emptyList());
                        setText(choice.pin.name + "  ·  " + choice.pin.function
                            + (using.isEmpty() ? "" : "  —  used by " + String.join(", ", using)));
                    }
                    return this;
                }
            });
            combo.addActionListener(e -> {
                if (!filling) {
                    stopCellEditing();
                }
            });
        }

        @Override
        public Component getTableCellEditorComponent(JTable t, Object value, boolean isSelected, int row, int column) {
            PinAssignments.Candidate setting = model.rows.get(t.convertRowIndexToModel(row));
            filling = true;
            try {
                users = usersByPin();
                DefaultComboBoxModel<PinChoice> items = new DefaultComboBoxModel<>();
                PinChoice current = choiceOf(setting);
                for (PinChoice choice : choicesFor(setting)) {
                    items.addElement(choice);
                }
                combo.setModel(items);
                combo.setSelectedItem(current);
            } finally {
                filling = false;
            }
            // the click that started editing also opens the list
            javax.swing.SwingUtilities.invokeLater(() -> {
                if (combo.isShowing()) {
                    combo.showPopup();
                }
            });
            return combo;
        }

        @Override
        public Object getCellEditorValue() {
            return combo.getSelectedItem();
        }
    }

    /** A setting (marked when Tuning hides it), its page muted, its pin in bold, and the pin's function. */
    private final class CellRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable t, Object value, boolean isSelected, boolean hasFocus,
                                                       int row, int column) {
            super.getTableCellRendererComponent(t, value, isSelected, false, row, column);
            PinAssignments.Candidate setting = model.rows.get(t.convertRowIndexToModel(row));
            setBorder(BorderFactory.createEmptyBorder(0, column == 0 ? 10 : 6, 0, 6));
            setFont(t.getFont());
            Color muted = isSelected ? t.getSelectionForeground() : UIManager.getColor("Label.disabledForeground");
            setForeground(isSelected ? t.getSelectionForeground() : t.getForeground());
            setToolTipText(null);
            switch (t.convertColumnIndexToModel(column)) {
                case 0:
                    setText(settingText(setting));
                    List<String> notes = new ArrayList<>();
                    if (setting.hidden) {
                        notes.add(PinAssignPanel.hiddenReason(setting));
                    }
                    if (setting.needsRestart) {
                        notes.add("A change takes effect after you burn and restart the ECU");
                    }
                    setToolTipText(notes.isEmpty() ? null : String.join(". ", notes));
                    break;
                case 1:
                    setForeground(muted);
                    break;
                case 2:
                    PinChoice pin = choiceOf(setting);
                    if (pin.tsName == null) {
                        setText(pinSetter != null ? "Not set  ▾" : "Not set");
                        setForeground(muted);
                    } else {
                        setText(pin.name() + (pin.connector == null ? "" : "  ·  " + pin.connector)
                            + (pinSetter != null ? "  ▾" : ""));
                        setFont(ConsoleFonts.mono(Font.BOLD, t.getFont().getSize()));
                        if (pin.pin == null) {
                            setToolTipText("Not on this board's pinout");
                        }
                    }
                    break;
                default:
                    setForeground(muted);
                    setToolTipText(value == null || value.toString().isEmpty() ? null : value.toString());
            }
            return this;
        }
    }
}
