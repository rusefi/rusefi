package com.rusefi.ui;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.io.ConnectionStatusLogic;
import org.yaml.snakeyaml.Yaml;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.util.*;
import java.util.List;
import java.util.function.Consumer;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Displays ECU connector pinouts for the currently connected board.
 * Board is identified from the ECU signature (bundleTarget field).
 * Data is loaded from pinouts_raw/boards_meta.yaml and pinouts_raw/connectors.zip.
 * Each connector is shown as a tab with an interactive image (pin markers) above a table.
 */
public class PinoutPane {
    private static final String BOARDS_META = "pinouts_raw/boards_meta.yaml";
    private static final String CONNECTORS_ZIP = "pinouts_raw/connectors.zip";
    private static final String[] COLUMNS = {"Pin", "Function", "Type", "Class", "TS Name", "Color"};

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel statusLabel = new JLabel("Not connected", SwingConstants.CENTER);
    private JComponent centerPanel;

    private Map<String, List<String>> boardsData;

    // ---- Data models ----

    private static class PinCoord {
        final String pin;
        final int x, y;

        PinCoord(String pin, int x, int y) {
            this.pin = pin;
            this.x = x;
            this.y = y;
        }
    }

    private static class ConnectorData {
        final String title;
        final BufferedImage image;       // may be null if not found/loadable
        final List<PinCoord> coords;     // original-scale coords (empty if no image)
        final List<Object[]> rows;       // table rows matching COLUMNS order

        ConnectorData(String title, BufferedImage image, List<PinCoord> coords, List<Object[]> rows) {
            this.title = title;
            this.image = image;
            this.coords = coords;
            this.rows = rows;
        }
    }

    // ---- Image panel with clickable pin markers ----

    private static class ConnectorImagePanel extends JPanel {
        private BufferedImage image;
        private List<PinCoord> coords = Collections.emptyList();
        private String highlightedPin;
        private Consumer<String> pinClickListener;

        // Cached transform (recalculated each paint from current size)
        private double scale = 1.0;
        private int imgOffsetX, imgOffsetY;

        private static final int MARKER_RADIUS = 12;
        private static final int CLICK_RADIUS = 16;
        private static final Color COLOR_NORMAL = new Color(0, 120, 255, 170);
        private static final Color COLOR_HIGHLIGHT = new Color(255, 80, 0, 220);

