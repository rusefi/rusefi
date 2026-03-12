package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
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
 * Pin marker color can be switched between type-based and pigtail wire color.
 */
public class PinoutPane {
    private static final String BOARDS_META = "pinouts_raw/boards_meta.yaml";
    private static final String CONNECTORS_ZIP = "pinouts_raw/connectors.zip";
    private static final String[] COLUMNS = {"Pin", "Function", "Type", "Class", "TS Name", "Pigtail color", "Tune use"};

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel statusLabel = new JLabel("Not connected", SwingConstants.CENTER);
    private JComponent centerPanel;

    private final Map<String, List<String>> boardsData;
    private final UIContext uiContext;
    /** All image panels currently displayed — updated when a board's tabs are built. */
    private final List<ConnectorImagePanel> activeImagePanels = new ArrayList<>();

    // ---- Color mode ----

    enum ColorMode { TYPE, PIGTAIL }
    private ColorMode colorMode = ColorMode.TYPE;

    // ---- Data models ----

    private static class PinCoord {
        final String pin;
        final int x, y;
        /** Pin type string from yaml (e.g. "ign", "inj", "ls"). May be empty. */
        final String type;
        /** Pigtail wire color string from yaml (e.g. "red", "orange/brown"). May be empty. */
        final String wireColor;

        PinCoord(String pin, int x, int y, String type, String wireColor) {
            this.pin = pin;
            this.x = x;
            this.y = y;
            this.type = type;
            this.wireColor = wireColor;
        }
    }

    private static class ConnectorData {
        final String title;
        final BufferedImage image;
        final List<PinCoord> coords;
        final List<Object[]> rows;

        ConnectorData(String title, BufferedImage image, List<PinCoord> coords, List<Object[]> rows) {
            this.title = title;
            this.image = image;
            this.coords = coords;
            this.rows = rows;
        }
    }

    // ---- Color lookup ----

    /**
     * Maps type string to marker color using the same substring-match rules as the web CSS:
     * ign=#f0f, inj=maroon, ls=#90ee90, mr=#b22222, pgnd=coral, sgnd=olive, usb=#20b2aa, vr=sienna
     */
    private static Color typeToColor(String type) {
        if (type == null || type.isEmpty()) return null;
        String t = type.toLowerCase();
        // Check longer/more-specific strings first to avoid partial overlaps
        if (t.contains("pgnd")) return new Color(0xFF, 0x7F, 0x50); // coral
        if (t.contains("sgnd")) return new Color(0x80, 0x80, 0x00); // olive
        if (t.contains("ign"))  return new Color(0xFF, 0x00, 0xFF); // #f0f
        if (t.contains("inj"))  return new Color(0x80, 0x00, 0x00); // maroon
        if (t.contains("ls"))   return new Color(0x90, 0xEE, 0x90); // #90ee90
        if (t.contains("mr"))   return new Color(0xB2, 0x22, 0x22); // #b22222
        if (t.contains("usb"))  return new Color(0x20, 0xB2, 0xAA); // #20b2aa
        if (t.contains("vr"))   return new Color(0xA0, 0x52, 0x2D); // sienna
        return null;
    }

    /** Maps pigtail wire color string (first token before "/" or space) to a Color. */
    private static Color pigtailToColor(String wireColor) {
        if (wireColor == null || wireColor.isEmpty()) return null;
        String c = wireColor.toLowerCase().split("[/,\\s]+")[0].trim();
        switch (c) {
            case "black":  return new Color(30, 30, 30);
            case "red":    return new Color(200, 0, 0);
            case "white":  return new Color(230, 230, 230);
            case "yellow": return new Color(220, 200, 0);
            case "green":  return new Color(0, 150, 0);
            case "blue":   return new Color(0, 60, 200);
            case "orange": return new Color(255, 140, 0);
            case "purple":
            case "violet": return new Color(128, 0, 128);
            case "brown":  return new Color(139, 69, 19);
            case "grey":
            case "gray":   return new Color(120, 120, 120);
            case "pink":   return new Color(255, 160, 160);
            default:       return null;
        }
    }

