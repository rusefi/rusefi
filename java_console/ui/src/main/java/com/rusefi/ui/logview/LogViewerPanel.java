package com.rusefi.ui.logview;

import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.logview.CsvLogReader;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.ListCellRenderer;
import javax.swing.SwingConstants;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Locale;

import static com.rusefi.ui.logview.ThemedStrip.themed;

/**
 * Log viewer: a searchable signal list on the left (click or Space plots a signal; a dot shows its trace
 * colour), the chart with its toolbar on the right, and a status strip below. [tag:console_theme]
 */
public final class LogViewerPanel extends JPanel {
    private static final int SIDEBAR_WIDTH = 280;

    private final CsvLogReader log;
    private final TraceColors colors;
    private final ChartSettings settings = new ChartSettings();
    private final DefaultListModel<Integer> visibleFields = new DefaultListModel<>();
    private final JList<Integer> fieldList = new JList<Integer>(visibleFields) {
        @Override
        public String getToolTipText(MouseEvent event) {
            int index = locationToIndex(event.getPoint());
            if (index < 0 || !getCellBounds(index, index).contains(event.getPoint())) {
                return null;
            }
            int field = visibleFields.get(index);
            String unit = log.unit(field);
            String name = log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")");
            return name + (colors.isPlotted(field) ? " · click to remove" : " · click to plot");
        }

