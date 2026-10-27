package com.rusefi.ui;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.ui.util.PinColors;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.ConsoleFonts;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.tune.TuningStyle;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JToggleButton;
import javax.swing.ListSelectionModel;
import javax.swing.RowFilter;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableRowSorter;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.function.Function;

/**
 * One connector on the Pinout tab: its drawing ({@link PinoutDrawing}) on a card with a legend, the pin table
 * with an All / Used / Free / Fixed filter, and a card for the selected pin with Open in Tuning
 * [tag:console_theme]. The legend's chips, the filter and the tab's search narrow the same set of pins: the
 * table lists only those and the drawing fades the others. The drawing, table and pin card always show the
 * same selected pin.
 */
final class PinoutConnectorPage extends JPanel {
    static final String[] COLUMNS = {"Pin", "Function", "Type", "Pigtail", "Tune use"};
    static final int TUNE_USE_COLUMN = 4;

    final String title;
    final List<PinoutPin> pins;
    final PinoutDrawing drawing = new PinoutDrawing();
    final JPanel legend = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
    final JLabel summary = new JLabel();
    final DefaultTableModel model = new DefaultTableModel(COLUMNS, 0) {
        @Override
        public boolean isCellEditable(int row, int column) {
            return false;
        }
    };
    final JTable table = new JTable(model);
    private final TableRowSorter<DefaultTableModel> sorter = new TableRowSorter<>(model);
    final Map<PinoutPin.Use, JToggleButton> filterButtons = new EnumMap<>(PinoutPin.Use.class);
    final JToggleButton allButton = SegmentedControl.segment("");
    final PinCard card = new PinCard();
    /** The setting using a pin as {fieldKey, dialogKey}, or null; and what opens it in Tuning. */
    private final Function<PinoutPin, String[]> assignmentOf;
    private final Consumer<String[]> openInTuning;

    private PinoutDrawing.ColorMode mode;
    /** The legend chip chosen: a {@link PinColors.PinType}, a wire colour or a {@link PinoutPin.Use}; null for all. */
    private Object focus;
    /** The table filter; null for All. */
    private PinoutPin.Use filter;
    private String query = "";
    private PinoutPin selected;