    /** Returns whether a color is perceived as light (needs dark text). */
    private static boolean isLight(Color c) {
        // Standard luminance formula
        double lum = 0.299 * c.getRed() + 0.587 * c.getGreen() + 0.114 * c.getBlue();
        return lum > 160;
    }

    private static final Color FALLBACK_NORMAL = new Color(0, 120, 255, 200);
    private static final Color COLOR_HIGHLIGHT  = new Color(255, 80, 0, 230);

    // ---- Image panel with clickable pin markers ----

    private static class ConnectorImagePanel extends JPanel {
        private BufferedImage image;
        private List<PinCoord> coords = Collections.emptyList();
        private String highlightedPin;
        private Consumer<String> pinClickListener;
        private ColorMode colorMode = ColorMode.TYPE;

        // Cached transform (recalculated each paint)
        private double scale = 1.0;
        private int imgOffsetX, imgOffsetY;

        private static final int MARKER_RADIUS = 12;
        private static final int CLICK_RADIUS  = 16;

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

        void setColorMode(ColorMode mode) {
            this.colorMode = mode;
            repaint();
        }

        private void recalcTransform() {
            if (image == null || getWidth() <= 0 || getHeight() <= 0) {
                scale = 1.0; imgOffsetX = 0; imgOffsetY = 0;
                return;
            }
            double sx = (double) getWidth() / image.getWidth();
            double sy = (double) getHeight() / image.getHeight();
            scale = Math.min(sx, sy);
            imgOffsetX = (int) ((getWidth()  - image.getWidth()  * scale) / 2);
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

        private Color markerFillColor(PinCoord c) {
            if (c.pin.equals(highlightedPin)) return COLOR_HIGHLIGHT;
            Color base;
            if (colorMode == ColorMode.PIGTAIL) {
                base = pigtailToColor(c.wireColor);
            } else {
                base = typeToColor(c.type);
            }
            return base != null ? new Color(base.getRed(), base.getGreen(), base.getBlue(), 200) : FALLBACK_NORMAL;
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            if (image == null) return;
            recalcTransform();

            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,       RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION,      RenderingHints.VALUE_INTERPOLATION_BILINEAR);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,  RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            int w = (int) (image.getWidth()  * scale);
            int h = (int) (image.getHeight() * scale);
            g2.drawImage(image, imgOffsetX, imgOffsetY, w, h, null);

            int fontSize = MARKER_RADIUS - 1;
            Font markerFont = g2.getFont().deriveFont(Font.BOLD, (float) fontSize);
            g2.setFont(markerFont);
            FontMetrics fm = g2.getFontMetrics();

            for (PinCoord c : coords) {
                int sx = screenX(c);
                int sy = screenY(c);
                boolean hl = c.pin.equals(highlightedPin);

                Color fill = markerFillColor(c);
                g2.setColor(fill);
                g2.fillOval(sx - MARKER_RADIUS, sy - MARKER_RADIUS, MARKER_RADIUS * 2, MARKER_RADIUS * 2);

                // Border: white normally, bright orange when highlighted
                g2.setColor(hl ? Color.ORANGE : Color.WHITE);
                g2.setStroke(new BasicStroke(hl ? 2f : 1f));
                g2.drawOval(sx - MARKER_RADIUS, sy - MARKER_RADIUS, MARKER_RADIUS * 2, MARKER_RADIUS * 2);

                // Pin label: pick black or white for contrast, draw with a thin shadow
                String label = c.pin;
                int textW = fm.stringWidth(label);
                int textX = sx - textW / 2;
                int textY = sy + fm.getAscent() / 2 - 1;

                Color textColor = isLight(fill) ? Color.BLACK : Color.WHITE;
                Color shadowColor = isLight(fill) ? new Color(200, 200, 200, 120) : new Color(0, 0, 0, 120);

                g2.setColor(shadowColor);
                g2.drawString(label, textX + 1, textY + 1);
                g2.setColor(textColor);
                g2.drawString(label, textX, textY);
            }
            g2.dispose();
        }
    }

