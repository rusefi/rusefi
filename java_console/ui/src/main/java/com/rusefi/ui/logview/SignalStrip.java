package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.rusefi.logview.CsvLogReader;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.widgets.JoinedButtons;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JToggleButton;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.function.BiFunction;
import java.util.function.IntUnaryOperator;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * The strip above the Histogram and Scatter views: the X / Y / Z signals (a click on a signal in the sidebar puts it
 * on the highlighted axis, or drag one onto an axis), a warning when Z looks saturated, and the filters as pills that
 * switch on and off. Each view has its own, with its own remembered signals and filters. Only the axes the user sets
 * are remembered; the rest are picked again for each log by the view's rule. [tag:console_theme]
 */
final class SignalStrip extends JPanel {
    static final char[] AXES = {'x', 'y', 'z'};

    private final CsvLogReader log;
    private final HistogramSettings settings;
    /** The view's pick for an axis (0, 1, 2) the user did not set, or -1 for the generic fallback. */
    private final IntUnaryOperator automaticPick;
    /** After any change of signals or filters: the view recomputes. */
    private final Runnable onChange;
    /** After a change of signals: e.g. the sidebar marks them. */
    private final Runnable onSignalsChanged;

    final AxisSlot[] slots = new AxisSlot[3];
    final JButton swapButton = new JButton("Swap X ↔ Y");
    /** The view's notes on the axes, e.g. a missing lambda or bins that do not match. */
    final JLabel zNoticeLabel = noticeLabel();
    final JLabel noticeLabel = noticeLabel();
    final JPanel banner = new Banner();
    final JLabel bannerText = new JLabel();
    final JLabel bannerDetail = ChartToolbar.muted(new JLabel());
    final JButton excludeButton = fullLabel("Exclude them");
    final JButton keepButton = fullLabel("Keep");
    final JPanel filterRow = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
    final JButton addFilterButton = new JButton("+ Add filter");
    /** Edits a filter: dialog title and the filter to start from (null for a new one); null when cancelled. */
    BiFunction<String, HistogramFilter, HistogramFilter> filterPrompt = this::promptFilter;

    private final int[] fields = new int[3];
    /** Per axis: picked by the view's rule rather than by the user, so not remembered. */
    private final boolean[] automatic = new boolean[3];
    private char activeAxis = 'z';
    private final List<HistogramFilter> filters;
    /** Z signals whose saturation warning was answered with Keep, for this log. */
    private final Set<String> keptSaturation = new HashSet<>();
    private String saturationSignal;
    private HistogramSignals.Saturation saturation;

    /**
     * @param purpose e.g. "histogram", for tooltips
     */
    SignalStrip(CsvLogReader log, HistogramSettings settings, IntUnaryOperator automaticPick, String purpose,
                Runnable onChange, Runnable onSignalsChanged) {
        this.log = log;
        this.settings = settings;
        this.automaticPick = automaticPick;
        this.onChange = onChange;
        this.onSignalsChanged = onSignalsChanged;
        filters = settings.filters();
        pickSignals();
        build(purpose);
    }

    // ---- signals ----

    /** The signals the user picked where this log has them, otherwise the view's picks or the first other signals. */
    void pickSignals() {
        for (int i = 0; i < 3; i++) {
            int field = HistogramSignals.byName(log, settings.signal(AXES[i]));
            automatic[i] = field < 0;
            if (field < 0) {
                field = automaticPick.applyAsInt(i);
            }
            for (int j = 0; j < i; j++) {
                if (fields[j] == field) {
                    field = -1;
                }
            }
            fields[i] = field >= 0 ? field : HistogramSignals.fallbackExcept(log,
                    i == 2 ? HistogramSignals.Z_EXCLUDED : null, Arrays.copyOf(fields, i));
        }
    }

    int field(char axis) {
        return fields[axisIndex(axis)];
    }