        ConnectorImagePanel() {
            setBackground(Color.DARK_GRAY);
            addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    String pin = findNearestPin(e.getX(), e.getY());
                    if (pin != null) {
                        highlightedPin = pin;
                        repaint();
                        if (pinClickListener != null) pinClickListener.accept(pin);
                    }
                }
            });
        }

        void setData(BufferedImage img, List<PinCoord> coords) {
            this.image = img;
            this.coords = coords != null ? coords : Collections.emptyList();
            highlightedPin = null;
            repaint();
        }

        void setPinClickListener(Consumer<String> listener) {
            this.pinClickListener = listener;
        }

        void setHighlightedPin(String pin) {
            highlightedPin = pin;
            repaint();
        }

        /** Scale image to fit width×height, letterbox-center, no scroll needed. */
        private void recalcTransform() {
            if (image == null || getWidth() <= 0 || getHeight() <= 0) {
                scale = 1.0; imgOffsetX = 0; imgOffsetY = 0;
                return;
            }
            double sx = (double) getWidth() / image.getWidth();
            double sy = (double) getHeight() / image.getHeight();
            scale = Math.min(sx, sy);
            imgOffsetX = (int) ((getWidth() - image.getWidth() * scale) / 2);
            imgOffsetY = (int) ((getHeight() - image.getHeight() * scale) / 2);
        }

        private int screenX(PinCoord c) { return (int) (c.x * scale) + imgOffsetX; }
        private int screenY(PinCoord c) { return (int) (c.y * scale) + imgOffsetY; }

        private String findNearestPin(int mx, int my) {
            String nearest = null;
            double bestDist = (double) CLICK_RADIUS * CLICK_RADIUS;
            for (PinCoord c : coords) {
                double dx = mx - screenX(c);
                double dy = my - screenY(c);
                double dist2 = dx * dx + dy * dy;
                if (dist2 <= bestDist) {
                    bestDist = dist2;
                    nearest = c.pin;
                }
            }
            return nearest;
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (image == null) return;
            recalcTransform();

            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            int w = (int) (image.getWidth() * scale);
            int h = (int) (image.getHeight() * scale);
            g2.drawImage(image, imgOffsetX, imgOffsetY, w, h, null);

            int r = MARKER_RADIUS;
            // Font sized to fit inside the marker; cap between 7 and 11 px
            int fontSize = Math.max(7, Math.min(11, r));
            Font markerFont = g2.getFont().deriveFont(Font.BOLD, (float) fontSize);
            g2.setFont(markerFont);
            FontMetrics fm = g2.getFontMetrics();

            for (PinCoord c : coords) {
                int sx = screenX(c);
                int sy = screenY(c);
                boolean hl = c.pin.equals(highlightedPin);

                // Fill circle
                g2.setColor(hl ? COLOR_HIGHLIGHT : COLOR_NORMAL);
                g2.fillOval(sx - r, sy - r, r * 2, r * 2);

                // Border
                g2.setColor(Color.WHITE);
                g2.setStroke(new BasicStroke(hl ? 2f : 1f));
                g2.drawOval(sx - r, sy - r, r * 2, r * 2);

                // Pin label centred inside the circle
                String label = c.pin;
                int textW = fm.stringWidth(label);
                int textX = sx - textW / 2;
                int textY = sy + fm.getAscent() / 2 - 1;
                g2.setColor(Color.WHITE);
                g2.drawString(label, textX, textY);
            }
            g2.dispose();
        }
    }

    // ---- Main pane ----

    public PinoutPane(UIContext uiContext) {
        boardsData = loadBoardsMeta();

        statusLabel.setFont(statusLabel.getFont().deriveFont(Font.BOLD, 13f));
        statusLabel.setBorder(BorderFactory.createEmptyBorder(4, 0, 4, 0));

        content.add(statusLabel, BorderLayout.NORTH);

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

    private void setCenterPanel(JComponent c) {
        if (centerPanel != null) content.remove(centerPanel);
        centerPanel = c;
        if (c != null) content.add(c, BorderLayout.CENTER);
        content.revalidate();
        content.repaint();
    }

    private void showDisconnected() {
        statusLabel.setText("Not connected");
        setCenterPanel(null);
    }

    private void showBoard(String signature) {
        if (signature == null) {
            statusLabel.setText("Connected — board unknown");
            setCenterPanel(null);
            return;
        }
        RusEfiSignature parsed = SignatureHelper.parse(signature);
        if (parsed == null) {
            statusLabel.setText("Connected — unrecognized signature: " + signature);
            setCenterPanel(null);
            return;
        }
        String boardKey = parsed.getBundleTarget();
        statusLabel.setText("Board: " + boardKey);

        if (boardsData == null) {
            statusLabel.setText("Board: " + boardKey + "  [pinout data not found — expected: " + new File(BOARDS_META).getAbsolutePath() + "]");
            setCenterPanel(null);
            return;
        }

        List<String> connectorFiles = boardsData.get(boardKey);
        if (connectorFiles == null) {
            statusLabel.setText("Board: " + boardKey + "  [no pinout available]");
            setCenterPanel(null);
            return;
        }
        buildConnectorTabs(connectorFiles);
    }

    private void buildConnectorTabs(List<String> connectorPaths) {
        File zipFile = findFile(CONNECTORS_ZIP);
        if (zipFile == null) {
            setCenterPanel(new JLabel("connectors.zip not found", SwingConstants.CENTER));
            return;
        }

        List<ConnectorData> connectors = new ArrayList<>();
        try (ZipFile zip = new ZipFile(zipFile)) {
            for (String path : connectorPaths) {
                ConnectorData cd = loadConnector(zip, path);
                if (cd != null) connectors.add(cd);
            }
        } catch (IOException e) {
            // leave connectors as-is
        }

        if (connectors.isEmpty()) {
            setCenterPanel(new JLabel("No pinout data loaded", SwingConstants.CENTER));
            return;
        }

        JTabbedPane tabs = new JTabbedPane();
        for (ConnectorData cd : connectors) {
            tabs.addTab(cd.title, buildConnectorPanel(cd));
        }
        setCenterPanel(tabs);
    }

    private JPanel buildConnectorPanel(ConnectorData cd) {
        // Table
        DefaultTableModel model = new DefaultTableModel(COLUMNS, 0) {
            @Override
            public boolean isCellEditable(int row, int column) { return false; }
        };
        for (Object[] row : cd.rows) model.addRow(row);

        JTable table = new JTable(model);
        table.setAutoCreateRowSorter(true);
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        JPanel panel = new JPanel(new BorderLayout());

        if (cd.image != null) {
            ConnectorImagePanel imagePanel = new ConnectorImagePanel();
            imagePanel.setData(cd.image, cd.coords);

            // Map pin name → table row index
            Map<String, Integer> pinToRow = new HashMap<>();
            for (int i = 0; i < cd.rows.size(); i++) {
                Object pinVal = cd.rows.get(i)[0];
                if (pinVal != null) pinToRow.put(pinVal.toString(), i);
            }

            // Image click → highlight row in table
            imagePanel.setPinClickListener(pin -> {
                Integer row = pinToRow.get(pin);
                if (row != null) {
                    int viewRow = table.convertRowIndexToView(row);
                    table.setRowSelectionInterval(viewRow, viewRow);
                    table.scrollRectToVisible(table.getCellRect(viewRow, 0, true));
                }
            });

            // Table row selection → highlight pin on image
            table.getSelectionModel().addListSelectionListener(e -> {
                if (e.getValueIsAdjusting()) return;
                int viewRow = table.getSelectedRow();
                if (viewRow < 0) {
                    imagePanel.setHighlightedPin(null);
                    return;
                }
                int modelRow = table.convertRowIndexToModel(viewRow);
                Object pinVal = model.getValueAt(modelRow, 0);
                imagePanel.setHighlightedPin(pinVal != null ? pinVal.toString() : null);
            });

            JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT,
                    imagePanel, new JScrollPane(table));
            split.setResizeWeight(0.6);
            panel.add(split, BorderLayout.CENTER);
        } else {
            panel.add(new JScrollPane(table), BorderLayout.CENTER);
        }

        return panel;
    }

    @SuppressWarnings("unchecked")
    private ConnectorData loadConnector(ZipFile zip, String yamlPath) {
        ZipEntry entry = zip.getEntry(yamlPath);
        if (entry == null) return null;

        Map<String, Object> doc;
        try (InputStream is = zip.getInputStream(entry)) {
            doc = new Yaml().load(is);
        } catch (IOException e) {
            return null;
        }

        if (doc == null) return null;

        // Pin data rows
        List<Object[]> rows = new ArrayList<>();
        List<Map<String, Object>> pins = (List<Map<String, Object>>) doc.get("pins");
        if (pins != null) {
            for (Map<String, Object> pin : pins) {
                rows.add(new Object[]{
                        str(pin.get("pin")),
                        str(pin.get("function")),
                        str(pin.get("type")),
                        classStr(pin.get("class")),
                        str(pin.get("ts_name")),
                        str(pin.get("color")),
                });
            }
        }

        // info section: title, image, coords
        Map<String, Object> info = (Map<String, Object>) doc.get("info");
        String title = yamlPath; // fallback
        BufferedImage image = null;
        List<PinCoord> coords = new ArrayList<>();

        if (info != null) {
            String t = str(info.get("title"));
            if (t.isEmpty()) t = str(info.get("name"));
            if (!t.isEmpty()) title = t;

            Map<String, Object> imageInfo = (Map<String, Object>) info.get("image");
            if (imageInfo != null) {
                String imageFile = str(imageInfo.get("file"));
                if (!imageFile.isEmpty()) {
                    // image is in same directory as the yaml
                    String dir = yamlPath.substring(0, yamlPath.lastIndexOf('/') + 1);
                    String imagePath = dir + imageFile;
                    image = loadImageFromZip(zip, imagePath);
                }
            }

            List<Map<String, Object>> coordList = (List<Map<String, Object>>) info.get("pins");
            if (coordList != null) {
                for (Map<String, Object> c : coordList) {
                    String pin = str(c.get("pin"));
                    int x = toInt(c.get("x"));
                    int y = toInt(c.get("y"));
                    if (!pin.isEmpty()) coords.add(new PinCoord(pin, x, y));
                }
            }
        }

        return new ConnectorData(title, image, coords, rows);
    }

    private static BufferedImage loadImageFromZip(ZipFile zip, String imagePath) {
        ZipEntry entry = zip.getEntry(imagePath);
        if (entry == null) return null;
        try (InputStream is = zip.getInputStream(entry)) {
            return ImageIO.read(is);
        } catch (IOException e) {
            return null;
        }
    }

    @SuppressWarnings("unchecked")
    private Map<String, List<String>> loadBoardsMeta() {
        File metaFile = findFile(BOARDS_META);
        if (metaFile == null) return null;
        try (java.io.InputStream is = Files.newInputStream(metaFile.toPath())) {
            Map<String, Object> root = new Yaml().load(is);
            return (Map<String, List<String>>) root.get("data");
        } catch (IOException e) {
            return null;
        }
    }

    private static String str(Object val) {
        return val == null ? "" : val.toString();
    }

    private static int toInt(Object val) {
        if (val instanceof Number) return ((Number) val).intValue();
        if (val instanceof String) {
            try { return Integer.parseInt((String) val); } catch (NumberFormatException ignored) {}
        }
        return 0;
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
