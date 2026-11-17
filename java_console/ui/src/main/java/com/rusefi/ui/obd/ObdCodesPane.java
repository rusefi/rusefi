package com.rusefi.ui.obd;

import com.rusefi.core.ObdCodeDictionary;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import com.rusefi.io.ConnectionStatusLogic;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import java.awt.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

/**
 * Console top level tab listing OBD error codes reported by the lastErrorCode and recentErrorCode[] output channels.
 *
 * @see ObdCodeHistory
 */
public class ObdCodesPane {
    public static final String TITLE = "OBD Codes";
    static final String LAST_ERROR_CODE = "lastErrorCode";
    /**
     * see uint16_t[8 iterate] recentErrorCode in output_channels.txt
     */
    static final int RECENT_COUNT = 8;
    static final String RECENT_ERROR_CODE = "recentErrorCode";

    private static final String[] COLUMNS = {"Code", "Description", "Status", "Times raised", "First seen", "Last seen"};

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel lastCodeLabel = new JLabel();
    private final JLabel statusLabel = new JLabel();
    private final ObdCodeHistory history = new ObdCodeHistory();
    private final ObdCodeDictionary dictionary = ObdCodeDictionary.getInstance();
    private List<ObdCodeHistory.Item> rows = Collections.emptyList();
    private final CodesTableModel tableModel = new CodesTableModel();
    private final JTable table = new JTable(tableModel);

    public ObdCodesPane() {
        JPanel topPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JButton clear = new JButton("Clear history");
        clear.setToolTipText("Forget codes seen during this Console session; active codes reappear on the next update");
        clear.addActionListener(e -> {
            history.clear();
            refresh();
        });
        topPanel.add(lastCodeLabel);
        topPanel.add(statusLabel);
        topPanel.add(clear);
        content.add(topPanel, BorderLayout.NORTH);

        table.setAutoCreateRowSorter(false);
        table.getColumnModel().getColumn(0).setPreferredWidth(70);
        table.getColumnModel().getColumn(1).setPreferredWidth(400);
        content.add(new JScrollPane(table), BorderLayout.CENTER);

        String[] channels = new String[RECENT_COUNT + 1];
        channels[0] = LAST_ERROR_CODE;
        for (int i = 0; i < RECENT_COUNT; i++) {
            channels[i + 1] = RECENT_ERROR_CODE + (i + 1);
        }
        // always subscribed, codes are collected even while another tab is selected
        SensorCentral.getInstance().addListener(this::onOutputChannels, new SensorSubscription(channels));
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> SwingUtilities.invokeLater(() -> {
            if (!isConnected) {
                history.markAllInactive();
            }
            refresh();
        }));
        refresh();
    }

    private void onOutputChannels() {
        if (!ConnectionStatusLogic.INSTANCE.isConnected()) {
            return;
        }
        SensorCentral sensors = SensorCentral.getInstance();
        int lastErrorCode = toCode(sensors.getValue(LAST_ERROR_CODE));
        Set<Integer> active = new LinkedHashSet<>();
        for (int i = 0; i < RECENT_COUNT; i++) {
            int code = toCode(sensors.getValue(RECENT_ERROR_CODE + (i + 1)));
            if (code != 0) {
                active.add(code);
            }
        }
        long now = System.currentTimeMillis();
        SwingUtilities.invokeLater(() -> {
            history.update(lastErrorCode, active, now);
            refresh();
        });
    }

    /**
     * @return 0 for a channel missing in the current .ini
     */
    private static int toCode(double value) {
        return Double.isNaN(value) ? 0 : (int) value;
    }

    private void refresh() {
        int last = history.getLastErrorCode();
        lastCodeLabel.setText("Last error code: " + (last == 0 ? "none" : formatCode(last) + " " + dictionary.getDescription(last)));
        if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            statusLabel.setText("Active: " + history.getActiveCount());
        } else {
            statusLabel.setText("Not connected");
        }
        List<ObdCodeHistory.Item> newRows = new ArrayList<>(history.getItems());
        if (sameCodes(rows, newRows)) {
            // refresh happens on every output channels frame: fireTableDataChanged() would
            // clear the user's selection each time, a rows update keeps it
            rows = newRows;
            if (!rows.isEmpty()) {
                tableModel.fireTableRowsUpdated(0, rows.size() - 1);
            }
            return;
        }
        Set<Integer> selectedCodes = new LinkedHashSet<>();
        for (int row : table.getSelectedRows()) {
            selectedCodes.add(rows.get(row).code);
        }
        rows = newRows;
        tableModel.fireTableDataChanged();
        for (int i = 0; i < rows.size(); i++) {
            if (selectedCodes.contains(rows.get(i).code)) {
                table.addRowSelectionInterval(i, i);
            }
        }
    }

    private static boolean sameCodes(List<ObdCodeHistory.Item> a, List<ObdCodeHistory.Item> b) {
        if (a.size() != b.size()) {
            return false;
        }
        for (int i = 0; i < a.size(); i++) {
            if (a.get(i).code != b.get(i).code) {
                return false;
            }
        }
        return true;
    }

    static String formatCode(int code) {
        return ObdCodeDictionary.formatCode(code);
    }

    private static String formatTime(long millis) {
        return new SimpleDateFormat("HH:mm:ss").format(new Date(millis));
    }

    public JComponent getContent() {
        return content;
    }

    private class CodesTableModel extends AbstractTableModel {
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
        public Object getValueAt(int rowIndex, int columnIndex) {
            ObdCodeHistory.Item item = rows.get(rowIndex);
            switch (columnIndex) {
                case 0:
                    return formatCode(item.code);
                case 1:
                    return dictionary.getDescription(item.code);
                case 2:
                    return item.isActive() ? "Active" : "Cleared";
                case 3:
                    return item.getRaisedCount();
                case 4:
                    return formatTime(item.firstSeenMillis);
                case 5:
                    return formatTime(item.getLastSeenMillis());
                default:
                    return null;
            }
        }
    }
}