    /** The axis (0, 1, 2) was picked by the view's rule, not by the user. */
    boolean isAutomatic(int axis) {
        return automatic[axis];
    }

    char activeAxis() {
        return activeAxis;
    }

    /** The axis a field is on ('x', 'y', 'z'), or 0. */
    char axisOf(int field) {
        for (int i = 0; i < 3; i++) {
            if (fields[i] == field) {
                return AXES[i];
            }
        }
        return 0;
    }

    static int axisIndex(char axis) {
        return axis == 'x' ? 0 : axis == 'y' ? 1 : 2;
    }

    void setActiveAxis(char axis) {
        activeAxis = axis;
        slots[axisIndex(axis)].setSelected(true);
    }

    /** A signal clicked in the sidebar goes on the highlighted axis. */
    void assignToActiveAxis(int field) {
        assign(activeAxis, field);
    }

    /** Puts a signal on an axis; a signal already on another axis trades places with it. */
    void assign(char axis, int field) {
        int index = axisIndex(axis);
        if (fields[index] == field) {
            return;
        }
        char other = axisOf(field);
        if (other != 0) {
            // trading places is the user's choice for both axes
            int otherIndex = axisIndex(other);
            fields[otherIndex] = fields[index];
            automatic[otherIndex] = false;
            settings.setSignal(other, log.name(fields[otherIndex]));
        }
        fields[index] = field;
        automatic[index] = false;
        settings.setSignal(axis, log.name(field));
        onChange.run();
        onSignalsChanged.run();
    }

    /** Puts several signals on their axes at once, e.g. from a recipe, as the user's choice. */
    void assignAll(int x, int y, int z) {
        int[] wanted = {x, y, z};
        for (int i = 0; i < 3; i++) {
            fields[i] = wanted[i];
            automatic[i] = false;
            settings.setSignal(AXES[i], log.name(wanted[i]));
        }
        onChange.run();
        onSignalsChanged.run();
    }

    void swapXY() {
        assign('x', fields[1]);
    }

    // ---- filters ----

    List<HistogramFilter> filters() {
        return new ArrayList<>(filters);
    }

    void addFilter(HistogramFilter filter) {
        filters.add(filter);
        filtersChanged();
    }

    void replaceFilter(int index, HistogramFilter filter) {
        filters.set(index, filter);
        filtersChanged();
    }

    void removeFilter(int index) {
        filters.remove(index);
        filtersChanged();
    }

    void toggleFilter(int index) {
        filters.set(index, filters.get(index).withEnabled(!filters.get(index).enabled));
        filtersChanged();
    }

    private void filtersChanged() {
        settings.setFilters(filters);
        onChange.run();
    }

    private HistogramFilter promptFilter(String title, HistogramFilter initial) {
        HistogramFilterEditor editor = new HistogramFilterEditor(log, initial, field('x'), field('y'), field('z'));
        // typing goes straight to the signal search
        editor.search.addAncestorListener(new AncestorListener() {
            @Override
            public void ancestorAdded(AncestorEvent event) {
                editor.search.requestFocusInWindow();
            }

            @Override
            public void ancestorRemoved(AncestorEvent event) {
            }

            @Override
            public void ancestorMoved(AncestorEvent event) {
            }
        });
        while (JOptionPane.showConfirmDialog(this, editor, title, JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE) == JOptionPane.OK_OPTION) {
            HistogramFilter filter = editor.filter();
            if (filter != null) {
                return filter;
            }
            JOptionPane.showMessageDialog(this, "Pick a signal and enter a number.", title,
                    JOptionPane.WARNING_MESSAGE);
        }
        return null;
    }

    void newFilter() {
        HistogramFilter filter = filterPrompt.apply("Add filter", null);
        if (filter != null) {
            addFilter(filter);
        }
    }

    void editFilter(int index) {
        HistogramFilter old = filters.get(index);
        if (old.condition.isBox()) {
            // a box is drawn on the scatter plot; its pill only switches on and off, or is removed
            return;
        }
        HistogramFilter filter = filterPrompt.apply("Edit filter", old);
        if (filter != null) {
            replaceFilter(index, filter.withEnabled(old.enabled));
        }
    }

