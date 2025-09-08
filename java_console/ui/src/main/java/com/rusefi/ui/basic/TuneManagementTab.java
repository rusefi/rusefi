package com.rusefi.ui.basic;

import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.tune_manifest.TuneManifestHelper;
import com.rusefi.tune_manifest.TuneModel;
import com.rusefi.ui.table.ButtonEditor;
import com.rusefi.ui.table.ButtonRenderer;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;
import java.awt.*;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class TuneManagementTab {
    private static final int BUTTON_COLUMN = 3;
    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel status = new JLabel("Downloading tunes...");
    private final JTable table = new JTable(new MyTableModel());
    private List<TuneModel> tunes = new ArrayList<>();

    public TuneManagementTab(Component importTuneButton, SingleAsyncJobExecutor singleAsyncJobExecutor) {
//        content.setBorder(BorderFactory.createLineBorder(Color.GREEN));

        String tunesManifestUrl = getTunesManifestUrl();
        if (tunesManifestUrl != null) {
            content.add(status, BorderLayout.NORTH);
            content.add(table, BorderLayout.CENTER);

//            table.setBorder(BorderFactory.createLineBorder(Color.BLUE));

            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        TuneManifestHelper.downloadAllTunes(tunesManifestUrl, new TuneManifestHelper.Callback() {
                            @Override
                            public void onDownloaded(List<TuneModel> tunes) {
                                SwingUtilities.invokeLater(() -> displayTunes(tunes));
                            }
                        });
                    } catch (IOException | ParseException e) {
                        SwingUtilities.invokeLater(() -> status.setText("Error " + e));
                    }
                }
            }).start();
        }
        table.getColumnModel().getColumn(0).setPreferredWidth(450);
        table.getColumnModel().getColumn(1).setPreferredWidth(10);

        TableColumn buttonColumn = table.getColumnModel().getColumn(2);
        buttonColumn.setPreferredWidth(100);

        buttonColumn.setCellRenderer(new ButtonRenderer());
        buttonColumn.setCellEditor(new ButtonEditor(new JButton(), "Import", table, new ButtonEditor.Clicked() {
            @Override
            public void clicked(int row) {
                TuneModel model = tunes.get(row);
                // Fire a message box with information about the clicked button
                JOptionPane.showMessageDialog(table, "Button clicked at Row " + model.getSaferLocalFileName());
            }
        }));

        content.add(importTuneButton, BorderLayout.SOUTH);
    }

    private void displayTunes(List<TuneModel> tunes) {
        this.tunes = tunes;
        status.setText(tunes.size() + " tunes downloaded!");
        AutoupdateUtil.trueLayoutAndRepaint(table);
    }

    public Component getContent() {
        return content;
    }

    public static String getTunesManifestUrl() {
        return PropertiesHolder.getProperty("tunes_manifest");
    }

    class MyTableModel extends AbstractTableModel {
        private final String[] columnNames = {"Notes", "Second", "3rd"};

        @Override
        public int getColumnCount() {
            return columnNames.length;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            if (rowIndex == 0)
                return columnNames[columnIndex];
            if (columnIndex == 0)
                return tunes.get(rowIndex - 1).getNotes();
            if (columnIndex == BUTTON_COLUMN - 1)
                return "Import Tune";
            return rowIndex + " " + columnIndex;
        }

        @Override
        public int getRowCount() {
            return tunes.size() + 1 /*extra row for header */;
        }

        @Override
        public boolean isCellEditable(int row, int column) {
            if (row == 0)
                return false;
            return column == BUTTON_COLUMN - 1;
        }

        @Override
        public Class<?> getColumnClass(int column) {
            if (column == BUTTON_COLUMN) {
                return JButton.class;
            }
            return Object.class;
        }

    }
}
