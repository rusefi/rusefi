package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.LinkManager;
import com.rusefi.maintenance.OfflineTuneLoader;
import com.rusefi.maintenance.jobs.ImportTuneJob;
import com.rusefi.ui.UIContext;
import com.rusefi.tune_manifest.ManifestParseException;
import com.rusefi.tune_manifest.TuneManifestHelper;
import com.rusefi.tune_manifest.TuneModel;
import com.rusefi.ui.table.ButtonEditor;
import com.rusefi.ui.table.ButtonRenderer;
import com.rusefi.ui.widgets.StatusPanel;
import org.json.simple.parser.ParseException;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.tune_manifest.TuneManifestHelper.getLocalFolder;

public class TuneManagementTab {
    private static final Logging log = getLogging(TuneManagementTab.class);

    private static final int MAP_ID_INDEX = 0;
    private static final int SIZE_INDEX = 2;
    private static final int BUTTON_COLUMN = 3;

    private static final String MAIN_CARD = "main";
    private static final String IMPORT_CARD = "import";

    private final JPanel totalContent = new JPanel(new CardLayout());
    private final JPanel mainContent = new JPanel(new BorderLayout());
    private final JLabel status = new JLabel("Downloading tunes...");
    private final JTable table = new JTable(new MyTableModel());
    private final JPanel centerPanel = new JPanel(new BorderLayout());
    private JScrollPane tableScroll;
    private final TuneOperationStatusPanel importStatusPanel;
    private final AtomicBoolean awaitingCompletion = new AtomicBoolean(false);
    private List<TuneModel> tunes = new ArrayList<>();

    /** [tag:offline_tune] Hands an offline-loaded tune back to the splash so it can open the console on the shared uiContext. */
    private final java.util.function.BiConsumer<IniFileModel, ConfigurationImage> offlineConsoleLauncher;