    /** The saturation warning is shown: Z has a pile of readings at a limit and no filter for it yet. */
    boolean isSaturationWarningShown() {
        return banner.isVisible();
    }

    void excludeSaturated() {
        if (saturation != null) {
            addFilter(saturation.filter(saturationSignal));
        }
    }

    void keepSaturated() {
        keptSaturation.add(saturationSignal);
        updateBanner();
    }

    // ---- showing ----

    /** Brings the axis buttons, the filter pills and the warning up to date; the view calls it as it recomputes. */
    void refresh() {
        for (int i = 0; i < 3; i++) {
            slots[i].setSignal(fields[i]);
        }
        rebuildFilterRow();
        updateBanner();
    }

    /** The view's note on Z (null hides it), e.g. that the log has no measured lambda. */
    void setZNotice(String text) {
        show(zNoticeLabel, text);
    }

    /** The view's other note on the axes (null hides it). */
    void setNotice(String text) {
        show(noticeLabel, text);
    }

    private static void show(JLabel label, String text) {
        label.setText(text == null ? "" : text);
        label.setVisible(text != null);
    }

    private void updateBanner() {
        String zName = log.name(field('z'));
        if (!zName.equals(saturationSignal)) {
            saturationSignal = zName;
            saturation = HistogramSignals.saturation(log, field('z'));
        }
        boolean show = saturation != null && !keptSaturation.contains(zName)
                && filters.stream().noneMatch(f -> f.signal.equals(zName) && f.condition == saturation.filter(zName).condition
                && f.value == saturation.value);
        if (show) {
            bannerText.setText(String.format(Locale.ROOT, "%s reads exactly %s in %,d records (%.1f%%)", zName,
                    ValueFormat.format(log, field('z'), saturation.value), saturation.records, saturation.share * 100));
            bannerDetail.setText(String.format(Locale.ROOT, "That looks like the sensor's %s limit, not real readings",
                    saturation.low ? "lower" : "upper"));
            // a narrow window shortens the text, never the buttons: the whole message stays in the tooltip
            banner.setToolTipText(bannerText.getText() + ". " + bannerDetail.getText() + ".");
            bannerText.setToolTipText(banner.getToolTipText());
            bannerDetail.setToolTipText(banner.getToolTipText());
        }
        banner.setVisible(show);
    }

