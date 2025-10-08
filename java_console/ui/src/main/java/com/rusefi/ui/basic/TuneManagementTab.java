package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.core.ui.ErrorMessageHelper;
import com.rusefi.maintenance.jobs.ImportTuneJob;
import com.rusefi.tune_manifest.TuneManifestHelper;
import com.rusefi.tune_manifest.TuneModel;
import com.rusefi.ui.table.ButtonEditor;
import com.rusefi.ui.table.ButtonRenderer;
import com.rusefi.ui.widgets.StatusPanel;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.tune_manifest.TuneManifestHelper.getLocalFolder;

/**
 * @see BasicStartupFrame#main dedicated sandbox not needed?
 */
public class TuneManagementTab {
    private static final Logging log = getLogging(TuneManagementTab.class);

    private static final int MAP_ID_INDEX = 0;
    private static final int SIZE_INDEX = 2;
    private static final int BUTTON_COLUMN = 3;

    private final JPanel totalContent = new JPanel(new BorderLayout());
    private final JLabel status = new JLabel("Downloading tunes...");
    private final JTable table = new JTable(new MyTableModel());
    private List<TuneModel> tunes = new ArrayList<>();

    public TuneManagementTab(ConnectivityContext connectivityContext,
                             AtomicReference<Optional<PortResult>> ecuPortToUse,
                             Component importTuneButton,
                             SingleAsyncJobExecutor singleAsyncJobExecutor,
                             StatusPanel statusPanelTuneTab) {
//        content.setBorder(BorderFactory.createLineBorder(Color.GREEN));

        String tunesManifestUrl = getTunesManifestUrl();
        if (tunesManifestUrl != null) {
            totalContent.add(status, BorderLayout.NORTH);

            JScrollPane tableScroll = new JScrollPane(table, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
                @Override
                public Dimension getPreferredSize() {
                    return new Dimension(400, 200);
                }
            };

            JPanel centerPanel = new JPanel(new BorderLayout());
            centerPanel.add(tableScroll, BorderLayout.CENTER);
            centerPanel.add(statusPanelTuneTab, BorderLayout.SOUTH);

            totalContent.add(centerPanel, BorderLayout.CENTER);

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
        table.getColumnModel().getColumn(MAP_ID_INDEX).setPreferredWidth(50);
        table.getColumnModel().getColumn(1).setPreferredWidth(450);
        table.getColumnModel().getColumn(SIZE_INDEX).setPreferredWidth(40);

        TableColumn buttonColumn = table.getColumnModel().getColumn(BUTTON_COLUMN);
        buttonColumn.setPreferredWidth(100);

        buttonColumn.setCellRenderer(new ButtonRenderer());
        buttonColumn.setCellEditor(new ButtonEditor(new JButton(), "Import", table, new ButtonEditor.Clicked() {
            @Override
            public void clicked(int row) {
                TuneModel model = tunes.get(row);
                String localFolderForSpecificUrl = getLocalFolder(tunesManifestUrl);
                String tuneFileName = localFolderForSpecificUrl + model.getSaferLocalFileName();
                Optional<PortResult> portResult = ecuPortToUse.get();
                if (!new File(tuneFileName).exists()) {
                    ErrorMessageHelper.showErrorDialog("Failed to load " + model.getUrl(), "Tune error");
                } else if (portResult.isPresent()) {
                    log.info("Let's load " + tuneFileName + " into " + portResult);
                    ImportTuneJob.importTuneIntoDevice(portResult.get(), status, connectivityContext, tuneFileName, singleAsyncJobExecutor);
                } else {
                    statusPanelTuneTab.logLine("Not connected?");
                }
            }
        }));

        totalContent.add(importTuneButton, BorderLayout.SOUTH);
    }

    private void displayTunes(List<TuneModel> tunes) {
        this.tunes = tunes;
        status.setText(tunes.size() + " tunes downloaded!");
        AutoupdateUtil.trueLayoutAndRepaint(table);
    }

    public Component getContent() {
        return totalContent;
    }

    public static String getTunesManifestUrl() {
        return PropertiesHolder.getProperty("tunes_manifest");
    }

    class MyTableModel extends AbstractTableModel {
        private final String[] columnNames = {"MapID", "Name", "Size", ""};

        public String getColumnName(int column) {
            return columnNames[column];
        }

        @Override
        public int getColumnCount() {
            return columnNames.length;
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            if (rowIndex == 0)
                return columnNames[columnIndex];
            TuneModel model = tunes.get(rowIndex - 1);
            if (columnIndex == MAP_ID_INDEX) {
                return model.getMapId();
            }
            if (columnIndex == 1) {
                return model.getFileName();
            }
            if (columnIndex == SIZE_INDEX) {
                return model.getSize();
            }
            if (columnIndex == BUTTON_COLUMN) {
                return model.isError() ? "Failed to Download" : "Load Tune";
            }
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
            return column == BUTTON_COLUMN;
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