    public TuneManagementTab(ConnectivityContext connectivityContext,
                             UIContext uiContext,
                             ImportTuneControl importTuneControl,
                             SingleAsyncJobExecutor singleAsyncJobExecutor,
                             StatusPanel statusPanelTuneTab,
                             Runnable showTuneTab,
                             java.util.function.BiConsumer<IniFileModel, ConfigurationImage> offlineConsoleLauncher) {
        this.offlineConsoleLauncher = offlineConsoleLauncher;
        Component importTuneButton = importTuneControl.getContent();
        importStatusPanel = new TuneOperationStatusPanel(statusPanelTuneTab, this::showMainContent);

        importTuneControl.setImportErrorHandler(message -> showImportError(message, statusPanelTuneTab, showTuneTab));

        singleAsyncJobExecutor.addOnJobAboutToStartListener(() -> {
            if (singleAsyncJobExecutor.getJobInProgress().filter(ImportTuneJob.class::isInstance).isPresent()) {
                awaitingCompletion.set(true);
                SwingUtilities.invokeLater(() -> {
                    showTuneTab.run();
                    showImportProgress();
                });
            }
        });

        singleAsyncJobExecutor.addOnJobInProgressFinishedListener(() -> {
            if (!awaitingCompletion.compareAndSet(true, false)) {
                return;
            }
            SwingUtilities.invokeLater(() -> {
                showTuneTab.run();
                showImportResult(statusPanelTuneTab.isInErrorState());
            });
        });

        String tunesManifestUrl = getTunesManifestUrl();
        if (tunesManifestUrl != null) {
            mainContent.add(status, BorderLayout.NORTH);

            tableScroll = new JScrollPane(table, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
                @Override
                public Dimension getPreferredSize() {
                    return new Dimension(400, 200);
                }
            };

            centerPanel.add(tableScroll, BorderLayout.CENTER);

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
                        log.error("Failed to download tunes manifest from " + tunesManifestUrl, e);
                        String body = (e instanceof ManifestParseException) ? ((ManifestParseException) e).getBody() : null;
                        SwingUtilities.invokeLater(() -> showManifestError(e, body));
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
                if (!new File(tuneFileName).exists()) {
                    showImportError("Failed to load " + model.getUrl(), statusPanelTuneTab, showTuneTab);
                } else if (!singleAsyncJobExecutor.isNotInProgress()) {
                    showImportError("Another job is already running, please wait.", statusPanelTuneTab, showTuneTab);
                } else {
                    BinaryProtocol liveBp = uiContext.getBinaryProtocol();
                    LinkManager lm = uiContext.getLinkManager();
                    if (liveBp != null && lm != null) {
                        log.info("Let's load " + tuneFileName + " via live connection");
                        ImportTuneJob.importTuneIntoDeviceViaLiveConnection(
                            liveBp, lm, status, connectivityContext, tuneFileName, singleAsyncJobExecutor,
                            message -> showImportError(message, statusPanelTuneTab, showTuneTab));
                    } else if (lm != null) {
                        log.info("Let's load " + tuneFileName + " via LM");
                        ImportTuneJob.importTuneIntoDeviceViaLiveConnection(
                            lm, status, connectivityContext, tuneFileName, singleAsyncJobExecutor,
                            message -> showImportError(message, statusPanelTuneTab, showTuneTab));
                    } else {
                        showImportError("Device is not connected", statusPanelTuneTab, showTuneTab);
                    }
                }
            }
        }));

        JButton loadTuneFileButton = new JButton("Load Tune File");
        loadTuneFileButton.addActionListener(e -> loadTuneFileOffline());
        // A bit bigger so it reads as the primary offline action (#9715).
        loadTuneFileButton.setFont(loadTuneFileButton.getFont().deriveFont(Font.BOLD, 16f));
        loadTuneFileButton.setMargin(new Insets(12, 28, 12, 28));

        // [tag:offline_tune] Explain what this screen does — without it the splash is three silent
        // buttons and no hint that a tune can be edited with no ECU attached (#9730).
        JLabel offlineHint = new JLabel("<html><div style='text-align:center;'>"
                + "No ECU connected. Load a tune file to view and edit it offline,<br>"
                + "then connect an ECU to burn your changes.</div></html>");
        offlineHint.setForeground(Color.DARK_GRAY);

        // [tag:offline_tune] The import pair pushes a tune to a *connected* ECU; on this pre-connection
        // splash it has nothing to talk to (clicking just prints "Not connected?"), so only show it
        // once an ECU is actually connected.
        Runnable updateImportVisibility = () ->
                importTuneButton.setVisible(com.rusefi.io.ConnectionStatusLogic.INSTANCE.isConnected());
        updateImportVisibility.run();
        com.rusefi.io.ConnectionStatusLogic.INSTANCE.addListener(
                c -> SwingUtilities.invokeLater(updateImportVisibility));

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.add(centerHorizontally(offlineHint));
        buttonPanel.add(Box.createVerticalStrut(20));
        buttonPanel.add(centerHorizontally(importTuneButton));
        buttonPanel.add(Box.createVerticalStrut(28));
        buttonPanel.add(centerHorizontally(loadTuneFileButton));

        if (tunesManifestUrl != null) {
            // Keep the tune list but cap it at ~80% of the height, with the buttons in the lower 20%,
            // so they aren't pinned to the very bottom edge of the now-maximized splash (#9715).
            JPanel body = new JPanel(new GridBagLayout());
            GridBagConstraints c = new GridBagConstraints();
            c.gridx = 0;
            c.gridy = 0;
            c.weightx = 1;
            c.weighty = 0.8;
            c.fill = GridBagConstraints.BOTH;
            body.add(centerPanel, c);
            c.gridy = 1;
            c.weighty = 0.2;
            c.fill = GridBagConstraints.NONE;
            c.anchor = GridBagConstraints.CENTER;
            body.add(buttonPanel, c);
            mainContent.add(body, BorderLayout.CENTER);
        } else {
            // No tune list available — just center the buttons in the tab (#9715).
            JPanel centered = new JPanel(new GridBagLayout());
            centered.add(buttonPanel, new GridBagConstraints());
            mainContent.add(centered, BorderLayout.CENTER);
        }

        totalContent.add(mainContent, MAIN_CARD);
        totalContent.add(importStatusPanel.getContent(), IMPORT_CARD);
        showMainContent();
    }

    private static JPanel centerHorizontally(Component c) {
        JPanel row = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
        row.add(c);
        return row;
    }

    private void showImportProgress() {
        importStatusPanel.showProgress("Importing tune...");
        ((CardLayout) totalContent.getLayout()).show(totalContent, IMPORT_CARD);
    }

    private void showImportResult(boolean failed) {
        importStatusPanel.showResult("Tune imported successfully", "Tune import failed", failed);
        ((CardLayout) totalContent.getLayout()).show(totalContent, IMPORT_CARD);
    }

    private void showImportError(String message, StatusPanel statusPanelTuneTab, Runnable showTuneTab) {
        SwingUtilities.invokeLater(() -> {
            showTuneTab.run();
            statusPanelTuneTab.clear();
            statusPanelTuneTab.logLine(message);
            statusPanelTuneTab.error();
            SwingUtilities.invokeLater(() -> showImportResult(true));
        });
    }

    private void showMainContent() {
        ((CardLayout) totalContent.getLayout()).show(totalContent, MAIN_CARD);
    }

    private void displayTunes(List<TuneModel> tunes) {
        this.tunes = tunes;
        status.setText(tunes.size() + " tunes downloaded!");
        AutoupdateUtil.trueLayoutAndRepaint(table);
    }

    private void showManifestError(Exception e, String body) {
        // Keep the header label short: JLabel's preferred width tracks its full text, and a long
        // error message there would inflate the tab (and the splash frame on the next pack).
        status.setText("Failed to load tunes.");

        String content = (body == null || body.isEmpty()) ? e.toString() : body;
        JTextArea textArea = new JTextArea(content);
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        textArea.setCaretPosition(0);
        JScrollPane errorScroll = new JScrollPane(textArea,
            JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED) {
            @Override
            public Dimension getPreferredSize() {
                return new Dimension(400, 200);
            }
        };
        if (tableScroll != null) {
            centerPanel.remove(tableScroll);
        }
        centerPanel.add(errorScroll, BorderLayout.CENTER);
        AutoupdateUtil.trueLayoutAndRepaint(centerPanel);
    }

    public Component getContent() {
        return totalContent;
    }

    public static String getTunesManifestUrl() {
        return PropertiesHolder.getProperty("tunes_manifest");
    }

    /** [tag:offline_tune] Lets the user pick an .msq and open the console offline (no ECU connection). */
    private void loadTuneFileOffline() {
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));
        if (chooser.showOpenDialog(totalContent) != JFileChooser.APPROVE_OPTION) {
            return;
        }

        String path = chooser.getSelectedFile().getAbsolutePath();
        OfflineTuneLoader.Result result = OfflineTuneLoader.loadTuneFromFile(path, totalContent);
        if (result == null) {
            return;
        }

        // Hand off to the splash, which disposes itself and opens the console on the shared uiContext
        // (keeping the scanner alive so a later ECU connect transitions this console online — no 2nd window).
        SwingUtilities.invokeLater(() -> offlineConsoleLauncher.accept(result.ini, result.image));
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