        // Always fit the sidebar: a long name is shortened with "…" instead of pushing units under the scrollbar.
        @Override
        public boolean getScrollableTracksViewportWidth() {
            return true;
        }
    };
    private final JTextField search = new JTextField();
    private final JLabel plottedLabel = ChartToolbar.muted(new JLabel());
    private final LogChart chart;
    private final LogStatusBar statusBar;

    public LogViewerPanel(CsvLogReader log) {
        this(log, null);
    }

    public LogViewerPanel(CsvLogReader log, String fileName) {
        super(new BorderLayout());
        this.log = log;
        boolean[] initial = new boolean[log.fieldCount()];
        for (int field = 1; field < log.fieldCount(); field++) {
            String name = log.name(field);
            initial[field] = name.equalsIgnoreCase("RPM") || name.equalsIgnoreCase("MAP")
                    || name.equalsIgnoreCase("TPS");
        }
        colors = TraceColors.plotted(log.fieldCount(), initial);
        chart = new LogChart(log, colors, settings);
        statusBar = new LogStatusBar(fileName, log);
        chart.setViewListener(() -> statusBar.update(chart.timeWindow(), chart.cursorTime()));

        JPanel plotArea = new JPanel(new BorderLayout());
        plotArea.add(new ChartToolbar(settings, () -> {
            chart.repaint();
            // the list dots use the plot palette, which follows the Light / Dark switch
            fieldList.repaint();
        }), BorderLayout.NORTH);
        plotArea.add(chart, BorderLayout.CENTER);

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, createSidebar(), plotArea);
        split.setResizeWeight(0);
        split.setContinuousLayout(true);
        split.setDividerSize(5);
        split.setDividerLocation(SIDEBAR_WIDTH);
        split.setBorder(BorderFactory.createEmptyBorder());
        add(split, BorderLayout.CENTER);
        add(statusBar, BorderLayout.SOUTH);
        setPreferredSize(new Dimension(1400, 840));
        statusBar.update(chart.timeWindow(), chart.cursorTime());
        updatePlottedLabel();
    }

    private JComponent createSidebar() {
        JPanel sidebar = new JPanel(new BorderLayout(0, 10));
        sidebar.setBackground(themed("Console.LogViewer.sidebar.background", Color.WHITE));
        sidebar.setBorder(BorderFactory.createEmptyBorder(12, 12, 0, 8));

        JLabel title = new JLabel("Signals");
        title.setFont(title.getFont().deriveFont(Font.BOLD));
        JPanel titleRow = new JPanel(new BorderLayout(8, 0));
        titleRow.setOpaque(false);
        JPanel titleLeft = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
        titleLeft.setOpaque(false);
        titleLeft.add(title);
        titleLeft.add(Box.createHorizontalStrut(8));
        titleLeft.add(new Chip(String.format(Locale.ROOT, "%,d", log.fieldCount() - 1)));
        titleRow.add(titleLeft, BorderLayout.WEST);
        titleRow.add(plottedLabel, BorderLayout.EAST);

        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Search signals");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void removeUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void changedUpdate(DocumentEvent event) { filter(search.getText()); }
        });

        JPanel heading = new JPanel(new BorderLayout(0, 10));
        heading.setOpaque(false);
        heading.add(titleRow, BorderLayout.NORTH);
        heading.add(search, BorderLayout.SOUTH);
        sidebar.add(heading, BorderLayout.NORTH);

        fieldList.setCellRenderer(new SignalRenderer());
        fieldList.setFixedCellHeight(28);
        fieldList.setToolTipText("");
        fieldList.setBackground(sidebar.getBackground());
        fieldList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                int index = fieldList.locationToIndex(event.getPoint());
                if (index >= 0 && fieldList.getCellBounds(index, index).contains(event.getPoint())) {
                    toggle(visibleFields.get(index));
                }
            }
        });
        fieldList.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, 0), "togglePlot");
        fieldList.getActionMap().put("togglePlot", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent event) {
                Integer field = fieldList.getSelectedValue();
                if (field != null) {
                    toggle(field);
                }
            }
        });
        filter("");
        JScrollPane fieldScroll = new JScrollPane(fieldList);
        fieldScroll.setBorder(BorderFactory.createEmptyBorder());
        fieldScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        sidebar.add(fieldScroll, BorderLayout.CENTER);
        return sidebar;
    }

    JList<Integer> fieldList() {
        return fieldList;
    }

    JTextField searchField() {
        return search;
    }

    LogChart chart() {
        return chart;
    }

    LogStatusBar statusBar() {
        return statusBar;
    }

    JLabel plottedLabel() {
        return plottedLabel;
    }

    void toggle(int field) {
        colors.toggle(field);
        int index = visibleFields.indexOf(field);
        if (index >= 0) {
            fieldList.repaint(fieldList.getCellBounds(index, index));
        }
        updatePlottedLabel();
        chart.repaint();
    }

    private void updatePlottedLabel() {
        int count = colors.plottedCount();
        plottedLabel.setText(count == 0 ? "none plotted" : count + " plotted");
    }

    /** Plotted signals first, then the rest, both in log order. Time (field 0) is the x axis, not a signal. */
    private void filter(String query) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        visibleFields.clear();
        for (int field = 1; field < log.fieldCount(); field++) {
            if (colors.isPlotted(field) && log.name(field).toLowerCase(Locale.ROOT).contains(needle)) {
                visibleFields.addElement(field);
            }
        }
        for (int field = 1; field < log.fieldCount(); field++) {
            if (!colors.isPlotted(field) && log.name(field).toLowerCase(Locale.ROOT).contains(needle)) {
                visibleFields.addElement(field);
            }
        }
    }

    /** Small rounded count badge, like the Gauges RPM readout. */
    private static final class Chip extends JLabel {
        Chip(String text) {
            super(text);
            setBorder(BorderFactory.createEmptyBorder(1, 7, 1, 7));
            setFont(getFont().deriveFont(getFont().getSize2D() - 1));
            setForeground(themed("Console.LogViewer.mutedForeground", Color.GRAY));
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                g2.setColor(themed("Console.LogViewer.chipBackground", new Color(0xeef0f3)));
                g2.fillRoundRect(0, 0, getWidth(), getHeight(), getHeight(), getHeight());
            } finally {
                g2.dispose();
            }
            super.paintComponent(g);
        }
    }

    /** A filled dot in the trace colour for plotted signals, an empty ring for the rest. */
    private static final class TraceDot implements Icon {
        private static final int SIZE = 10;
        private Color fill;
        private Color ring;

        void set(Color fill, Color ring) {
            this.fill = fill;
            this.ring = ring;
        }

        @Override
        public void paintIcon(Component c, Graphics g, int x, int y) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
                if (fill != null) {
                    g2.setColor(fill);
                    g2.fillOval(x, y, SIZE, SIZE);
                }
                g2.setColor(ring);
                g2.setStroke(new BasicStroke(1.2f));
                g2.drawOval(x, y, SIZE - 1, SIZE - 1);
            } finally {
                g2.dispose();
            }
        }

        @Override
        public int getIconWidth() {
            return SIZE;
        }

        @Override
        public int getIconHeight() {
            return SIZE;
        }
    }

    private final class SignalRenderer extends JPanel implements ListCellRenderer<Integer> {
        private final TraceDot dot = new TraceDot();
        private final JLabel name = new JLabel(dot, SwingConstants.LEADING);
        private final JLabel unit = new JLabel();

        SignalRenderer() {
            super(new BorderLayout(8, 0));
            setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 8));
            name.setIconTextGap(10);
            add(name, BorderLayout.CENTER);
            add(unit, BorderLayout.EAST);
        }

        // Like DefaultListCellRenderer: lay out whenever the list sizes the row (validate() is a no-op
        // without a native peer, e.g. when painting offscreen) and skip revalidation per row.
        @Override
        public void setBounds(int x, int y, int width, int height) {
            super.setBounds(x, y, width, height);
            doLayout();
        }

        @Override
        public void invalidate() {
        }

        @Override
        public void validate() {
        }

        @Override
        public void revalidate() {
        }

        @Override
        public Component getListCellRendererComponent(JList<? extends Integer> list, Integer field,
                                                      int index, boolean selected, boolean focused) {
            Color muted = themed("Console.LogViewer.mutedForeground", Color.GRAY);
            boolean plotted = colors.isPlotted(field);
            if (plotted) {
                Color trace = chart.palette().trace(colors.slot(field));
                // a darker outline keeps pale dark-plot colours visible on the light list
                dot.set(trace, trace.darker());
            } else {
                dot.set(null, new Color(muted.getRed(), muted.getGreen(), muted.getBlue(), 110));
            }
            name.setText(log.name(field));
            name.setFont(list.getFont().deriveFont(plotted ? Font.BOLD : Font.PLAIN));
            name.setForeground(list.getForeground());
            unit.setText(log.unit(field));
            unit.setFont(list.getFont());
            unit.setForeground(muted);
            // a soft highlight for the keyboard row instead of the strong default selection colour
            setBackground(selected ? themed("Console.LogViewer.chipBackground", list.getSelectionBackground())
                    : list.getBackground());
            return this;
        }
    }
}