    // ---- Main pane ----

    public PinoutPane(UIContext uiContext) {
        this.uiContext = uiContext;
        boardsData = loadBoardsMeta();

        statusLabel.setFont(statusLabel.getFont().deriveFont(Font.BOLD, 13f));

        JComboBox<String> colorCombo = new JComboBox<>(new String[]{"Type", "Pigtail"});
        colorCombo.addActionListener(e -> {
            colorMode = "Pigtail".equals(colorCombo.getSelectedItem()) ? ColorMode.PIGTAIL : ColorMode.TYPE;
            for (ConnectorImagePanel p : activeImagePanels) p.setColorMode(colorMode);
        });

        JPanel northPanel = new JPanel(new BorderLayout());
        northPanel.setBorder(BorderFactory.createEmptyBorder(2, 4, 2, 4));
        northPanel.add(statusLabel, BorderLayout.CENTER);

        JPanel colorToolbar = new JPanel(new FlowLayout(FlowLayout.RIGHT, 4, 0));
        colorToolbar.add(new JLabel("Color by:"));
        colorToolbar.add(colorCombo);
        northPanel.add(colorToolbar, BorderLayout.EAST);

        content.add(northPanel, BorderLayout.NORTH);

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
        activeImagePanels.clear();
        setCenterPanel(null);
    }

    private void showBoard(String signature) {
        if (signature == null) {
            statusLabel.setText("Connected — board unknown");
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }
        RusEfiSignature parsed = SignatureHelper.parse(signature);
        if (parsed == null) {
            statusLabel.setText("Connected — unrecognized signature: " + signature);
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }
        String boardKey = parsed.getBundleTarget();
        statusLabel.setText("Board: " + boardKey);

        if (boardsData == null) {
            statusLabel.setText("Board: " + boardKey + "  [pinout data not found — expected: " + new File(BOARDS_META).getAbsolutePath() + "]");
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }

        List<String> connectorFiles = boardsData.get(boardKey);
        if (connectorFiles == null) {
            statusLabel.setText("Board: " + boardKey + "  [no pinout available]");
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }
        buildConnectorTabs(connectorFiles);
    }

    private void buildConnectorTabs(List<String> connectorPaths) {
        activeImagePanels.clear();

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

        Map<String, String> tuneUseMap = buildTuneUseMap();

        JTabbedPane tabs = new JTabbedPane();
        for (ConnectorData cd : connectors) {
            tabs.addTab(cd.title, buildConnectorPanel(cd, tuneUseMap));
        }
        setCenterPanel(tabs);
    }

