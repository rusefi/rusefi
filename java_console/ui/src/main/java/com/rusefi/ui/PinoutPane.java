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
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.ui.util.YamlUtil;
import com.formdev.flatlaf.FlatClientProperties;
import com.formdev.flatlaf.icons.FlatSearchIcon;
import com.rusefi.ui.widgets.JoinedButtons;
import com.rusefi.ui.widgets.SegmentedControl;
import com.rusefi.ui.widgets.ToolbarStrip;
import com.rusefi.ui.widgets.tune.TuningStyle;
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
import java.util.function.BiConsumer;
import java.util.function.Consumer;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Displays ECU connector pinouts for the currently connected board.
 *
 * <p>Board connector YAML is the source of pin names and capabilities, colors, and images. For
 * example, uaEFI uses {@code firmware/config/boards/hellen/uaefi/connectors}. The metadata from
 * those files is processed by {@code .github/workflows/upload-pinouts-raw.yaml}.</p>
 *
 * <p>Published metadata lives at
 * {@code https://rusefi.com/docs/pinouts_raw/boards_meta.yaml}. Each metadata entry maps an ECU
 * bundle target to its YAML files, {@code zip_file}, and SHA-256; archives live at
 * {@code /pinouts_raw/<zip_file>}.</p>
 *
 * <p>The Console selects the target from the ECU or offline-tune signature, verifies the archive,
 * then renders the pinouts and tune assignments. A toolbar switch picks the connector, shown as
 * its image with a marker on each pin ({@link PinoutDrawing}) above a table. Markers are coloured
 * by pin type, pigtail wire colour or tune use [tag:console_theme].</p>
 */
public class PinoutPane {
    private static final Logging log = getLogging(PinoutPane.class);

    private static final String PINOUTS_DIR = "pinouts_raw";

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
    private final JLabel statusLabel = new JLabel("Not connected");
    private JComponent centerPanel;

    private Map<String, Map<String, Object>> boardsData;
    private final UIContext uiContext;
    private final Set<String> pinoutDownloads = Collections.synchronizedSet(new HashSet<>());
    /** The connectors of the board shown, in switch order. */
    private final List<PinoutConnectorPage> activeConnectors = new ArrayList<>();
    /** Finds pins across every connector of the board. */
    final JTextField search = new JTextField(22);
    /** "Connector C  ·  20", the switch's text when not searching. */
    private final List<String> connectorLabels = new ArrayList<>();
    /** Holds one page per connector; {@link #highlightByEnumValue} switches between them. */
    private JPanel connectorPages;
    private final CardLayout connectorCards = new CardLayout();
    /** The connector switch in the toolbar; rebuilt for each board. */
    private final JPanel connectorSwitchHolder = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
    private final List<JToggleButton> connectorButtons = new ArrayList<>();
    /** Callback fired on double-click of a pin row that has a tune field currently assigned to it. Args: dialogKey, fieldKey. */
    private BiConsumer<String, String> navigateToTune;
    /** Latest config image observed via uiContext.addConfigImageListener — reflects unburned Tune edits. Prefer over BinaryProtocol.getControllerConfiguration(). */
    private ConfigurationImage liveConfigImage;
    private PinoutDrawing.ColorMode colorMode = PinoutDrawing.ColorMode.TYPE;
    private final Map<PinoutDrawing.ColorMode, JToggleButton> colorModeSegments = new EnumMap<>(PinoutDrawing.ColorMode.class);

    // ---- Data models ----

    private static class ConnectorData {
        final String title;
        final BufferedImage image;
        final List<PinoutPin> pins;

        ConnectorData(String title, BufferedImage image, List<PinoutPin> pins) {
            this.title = title;
            this.image = image;
            this.pins = pins;
        }
    }

    // ---- Main pane ----

    public PinoutPane(UIContext uiContext) {
        this(uiContext, true);
    }

    /** Tests can supply a connector archive without board discovery or background downloads. */
    PinoutPane(UIContext uiContext, boolean startBoardUpdates) {
        this.uiContext = uiContext;
        if (startBoardUpdates) {
            boardsData = loadBoardsMeta();

            Thread updater = new Thread(this::checkAndUpdatePinoutData, "pinout-updater");
            updater.setDaemon(true);
            updater.start();
        }

        content.add(createToolbar(), BorderLayout.NORTH);
        setCenterPanel(null);

        if (startBoardUpdates) {
            ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> SwingUtilities.invokeLater(() -> {
                if (isConnected) {
                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                    String signature = bp != null ? bp.signature : null;
                    showBoard(signature);
                } else if (uiContext.isOfflineMode()) {
                    // [tag:offline_tune] No ECU, but a tune is loaded — show its board pinout + Tune use
                    // from the offline INI signature and image instead of the "Not connected" placeholder.
                    showOfflineBoard();
                } else {
                    showDisconnected();
                }
            }));
        }

        uiContext.addConfigImageListener(ci -> SwingUtilities.invokeLater(() -> {
            liveConfigImage = ci;
            refreshTuneUse(ci);
        }));
    }

    /** Board (or status), the connector switch, and Color by: Type | Pigtail | Tune use. */
    private JComponent createToolbar() {
        statusLabel.setFont(statusLabel.getFont().deriveFont(Font.BOLD));
        JPanel left = new JPanel(new WrapLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP + 4, 0));
        left.setOpaque(false);
        left.add(statusLabel);
        connectorSwitchHolder.setOpaque(false);
        left.add(connectorSwitchHolder);
        search.putClientProperty(FlatClientProperties.PLACEHOLDER_TEXT, "Find a pin, function or tune use");
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_LEADING_ICON, new FlatSearchIcon());
        search.putClientProperty(FlatClientProperties.TEXT_FIELD_SHOW_CLEAR_BUTTON, true);
        search.setToolTipText("Searches every connector · Enter selects the first match");
        search.setPreferredSize(new Dimension(search.getPreferredSize().width, JoinedButtons.HEIGHT));
        search.getDocument().addDocumentListener(new javax.swing.event.DocumentListener() {
            @Override
            public void insertUpdate(javax.swing.event.DocumentEvent e) {
                applySearch();
            }

            @Override
            public void removeUpdate(javax.swing.event.DocumentEvent e) {
                applySearch();
            }

            @Override
            public void changedUpdate(javax.swing.event.DocumentEvent e) {
                applySearch();
            }
        });
        search.addActionListener(e -> selectFirstMatch());
        left.add(search);

        ButtonGroup group = new ButtonGroup();
        JToggleButton type = colorModeSegment("Type", "What each pin is for", PinoutDrawing.ColorMode.TYPE, group);
        JToggleButton pigtail = colorModeSegment("Pigtail", "The wire colour on the pigtail",
                PinoutDrawing.ColorMode.PIGTAIL, group);
        JToggleButton tune = colorModeSegment("Tune use", "Used by the tune, free to use, or fixed (power and ground)",
                PinoutDrawing.ColorMode.TUNE_USE, group);
        type.setSelected(true);
        JLabel colorBy = new JLabel("Color by");
        colorBy.setForeground(UIManager.getColor("Label.disabledForeground"));
        JPanel right = new JPanel(new FlowLayout(FlowLayout.RIGHT, JoinedButtons.ROW_GAP, 0));
        right.setOpaque(false);
        right.add(colorBy);
        right.add(new SegmentedControl(type, pigtail, tune));

        JPanel toolbar = new ToolbarStrip(new BorderLayout(JoinedButtons.ROW_GAP, 0));
        toolbar.setBorder(BorderFactory.createEmptyBorder(8, 12, 9, JoinedButtons.ROW_END_INSET));
        toolbar.add(left, BorderLayout.CENTER);
        toolbar.add(right, BorderLayout.EAST);
        return toolbar;
    }

    private JToggleButton colorModeSegment(String text, String tooltip, PinoutDrawing.ColorMode mode, ButtonGroup group) {
        JToggleButton segment = SegmentedControl.segment(text);
        segment.setToolTipText(tooltip);
        segment.addActionListener(e -> setColorMode(mode));
        group.add(segment);
        colorModeSegments.put(mode, segment);
        return segment;
    }

    void setColorMode(PinoutDrawing.ColorMode mode) {
        colorMode = mode;
        JToggleButton segment = colorModeSegments.get(mode);
        if (segment != null && !segment.isSelected()) {
            segment.setSelected(true);
        }
        for (PinoutConnectorPage page : activeConnectors) {
            page.setColorMode(mode);
        }
    }

    /**
     * Narrows every connector to the pins matching the search, shows each connector's matches on its switch
     * button, and moves to the first connector with a match when the shown one has none.
     */
    void applySearch() {
        String query = search.getText();
        boolean searching = !query.trim().isEmpty();
        int firstWithMatches = -1;
        int[] matches = new int[activeConnectors.size()];
        for (int i = 0; i < activeConnectors.size(); i++) {
            matches[i] = activeConnectors.get(i).setQuery(query);
            if (matches[i] > 0 && firstWithMatches < 0) {
                firstWithMatches = i;
            }
            if (i < connectorButtons.size()) {
                connectorButtons.get(i).setText(!searching ? connectorLabels.get(i)
                        : connectorLabels.get(i).replaceAll("  ·  \\d+$", "") + "  ·  "
                        + (matches[i] == 0 ? "none" : matches[i] + " found"));
            }
        }
        int shown = selectedConnector();
        if (searching && shown >= 0 && matches[shown] == 0 && firstWithMatches >= 0) {
            selectConnector(firstWithMatches);
        }
        connectorSwitchHolder.revalidate();
    }

    /** Enter in the search: selects the first matching pin of the connector shown. */
    void selectFirstMatch() {
        int shown = selectedConnector();
        if (shown < 0) return;
        PinoutConnectorPage page = activeConnectors.get(shown);
        for (PinoutPin pin : page.pins) {
            if (page.isShown(pin)) {
                page.select(pin);
                return;
            }
        }
    }

    public JPanel getContent() {
        return content;
    }

    /** Shows a page, or a centred note repeating the status when there is nothing to show. */
    private void setCenterPanel(JComponent c) {
        if (centerPanel != null) content.remove(centerPanel);
        if (c == null) {
            JLabel note = new JLabel(statusLabel.getText(), SwingConstants.CENTER);
            note.setForeground(UIManager.getColor("Label.disabledForeground"));
            c = note;
            connectorSwitchHolder.removeAll();
            connectorSwitchHolder.revalidate();
        }
        centerPanel = c;
        content.add(c, BorderLayout.CENTER);
        content.revalidate();
        content.repaint();
    }

    private void clearConnectors() {
        activeConnectors.clear();
        connectorButtons.clear();
        connectorLabels.clear();
        connectorPages = null;
    }

    private void showDisconnected() {
        statusLabel.setText("Not connected");
        clearConnectors();
        liveConfigImage = null;
        setCenterPanel(null);
    }

    /** [tag:offline_tune] Renders the board pinout for an offline-loaded tune, identified by the INI signature. */
    private void showOfflineBoard() {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        String signature = ini != null ? ini.getSignature() : null;
        if (signature == null) {
            statusLabel.setText("Offline tune — board unknown");
            clearConnectors();
            setCenterPanel(null);
            return;
        }
        showBoard(signature);
    }

    private void showBoard(String signature) {
        if (signature == null) {
            statusLabel.setText("Connected — board unknown");
            clearConnectors();
            setCenterPanel(null);
            return;
        }
        RusEfiSignature parsed = SignatureHelper.parse(signature);
        if (parsed == null) {
            statusLabel.setText("Connected — unrecognized signature: " + signature);
            clearConnectors();
            setCenterPanel(null);
            return;
        }
        String boardKey = parsed.getBundleTarget();
        statusLabel.setText("Board: " + boardKey);

        if (boardsData == null) {
            statusLabel.setText("Board: " + boardKey + "  [pinout data not found — expected: " + new File(boardsMetaRelativePath()).getAbsolutePath() + "]");
            clearConnectors();
            setCenterPanel(null);
            return;
        }

        Map<String, Object> boardEntry = boardsData.get(boardKey);
        if (boardEntry == null) {
            statusLabel.setText("Board: " + boardKey + "  [no pinout available]");
            clearConnectors();
            setCenterPanel(null);
            return;
        }
        @SuppressWarnings("unchecked")
        List<String> connectorFiles = (List<String>) boardEntry.get("files");
        String zipName = boardEntry.get("zip_file") instanceof String
                ? (String) boardEntry.get("zip_file") : "connectors.zip";
        String expectedSha = YamlUtil.toStr(boardEntry.get("sha"));
        File zipFile = findPinoutZip(zipName);
        if (!PinoutCache.isCurrent(zipFile, expectedSha, connectorFiles)) {
            boolean canUseCachedPinout = PinoutCache.containsEntries(zipFile, connectorFiles);
            statusLabel.setText("Board: " + boardKey + (canUseCachedPinout
                ? "  [updating pinout data]" : "  [downloading pinout data]"));
            if (!canUseCachedPinout) {
                clearConnectors();
                setCenterPanel(new JLabel("Downloading " + zipName + "...", SwingConstants.CENTER));
            }
            requestPinoutZip(boardKey, zipName, expectedSha, connectorFiles);
            if (!canUseCachedPinout) return;
        }
        buildConnectorTabs(connectorFiles, zipFile);
    }

    void buildConnectorTabs(List<String> connectorPaths, File zipFile) {
        clearConnectors();

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

        connectors.sort(Comparator.comparing(cd -> cd.title, String.CASE_INSENSITIVE_ORDER));
        Map<String, String> tuneUseMap = buildTuneUseMap();

        JPanel pages = new JPanel(connectorCards);
        ButtonGroup group = new ButtonGroup();
        for (int i = 0; i < connectors.size(); i++) {
            ConnectorData cd = connectors.get(i);
            pages.add(buildConnectorPanel(cd, tuneUseMap), String.valueOf(i));
            connectorLabels.add(cd.title + "  ·  " + cd.pins.size());
            JToggleButton button = SegmentedControl.segment(connectorLabels.get(i));
            button.setToolTipText(cd.title + ": " + cd.pins.size() + " pins");
            int index = i;
            button.addActionListener(e -> selectConnector(index));
            group.add(button);
            connectorButtons.add(button);
        }
        connectorPages = pages;
        setCenterPanel(pages);
        connectorSwitchHolder.removeAll();
        if (connectors.size() > 1) {
            connectorSwitchHolder.add(new SegmentedControl(connectorButtons.toArray(new JToggleButton[0])));
        }
        connectorSwitchHolder.revalidate();
        selectConnector(0);
        applySearch();
    }

    /** Shows a connector's page and marks it in the switch. */
    void selectConnector(int index) {
        if (connectorPages == null || index < 0 || index >= connectorButtons.size()) return;
        connectorButtons.get(index).setSelected(true);
        connectorCards.show(connectorPages, String.valueOf(index));
    }

    /** The connector shown, or -1. */
    int selectedConnector() {
        for (int i = 0; i < connectorButtons.size(); i++) {
            if (connectorButtons.get(i).isSelected()) return i;
        }
        return -1;
    }

    private JPanel buildConnectorPanel(ConnectorData cd, Map<String, String> tuneUseMap) {
        for (PinoutPin pin : cd.pins) {
            pin.tuneUse = tuneUseMap.getOrDefault(pin.tsName, "");
        }
        PinoutConnectorPage page = new PinoutConnectorPage(cd.title, cd.image, cd.pins, colorMode,
                pin -> navigateToTune == null || pin.tsName.isEmpty() ? null : findPinAssignment(pin.tsName),
                assignment -> navigateToTune.accept(assignment[1], assignment[0]));
        JTable table = page.table;
        // Double-click a row → jump to the tune field currently driving that pin, if any.
        table.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() != 2 || navigateToTune == null) return;
                int viewRow = table.rowAtPoint(e.getPoint());
                if (viewRow < 0) return;
                page.open(page.pins.get(table.convertRowIndexToModel(viewRow)));
            }
        });
        activeConnectors.add(page);
        return page;
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

        List<PinoutPin> pinList = new ArrayList<>();
        Map<String, PinoutPin> byName = new HashMap<>();
        List<Map<String, Object>> pins = (List<Map<String, Object>>) doc.get("pins");
        if (pins != null) {
            for (Map<String, Object> pin : pins) {
                String pinName = YamlUtil.toStr(pin.get("pin"));
                PinoutPin p = new PinoutPin(pinName,
                        YamlUtil.toStr(pin.get("function")),
                        YamlUtil.toStr(pin.get("type")),
                        YamlUtil.toCommaSeparated(pin.get("class")),
                        // original yaml has ___ has placeholder for the pin
                        YamlUtil.toStr(pin.get("ts_name")).replace("___", pinName).trim(),
                        YamlUtil.toStr(pin.get("color")));
                pinList.add(p);
                if (!pinName.isEmpty()) byName.putIfAbsent(pinName, p);
            }
        }

        // info section: title, image file, pin coordinates
        Map<String, Object> info = (Map<String, Object>) doc.get("info");
        String title = yamlPath;
        BufferedImage image = null;

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
                    PinoutPin pin = byName.get(YamlUtil.toStr(c.get("pin")));
                    if (pin == null) continue;
                    pin.x = YamlUtil.toInt(c.get("x"));
                    pin.y = YamlUtil.toInt(c.get("y"));
                }
            }
        }

        if (!title.isEmpty()) {
            title = Character.toUpperCase(title.charAt(0)) + title.substring(1);
        }
        return new ConnectorData(title, image, pinList);
    }

    /** Updates the "Tune use" column, the markers and the legends without rebuilding the connectors. */
    private void refreshTuneUse(ConfigurationImage ci) {
        if (activeConnectors.isEmpty()) return;
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null || ci == null) return;
        Map<String, String> tuneUseMap = buildTuneUseMap(ini, ci);
        for (PinoutConnectorPage page : activeConnectors) {
            page.setTuneUse(tuneUseMap);
        }
        // tune use is searchable
        applySearch();
    }

    public void setNavigateToTune(BiConsumer<String, String> navigateToTune) {
        this.navigateToTune = navigateToTune;
    }

    /**
     * Selects the connector tab + row whose expanded ts_name matches the given enum value,
     * causing the existing selection listener to also highlight the pin marker on the image.
     * No-op if no row matches (e.g. value is NONE, or pin lives on a connector not currently shown).
     */
    public void highlightByEnumValue(String enumValue) {
        if (enumValue == null || connectorPages == null) return;
        String target = enumValue.replace("\"", "").trim();
        if (target.isEmpty() || "NONE".equalsIgnoreCase(target) || "INVALID".equalsIgnoreCase(target)) return;
        for (int i = 0; i < activeConnectors.size(); i++) {
            PinoutConnectorPage page = activeConnectors.get(i);
            for (PinoutPin pin : page.pins) {
                if (target.equals(pin.tsName)) {
                    // the pin must be listed: drop a search, chip or filter that hides it
                    search.setText("");
                    page.showAll();
                    selectConnector(i);
                    page.select(pin);
                    return;
                }
            }
        }
    }

    /**
     * Returns {fieldKey, dialogKey} for whichever pin-enum field currently resolves to {@code iniKey},
     * or null if no field (or dialog) currently holds that assignment.
     */
    private String[] findPinAssignment(String iniKey) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) return null;
        ConfigurationImage ci = currentConfigImage();
        if (ci == null) return null;

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
            if (!iniKey.equals(value)) continue;

            String dialogKey = findDialogForField(ini, key);
            if (dialogKey != null) return new String[]{key, dialogKey};
        }
        return null;
    }

    private static String findDialogForField(IniFileModel ini, String fieldKey) {
        for (DialogModel d : ini.getDialogs().values()) {
            for (DialogModel.Field f : d.getFields()) {
                if (fieldKey.equals(f.getKey())) return d.getKey();
            }
        }
        return null;
    }

    /**
     * Builds a map from TunerStudio pin name (ts_name) to the human-readable label(s) of
     * any tune field currently assigned to that pin.
     * Fields are identified by name ending in "pin" or "pins" (case-insensitive), matching
     * the convention used throughout rusEFI for pin-selector enum fields.
     */
    private Map<String, String> buildTuneUseMap() {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) return Collections.emptyMap();
        ConfigurationImage ci = currentConfigImage();
        if (ci == null) return Collections.emptyMap();
        return buildTuneUseMap(ini, ci);
    }

    /** Prefer the live (possibly unburned) image if we've seen one; otherwise fall back to the last burned image. */
    private ConfigurationImage currentConfigImage() {
        if (liveConfigImage != null) return liveConfigImage;
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        return bp != null ? bp.getControllerConfiguration() : null;
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
        return loadBoardsMeta(findFile(boardsMetaRelativePath()));
    }

    @SuppressWarnings("unchecked")
    private Map<String, Map<String, Object>> loadBoardsMeta(File metaFile) {
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

    private static File findPinoutZip(String zipName) {
        File cached = cachedPinoutFile(zipName);
        if (cached == null) return null;
        if (cached.exists()) return cached;
        return findFile(PINOUTS_DIR + "/" + zipName);
    }

    private static File cachedPinoutFile(String zipName) {
        if (zipName == null || zipName.isEmpty() || !new File(zipName).getName().equals(zipName)) return null;
        return new File(PINOUT_CACHE_DIR, zipName);
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

    private void requestPinoutZip(String boardKey, String zipName, String expectedSha, List<String> connectorFiles) {
        File cachedZip = cachedPinoutFile(zipName);
        if (cachedZip == null) {
            statusLabel.setText("Board: " + boardKey + "  [invalid pinout archive name]");
            setCenterPanel(new JLabel("Invalid pinout archive name", SwingConstants.CENTER));
            return;
        }

        String downloadKey = zipName + "@" + expectedSha;
        if (!pinoutDownloads.add(downloadKey)) return;

        Thread downloader = new Thread(() -> {
            try {
                log.info("Downloading pinout zip " + zipName + " for " + boardKey + " to " + cachedZip.getAbsolutePath());
                ConnectionAndMeta meta = new ConnectionAndMeta("pinouts_raw/" + zipName).invoke(pinoutBaseUrl());
                log.info("Zip " + zipName + " remote size=" + meta.getCompleteFileSize());
                PinoutCache.downloadAndReplace(cachedZip, expectedSha, connectorFiles,
                    destination -> AutoupdateUtil.downloadAutoupdateFile(
                        destination.getAbsolutePath(), meta, "Updating " + boardKey + " pinout"));
                log.info("Pinout zip " + zipName + " downloaded and verified successfully");
                pinoutDownloads.remove(downloadKey);
                SwingUtilities.invokeLater(this::reloadAndRefresh);
            } catch (IOException e) {
                log.warn("Could not download pinout zip " + zipName + " for " + boardKey + ": " + e, e);
                SwingUtilities.invokeLater(() -> {
                    if (isBoardStatus(boardKey)) {
                        if (PinoutCache.containsEntries(findPinoutZip(zipName), connectorFiles)) {
                            statusLabel.setText("Board: " + boardKey + "  [pinout update failed—using cached data]");
                        } else {
                            statusLabel.setText("Board: " + boardKey + "  [pinout download failed]");
                            setCenterPanel(new JLabel("Could not download " + zipName, SwingConstants.CENTER));
                        }
                    }
                });
            } finally {
                pinoutDownloads.remove(downloadKey);
            }
        }, "pinout-zip-" + boardKey);
        downloader.setDaemon(true);
        downloader.start();
    }

    private boolean isBoardStatus(String boardKey) {
        String boardStatus = "Board: " + boardKey;
        String currentStatus = statusLabel.getText();
        return currentStatus.equals(boardStatus) || currentStatus.startsWith(boardStatus + "  ");
    }

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

        // Validate the metadata before replacing the cached copy. The archive named by the
        // selected board is downloaded on demand after its actual SHA-256 has been checked.
        Map<String, String> remoteZipShas = extractZipShas(remoteYaml);
        log.info("Remote zip/sha entries: " + remoteZipShas);
        if (remoteZipShas.isEmpty()) {
            log.warn("Remote pinout yaml has no zip/sha entries");
            return;
        }

        try {
            PinoutCache.writeAndReplace(cachedYaml, remoteYaml);
            log.info("Saved pinout yaml to " + cachedYaml.getAbsolutePath());
        } catch (IOException e) {
            log.warn("Could not save pinout yaml: " + e, e);
            return;
        }

        log.info("Pinout metadata update complete, scheduling UI refresh");
        SwingUtilities.invokeLater(() -> reloadAndRefresh(cachedYaml));
    }

    private void reloadAndRefresh() {
        File cachedYaml = new File(PINOUT_CACHE_DIR, pinoutMetaName());
        reloadAndRefresh(cachedYaml.exists() ? cachedYaml : null);
    }

    private void reloadAndRefresh(File preferredMetaFile) {
        boardsData = preferredMetaFile != null ? loadBoardsMeta(preferredMetaFile) : loadBoardsMeta();
        log.info("reloadAndRefresh: boardsData=" + (boardsData != null ? boardsData.size() + " boards" : "null")
            + " connected=" + ConnectionStatusLogic.INSTANCE.isConnected());
        if (ConnectionStatusLogic.INSTANCE.isConnected()) {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            showBoard(bp != null ? bp.signature : null);
        } else if (uiContext.isOfflineMode()) {
            showOfflineBoard();
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
