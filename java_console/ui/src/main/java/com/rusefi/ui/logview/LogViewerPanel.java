package com.rusefi.ui.logview;

import com.rusefi.logview.CsvLogReader;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JCheckBox;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.ListCellRenderer;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Locale;

/** Select fields on the left and compare their time series on the right. */
public final class LogViewerPanel extends JPanel {
    private final CsvLogReader log;
    private final boolean[] displayed;
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
            return log.name(field) + (unit.isEmpty() ? "" : " (" + unit + ")");
        }
    };
    private final LogChart chart;

    public LogViewerPanel(CsvLogReader log) {
        super(new BorderLayout());
        this.log = log;
        this.displayed = new boolean[log.fieldCount()];
        for (int field = 1; field < log.fieldCount(); field++) {
            String name = log.name(field);
            displayed[field] = name.equalsIgnoreCase("RPM") || name.equalsIgnoreCase("MAP")
                    || name.equalsIgnoreCase("TPS");
        }
        chart = new LogChart(log, displayed);

        JPanel fields = new JPanel(new BorderLayout(0, 8));
        fields.setBorder(BorderFactory.createEmptyBorder(12, 10, 10, 10));
        JLabel title = new JLabel("Fields  ·  " + (log.fieldCount() - 1));
        JTextField search = new JTextField();
        search.putClientProperty("JTextField.placeholderText", "Filter fields");
        JPanel heading = new JPanel(new BorderLayout(0, 8));
        heading.add(title, BorderLayout.NORTH);
        heading.add(search, BorderLayout.SOUTH);
        fields.add(heading, BorderLayout.NORTH);

        fieldList.setCellRenderer(new FieldRenderer());
        fieldList.setFixedCellHeight(27);
        fieldList.setToolTipText("");
        fieldList.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent event) {
                int index = fieldList.locationToIndex(event.getPoint());
                if (index < 0 || !fieldList.getCellBounds(index, index).contains(event.getPoint())) {
                    return;
                }
                int field = visibleFields.get(index);
                displayed[field] = !displayed[field];
                fieldList.repaint(fieldList.getCellBounds(index, index));
                chart.repaint();
            }
        });
        search.getDocument().addDocumentListener(new DocumentListener() {
            @Override public void insertUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void removeUpdate(DocumentEvent event) { filter(search.getText()); }
            @Override public void changedUpdate(DocumentEvent event) { filter(search.getText()); }
        });
        filter("");
        JScrollPane fieldScroll = new JScrollPane(fieldList);
        fieldScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        fields.add(fieldScroll, BorderLayout.CENTER);
        fields.add(new JLabel(log.sampleCount() + " samples"), BorderLayout.SOUTH);

        JSplitPane split = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, fields, chart);
        split.setResizeWeight(0.15);
        split.setContinuousLayout(true);
        split.setDividerSize(5);
        add(split, BorderLayout.CENTER);
        setPreferredSize(new Dimension(1400, 840));
        SwingUtilities.invokeLater(() -> split.setDividerLocation(0.15));
    }

    private void filter(String query) {
        String needle = query.trim().toLowerCase(Locale.ROOT);
        visibleFields.clear();
        for (int field = 1; field < log.fieldCount(); field++) {
            if (displayed[field] && log.name(field).toLowerCase(Locale.ROOT).contains(needle)) {
                visibleFields.addElement(field);
            }
        }
        for (int field = 1; field < log.fieldCount(); field++) {
            if (!displayed[field] && log.name(field).toLowerCase(Locale.ROOT).contains(needle)) {
                visibleFields.addElement(field);
            }
        }
    }

    private final class FieldRenderer extends JCheckBox implements ListCellRenderer<Integer> {
        @Override
        public java.awt.Component getListCellRendererComponent(JList<? extends Integer> list, Integer field,
                                                                  int index, boolean selected, boolean focused) {
            String unit = log.unit(field);
            setText(log.name(field) + (unit.isEmpty() ? "" : "  ·  " + unit));
            setSelected(displayed[field]);
            setFont(list.getFont());
            setOpaque(true);
            setBackground(selected ? list.getSelectionBackground() : list.getBackground());
            setForeground(selected ? list.getSelectionForeground() : list.getForeground());
            return this;
        }
    }
}