    private void build(String purpose) {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        setOpaque(false);
        setBorder(BorderFactory.createEmptyBorder(10, 12, 4, 12));

        JPanel axes = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
        axes.setOpaque(false);
        ButtonGroup group = new ButtonGroup();
        for (int i = 0; i < 3; i++) {
            char axis = AXES[i];
            AxisSlot slot = new AxisSlot(axis);
            slot.addActionListener(e -> activeAxis = axis);
            group.add(slot);
            slots[i] = slot;
            axes.add(new JoinedButtons(slot));
        }
        slots[2].setSelected(true);
        swapButton.setToolTipText("Swap the X and Y signals");
        swapButton.addActionListener(e -> swapXY());
        axes.add(new JoinedButtons(swapButton));
        axes.add(Box.createHorizontalStrut(6));
        axes.add(zNoticeLabel);
        axes.add(noticeLabel);
        axes.add(ChartToolbar.muted(new JLabel("Click a signal in the list to put it on the highlighted axis, "
                + "or drag it onto an axis")));

        // the message on two short lines that may shorten; the buttons always keep their full labels
        banner.setLayout(new GridBagLayout());
        banner.setBorder(BorderFactory.createEmptyBorder(8, 12, 8, 8));
        banner.setVisible(false);
        bannerText.setForeground(themed("Label.foreground", Color.BLACK));
        bannerText.setFont(bannerText.getFont().deriveFont(Font.BOLD));
        JPanel message = new JPanel(new GridLayout(0, 1, 0, 2)) {
            @Override
            public Dimension getMinimumSize() {
                return new Dimension(0, getPreferredSize().height);
            }
        };
        message.setOpaque(false);
        message.add(bannerText);
        message.add(bannerDetail);
        excludeButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        excludeButton.setToolTipText("Add a filter that leaves these readings out");
        excludeButton.addActionListener(e -> excludeSaturated());
        keepButton.setToolTipText("Keep them and hide this warning");
        keepButton.addActionListener(e -> keepSaturated());
        GridBagConstraints text = new GridBagConstraints();
        text.weightx = 1;
        text.fill = GridBagConstraints.HORIZONTAL;
        text.anchor = GridBagConstraints.WEST;
        banner.add(message, text);
        GridBagConstraints button = new GridBagConstraints();
        button.insets = new Insets(0, 12, 0, 0);
        banner.add(excludeButton, button);
        button.insets = new Insets(0, 6, 0, 0);
        banner.add(keepButton, button);

        filterRow.setOpaque(false);
        addFilterButton.setToolTipText("Leave records out of the " + purpose
                + " by a signal's value or rate of change");
        addFilterButton.addActionListener(e -> newFilter());

        for (JComponent row : new JComponent[]{axes, banner, filterRow}) {
            row.setAlignmentX(Component.LEFT_ALIGNMENT);
            add(row);
            add(Box.createVerticalStrut(8));
        }
    }

    private static JLabel noticeLabel() {
        JLabel label = new JLabel();
        label.setForeground(themed("Console.Banner.warning.accent", new Color(0xb7791f)));
        label.setFont(label.getFont().deriveFont(Font.BOLD));
        label.setVisible(false);
        return label;
    }

    private void rebuildFilterRow() {
        filterRow.removeAll();
        JLabel title = ChartToolbar.muted(new JLabel("FILTERS"));
        title.setFont(title.getFont().deriveFont(Font.BOLD, title.getFont().getSize2D() - 1));
        filterRow.add(title);
        for (int i = 0; i < filters.size(); i++) {
            filterRow.add(new FilterChip(i, filters.get(i)));
        }
        filterRow.add(new JoinedButtons(addFilterButton));
        filterRow.revalidate();
        filterRow.repaint();
    }

    /** One axis: its letter and signal; selected when a sidebar click goes to it; takes dropped signals. */
    final class AxisSlot extends JToggleButton {
        final char axis;

        AxisSlot(char axis) {
            this.axis = axis;
            putClientProperty(FlatClientProperties.STYLE_CLASS, "viewToggle");
            setTransferHandler(new SignalTransfer.Handler() {
                @Override
                boolean canImportSignal(TransferSupport support) {
                    return true;
                }

                @Override
                boolean importSignal(TransferSupport support, int field) {
                    assign(axis, field);
                    setActiveAxis(axis);
                    return true;
                }
            });
        }

