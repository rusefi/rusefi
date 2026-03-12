package com.rusefi.ui;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.io.ConnectionStatusLogic;
import org.yaml.snakeyaml.Yaml;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.util.*;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Displays ECU connector pinouts for the currently connected board.
 * Board is identified from the ECU signature (bundleTarget field).
 * Data is loaded from pinouts_raw/boards_meta.yaml and pinouts_raw/connectors.zip.
 */
public class PinoutPane {
    private static final String BOARDS_META = "pinouts_raw/boards_meta.yaml";
    private static final String CONNECTORS_ZIP = "pinouts_raw/connectors.zip";
    private static final String[] COLUMNS = {"Pin", "Function", "Type", "Class", "TS Name", "Color"};

    private final JPanel content = new JPanel(new BorderLayout());

    private final JLabel statusLabel = new JLabel("Not connected", SwingConstants.CENTER);
    private final DefaultTableModel tableModel = new DefaultTableModel(COLUMNS, 0) {
        @Override
        public boolean isCellEditable(int row, int column) { return false; }
    };

    private Map<String, List<String>> boardsData;

    public PinoutPane(UIContext uiContext) {
        boardsData = loadBoardsMeta();

        statusLabel.setFont(statusLabel.getFont().deriveFont(Font.BOLD, 13f));
        statusLabel.setBorder(BorderFactory.createEmptyBorder(4, 0, 4, 0));

        JTable pinTable = new JTable(tableModel);
        pinTable.setAutoCreateRowSorter(true);
        pinTable.getTableHeader().setReorderingAllowed(false);

        content.add(statusLabel, BorderLayout.NORTH);
        content.add(new JScrollPane(pinTable), BorderLayout.CENTER);

        ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> SwingUtilities.invokeLater(() -> {
            if (!isConnected) {
                showDisconnected();
            } else {
                BinaryProtocol bp = uiContext.getBinaryProtocol();
                String signature = bp != null ? bp.signature : null;
                showBoard(signature);
            }
        }));
    }

    public JPanel getContent() {
        return content;
    }

    private void showDisconnected() {
        statusLabel.setText("Not connected");
        tableModel.setRowCount(0);
    }

    private void showBoard(String signature) {
        if (signature == null) {
            statusLabel.setText("Connected — board unknown");
            tableModel.setRowCount(0);
            return;
        }
        RusEfiSignature parsed = SignatureHelper.parse(signature);
        if (parsed == null) {
            statusLabel.setText("Connected — unrecognized signature: " + signature);
            tableModel.setRowCount(0);
            return;
        }
        String boardKey = parsed.getBundleTarget();
        statusLabel.setText("Board: " + boardKey);

        if (boardsData == null) {
            statusLabel.setText("Board: " + boardKey + "  [pinout data not found — expected: " + new File(BOARDS_META).getAbsolutePath() + "]");
            tableModel.setRowCount(0);
            return;
        }

        List<String> connectorFiles = boardsData.get(boardKey);
        if (connectorFiles == null) {
            statusLabel.setText("Board: " + boardKey + "  [no pinout available]");
            tableModel.setRowCount(0);
            return;
        }
        populateTable(connectorFiles);
    }

    @SuppressWarnings("unchecked")
    private Map<String, List<String>> loadBoardsMeta() {
        File metaFile = findFile(BOARDS_META);
        if (metaFile == null) return null;
        try (InputStream is = Files.newInputStream(metaFile.toPath())) {
            Map<String, Object> root = new Yaml().load(is);
            return (Map<String, List<String>>) root.get("data");
        } catch (IOException e) {
            return null;
        }
    }

    @SuppressWarnings("unchecked")
    private void populateTable(List<String> connectorPaths) {
        tableModel.setRowCount(0);
        File zipFile = findFile(CONNECTORS_ZIP);
        if (zipFile == null) return;
        try (ZipFile zip = new ZipFile(zipFile)) {
            for (String path : connectorPaths) {
                ZipEntry entry = zip.getEntry(path);
                if (entry == null) continue;
                try (InputStream is = zip.getInputStream(entry)) {
                    Map<String, Object> doc = new Yaml().load(is);
                    List<Map<String, Object>> pins = (List<Map<String, Object>>) doc.get("pins");
                    if (pins == null) continue;
                    for (Map<String, Object> pin : pins) {
                        tableModel.addRow(new Object[]{
                            str(pin.get("pin")),
                            str(pin.get("function")),
                            str(pin.get("type")),
                            classStr(pin.get("class")),
                            str(pin.get("ts_name")),
                            str(pin.get("color")),
                        });
                    }
                }
            }
        } catch (IOException e) {
            // leave table as-is on error
        }
    }

    private static String str(Object val) {
        return val == null ? "" : val.toString();
    }

    private static String classStr(Object val) {
        if (val == null) return "";
        if (val instanceof List) {
            List<?> list = (List<?>) val;
            return String.join(", ", list.stream().map(Object::toString).toArray(String[]::new));
        }
        return val.toString();
    }

    private static File findFile(String relativePath) {
        File f = new File(relativePath);
        if (f.exists()) return f;
        f = new File("../" + relativePath);
        if (f.exists()) return f;
        return null;
    }
}
