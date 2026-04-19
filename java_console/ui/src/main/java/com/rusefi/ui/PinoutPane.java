package com.rusefi.ui;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.FileUtil;
import com.rusefi.core.SignatureHelper;
import com.rusefi.core.RusEfiSignature;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.net.PropertiesHolder;
import com.rusefi.core.ui.AutoupdateUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.ui.util.PinColors;
import com.rusefi.ui.util.YamlUtil;
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
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.*;
import java.util.List;
import java.util.function.Consumer;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import static com.devexperts.logging.Logging.getLogging;

import static com.rusefi.ui.util.PinColors.COLOR_HIGHLIGHT;
import static com.rusefi.ui.util.PinColors.FALLBACK_NORMAL;

/**
 * Displays ECU connector pinouts for the currently connected board.
 * Board is identified from the ECU signature (bundleTarget field).
 * Data is loaded from pinouts_raw/boards_meta.yaml and pinouts_raw/connectors.zip. if not custom metaname/path are loaded
 * Each connector is shown as a tab with an interactive image (pin markers) above a table.
 * Pin marker color can be switched between type-based and pigtail wire color.
 */
public class PinoutPane {
    private static final Logging log = getLogging(PinoutPane.class);

    private static final String PINOUTS_DIR = "pinouts_raw";
    private static final String[] COLUMNS = {"Pin", "Function", "Type", "Class", "TS Name", "Pigtail color", "Tune use"};

    private static final String PINOUT_CACHE_DIR = FileUtil.RUSEFI_SETTINGS_FOLDER + "pinouts" + File.separator;

    private static String pinoutBaseUrl() {
        return PropertiesHolder.getPinoutBaseUrl();
    }

    private static String pinoutMetaName() {
        return PropertiesHolder.getPinoutMetaName();
    }

    private static String boardsMetaRelativePath() {
        return PINOUTS_DIR + "/" + pinoutMetaName();
    }

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel statusLabel = new JLabel("Not connected", SwingConstants.CENTER);
    private JComponent centerPanel;

    private Map<String, Map<String, Object>> boardsData;
    private final UIContext uiContext;
    /** All image panels currently displayed — updated when a board's tabs are built. */
    private final List<ConnectorImagePanel> activeImagePanels = new ArrayList<>();
    /** All table models currently displayed — updated alongside activeImagePanels. */
    private final List<DefaultTableModel> activeTableModels = new ArrayList<>();
    // ---- Color mode ----

    enum ColorMode { TYPE, PIGTAIL }
    private ColorMode colorMode = ColorMode.TYPE;

    // ---- Legend panel  ----

    private static class LegendPanel extends JPanel {
        private static final int SWATCH  = 12;
        private static final int ROW_H   = 18;
        private static final int PAD_X   = 8;
        private static final int PAD_Y   = 6;
        private static final int GAP     = 5;

        LegendPanel() {
            setBorder(BorderFactory.createMatteBorder(0, 1, 0, 0, UIManager.getColor("Separator.foreground")));
        }

        @Override
        public Dimension getPreferredSize() {
            FontMetrics fm = getFontMetrics(getFont().deriveFont(Font.PLAIN, 10f));
            int maxW = 0;
            for (PinColors.TypeEntry e : PinColors.TYPE_ENTRIES) maxW = Math.max(maxW, fm.stringWidth(e.keyword));
            int w = PAD_X * 2 + SWATCH + GAP + maxW + 4;
            int h = PAD_Y * 2 + 16 + PinColors.TYPE_ENTRIES.size() * ROW_H;
            return new Dimension(Math.max(w, 80), h);
        }

        @Override
        public Dimension getMinimumSize() { return getPreferredSize(); }
        @Override
        public Dimension getMaximumSize() { return new Dimension(getPreferredSize().width, Integer.MAX_VALUE); }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,      RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            Font titleFont = getFont().deriveFont(Font.BOLD, 11f);
            g2.setFont(titleFont);
            FontMetrics titleFm = g2.getFontMetrics();
            g2.setColor(getForeground());
            g2.drawString("Type", PAD_X, PAD_Y + titleFm.getAscent());

            Font rowFont = getFont().deriveFont(Font.PLAIN, 10f);
            g2.setFont(rowFont);
            FontMetrics rowFm = g2.getFontMetrics();