        void setSignal(int field) {
            String unit = log.unit(field);
            setText("<html><b>" + Character.toUpperCase(axis) + "</b>&nbsp;&nbsp;" + escape(log.name(field)) + "</html>");
            setToolTipText(log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")") + " on "
                    + Character.toUpperCase(axis) + " · highlighted: a click on a signal in the list puts it here; "
                    + "or drag a signal onto it");
        }
    }

    /** A button no layout shrinks below its preferred size, so its label is never cut to "…". */
    static JButton fullLabel(String text) {
        return new JButton(text) {
            @Override
            public Dimension getMinimumSize() {
                return getPreferredSize();
            }
        };
    }

    static String escape(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** A filter as a pill: click switches it on or off, right-click edits or removes it. */
    final class FilterChip extends JToggleButton {
        final int index;

        FilterChip(int index, HistogramFilter filter) {
            this.index = index;
            int hidden = filter.field(log) < 0 ? 0 : filter.count(log);
            String hold = filter.holdLabel();
            setText(filter.label() + (hold.isEmpty() ? "" : "  " + hold)
                    + (filter.field(log) < 0 ? "  · not in this log" : String.format(Locale.ROOT, "  −%,d", hidden)));
            setSelected(filter.enabled);
            setForeground(filter.enabled ? themed("Console.Banner.info.accent", new Color(0x1d4ed8))
                    : themed("Console.LogViewer.mutedForeground", Color.GRAY));
            setContentAreaFilled(false);
            setBorderPainted(false);
            setFocusPainted(false);
            setBorder(BorderFactory.createEmptyBorder(5, 24, 5, 12));
            setToolTipText("<html>" + escape(filter.toString()) + String.format(Locale.ROOT,
                    "<br>Hides %,d records on its own · click to switch %s · right-click to edit or remove</html>",
                    hidden, filter.enabled ? "off" : "on"));
            addActionListener(e -> toggleFilter(index));
            addMouseListener(new MouseAdapter() {
                @Override
                public void mousePressed(MouseEvent event) {
                    menu(event);
                }

                @Override
                public void mouseReleased(MouseEvent event) {
                    menu(event);
                }

                private void menu(MouseEvent event) {
                    if (event.isPopupTrigger()) {
                        chipMenu(index).show(FilterChip.this, event.getX(), event.getY());
                    }
                }
            });
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension size = super.getPreferredSize();
            size.height = JoinedButtons.HEIGHT - 6;
            return size;
        }

        @Override
        protected void paintComponent(Graphics graphics) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                boolean on = isSelected();
                int arc = getHeight();
                g.setColor(on ? themed("Console.Banner.info.background", new Color(0xeef4ff))
                        : themed("Console.Card.background", Color.WHITE));
                g.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
                g.setColor(on ? themed("Console.Banner.info.borderColor", new Color(0xc9dafb))
                        : themed("Separator.foreground", Color.LIGHT_GRAY));
                g.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, arc, arc);
                Color accent = themed("Console.Banner.info.accent", new Color(0x1d4ed8));
                Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
                int d = 8;
                int cx = 11;
                int cy = (getHeight() - d) / 2;
                if (on) {
                    g.setColor(accent);
                    g.fillOval(cx, cy, d, d);
                } else {
                    g.setColor(muted);
                    g.setStroke(new BasicStroke(1.5f));
                    g.drawOval(cx, cy, d, d);
                }
            } finally {
                g.dispose();
            }
            super.paintComponent(graphics);
        }
    }

    JPopupMenu chipMenu(int index) {
        JPopupMenu menu = new JPopupMenu();
        JMenuItem toggle = new JMenuItem(filters.get(index).enabled ? "Switch off" : "Switch on");
        toggle.addActionListener(e -> toggleFilter(index));
        menu.add(toggle);
        JMenuItem edit = new JMenuItem("Edit…");
        edit.setEnabled(!filters.get(index).condition.isBox());
        edit.addActionListener(e -> editFilter(index));
        menu.add(edit);
        JMenuItem remove = new JMenuItem("Remove");
        remove.addActionListener(e -> removeFilter(index));
        menu.add(remove);
        return menu;
    }

    /** The warning strip's rounded background and border, in the theme's warning banner colours. */
    private static final class Banner extends JPanel {
        Banner() {
            setOpaque(false);
        }

        @Override
        public Dimension getMaximumSize() {
            return new Dimension(Integer.MAX_VALUE, getPreferredSize().height);
        }

        @Override
        protected void paintComponent(Graphics graphics) {
            Graphics2D g = (Graphics2D) graphics.create();
            try {
                g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g.setColor(themed("Console.Banner.warning.background", new Color(0xfff8e6)));
                g.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 10, 10);
                g.setColor(themed("Console.Banner.warning.borderColor", new Color(0xf3dca7)));
                g.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 10, 10);
            } finally {
                g.dispose();
            }
        }
    }
}