    PinoutConnectorPage(String title, BufferedImage image, List<PinoutPin> pins, PinoutDrawing.ColorMode mode,
                        Function<PinoutPin, String[]> assignmentOf, Consumer<String[]> openInTuning) {
        super(new BorderLayout(12, 0));
        this.title = title;
        this.pins = pins;
        this.mode = mode;
        this.assignmentOf = assignmentOf;
        this.openInTuning = openInTuning;
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));

        for (PinoutPin pin : pins) {
            model.addRow(new Object[]{pin.name, pin.function, pin.type.type.label, pin.wire, pin.tuneUse});
        }
        JComponent tableCard = createTableCard();
        drawing.setData(image, pins);
        drawing.setColorMode(mode);
        drawing.setShown(this::isShown);
        drawing.setPinClickListener(name -> select(find(name)));

        if (image != null) {
            JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, createDrawingCard(), tableCard);
            split.setResizeWeight(0.6);
            split.setBorder(BorderFactory.createEmptyBorder());
            split.setDividerSize(10);
            split.setOpaque(false);
            add(split, BorderLayout.CENTER);
        } else {
            add(tableCard, BorderLayout.CENTER);
        }
        add(card, BorderLayout.EAST);
        refresh();
        card.show(null);
    }

    // ---- building

    private JComponent createDrawingCard() {
        JLabel name = new JLabel(title);
        name.setFont(resized(name.getFont(), Font.BOLD, 1));
        withSlack(name);
        JLabel count = withSlack(muted(new JLabel(pins.size() + " pins")));
        JPanel titleRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 0));
        titleRow.setOpaque(false);
        titleRow.add(name);
        titleRow.add(count);
        muted(summary);
        JPanel header = new JPanel(new BorderLayout());
        header.setOpaque(false);
        header.setBorder(BorderFactory.createEmptyBorder(4, 2, 4, 8));
        header.add(titleRow, BorderLayout.WEST);
        header.add(summary, BorderLayout.EAST);
        legend.setOpaque(false);
        // the drawing takes most of the height at first; the divider moves the rest to the table
        drawing.setPreferredSize(new Dimension(800, 380));
        JPanel card = new JPanel(new BorderLayout());
        TuningStyle.styleCard(card, 8, 10);
        card.add(header, BorderLayout.NORTH);
        card.add(drawing, BorderLayout.CENTER);
        card.add(legend, BorderLayout.SOUTH);
        return card;
    }

    private JComponent createTableCard() {
        // the sorter was made with the empty model; without this a tune update throws "Invalid range"
        sorter.modelStructureChanged();
        table.setRowSorter(sorter);
        sorter.setRowFilter(new RowFilter<DefaultTableModel, Integer>() {
            @Override
            public boolean include(Entry<? extends DefaultTableModel, ? extends Integer> entry) {
                return isShown(pins.get(entry.getIdentifier()));
            }
        });
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.setRowHeight(30);
        table.setShowGrid(false);
        table.setShowHorizontalLines(true);
        table.setGridColor(PinoutDrawing.themed("Console.Card.borderColor", new Color(0xeef0f3)));
        table.setIntercellSpacing(new Dimension(0, 1));
        table.setFillsViewportHeight(true);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);
        int[] widths = {90, 420, 170, 140, 240};
        for (int col = 0; col < widths.length; col++) {
            table.getColumnModel().getColumn(col).setPreferredWidth(widths[col]);
        }
        table.setDefaultRenderer(Object.class, new CellRenderer());
        table.getSelectionModel().addListSelectionListener(e -> {
            if (e.getValueIsAdjusting()) return;
            int viewRow = table.getSelectedRow();
            if (viewRow >= 0) {
                PinoutPin pin = pins.get(table.convertRowIndexToModel(viewRow));
                if (pin != selected) {
                    select(pin);
                }
            }
        });
        MouseAdapter tuneUseLink = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                PinoutPin pin = tuneUseLinkAt(e);
                if (pin != null && e.getClickCount() == 1) {
                    open(pin);
                }
            }

            @Override
            public void mouseMoved(MouseEvent e) {
                table.setCursor(Cursor.getPredefinedCursor(tuneUseLinkAt(e) != null ? Cursor.HAND_CURSOR
                        : Cursor.DEFAULT_CURSOR));
            }
        };
        table.addMouseListener(tuneUseLink);
        table.addMouseMotionListener(tuneUseLink);

        ButtonGroup group = new ButtonGroup();
        allButton.setToolTipText("Every pin");
        allButton.addActionListener(e -> setFilter(null));
        group.add(allButton);
        allButton.setSelected(true);
        List<JToggleButton> segments = new ArrayList<>();
        segments.add(allButton);
        String[][] filters = {{"Pins the tune uses"}, {"Pins the tune could use but does not"},
                {"Power and ground pins, which the tune does not set"}};
        for (PinoutPin.Use use : PinoutPin.Use.values()) {
            JToggleButton button = SegmentedControl.segment("");
            button.setToolTipText(filters[use.ordinal()][0]);
            button.addActionListener(e -> setFilter(use));
            group.add(button);
            filterButtons.put(use, button);
            segments.add(button);
        }
        JLabel heading = new JLabel("Pins");
        heading.setFont(heading.getFont().deriveFont(Font.BOLD));
        withSlack(heading);
        JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP + 4, 0));
        left.setOpaque(false);
        left.add(heading);
        left.add(new SegmentedControl(segments.toArray(new JToggleButton[0])));
        JLabel hint = withSlack(muted(new JLabel("Click a Tune use to open it in Tuning")));
        JPanel header = new JPanel(new BorderLayout());
        header.setOpaque(false);
        header.setBorder(BorderFactory.createEmptyBorder(2, 2, 6, 8));
        header.add(left, BorderLayout.WEST);
        header.add(hint, BorderLayout.EAST);

        JScrollPane scroll = new JScrollPane(table);
        scroll.setBorder(BorderFactory.createEmptyBorder());
        scroll.setPreferredSize(new Dimension(0, 200));
        JPanel card = new JPanel(new BorderLayout());
        TuningStyle.styleCard(card, 8, 10);
        card.add(header, BorderLayout.NORTH);
        card.add(scroll, BorderLayout.CENTER);
        return card;
    }

    // ---- what is shown

    /** Whether a pin passes the legend chip, the filter and the search. */
    boolean isShown(PinoutPin pin) {
        return (focus == null || focus.equals(legendKey(pin))) && (filter == null || pin.use() == filter)
                && matches(pin, query);
    }

    /** Whether a pin matches the tab's search: its name, function, type, wire, tune name or tune use. */
    static boolean matches(PinoutPin pin, String query) {
        String needle = query == null ? "" : query.trim().toLowerCase(Locale.ROOT);
        if (needle.isEmpty()) {
            return true;
        }
        for (String text : new String[]{pin.name, pin.function, pin.type.type.label, pin.wire, pin.tsName, pin.tuneUse}) {
            if (text.toLowerCase(Locale.ROOT).contains(needle)) {
                return true;
            }
        }
        return false;
    }

    /** The search; returns how many pins of this connector match it. */
    int setQuery(String query) {
        this.query = query;
        refresh();
        int count = 0;
        for (PinoutPin pin : pins) {
            if (matches(pin, query)) {
                count++;
            }
        }
        return count;
    }

    void setColorMode(PinoutDrawing.ColorMode mode) {
        this.mode = mode;
        // the chips change meaning with the colours
        focus = null;
        drawing.setColorMode(mode);
        refresh();
        // the card's marker follows the colours
        card.show(selected);
    }

    void setFilter(PinoutPin.Use use) {
        filter = use;
        (use == null ? allButton : filterButtons.get(use)).setSelected(true);
        refresh();
    }

    /** A legend chip: shows only the pins it describes, or all again when chosen twice. */
    void setFocus(Object key) {
        focus = Objects.equals(focus, key) ? null : key;
        refresh();
    }

    Object focus() {
        return focus;
    }

    /** Clears the legend chip and the filter, e.g. before jumping to a pin from Tuning. */
    void showAll() {
        focus = null;
        setFilter(null);
    }

    /** New tune assignments: expanded ts_name to the labels of the settings using it. */
    void setTuneUse(Map<String, String> tuneUse) {
        for (int row = 0; row < pins.size(); row++) {
            PinoutPin pin = pins.get(row);
            pin.tuneUse = tuneUse.getOrDefault(pin.tsName, "");
            model.setValueAt(pin.tuneUse, row, TUNE_USE_COLUMN);
        }
        refresh();
        card.show(selected);
    }

    private Object legendKey(PinoutPin pin) {
        switch (mode) {
            case PIGTAIL:
                return pin.wire.isEmpty() ? "not listed" : pin.wire;
            case TUNE_USE:
                return pin.use();
            default:
                return pin.type.type;
        }
    }

    private static String legendLabel(Object key) {
        if (key instanceof PinColors.PinType) {
            return ((PinColors.PinType) key).label;
        }
        if (key instanceof PinoutPin.Use) {
            switch ((PinoutPin.Use) key) {
                case USED:
                    return "Used by the tune";
                case FREE:
                    return "Free";
                default:
                    return "Fixed function";
            }
        }
        return key.toString();
    }

    /** Legend, counts, table rows and faded markers after any change. */
    private void refresh() {
        Map<Object, Integer> counts = new LinkedHashMap<>();
        Map<Object, PinoutPin> samples = new LinkedHashMap<>();
        List<PinoutPin> ordered = new ArrayList<>(pins);
        if (mode == PinoutDrawing.ColorMode.TYPE) {
            ordered.sort((a, b) -> a.type.type.ordinal() - b.type.type.ordinal());
        } else if (mode == PinoutDrawing.ColorMode.TUNE_USE) {
            ordered.sort((a, b) -> a.use().ordinal() - b.use().ordinal());
        }
        for (PinoutPin pin : ordered) {
            Object key = legendKey(pin);
            counts.merge(key, 1, Integer::sum);
            samples.putIfAbsent(key, pin);
        }
        legend.removeAll();
        for (Map.Entry<Object, Integer> entry : counts.entrySet()) {
            Object key = entry.getKey();
            PinoutPin sample = samples.get(key);
            LegendChip chip = new LegendChip(legendLabel(key) + "  " + entry.getValue(),
                    new MarkerIcon(PinoutDrawing.fills(sample, mode), PinoutDrawing.dashed(sample, mode), 12, null));
            chip.setSelected(key.equals(focus));
            chip.setToolTipText(key.equals(focus) ? "Showing only these pins · click to show all"
                    : "Show only these pins");
            chip.addActionListener(e -> setFocus(key));
            legend.add(chip);
        }
        legend.revalidate();
        legend.repaint();

        Map<PinoutPin.Use, Integer> uses = new EnumMap<>(PinoutPin.Use.class);
        int shown = 0;
        for (PinoutPin pin : pins) {
            uses.merge(pin.use(), 1, Integer::sum);
            if (isShown(pin)) {
                shown++;
            }
        }
        int used = uses.getOrDefault(PinoutPin.Use.USED, 0);
        int free = uses.getOrDefault(PinoutPin.Use.FREE, 0);
        int fixed = uses.getOrDefault(PinoutPin.Use.FIXED, 0);
        summary.setText(shown < pins.size() ? shown + " of " + pins.size() + " pins shown"
                : used + " used  ·  " + free + " free  ·  " + fixed + " fixed");
        allButton.setText("All  " + pins.size());
        filterButtons.get(PinoutPin.Use.USED).setText("Used  " + used);
        filterButtons.get(PinoutPin.Use.FREE).setText("Free  " + free);
        filterButtons.get(PinoutPin.Use.FIXED).setText("Fixed  " + fixed);

        sorter.sort();
        drawing.repaint();
        table.repaint();
        if (selected != null) {
            selectRow(selected);
        }
    }

    // ---- selection

    PinoutPin find(String name) {
        for (PinoutPin pin : pins) {
            if (pin.name.equals(name)) {
                return pin;
            }
        }
        return null;
    }

    PinoutPin selected() {
        return selected;
    }

    /** Selects a pin in the drawing, the table (when listed) and the card. */
    void select(PinoutPin pin) {
        selected = pin;
        drawing.setSelectedPin(pin == null ? null : pin.name);
        selectRow(pin);
        card.show(pin);
    }

    private void selectRow(PinoutPin pin) {
        int row = pin == null ? -1 : table.convertRowIndexToView(pins.indexOf(pin));
        if (row >= 0) {
            if (table.getSelectedRow() != row) {
                table.setRowSelectionInterval(row, row);
            }
            table.scrollRectToVisible(table.getCellRect(row, 0, true));
        } else {
            table.clearSelection();
        }
    }

    /** The pin whose Tune use link is under the mouse, or null. */
    private PinoutPin tuneUseLinkAt(MouseEvent e) {
        int row = table.rowAtPoint(e.getPoint());
        int col = table.columnAtPoint(e.getPoint());
        if (row < 0 || col < 0 || table.convertColumnIndexToModel(col) != TUNE_USE_COLUMN) {
            return null;
        }
        PinoutPin pin = pins.get(table.convertRowIndexToModel(row));
        return pin.use() == PinoutPin.Use.USED && assignmentOf.apply(pin) != null ? pin : null;
    }

    void open(PinoutPin pin) {
        String[] assignment = assignmentOf.apply(pin);
        if (assignment != null) {
            openInTuning.accept(assignment);
        }
    }

    // ---- small parts

    /**
     * Room for text that renders a little wider than measured (bold text, fractional display scaling), so a
     * label laid out at its preferred width keeps its last letters instead of ending in "…".
     */
    private static JLabel withSlack(JLabel label) {
        label.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 4));
        return label;
    }

    /**
     * The font {@code delta} points larger or smaller, rounded to whole points. With display scaling the base
     * size is fractional (e.g. 16.25); a label measured at such a size is narrower than it draws, so a bold title
     * showed as "Connecto…" even with {@link #withSlack}.
     */
    static Font resized(Font font, int style, int delta) {
        return font.deriveFont(style, (float) Math.round(font.getSize2D() + delta));
    }

    private static <T extends JComponent> T muted(T component) {
        component.setForeground(UIManager.getColor("Label.disabledForeground"));
        return component;
    }

    static String useText(PinoutPin pin) {
        switch (pin.use()) {
            case USED:
                return pin.tuneUse;
            case FREE:
                return "Free";
            default:
                return "Fixed function";
        }
    }

    /** Pin with its marker, type with its colour, wire with its swatch, tune use as a link when used. */
    private final class CellRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable t, Object value, boolean isSelected, boolean hasFocus,
                                                       int row, int column) {
            super.getTableCellRendererComponent(t, value, isSelected, false, row, column);
            PinoutPin pin = pins.get(t.convertRowIndexToModel(row));
            setIcon(null);
            setIconTextGap(8);
            setBorder(BorderFactory.createEmptyBorder(0, column == 0 ? 10 : 6, 0, 6));
            setFont(t.getFont());
            setForeground(isSelected ? t.getSelectionForeground() : t.getForeground());
            setToolTipText(null);
            switch (t.convertColumnIndexToModel(column)) {
                case 0:
                    setIcon(new MarkerIcon(PinoutDrawing.fills(pin, mode), PinoutDrawing.dashed(pin, mode), 12, null));
                    setFont(ConsoleFonts.mono(Font.BOLD, t.getFont().getSize()));
                    break;
                case 1:
                    setToolTipText(pin.function.isEmpty() ? null : pin.function);
                    break;
                case 2:
                    setIcon(new MarkerIcon(new Color[]{pin.type.type.color()},
                            pin.type.type == PinColors.PinType.UNTYPED, 10, null));
                    if (pin.type.isInferred()) {
                        setToolTipText(pin.type.type.label + ", from its " + pin.type.inferredFrom
                                + " (the board file gives no type)");
                    }
                    break;
                case 3:
                    setIcon(new WireIcon(pin.wire));
                    setText(pin.wire.isEmpty() ? "not listed" : pin.wire);
                    if (pin.wire.isEmpty()) {
                        setForeground(UIManager.getColor("Label.disabledForeground"));
                    }
                    break;
                default:
                    setText(useText(pin));
                    if (pin.use() == PinoutPin.Use.USED) {
                        setForeground(PinoutDrawing.themed("Console.Pinout.used", new Color(0x2563eb)));
                        setFont(t.getFont().deriveFont(Font.BOLD));
                        setToolTipText("Open " + pin.tuneUse + " in Tuning");
                    } else {
                        setForeground(UIManager.getColor("Label.disabledForeground"));
                    }
            }
            return this;
        }
    }

    /** A legend entry as a pill; selected while it narrows the pins. */
    static final class LegendChip extends JToggleButton {
        LegendChip(String text, Icon icon) {
            super(text, icon);
            setIconTextGap(6);
            setContentAreaFilled(false);
            setBorderPainted(false);
            setFocusPainted(false);
            setBorder(BorderFactory.createEmptyBorder(4, 10, 4, 12));
            setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                int arc = getHeight();
                g2.setColor(isSelected() ? PinoutDrawing.themed("Console.Banner.info.background", new Color(0xeef4ff))
                        : PinoutDrawing.themed("Console.Card.background", Color.WHITE));
                g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
                g2.setColor(isSelected() ? PinoutDrawing.themed("Console.Pinout.selection", new Color(0x2563eb))
                        : PinoutDrawing.themed("Console.Card.borderColor", new Color(0xdfe3e9)));
                g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
            } finally {
                g2.dispose();
            }
            setForeground(isSelected() ? PinoutDrawing.themed("Console.Banner.info.accent", new Color(0x1d4ed8))
                    : UIManager.getColor("Label.foreground"));
            super.paintComponent(g);
        }
    }

    /** A pin marker as an icon: its fill (split for a two-colour wire), dashed when the marker is, maybe a label. */
    static final class MarkerIcon implements Icon {
        private final Color[] fills;
        private final boolean dashed;
        private final int size;
        private final String label;

        MarkerIcon(Color[] fills, boolean dashed, int size, String label) {
            this.fills = fills;
            this.dashed = dashed;
            this.size = size;
            this.label = label;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                g2.setColor(fills[0]);
                g2.fillOval(x, y, size, size);
                if (fills.length == 2) {
                    g2.setClip(x + size / 2, y, size / 2 + 1, size);
                    g2.setColor(fills[1]);
                    g2.fillOval(x, y, size, size);
                    g2.setClip(null);
                }
                float stroke = size > 20 ? 2f : 1f;
                g2.setColor(dashed ? new Color(0x9ca3af) : PinColors.isLight(fills[0]) ? new Color(0xb6bcc6)
                        : new Color(0, 0, 0, 0));
                g2.setStroke(dashed ? new BasicStroke(stroke, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND, 1f,
                        new float[]{2f, 1.5f}, 0f) : new BasicStroke(stroke));
                g2.drawOval(x, y, size - 1, size - 1);
                if (label != null) {
                    Font font = c.getFont().deriveFont(Font.BOLD, Math.min(15f, size * 0.32f));
                    g2.setFont(font);
                    g2.setColor(PinColors.isLight(fills[0]) ? new Color(0x1f2937) : Color.WHITE);
                    int w = g2.getFontMetrics().stringWidth(label);
                    g2.drawString(label, x + (size - w) / 2f,
                            y + (size + g2.getFontMetrics().getAscent() - g2.getFontMetrics().getDescent()) / 2f);
                }
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return size;
        }

        @Override
        public int getIconHeight() {
            return size;
        }
    }

    /** A pigtail wire swatch: one or two colours, hatched when none is listed. */
    static final class WireIcon implements Icon {
        private static final int W = 20;
        private static final int H = 10;
        private final Color[] colors;

        WireIcon(String wire) {
            colors = PinColors.pigtailToColors(wire);
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                if (colors == null) {
                    g2.setColor(new Color(0xc7ccd4));
                    for (int i = -H; i < W; i += 4) {
                        g2.drawLine(x + i, y + H, x + i + H, y);
                    }
                } else {
                    g2.setColor(colors[0]);
                    g2.fillRect(x, y, colors.length == 2 ? W / 2 : W, H);
                    if (colors.length == 2) {
                        g2.setColor(colors[1]);
                        g2.fillRect(x + W / 2, y, W / 2, H);
                    }
                }
                g2.setColor(new Color(0xc7ccd4));
                g2.drawRoundRect(x, y, W - 1, H - 1, 3, 3);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return W;
        }

        @Override
        public int getIconHeight() {
            return H;
        }
    }

    /**
     * The selected pin: its marker and name, function, a Tune use box (with Open in Tuning when a setting uses
     * it), then type, pigtail, tune name and class. A note while no pin is selected.
     */
    final class PinCard extends JPanel {
        final JLabel name = new JLabel();
        final JLabel where = muted(new JLabel());
        final JTextArea function = textArea();
        final JLabel useValue = new JLabel();
        final JTextArea useHint = textArea();
        final JButton openButton = new JButton("Open in Tuning");
        final JLabel type = new JLabel();
        final JLabel typeNote = muted(new JLabel());
        final JLabel wire = new JLabel();
        /** Wraps: tune names can be long, e.g. "C18 VR1+ Discrete for low count wheel". */
        final JTextArea tsName = textArea();
        final JLabel classes = new JLabel();
        private final JPanel useBox = new JPanel(new GridBagLayout());
        private final JPanel details = new JPanel(new GridBagLayout());
        private final JLabel empty = muted(new JLabel("<html><div style='text-align:center'>Click a pin on the "
                + "drawing or in the table<br>to see it here.</div></html>", SwingConstants.CENTER));
        private PinoutPin shown;

        PinCard() {
            super(new BorderLayout());
            TuningStyle.styleCard(this, 14, 16);
            setPreferredSize(new Dimension(320, 0));

            name.setFont(resized(name.getFont(), Font.BOLD, 3));
            name.setIconTextGap(12);
            JPanel header = new JPanel(new BorderLayout(0, 2));
            header.setOpaque(false);
            header.add(name, BorderLayout.NORTH);
            where.setBorder(BorderFactory.createEmptyBorder(0, 56, 0, 0));
            header.add(where, BorderLayout.CENTER);

            JLabel useTitle = muted(new JLabel("TUNE USE"));
            useTitle.setFont(resized(useTitle.getFont(), Font.BOLD, -1));
            useValue.setFont(useValue.getFont().deriveFont(Font.BOLD));
            muted(useHint);
            openButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
            openButton.addActionListener(e -> {
                if (shown != null) {
                    open(shown);
                }
            });
            TuningStyle.styleSection(useBox, 10, 12);
            GridBagConstraints c = new GridBagConstraints();
            c.gridx = 0;
            c.fill = GridBagConstraints.HORIZONTAL;
            c.weightx = 1;
            c.anchor = GridBagConstraints.WEST;
            useBox.add(useTitle, c);
            c.insets = new Insets(4, 0, 0, 0);
            useBox.add(useValue, c);
            c.insets = new Insets(2, 0, 0, 0);
            useBox.add(useHint, c);
            c.insets = new Insets(10, 0, 0, 0);
            c.fill = GridBagConstraints.NONE;
            useBox.add(openButton, c);

            details.setOpaque(false);
            tsName.setFont(ConsoleFonts.mono(Font.PLAIN, UIManager.getFont("Label.font").getSize()));
            JPanel typeRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));
            typeRow.setOpaque(false);
            typeRow.add(type);
            typeRow.add(typeNote);
            addDetail(0, "Type", typeRow);
            addDetail(1, "Pigtail", wire);
            addDetail(2, "Tune name", tsName);
            addDetail(3, "Class", classes);

            JPanel body = new JPanel(new GridBagLayout());
            body.setOpaque(false);
            GridBagConstraints b = new GridBagConstraints();
            b.gridx = 0;
            b.fill = GridBagConstraints.HORIZONTAL;
            b.weightx = 1;
            b.insets = new Insets(0, 0, 14, 0);
            body.add(header, b);
            body.add(function, b);
            body.add(useBox, b);
            b.insets = new Insets(0, 0, 0, 0);
            body.add(details, b);
            b.weighty = 1;
            body.add(new JPanel() {
                {
                    setOpaque(false);
                }
            }, b);
            add(body, BorderLayout.CENTER);
            add(empty, BorderLayout.NORTH);
        }

        private void addDetail(int row, String label, JComponent value) {
            GridBagConstraints c = new GridBagConstraints();
            c.gridy = row;
            c.anchor = GridBagConstraints.WEST;
            c.insets = new Insets(0, 0, 8, 12);
            details.add(muted(new JLabel(label)), c);
            c.gridx = 1;
            c.weightx = 1;
            c.fill = GridBagConstraints.HORIZONTAL;
            c.insets = new Insets(0, 0, 8, 0);
            details.add(value, c);
        }

        private JTextArea textArea() {
            JTextArea area = new JTextArea();
            area.setLineWrap(true);
            area.setWrapStyleWord(true);
            area.setEditable(false);
            area.setFocusable(false);
            area.setOpaque(false);
            area.setBorder(BorderFactory.createEmptyBorder());
            area.setFont(UIManager.getFont("Label.font"));
            return area;
        }

        PinoutPin pin() {
            return shown;
        }

        void show(PinoutPin pin) {
            shown = pin;
            empty.setVisible(pin == null);
            for (Component child : getComponents()) {
                if (child != empty) {
                    child.setVisible(pin != null);
                }
            }
            if (pin == null) {
                revalidate();
                repaint();
                return;
            }
            name.setText("Pin " + pin.name);
            name.setIcon(new MarkerIcon(PinoutDrawing.fills(pin, mode), PinoutDrawing.dashed(pin, mode), 44,
                    pin.name));
            where.setText(title);
            function.setText(pin.function.isEmpty() ? "No function listed" : pin.function);
            String[] assignment = pin.use() == PinoutPin.Use.USED ? assignmentOf.apply(pin) : null;
            switch (pin.use()) {
                case USED:
                    useValue.setText(pin.tuneUse);
                    useValue.setForeground(PinoutDrawing.themed("Console.Pinout.used", new Color(0x2563eb)));
                    useHint.setText(assignment != null ? "Set in Tuning; change it there." : "Set in the tune.");
                    break;
                case FREE:
                    useValue.setText("Free");
                    useValue.setForeground(UIManager.getColor("Label.foreground"));
                    useHint.setText("Nothing in the tune uses this pin. In Tuning, choose \"" + pin.tsName
                            + "\" in a pin setting to use it.");
                    break;
                default:
                    useValue.setText("Fixed function");
                    useValue.setForeground(UIManager.getColor("Label.foreground"));
                    useHint.setText("Power or ground: wired, not set in the tune.");
            }
            openButton.setVisible(assignment != null);
            PinColors.PinType pinType = pin.type.type;
            type.setText(pinType.label);
            type.setIcon(new MarkerIcon(new Color[]{pinType.color()}, pinType == PinColors.PinType.UNTYPED, 10, null));
            typeNote.setText(pin.type.isInferred() ? "from its " + pin.type.inferredFrom
                    : pinType == PinColors.PinType.UNTYPED ? "the board file gives none" : "");
            wire.setText(pin.wire.isEmpty() ? "not listed" : pin.wire);
            wire.setIcon(new WireIcon(pin.wire));
            tsName.setText(pin.tsName.isEmpty() ? "—" : pin.tsName);
            classes.setText(pin.classes.isEmpty() ? "—" : pin.classes.replace('_', ' '));
            revalidate();
            repaint();
        }
    }
}