            int y = PAD_Y + titleFm.getHeight() + 2;
            for (PinColors.TypeEntry e : PinColors.TYPE_ENTRIES) {
                int swatchY = y + (ROW_H - SWATCH) / 2;

                // Swatch
                g2.setColor(e.color);
                g2.fillRoundRect(PAD_X, swatchY, SWATCH, SWATCH, 4, 4);
                g2.setColor(Color.GRAY);
                g2.setStroke(new BasicStroke(0.5f));
                g2.drawRoundRect(PAD_X, swatchY, SWATCH, SWATCH, 4, 4);

                // Label
                g2.setColor(getForeground());
                g2.drawString(e.keyword, PAD_X + SWATCH + GAP,
                        y + (ROW_H + rowFm.getAscent() - rowFm.getDescent()) / 2);

                y += ROW_H;
            }
            g2.dispose();
        }
    }

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

        private Color[] markerFillColors(PinCoord c) {
            if (c.pin.equals(highlightedPin)) return new Color[]{COLOR_HIGHLIGHT};
            if (colorMode == ColorMode.PIGTAIL) {
                Color[] cols = PinColors.pigtailToColors(c.wireColor);
                if (cols != null) {
                    Color[] result = new Color[cols.length];
                    for (int i = 0; i < cols.length; i++) {
                        result[i] = new Color(cols[i].getRed(), cols[i].getGreen(), cols[i].getBlue(), 200);
                    }
                    return result;
                }
                return new Color[]{FALLBACK_NORMAL};
            }
            Color base = PinColors.typeToColor(c.type);
            Color fill = base != null ? new Color(base.getRed(), base.getGreen(), base.getBlue(), 200) : FALLBACK_NORMAL;
            return new Color[]{fill};
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
                int r  = MARKER_RADIUS;
                boolean hl = c.pin.equals(highlightedPin);

                Color[] fills = markerFillColors(c);
                Shape savedClip = g2.getClip();

                if (fills.length == 2) {
                    // Left half — color[0]
                    g2.clip(new Rectangle(sx - r, sy - r, r, r * 2));
                    g2.setColor(fills[0]);
                    g2.fillOval(sx - r, sy - r, r * 2, r * 2);
                    g2.setClip(savedClip);
                    // Right half — color[1]
                    g2.clip(new Rectangle(sx, sy - r, r, r * 2));
                    g2.setColor(fills[1]);
                    g2.fillOval(sx - r, sy - r, r * 2, r * 2);
                    g2.setClip(savedClip);
                } else {
                    g2.setColor(fills[0]);
                    g2.fillOval(sx - r, sy - r, r * 2, r * 2);
                }

                // Border: white normally, bright orange when highlighted
                g2.setColor(hl ? Color.ORANGE : Color.WHITE);
                g2.setStroke(new BasicStroke(hl ? 2f : 1f));
                g2.drawOval(sx - r, sy - r, r * 2, r * 2);

                // Pin label: pick contrast color from the first (dominant) fill
                String label = c.pin;
                int textW = fm.stringWidth(label);
                int textX = sx - textW / 2;
                int textY = sy + fm.getAscent() / 2 - 1;

                Color textColor = PinColors.isLight(fills[0]) ? Color.BLACK : Color.WHITE;
                Color shadowColor = PinColors.isLight(fills[0]) ? new Color(200, 200, 200, 120) : new Color(0, 0, 0, 120);

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

        Thread updater = new Thread(this::checkAndUpdatePinoutData, "pinout-updater");
        updater.setDaemon(true);
        updater.start();

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

        uiContext.addConfigImageListener(ci -> SwingUtilities.invokeLater(() -> refreshTuneUse(ci)));
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
        activeTableModels.clear();
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
            statusLabel.setText("Board: " + boardKey + "  [pinout data not found — expected: " + new File(boardsMetaRelativePath()).getAbsolutePath() + "]");
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }

        Map<String, Object> boardEntry = boardsData.get(boardKey);
        if (boardEntry == null) {
            statusLabel.setText("Board: " + boardKey + "  [no pinout available]");
            activeImagePanels.clear();
            setCenterPanel(null);
            return;
        }
        @SuppressWarnings("unchecked")
        List<String> connectorFiles = (List<String>) boardEntry.get("files");
        String zipName = boardEntry.get("zip_file") instanceof String
                ? (String) boardEntry.get("zip_file") : "connectors.zip";
        buildConnectorTabs(connectorFiles, zipName);
    }

    private void buildConnectorTabs(List<String> connectorPaths, String zipName) {
        activeImagePanels.clear();
        activeTableModels.clear();

        File zipFile = findFile(PINOUTS_DIR + "/" + zipName);
        if (zipFile == null) {
            setCenterPanel(new JLabel(zipName + " not found", SwingConstants.CENTER));
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
        // ts_name is at index 4, pin at index 0, tune use at index 6.
        // ts_names may contain "___" as a placeholder for the pin ID (e.g. "___ injector output 1"
        // on pin B6 → ini enum value "B6 injector output 1"), so expand before lookup.
        for (Object[] row : cd.rows) {
            String pinName = row[0] != null ? row[0].toString() : "";
            String tsName  = row[4] != null ? row[4].toString() : "";
            String iniKey  = tsName.replace("___", pinName).trim();
            row[6] = tuneUseMap.getOrDefault(iniKey, "");
            model.addRow(row);
        }

        activeTableModels.add(model);

        JTable table = new JTable(model);
        table.setAutoCreateRowSorter(true);
        table.getTableHeader().setReorderingAllowed(false);
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        packColumns(table);

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

            LegendPanel legend = new LegendPanel();

            JPanel imageRow = new JPanel(new BorderLayout());
            imageRow.add(imagePanel, BorderLayout.CENTER);
            imageRow.add(legend, BorderLayout.EAST);

            JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT, imageRow, new JScrollPane(table));
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
                String pinName = YamlUtil.toStr(pin.get("pin"));
                String type    = YamlUtil.toStr(pin.get("type"));
                String color   = YamlUtil.toStr(pin.get("color"));
                if (!pinName.isEmpty()) pinMeta.put(pinName, new String[]{type, color});
                rows.add(new Object[]{
                        pinName,
                        YamlUtil.toStr(pin.get("function")),
                        type,
                        YamlUtil.toCommaSeparated(pin.get("class")),
                        // original yaml has ___ has placeholder for the pin
                        YamlUtil.toStr(pin.get("ts_name")).replace("___", pinName).trim(),
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
            String t = YamlUtil.toStr(info.get("name"));
            if (t.isEmpty()) t = YamlUtil.toStr(info.get("title"));
            if (!t.isEmpty()) title = t;

            Map<String, Object> imageInfo = (Map<String, Object>) info.get("image");
            if (imageInfo != null) {
                String imageFile = YamlUtil.toStr(imageInfo.get("file"));
                if (!imageFile.isEmpty()) {
                    String dir = yamlPath.substring(0, yamlPath.lastIndexOf('/') + 1);
                    image = loadImageFromZip(zip, dir + imageFile);
                }
            }

            List<Map<String, Object>> coordList = (List<Map<String, Object>>) info.get("pins");
            if (coordList != null) {
                for (Map<String, Object> c : coordList) {
                    String pin = YamlUtil.toStr(c.get("pin"));
                    if (pin.isEmpty()) continue;
                    int x = YamlUtil.toInt(c.get("x"));
                    int y = YamlUtil.toInt(c.get("y"));
                    String[] meta = pinMeta.getOrDefault(pin, new String[]{"", ""});
                    coords.add(new PinCoord(pin, x, y, meta[0], meta[1]));
                }
            }
        }

        return new ConnectorData(title, image, coords, rows);
    }

    /** Updates only the "Tune use" column in every active table without rebuilding the tabs. */
    private void refreshTuneUse(ConfigurationImage ci) {
        if (activeTableModels.isEmpty()) return;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null || ci == null) return;
        Map<String, String> tuneUseMap = buildTuneUseMap(ini, ci);
        int tuneUseCol = COLUMNS.length - 1; // last column
        for (DefaultTableModel model : activeTableModels) {
            for (int row = 0; row < model.getRowCount(); row++) {
                String pinName = YamlUtil.toStr(model.getValueAt(row, 0));
                String tsName  = YamlUtil.toStr(model.getValueAt(row, 4));
                String iniKey  = tsName.replace("___", pinName).trim();
                model.setValueAt(tuneUseMap.getOrDefault(iniKey, ""), row, tuneUseCol);
            }
        }
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
        return buildTuneUseMap(ini, ci);
    }

    /**
     * Builds a reverse map: expanded ts_name → human-readable label(s) of the tune field(s)
     * currently assigned to that pin. Fields are matched by name pattern .*pins?\d*
     * (rusEFI convention: mainRelayPin, injectionPins1, ignitionPins12, fuelPumpPin, etc.).
     */
    private static Map<String, String> buildTuneUseMap(IniFileModel ini, ConfigurationImage ci) {
        Map<String, DialogModel.Field> fieldsInOrder = ini.getFieldsInUiOrder();
        Map<String, String> result = new HashMap<>();

        for (Map.Entry<String, IniField> entry : ini.getAllIniFields().entrySet()) {
            IniField field = entry.getValue();
            if (!(field instanceof EnumIniField)) continue;
            String key = entry.getKey();
            if (!key.toLowerCase().matches(".*pins?\\d*")) continue;

            String rawValue;
            try {
                rawValue = ConfigurationImageGetterSetter.getStringValue(field, ci);
            } catch (Exception ignored) {
                continue;
            }
            String value = rawValue.replace("\"", "").trim();
            if (value.isEmpty() || "NONE".equalsIgnoreCase(value) || "INVALID".equalsIgnoreCase(value)) continue;

            DialogModel.Field meta = fieldsInOrder.get(key);
            String displayName = (meta != null && meta.getUiName() != null && !meta.getUiName().isEmpty())
                    ? meta.getUiName()
                    : key;

            result.merge(value, displayName, (a, b) -> a + ", " + b);
        }
        return result;
    }

    /**
     * Sets each column's preferred width to the widest of its header and cell content.
     */
    private static void packColumns(JTable table) {
        for (int col = 0; col < table.getColumnCount(); col++) {
            javax.swing.table.TableColumn column = table.getColumnModel().getColumn(col);
            javax.swing.table.TableCellRenderer hr = column.getHeaderRenderer();
            if (hr == null) hr = table.getTableHeader().getDefaultRenderer();
            int width = hr.getTableCellRendererComponent(
                    table, column.getHeaderValue(), false, false, -1, col)
                    .getPreferredSize().width;
            for (int row = 0; row < table.getRowCount(); row++) {
                width = Math.max(width,
                        table.prepareRenderer(table.getCellRenderer(row, col), row, col)
                             .getPreferredSize().width);
            }
            column.setPreferredWidth(width + 6);
        }
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
    private Map<String, Map<String, Object>> loadBoardsMeta() {
        File metaFile = findFile(boardsMetaRelativePath());
        log.info("loadBoardsMeta: metaFile=" + (metaFile != null ? metaFile.getAbsolutePath() : "not found"));
        if (metaFile == null) return null;
        try (InputStream is = Files.newInputStream(metaFile.toPath())) {
            Map<String, Object> root = new Yaml().load(is);
            return (Map<String, Map<String, Object>>) root.get("data");
        } catch (IOException e) {
            log.warn("loadBoardsMeta failed: " + e);
            return null;
        }
    }

    private static File findFile(String relativePath) {
        File f = new File(relativePath);
        if (f.exists()) return f;
        f = new File("../" + relativePath);
        if (f.exists()) return f;
        // Check the pinout cache in ~/.rusEFI/pinouts/
        f = new File(PINOUT_CACHE_DIR, new File(relativePath).getName());
        if (f.exists()) return f;
        return null;
    }

    // ---- Remote update ----

    private void checkAndUpdatePinoutData() {
        File cacheDir = new File(PINOUT_CACHE_DIR);
        log.info("Pinout cache dir: " + cacheDir.getAbsolutePath());
        cacheDir.mkdirs();
        File cachedYaml = new File(cacheDir, pinoutMetaName());

        String remoteMetaPath = boardsMetaRelativePath();
        log.info("Fetching remote pinout yaml from " + pinoutBaseUrl() + remoteMetaPath);
        String remoteYaml;
        try {
            remoteYaml = downloadText(remoteMetaPath);
            log.info("Remote pinout yaml fetched, length=" + remoteYaml.length());
        } catch (IOException e) {
            log.warn("Could not fetch remote pinout metadata: " + e, e);
            return;
        }

        // zip_file -> sha from the remote yaml
        Map<String, String> remoteZipShas = extractZipShas(remoteYaml);
        log.info("Remote zip/sha entries: " + remoteZipShas);
        if (remoteZipShas.isEmpty()) {
            log.warn("Remote pinout yaml has no zip/sha entries");
            return;
        }

        // zip_file -> sha from the local cached yaml (if present)
        Map<String, String> localZipShas = new HashMap<>();
        if (cachedYaml.exists()) {
            try {
                String localYaml = new String(Files.readAllBytes(cachedYaml.toPath()), StandardCharsets.UTF_8);
                localZipShas = extractZipShas(localYaml);
                log.info("Local cached zip/sha entries: " + localZipShas);
            } catch (IOException e) {
                log.warn("Could not read local pinout yaml: " + e);
            }
        } else {
            log.info("No local cached yaml found at " + cachedYaml.getAbsolutePath());
        }

        // Save the updated yaml first so that on the next run we don't re-download zips
        // that were already fetched (presence check handles partially-failed downloads)
        try {
            Files.write(cachedYaml.toPath(), remoteYaml.getBytes(StandardCharsets.UTF_8));
            log.info("Saved pinout yaml to " + cachedYaml.getAbsolutePath());
        } catch (IOException e) {
            log.warn("Could not save pinout yaml: " + e, e);
            return;
        }

        for (Map.Entry<String, String> entry : remoteZipShas.entrySet()) {
            String zipName = entry.getKey();
            String remoteSha = entry.getValue();
            File cachedZip = new File(cacheDir, zipName);
            String localSha = localZipShas.get(zipName);
            log.info("Zip " + zipName + ": remoteSha=" + remoteSha + " localSha=" + localSha + " cachedZipExists=" + cachedZip.exists());
            if (remoteSha.equals(localSha) && cachedZip.exists()) {
                log.info("Pinout zip " + zipName + " is up to date");
                continue;
            }
            log.info("Downloading pinout zip " + zipName + " to " + cachedZip.getAbsolutePath());
            try {
                ConnectionAndMeta meta = new ConnectionAndMeta("pinouts_raw/" + zipName).invoke(pinoutBaseUrl());
                log.info("Zip " + zipName + " remote size=" + meta.getCompleteFileSize());
                AutoupdateUtil.downloadAutoupdateFile(cachedZip.getAbsolutePath(), meta, "Updating pinout data: " + zipName);
                log.info("Pinout zip " + zipName + " downloaded successfully");
            } catch (IOException e) {
                log.warn("Could not download pinout zip " + zipName + ": " + e, e);
            }
        }

        log.info("Pinout update complete, scheduling UI refresh");
        SwingUtilities.invokeLater(this::reloadAndRefresh);
    }

    private void reloadAndRefresh() {
        boardsData = loadBoardsMeta();
        log.info("reloadAndRefresh: boardsData=" + (boardsData != null ? boardsData.size() + " boards" : "null")
            + " connected=" + ConnectionStatusLogic.INSTANCE.isConnected());
        if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            showBoard(bp != null ? bp.signature : null);
        }
    }

    private static String downloadText(String remotePath) throws IOException {
        ConnectionAndMeta meta = new ConnectionAndMeta(remotePath).invoke(pinoutBaseUrl());
        try (InputStream in = meta.getHttpConnection().getInputStream()) {
            return new String(in.readAllBytes(), StandardCharsets.UTF_8);
        }
    }

    /** Returns a map of zip_file name → sha extracted from the yaml, deduplicating by zip name. */
    @SuppressWarnings("unchecked")
    private static Map<String, String> extractZipShas(String yamlText) {
        Map<String, String> result = new LinkedHashMap<>();
        if (yamlText == null) return result;
        try {
            Map<String, Object> root = new Yaml().load(yamlText);
            log.info("extractZipShas: root keys=" + root.keySet());
            Object dataObj = root.get("data");
            log.info("extractZipShas: data type=" + (dataObj != null ? dataObj.getClass().getName() : "null"));
            Map<String, Map<String, Object>> data = (Map<String, Map<String, Object>>) dataObj;
            if (data == null) return result;
            // Log the first board entry to check field types
            Map.Entry<String, Map<String, Object>> firstEntry = data.entrySet().iterator().next();
            log.info("extractZipShas: first board=" + firstEntry.getKey() + " fields=" + firstEntry.getValue());
            for (Map<String, Object> board : data.values()) {
                Object zipFile = board.get("zip_file");
                Object sha = board.get("sha");
                if (zipFile instanceof String && sha instanceof String)
                    result.putIfAbsent((String) zipFile, (String) sha);
            }
        } catch (Exception e) {
            log.warn("extractZipShas failed: " + e, e);
        }
        return result;
    }
}