    private JPanel buildConnectorPanel(ConnectorData cd, Map<String, String> tuneUseMap) {
        DefaultTableModel model = new DefaultTableModel(COLUMNS, 0) {
            @Override
            public boolean isCellEditable(int row, int column) { return false; }
        };
        // ts_name is at index 4; tune use goes at index 6
        for (Object[] row : cd.rows) {
            String tsName = row[4] != null ? row[4].toString() : "";
            row[6] = tuneUseMap.getOrDefault(tsName, "");
            model.addRow(row);
        }

        JTable table = new JTable(model);
        table.setAutoCreateRowSorter(true);
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        JPanel panel = new JPanel(new BorderLayout());

        if (cd.image != null) {
            ConnectorImagePanel imagePanel = new ConnectorImagePanel();
            imagePanel.setData(cd.image, cd.coords);
            imagePanel.setColorMode(colorMode);
            activeImagePanels.add(imagePanel);

            // Map pin name → model row index
            Map<String, Integer> pinToRow = new HashMap<>();
            for (int i = 0; i < cd.rows.size(); i++) {
                Object v = cd.rows.get(i)[0];
                if (v != null) pinToRow.put(v.toString(), i);
            }

            // Image click → select table row
            imagePanel.setPinClickListener(pin -> {
                Integer row = pinToRow.get(pin);
                if (row != null) {
                    int viewRow = table.convertRowIndexToView(row);
                    table.setRowSelectionInterval(viewRow, viewRow);
                    table.scrollRectToVisible(table.getCellRect(viewRow, 0, true));
                }
            });

            // Table row selection → highlight image marker
            table.getSelectionModel().addListSelectionListener(e -> {
                if (e.getValueIsAdjusting()) return;
                int viewRow = table.getSelectedRow();
                if (viewRow < 0) { imagePanel.setHighlightedPin(null); return; }
                int modelRow = table.convertRowIndexToModel(viewRow);
                Object v = model.getValueAt(modelRow, 0);
                imagePanel.setHighlightedPin(v != null ? v.toString() : null);
            });

            JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, imagePanel, new JScrollPane(table));
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

        // Build pin name → {type, wireColor} from top-level pins array
        Map<String, String[]> pinMeta = new HashMap<>();
        List<Object[]> rows = new ArrayList<>();
        List<Map<String, Object>> pins = (List<Map<String, Object>>) doc.get("pins");
        if (pins != null) {
            for (Map<String, Object> pin : pins) {
                String pinName = str(pin.get("pin"));
                String type    = str(pin.get("type"));
                String color   = str(pin.get("color"));
                if (!pinName.isEmpty()) pinMeta.put(pinName, new String[]{type, color});
                rows.add(new Object[]{
                        pinName,
                        str(pin.get("function")),
                        type,
                        classStr(pin.get("class")),
                        str(pin.get("ts_name")),
                        color,
                        "",  // "Tune use" — filled in by buildConnectorPanel
                });
            }
        }

        // info section: title, image file, pin coordinates
        Map<String, Object> info = (Map<String, Object>) doc.get("info");
        String title = yamlPath;
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
                    String dir = yamlPath.substring(0, yamlPath.lastIndexOf('/') + 1);
                    image = loadImageFromZip(zip, dir + imageFile);
                }
            }

            List<Map<String, Object>> coordList = (List<Map<String, Object>>) info.get("pins");
            if (coordList != null) {
                for (Map<String, Object> c : coordList) {
                    String pin = str(c.get("pin"));
                    if (pin.isEmpty()) continue;
                    int x = toInt(c.get("x"));
                    int y = toInt(c.get("y"));
                    String[] meta = pinMeta.getOrDefault(pin, new String[]{"", ""});
                    coords.add(new PinCoord(pin, x, y, meta[0], meta[1]));
                }
            }
        }

        return new ConnectorData(title, image, coords, rows);
    }

    /**
     * Builds a map from TunerStudio pin name (ts_name) to the human-readable label(s) of
     * any tune field currently assigned to that pin.
     * Fields are identified by name ending in "pin" or "pins" (case-insensitive), matching
     * the convention used throughout rusEFI for pin-selector enum fields.
     */
    private Map<String, String> buildTuneUseMap() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (bp == null || ini == null) return Collections.emptyMap();
        ConfigurationImage ci = bp.getControllerConfiguration();
        if (ci == null) return Collections.emptyMap();

        Map<String, DialogModel.Field> fieldsInOrder = ini.getFieldsInUiOrder();
        Map<String, String> result = new HashMap<>();

        for (Map.Entry<String, IniField> entry : ini.getAllIniFields().entrySet()) {
            IniField field = entry.getValue();
            if (!(field instanceof EnumIniField)) continue;
            String key = entry.getKey();
            String keyLower = key.toLowerCase();
            if (!keyLower.endsWith("pin") && !keyLower.endsWith("pins")) continue;

            String rawValue;
            try {
                rawValue = ConfigurationImageGetterSetter.getStringValue(field, ci);
            } catch (Exception ignored) {
                continue;
            }
            String value = rawValue.replace("\"", "").trim();
            if (value.isEmpty() || "NONE".equalsIgnoreCase(value)) continue;

            DialogModel.Field meta = fieldsInOrder.get(key);
            String displayName = (meta != null && meta.getUiName() != null && !meta.getUiName().isEmpty())
                    ? meta.getUiName()
                    : key;

            result.merge(value, displayName, (a, b) -> a + ", " + b);
        }
        return result;
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
        try (InputStream is = Files.newInputStream(metaFile.toPath())) {
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
