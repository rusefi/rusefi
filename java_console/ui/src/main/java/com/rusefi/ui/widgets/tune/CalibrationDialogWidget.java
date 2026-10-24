package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.opensr5.ini.ReadoutModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.OrdinalOutOfRangeException;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorSubscription;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.CircularGauge;
import com.rusefi.ui.laf.GradientTitleBorder;
import com.rusefi.ui.util.ConstrainedBorderLayout;
import com.rusefi.ui.util.TransparentPanels;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.SquareGridLayout;
import com.rusefi.ui.util.SwingUtil;
import com.rusefi.ui.util.WrapLayout;
import com.rusefi.trigger.TriggerImage;
import com.rusefi.trigger.TriggerWheelInfo;
import com.opensr5.ConfigurationImageGetterSetter;
import java.io.File;
import java.text.DecimalFormat;

import com.devexperts.logging.Logging;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Area;
import java.awt.geom.Path2D;
import java.awt.geom.Point2D;
import java.awt.image.BufferedImage;
import java.awt.print.PrinterGraphics;
import java.util.*;
import java.util.Arrays;
import java.util.List;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Orchestrates layout of calibration dialogs by composing UI widgets
 * created by {@link CalibrationFieldFactory}.
 *
 * @see TuningTableView
 */
public class CalibrationDialogWidget {
    private static final Logging log = getLogging(CalibrationDialogWidget.class);

    private final JPanel contentPane = new ScrollablePanel();
    private final DialogViewport viewportPane = new DialogViewport(contentPane);
    private final UIContext uiContext;
    private final Node tableOrientationConfig;
    private final List<Runnable> triggerImageUpdaters = new ArrayList<>();
    private ConfigurationImage workingImage;
    private IniFileModel currentIniFileModel;
    private int fieldRowVerticalMargin = 3;
    private final List<ExpressionRow> expressionRows = new ArrayList<>();
    private final List<IndicatorPanel> indicatorPanels = new ArrayList<>();
    private final List<ReadoutLabelEntry> readoutEntries = new ArrayList<>();
    private final List<GaugeReadoutEntry> gaugeReadoutEntries = new ArrayList<>();
    private final SensorCentral.ResponseListenerToken readoutListenerToken;
    private boolean active;
    private boolean hasReadoutDemand;
    private static final int READOUT_GAUGE_SIZE = 90;
    private static final int MIN_GAUGE_SIZE = 60;
    /** Counts gauge groups so renderPanelEntry can mark panels that contain gauges as growing. */
    private int gaugeGroupsRendered;
    /** Called after each user edit with the current working image, so listeners can re-evaluate their own expressions. */
    private Consumer<ConfigurationImage> onConfigChange;
    /** Called when the user opens Wiring/Pinout from a pin-enum field; arg is the current enum value. */
    private Consumer<String> onShowInPinout;
    /** Restores the last navigated view; set on every update() call so the VE generator panel can return to it. */
    private Runnable currentViewRestorer;
    private String currentKey;
    private Runnable onViewChanged = () -> { };
    private Runnable onTableOrientationChange = () -> { };

    /** Also reports replacements from navigation, undo, discard, and loading another tune. */
    public void setOnViewChanged(Runnable listener) {
        onViewChanged = listener;
    }

    public void setOnTableOrientationChange(Runnable listener) {
        onTableOrientationChange = listener;
    }

    public String getCurrentKey() {
        return currentKey;
    }

    public IniFileModel getCurrentIniFileModel() {
        return currentIniFileModel;
    }

    public void setOnConfigChange(Consumer<ConfigurationImage> onConfigChange) {
        this.onConfigChange = onConfigChange;
    }

    public void setOnShowInPinout(Consumer<String> onShowInPinout) {
        this.onShowInPinout = onShowInPinout;
    }

    public void setFieldRowVerticalMargin(int margin) {
        fieldRowVerticalMargin = margin;
    }

    /**
     * Tracks a field row that has visibility or enabled expressions to re-evaluate on config changes.
     */
    private static class ExpressionRow {
        final JPanel row;
        final String enableExpression;
        final String visibleExpression;

        ExpressionRow(JPanel row, String enableExpression, String visibleExpression) {
            this.row = row;
            this.enableExpression = enableExpression;
            this.visibleExpression = visibleExpression;
        }
    }

    /** Tracks a readout value label for live update from SensorCentral. */
    private static class ReadoutLabelEntry {
        final JLabel titleLabel;
        final JLabel valueLabel;
        final String channel;
        final String units;
        final String title;
        final int valDigits;
        final boolean compact;

        ReadoutLabelEntry(JLabel valueLabel, String channel, String units, int valDigits) {
            this(valueLabel, channel, units, valDigits, false);
        }

        ReadoutLabelEntry(JLabel valueLabel, String channel, String units, int valDigits, boolean compact) {
            this(null, valueLabel, channel, null, units, valDigits, compact);
        }

        ReadoutLabelEntry(JLabel titleLabel, JLabel valueLabel, String channel, String title,
                          String units, int valDigits, boolean compact) {
            this.titleLabel = titleLabel;
            this.valueLabel = valueLabel;
            this.channel = channel;
            this.title = title;
            this.units = units;
            this.valDigits = valDigits;
            this.compact = compact;
        }
    }

    /** Tracks a gauge-ref readout rendered as a small circular gauge for live update. */
    private static class GaugeReadoutEntry {
        final CircularGauge radial;
        final String channel;
        final String gaugeName;
        final boolean hasExpressionLabels;

        GaugeReadoutEntry(CircularGauge radial, String channel, String gaugeName, boolean hasExpressionLabels) {
            this.radial = radial;
            this.channel = channel;
            this.gaugeName = gaugeName;
            this.hasExpressionLabels = hasExpressionLabels;
        }
    }

    /** Reads a field's bytes from a TS page other than the main settings page; delivers null on failure. */
    interface PageReader {
        void read(int pageIndex, int offset, int size, Consumer<byte[]> onResult);
    }

    private PageReader pageReader = this::readFromEcuPage;

    void setPageReaderForUnitTest(PageReader pageReader) {
        this.pageReader = pageReader;
    }

    private void readFromEcuPage(int pageIndex, int offset, int size, Consumer<byte[]> onResult) {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) {
            onResult.accept(null);
            return;
        }
        uiContext.getLinkManager().submit(() -> {
            byte[] data = bp.readFromPage(pageIndex, offset, size);
            SwingUtilities.invokeLater(() -> onResult.accept(data));
        });
    }

    /**
     * The working image holds only the main settings page. Tables whose values live on another
     * TS page (LTFT trims on 0x0200) are read from the ECU and shown read-only: drawing them from
     * the working image would show, and on edit overwrite, main-page settings at the same offsets.
     */
    private JComponent createTableView(IniFileModel iniFileModel, TableModel table, Runnable notifyEdit) {
        Optional<IniField> valuesField = iniFileModel.findIniField(table.getZBinsConstant());
        if (!valuesField.isPresent() || valuesField.get().getPageIndex() == 0) {
            TuningTableView view = new TuningTableView(table.getTitle(), false, tableOrientationConfig);
            view.displayTable(iniFileModel, table.getTableId(), workingImage);
            view.setOnEdit(notifyEdit);
            view.setOnOrientationChange(onTableOrientationChange);
            return view.getContent();
        }

        IniField field = valuesField.get();
        TuningTableView view = new TuningTableView(table.getTitle(), true, tableOrientationConfig);
        view.setOnOrientationChange(onTableOrientationChange);
        JLabel status = new JLabel("Reading from ECU...");
        JPanel panel = new JPanel(new BorderLayout());
        panel.add(status, BorderLayout.NORTH);
        panel.add(view.getContent(), BorderLayout.CENTER);
        ConfigurationImage axisImage = workingImage;
        pageReader.read(field.getPageIndex(), field.getOffset(), field.getSize(), data -> {
            if (data == null) {
                status.setText("Connect to the ECU to view this table");
                return;
            }
            byte[] page = new byte[field.getOffset() + data.length];
            System.arraycopy(data, 0, page, field.getOffset(), data.length);
            view.displayTable(iniFileModel, table.getTableId(), new ConfigurationImage(page), axisImage);
            panel.remove(status);
            panel.revalidate();
            panel.repaint();
        });
        return panel;
    }

    /** Curves on extra TS pages must use those page bytes, never same-offset main settings. */
    private JComponent createCurveView(IniFileModel ini, CurveModel curve, Runnable notifyEdit) {
        IniField xField = ini.findIniField(curve.getxBins()).orElse(null);
        IniField yField = ini.findIniField(curve.getyBins()).orElse(null);
        if (xField == null || yField == null || xField.getPageIndex() == 0 && yField.getPageIndex() == 0) {
            CurveWidget widget = new CurveWidget(curve, ini, workingImage);
            widget.setOnEdit(notifyEdit);
            return widget.getContentPane();
        }
        if (xField.getPageIndex() != yField.getPageIndex()) {
            return new JLabel("Curve bins are on different ECU pages");
        }

        int offset = Math.min(xField.getOffset(), yField.getOffset());
        int end = Math.max(xField.getOffset() + xField.getSize(), yField.getOffset() + yField.getSize());
        for (String label : Arrays.asList(curve.getTitle(), curve.getXLabel(), curve.getYLabel(),
            xField.getUnits(), yField.getUnits())) {
            for (String variable : ExpressionEvaluator.extractVariables(label)) {
                IniField labelField = ini.findIniField(variable).orElse(null);
                if (labelField != null && labelField.getPageIndex() == xField.getPageIndex()) {
                    offset = Math.min(offset, labelField.getOffset());
                    end = Math.max(end, labelField.getOffset() + Math.max(4, labelField.getSize()));
                }
            }
        }
        final int readOffset = offset;
        final int readEnd = end;
        JPanel panel = new JPanel(new BorderLayout());
        JLabel status = new JLabel("Reading from ECU...");
        panel.add(status, BorderLayout.NORTH);
        ConfigurationImage configImage = workingImage;
        pageReader.read(xField.getPageIndex(), readOffset, readEnd - readOffset, data -> {
            if (data == null || data.length != readEnd - readOffset) {
                status.setText("Connect to the ECU to view this curve");
                return;
            }
            byte[] page = new byte[readEnd];
            System.arraycopy(data, 0, page, readOffset, data.length);
            CurveWidget widget = new CurveWidget(curve, ini, new ConfigurationImage(page), configImage, true);
            panel.remove(status);
            panel.add(widget.getContentPane(), BorderLayout.CENTER);
            panel.revalidate();
            panel.repaint();
        });
        return panel;
    }

    public CalibrationDialogWidget(UIContext uiContext) {
        this(uiContext, new Node());
    }

    public CalibrationDialogWidget(UIContext uiContext, Node tableOrientationConfig) {
        this.uiContext = uiContext;
        this.tableOrientationConfig = tableOrientationConfig;
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
        contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
        // Refresh readouts whenever the ECU sends new output-channel data.
        // Indicator panels register their own SensorCentral listeners independently.
        readoutListenerToken = SensorCentral.getInstance().addListener(() -> {
            if (!readoutEntries.isEmpty() || !gaugeReadoutEntries.isEmpty()) {
                SwingUtilities.invokeLater(this::refreshReadouts);
            }
        }, new SensorSubscription());
        readoutListenerToken.setActive(false);
    }

    private static void applyLayout(JPanel panel, String layoutHint) {
        if ("border".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new ConstrainedBorderLayout(4, 4));
        } else if ("xAxis".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
        } else {
            panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        }
    }

    // Keep help outside the INI layout, especially panels already using BorderLayout.NORTH.
    private static JPanel prepareTopicHelp(JPanel container, String reference, IniFileModel ini) {
        JButton help = CalibrationFieldFactory.createTopicHelpButton(ini, reference);
        if (help == null) {
            return container;
        }
        JPanel toolbar = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 4));
        toolbar.add(help);
        JPanel body = new JPanel();
        body.setAlignmentX(Component.LEFT_ALIGNMENT);
        container.setLayout(new BorderLayout());
        container.add(toolbar, BorderLayout.NORTH);
        container.add(body, BorderLayout.CENTER);
        return body;
    }

    private static JComponent withTopicHelp(JComponent content, String reference, IniFileModel ini) {
        JPanel wrapper = new JPanel();
        JPanel body = prepareTopicHelp(wrapper, reference, ini);
        if (body == wrapper) {
            return content;
        }
        body.setLayout(new BorderLayout());
        body.add(content, BorderLayout.CENTER);
        return wrapper;
    }

    public ConfigurationImage getWorkingImage() {
        return workingImage;
    }

    /**
     * Drops any in-progress edits. Called when the ECU disconnects so stale calibrations
     * from the previous board are not carried over to the next connection.
     */
    public void reset() {
        viewportPane.fillWidth = false;
        workingImage = null;
        currentKey = null;
        currentIniFileModel = null;
        CalibrationFieldFactory.closeHelpPopup();
        clearLiveComponents();
        contentPane.removeAll();
        contentPane.revalidate();
        contentPane.repaint();
        onViewChanged.run();
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        currentKey = dialogModel == null ? null : dialogModel.getKey();
        viewportPane.fillWidth = false;
        CalibrationFieldFactory.closeHelpPopup();
        final DialogModel capturedDm = dialogModel;
        final IniFileModel capturedIni = iniFileModel;
        currentViewRestorer = () -> update(capturedDm, capturedIni, workingImage);
        workingImage = ci != null ? ci.clone() : null;
        currentIniFileModel = iniFileModel;
        clearLiveComponents();
        expressionRows.clear();
        triggerImageUpdaters.clear();
        contentPane.removeAll();
        if (dialogModel != null) {
            String uiName = dialogModel.getUiName();
            if (uiName == null || uiName.isEmpty()) {
                uiName = dialogModel.getKey();
            }
            contentPane.setName(uiName);

            JPanel body = prepareTopicHelp(contentPane, dialogModel.getTopicHelp(), iniFileModel);
            applyLayout(body, dialogModel.getLayoutHint());
            contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
            fillPanel(body, dialogModel, iniFileModel, ci,
                getFieldLabelWidth(dialogModel, iniFileModel, new HashSet<>()),
                getFieldEditorWidth(dialogModel, iniFileModel, ci, new HashSet<>()));

            if (TriggerImageHelper.isTriggerPanel(dialogModel.getKey(), uiName)) {
                addTriggerImage(body, dialogModel.getKey(), uiName);
            }
        }
        updateLiveDemand();
        contentPane.revalidate();
        contentPane.repaint();
        // After the initial layout gives children their actual widths,
        // WrapLayout can compute correct wrapped heights on the second pass.
        // invalidateTree is needed because revalidate() only invalidates the
        // component and its ancestors, not children — so children return
        // stale cached preferred sizes computed before wrapping.
        SwingUtilities.invokeLater(() -> {
            SwingUtil.invalidateTree(contentPane);
            contentPane.revalidate();
            contentPane.repaint();
        });
        onViewChanged.run();
    }

    public void update(String key) {
        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage ci = uiContext.getBinaryProtocol().getControllerConfiguration();
        update(key, iniFileModel, ci);
    }

    public void update(String key, IniFileModel iniFileModel, ConfigurationImage ci) {
        currentKey = key;
        currentIniFileModel = iniFileModel;
        viewportPane.fillWidth = false;
        CalibrationFieldFactory.closeHelpPopup();
        final String capturedKey = key;
        final IniFileModel capturedIniForRestore = iniFileModel;
        currentViewRestorer = () -> update(capturedKey, capturedIniForRestore, workingImage);
        clearLiveComponents();
        expressionRows.clear();
        triggerImageUpdaters.clear();
        contentPane.removeAll();
        workingImage = ci != null ? ci.clone() : null;
        if (key != null) {
            DialogModel dialog = iniFileModel.getDialogs().get(key);
            if (dialog != null) {
                update(dialog, iniFileModel, ci);
                return;
            }
            Runnable notifyEdit = () -> { if (onConfigChange != null) onConfigChange.accept(workingImage); };

            TableModel table = iniFileModel.getTable(key);
            if (table != null) {
                viewportPane.fillWidth = true;
                JPanel body = prepareTopicHelp(contentPane, table.getTopicHelp(), iniFileModel);
                body.setLayout(new BoxLayout(body, BoxLayout.Y_AXIS));
                //TODO: nicer injection of this button? maybe a comment on the .ini and then hook this?
                if ("veTableTbl".equals(table.getTableId())) {
                    final IniFileModel capturedIni = iniFileModel;
                    JButton genVeBtn = new JButton("Generate base VE...");
                    genVeBtn.addActionListener(e -> showVeGeneratorPanel(capturedIni));
                    JPanel veToolbar = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 4));
                    veToolbar.add(genVeBtn);
                    body.add(veToolbar);
                }
                body.add(createTableView(iniFileModel, table, notifyEdit));
            } else {
                CurveModel curve = iniFileModel.getCurves().get(key);
                if (curve != null) {
                    viewportPane.fillWidth = true;
                    JPanel body = prepareTopicHelp(contentPane, curve.getTopicHelp(), iniFileModel);
                    body.setLayout(new BoxLayout(body, BoxLayout.Y_AXIS));
                    body.add(createCurveView(iniFileModel, curve, notifyEdit));
                }
            }
        }
        updateLiveDemand();
        contentPane.revalidate();
        contentPane.repaint();
        onViewChanged.run();
    }

    private void fillPanel(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel,
                           ConfigurationImage ci, int fieldLabelWidth, int fieldEditorWidth) {
        Runnable notifyEdit = () -> { if (onConfigChange != null) onConfigChange.accept(workingImage); };

        List<DialogModel.DialogEntry> entries = dialogModel.getOrderedEntries();
        if (entries.isEmpty()) {
            entries = synthesizeOrderedEntries(dialogModel);
        }
        if (entries.isEmpty()) {
            // readoutPanel dialogs: only have readouts (no fields/commands/panels/etc.)
            renderReadouts(container, dialogModel, iniFileModel);
            return;
        }

        boolean isBorderLayout = container.getLayout() instanceof BorderLayout;
        JPanel[] horizontalPanelRef = {null};
        List<IndicatorModel> pendingIndicators = new ArrayList<>();
        List<String> pendingGauges = new ArrayList<>();

        for (DialogModel.DialogEntry entry : entries) {
            // Flush accumulated groups when the run of same-type entries ends
            if (entry.kind != DialogModel.DialogEntry.Kind.INDICATOR && !pendingIndicators.isEmpty()) {
                renderIndicatorGroup(container, pendingIndicators, iniFileModel, ci, dialogModel.getReadoutColumns());
                pendingIndicators.clear();
            }
            if (entry.kind != DialogModel.DialogEntry.Kind.GAUGE && !pendingGauges.isEmpty()) {
                renderGaugeGroup(container, pendingGauges, iniFileModel);
                pendingGauges.clear();
            }
            // Non-panel entries break any ongoing horizontal panel group (for non-border layouts)
            if (entry.kind != DialogModel.DialogEntry.Kind.PANEL && !isBorderLayout) {
                horizontalPanelRef[0] = null;
            }

            switch (entry.kind) {
                case FIELD:
                    renderField(container, entry.getAs(DialogModel.Field.class), iniFileModel, ci,
                        fieldLabelWidth, fieldEditorWidth);
                    break;
                case RUNTIME_VALUE:
                    renderRuntimeValue(container, entry.getAs(DialogModel.Field.class), iniFileModel, ci);
                    break;
                case COMMAND:
                    container.add(CalibrationFieldFactory.createCommandRow(
                        entry.getAs(DialogModel.Command.class),
                        buildCommandExecutor(entry.getAs(DialogModel.Command.class).getCommand())));
                    break;
                case INDICATOR:
                    pendingIndicators.add(entry.getAs(IndicatorModel.class));
                    break;
                case GAUGE:
                    pendingGauges.add(entry.getAs(String.class));
                    break;
                case PANEL:
                    renderPanelEntry(container, entry.getAs(PanelModel.class), iniFileModel, ci,
                            isBorderLayout, horizontalPanelRef, notifyEdit, fieldLabelWidth, fieldEditorWidth);
                    break;
            }
        }

        // Flush any remaining accumulated groups
        if (!pendingIndicators.isEmpty()) {
            renderIndicatorGroup(container, pendingIndicators, iniFileModel, ci, dialogModel.getReadoutColumns());
        }
        if (!pendingGauges.isEmpty()) {
            renderGaugeGroup(container, pendingGauges, iniFileModel);
        }
    }

    private static int getFieldLabelWidth(DialogModel dialog, IniFileModel iniFileModel, Set<String> visited) {
        if (dialog == null || !visited.add(dialog.getKey())) {
            return 0;
        }
        int width = 0;
        for (DialogModel.Field field : dialog.getFields()) {
            if (!iniFileModel.findIniField(field.getKey()).isPresent()) {
                continue;
            }
            JLabel label = new JLabel(field.getUiName());
            CalibrationFieldFactory.applyStyle(label);
            width = Math.max(width, label.getPreferredSize().width);
        }
        for (PanelModel panel : dialog.getPanels()) {
            width = Math.max(width, getFieldLabelWidth(panel.resolveDialog(iniFileModel), iniFileModel, visited));
        }
        return Math.min(width, CalibrationFieldFactory.MAX_LABEL_WIDTH);
    }

    private static int getFieldEditorWidth(DialogModel dialog, IniFileModel iniFileModel,
                                           ConfigurationImage ci, Set<String> visited) {
        if (dialog == null || !visited.add(dialog.getKey())) {
            return 0;
        }
        int width = 0;
        for (DialogModel.Field field : dialog.getFields()) {
            Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
            if (iniField.isPresent()) {
                try {
                    String currentValue = ci == null ? "" :
                        ConfigurationImageGetterSetter.getStringValue(iniField.get(), ci);
                    width = Math.max(width,
                        CalibrationFieldFactory.getFieldEditorPreferredWidth(iniField.get(), currentValue));
                } catch (OrdinalOutOfRangeException e) {
                    // The corresponding field row will render as a label instead of an editor.
                }
            }
        }
        for (PanelModel panel : dialog.getPanels()) {
            width = Math.max(width,
                getFieldEditorWidth(panel.resolveDialog(iniFileModel), iniFileModel, ci, visited));
        }
        return width;
    }

    private void renderField(JPanel container, DialogModel.Field field, IniFileModel iniFileModel,
                              ConfigurationImage ci, int fieldLabelWidth, int fieldEditorWidth) {
        Runnable onChange = () -> {
            refreshExpressions();
            if ("trigger_type".equalsIgnoreCase(field.getKey()) ||
                "vvtMode1".equalsIgnoreCase(field.getKey()) ||
                "vvtMode2".equalsIgnoreCase(field.getKey())) {
                updateTriggerImage();
            }
        };
        Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
        Map<String, String> tooltips = iniFileModel.getTooltips();
        String helpText = tooltips == null ? null : tooltips.get(field.getKey());
        JPanel row = iniField.map(value -> {
            try {
                return CalibrationFieldFactory.createFieldRow(
                    field, value, ci, workingImage, onChange, onShowInPinout,
                    fieldLabelWidth, fieldEditorWidth, helpText);
            } catch (OrdinalOutOfRangeException e) {
                log.warn("Skipping field " + field.getKey() + " with out-of-range ordinal: " + e.getMessage());
                return CalibrationFieldFactory.createLabelRow(field);
            }
        }).orElseGet(() -> CalibrationFieldFactory.createLabelRow(field));
        if (fieldRowVerticalMargin > 0 && iniField.isPresent()) {
            row.setBorder(BorderFactory.createEmptyBorder(
                fieldRowVerticalMargin, 0, fieldRowVerticalMargin, 0));
            CalibrationFieldFactory.fixRowHeight(row);
        }

        registerFieldExpressions(row, field, iniFileModel, ci);
        container.add(row);
    }

    private void renderRuntimeValue(JPanel container, DialogModel.Field field, IniFileModel ini,
                                    ConfigurationImage ci) {
        JPanel row = CalibrationFieldFactory.createLabelRow(field);
        row.add(Box.createHorizontalStrut(10));
        JLabel value = new JLabel("---");
        value.setFont(value.getFont().deriveFont(Font.BOLD));
        row.add(value);
        CalibrationFieldFactory.fixRowHeight(row);
        // runtimeValue has no precision/units arguments; show up to three decimal places.
        readoutEntries.add(new ReadoutLabelEntry(value, field.getKey(), "", 3, true));
        registerFieldExpressions(row, field, ini, ci);
        container.add(row);
    }

    private void registerFieldExpressions(JPanel row, DialogModel.Field field, IniFileModel iniFileModel,
                                         ConfigurationImage ci) {
        boolean hasExpressions = field.getEnableExpression() != null || field.getVisibleExpression() != null;
        if (hasExpressions) {
            ExpressionRow exprRow = new ExpressionRow(row, field.getEnableExpression(), field.getVisibleExpression());
            expressionRows.add(exprRow);
            ConfigurationImage evalImage = workingImage != null ? workingImage : ci;
            if (evalImage != null) {
                applyExpressionState(exprRow, iniFileModel, evalImage);
            }
        }
    }

    private void renderIndicatorGroup(JPanel container, List<IndicatorModel> indicators, IniFileModel iniFileModel, ConfigurationImage ci, int cols) {
        IndicatorPanel ip = new IndicatorPanel(indicators, iniFileModel, Math.max(1, cols));
        ip.refresh(workingImage != null ? workingImage : ci);
        ip.setActive(active);
        indicatorPanels.add(ip);
        container.add(ip.getPanel());
    }

    /**
     * Gauges scale with the width the dialog offers: the dialog fills the viewport width, panels
     * holding gauges are marked {@link ConstrainedBorderLayout#GROWS} so they take the spare
     * width, and the square grid fits as many gauges per row as that width allows and shares it
     * between them, so the gauges always fill the width they are given.
     */
    private void renderGaugeGroup(JPanel container, List<String> gaugeNames, IniFileModel iniFileModel) {
        JPanel gaugePanel = new JPanel(new SquareGridLayout(2, 4, MIN_GAUGE_SIZE, READOUT_GAUGE_SIZE));
        SquareGridLayout.revalidateOnWidthChange(gaugePanel);
        gaugePanel.putClientProperty(ConstrainedBorderLayout.GROWS, Boolean.TRUE);
        gaugePanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        viewportPane.fillWidth = true;
        gaugeGroupsRendered++;
        for (String gaugeName : gaugeNames) {
            GaugeModel gaugeModel = iniFileModel.getGauge(gaugeName);
            if (gaugeModel != null) {
                gaugePanel.add(buildGaugeCell(gaugeModel));
            }
        }
        container.add(gaugePanel);
    }

    private void clearLiveComponents() {
        for (IndicatorPanel indicatorPanel : indicatorPanels) {
            indicatorPanel.destroy();
        }
        indicatorPanels.clear();
        readoutEntries.clear();
        gaugeReadoutEntries.clear();
        hasReadoutDemand = false;
        readoutListenerToken.setActive(false);
    }

    private void updateLiveDemand() {
        Set<String> channels = new LinkedHashSet<>();
        for (ReadoutLabelEntry entry : readoutEntries) {
            channels.add(entry.channel);
            if (entry.title != null && TsStringFunction.containsStringFunction(entry.title)) {
                channels.addAll(ExpressionEvaluator.extractVariables(entry.title));
            }
            if (entry.units != null && TsStringFunction.containsStringFunction(entry.units)) {
                channels.addAll(ExpressionEvaluator.extractVariables(entry.units));
            }
        }
        for (GaugeReadoutEntry entry : gaugeReadoutEntries) {
            channels.add(entry.channel);
            if (entry.hasExpressionLabels && currentIniFileModel != null) {
                GaugeModel gauge = currentIniFileModel.getGauge(entry.gaugeName);
                if (gauge != null) {
                    if (gauge.getTitleValue().isExpression()) {
                        channels.addAll(ExpressionEvaluator.extractVariables(gauge.getTitle()));
                    }
                    if (gauge.getUnitsValue().isExpression()) {
                        channels.addAll(ExpressionEvaluator.extractVariables(gauge.getUnits()));
                    }
                }
            }
        }
        hasReadoutDemand = !channels.isEmpty();
        readoutListenerToken.setSubscription(new SensorSubscription(channels.toArray(new String[0])));
        readoutListenerToken.setActive(active && hasReadoutDemand);
    }

    public void setActive(boolean active) {
        this.active = active;
        readoutListenerToken.setActive(active && hasReadoutDemand);
        for (IndicatorPanel indicatorPanel : indicatorPanels) {
            indicatorPanel.setActive(active);
        }
    }

    public void destroy() {
        active = false;
        CalibrationFieldFactory.closeHelpPopup();
        clearLiveComponents();
        readoutListenerToken.remove();
    }

    private static String toBorderConstraint(String placement) {
        if (placement == null) return BorderLayout.CENTER;
        switch (placement.toLowerCase()) {
            case "north": return BorderLayout.NORTH;
            case "south": return BorderLayout.SOUTH;
            case "west":  return BorderLayout.WEST;
            case "east":  return BorderLayout.EAST;
            default:      return BorderLayout.CENTER;
        }
    }

    private void showVeGeneratorPanel(IniFileModel ini) {
        if (workingImage == null) return;
        Runnable restorer = currentViewRestorer;
        contentPane.removeAll();
        contentPane.setLayout(new BorderLayout());
        VeTableGeneratorPanel panel = new VeTableGeneratorPanel(
            ini, workingImage,
            patched -> { workingImage = patched; if (onConfigChange != null) onConfigChange.accept(workingImage); },
            () -> { if (restorer != null) restorer.run(); }
        );
        contentPane.add(panel, BorderLayout.CENTER);
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void renderPanelEntry(JPanel container, PanelModel panel, IniFileModel iniFileModel, ConfigurationImage ci,
                                   boolean isBorderLayout, JPanel[] horizontalPanelRef, Runnable notifyEdit,
                                   int fieldLabelWidth, int fieldEditorWidth) {
        String placement = panel.getPlacement();

        JPanel targetContainer;
        String constraint = null;
        if (isBorderLayout) {
            targetContainer = container;
            constraint = toBorderConstraint(placement);
        } else {
            boolean isHorizontal = "west".equalsIgnoreCase(placement) || "center".equalsIgnoreCase(placement) || "east".equalsIgnoreCase(placement);
            if (isHorizontal) {
                if (horizontalPanelRef[0] == null) {
                    horizontalPanelRef[0] = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 0));
                    horizontalPanelRef[0].setAlignmentX(Component.LEFT_ALIGNMENT);
                    container.add(horizontalPanelRef[0]);
                }
                targetContainer = horizontalPanelRef[0];
            } else {
                horizontalPanelRef[0] = null;
                targetContainer = container;
            }
        }

        CurveModel curve = iniFileModel.getCurves().get(panel.getPanelName());
        if (curve != null) {
            viewportPane.fillWidth = true;
            JComponent content = withTopicHelp(createCurveView(iniFileModel, curve, notifyEdit), curve.getTopicHelp(), iniFileModel);
            CalibrationFieldFactory.applyStyle(content);
            content.setAlignmentX(Component.LEFT_ALIGNMENT);
            if (constraint != null) targetContainer.add(content, constraint); else targetContainer.add(content);
            return;
        }

        TableModel table = iniFileModel.getTable(panel.getPanelName());
        if (table != null) {
            viewportPane.fillWidth = true;
            JComponent content = createTableView(iniFileModel, table, notifyEdit);
            if ("veTableTbl".equals(table.getTableId())) {
                final IniFileModel capturedIni = iniFileModel;
                JButton genVeBtn = new JButton("Generate base VE...");
                genVeBtn.addActionListener(e -> showVeGeneratorPanel(capturedIni));
                JPanel veToolbar = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 4));
                veToolbar.add(genVeBtn);
                JPanel wrapper = new JPanel(new BorderLayout());
                wrapper.add(veToolbar, BorderLayout.NORTH);
                wrapper.add(content, BorderLayout.CENTER);
                content = wrapper;
            }
            content = withTopicHelp(content, table.getTopicHelp(), iniFileModel);
            CalibrationFieldFactory.applyStyle(content);
            content.setAlignmentX(Component.LEFT_ALIGNMENT);
            if (constraint != null) targetContainer.add(content, constraint); else targetContainer.add(content);
            return;
        }

        JPanel panelWidget = new GrowAwarePanel();
        panelWidget.setAlignmentX(Component.LEFT_ALIGNMENT);
        int gaugeGroupsBefore = gaugeGroupsRendered;
        DialogModel subDialog = panel.resolveDialog(iniFileModel);
        String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
        JPanel body = prepareTopicHelp(panelWidget, subDialog == null ? null : subDialog.getTopicHelp(), iniFileModel);
        applyLayout(body, subLayoutHint);

        if (subDialog != null) {
            String uiName = subDialog.getUiName();
            panelWidget.setName(uiName == null || uiName.isEmpty() ? subDialog.getKey() : uiName);
            // Empty INI titles intentionally group controls without a visible heading/frame.
            // Indicator and readout panels use this too; their keys are only internal IDs.
            if (uiName != null && !uiName.trim().isEmpty()) {
                GradientTitleBorder.installBorder(uiName, panelWidget);
            }
            fillPanel(body, subDialog, iniFileModel, ci,
                Math.max(0, fieldLabelWidth - panelWidget.getInsets().left), fieldEditorWidth);

            if (TriggerImageHelper.isTriggerPanel(subDialog.getKey(), uiName) || "Sub Panel".equals(uiName)) {
                addTriggerImage(body, subDialog.getKey(), uiName);
            }
        } else {
            panelWidget.setName(panel.getPanelName());
            GradientTitleBorder.installBorder(panel.getPanelName(), panelWidget);
        }
        if (gaugeGroupsRendered > gaugeGroupsBefore) {
            panelWidget.putClientProperty(ConstrainedBorderLayout.GROWS, Boolean.TRUE);
        }
        if (constraint != null) targetContainer.add(panelWidget, constraint); else targetContainer.add(panelWidget);
    }

    /**
     * In a horizontal BoxLayout, Swing shares spare width among children up to their maximum
     * widths. Once a sibling holds gauges ({@link ConstrainedBorderLayout#GROWS}), the other
     * panels stay at their preferred width so the spare width reaches the gauges.
     */
    private static final class GrowAwarePanel extends JPanel {
        @Override
        public Dimension getMaximumSize() {
            Dimension maximum = super.getMaximumSize();
            Container parent = getParent();
            if (ConstrainedBorderLayout.grows(this) || parent == null || !isHorizontalBox(parent.getLayout())) {
                return maximum;
            }
            for (Component sibling : parent.getComponents()) {
                if (sibling.isVisible() && ConstrainedBorderLayout.grows(sibling)) {
                    return new Dimension(getPreferredSize().width, maximum.height);
                }
            }
            return maximum;
        }

        private static boolean isHorizontalBox(LayoutManager layout) {
            return layout instanceof BoxLayout
                && (((BoxLayout) layout).getAxis() == BoxLayout.X_AXIS || ((BoxLayout) layout).getAxis() == BoxLayout.LINE_AXIS);
        }
    }

    private static List<DialogModel.DialogEntry> synthesizeOrderedEntries(DialogModel dialog) {
        List<DialogModel.DialogEntry> list = new ArrayList<>();
        for (DialogModel.Field f : dialog.getFields())
            list.add(new DialogModel.DialogEntry(DialogModel.DialogEntry.Kind.FIELD, f));
        for (DialogModel.Command c : dialog.getCommandsOfCurrentDialog())
            list.add(new DialogModel.DialogEntry(DialogModel.DialogEntry.Kind.COMMAND, c));
        for (IndicatorModel i : dialog.getIndicators())
            list.add(new DialogModel.DialogEntry(DialogModel.DialogEntry.Kind.INDICATOR, i));
        for (String g : dialog.getGaugeNames())
            list.add(new DialogModel.DialogEntry(DialogModel.DialogEntry.Kind.GAUGE, g));
        for (PanelModel p : dialog.getPanels())
            list.add(new DialogModel.DialogEntry(DialogModel.DialogEntry.Kind.PANEL, p));
        return list;
    }

    private void renderReadouts(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel) {
        List<ReadoutModel> readouts = dialogModel.getReadouts();
        if (!readouts.isEmpty()) {
            int cols = Math.max(1, dialogModel.getReadoutColumns());
            JPanel grid = new JPanel(new GridLayout(0, cols, 4, 4));
            grid.setAlignmentX(Component.LEFT_ALIGNMENT);
            for (ReadoutModel readout : readouts) {
                JPanel cell = buildReadoutCell(readout, iniFileModel);
                grid.add(cell);
            }
            container.add(grid);
        }
    }

    /**
     * Re-evaluates all expression-controlled rows against the current workingImage.
     * Called whenever any field value changes.
     */
    private void refreshExpressions() {
        if (workingImage == null || currentIniFileModel == null) return;
        for (ExpressionRow exprRow : expressionRows) {
            applyExpressionState(exprRow, currentIniFileModel, workingImage);
        }
        refreshIndicators();
        if (onConfigChange != null) {
            onConfigChange.accept(workingImage);
        }
    }

    private void updateTriggerImage() {
        for (Runnable updater : triggerImageUpdaters) {
            updater.run();
        }
    }

    private void addTriggerImage(JPanel panel, String key, String uiName) {
        TriggerImageHelper helper = new TriggerImageHelper();
        helper.addTriggerPanelExtras(panel);
        TriggerImageHelper.Channel channel = TriggerImageHelper.getChannel(key, uiName);
        Runnable updater = channel == TriggerImageHelper.Channel.SECONDARY
            ? () -> helper.updateVvtTriggerImage(currentIniFileModel, workingImage)
            : () -> helper.updateTriggerImage(currentIniFileModel, workingImage, channel);
        triggerImageUpdaters.add(updater);
        updater.run();
    }

    /** Pushes the current working image to all indicator panels (called when config fields change). */
    private void refreshIndicators() {
        for (IndicatorPanel ip : indicatorPanels) {
            ip.refresh(workingImage);
        }
    }

    private void refreshReadouts() {
        for (ReadoutLabelEntry entry : readoutEntries) {
            if (entry.titleLabel != null) {
                entry.titleLabel.setText(resolveReadoutLabel(entry.title));
            }
            double val = SensorCentral.getInstance().getValue(entry.channel);
            String units = resolveReadoutLabel(entry.units);
            String text = Double.isNaN(val) ? "---" :
                    (entry.compact ? new DecimalFormat("0.###").format(val) :
                        String.format("%." + entry.valDigits + "f", val)) +
                    (!units.isEmpty() ? " " + units : "");
            entry.valueLabel.setText(text);
        }
        for (GaugeReadoutEntry entry : gaugeReadoutEntries) {
            double val = SensorCentral.getInstance().getValue(entry.channel);
            entry.radial.setValue(val);
            if (entry.hasExpressionLabels) {
                ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(entry.gaugeName);
                if (labels != null) {
                    entry.radial.setTitle(labels.getTitle());
                    entry.radial.setUnitString(labels.getUnits());
                }
            }
        }
    }

    private String resolveReadoutLabel(String raw) {
        if (raw == null || currentIniFileModel == null) {
            return "";
        }
        if (!raw.contains("bitStringValue") && !raw.contains("stringValue")) {
            return raw;
        }
        Map<String, Double> values = new HashMap<>();
        for (String variable : ExpressionEvaluator.extractVariables(raw)) {
            double value = SensorCentral.getInstance().getValue(variable);
            if (Double.isFinite(value)) {
                values.put(variable, value);
            }
        }
        return TsStringFunction.resolveDisplayLabel(raw, currentIniFileModel, workingImage, values);
    }

    private JPanel buildReadoutCell(ReadoutModel readout, IniFileModel ini) {
        String channel = readout.getChannelOrGaugeName();
        String title   = readout.getTitle();
        String units   = readout.getUnits();
        int    valDig  = readout.getValDigits();

        GaugeModel gauge = ini.getGauge(channel);
        if (gauge != null && title == null) {
            // Single-name ref matched a gauge — use gauge metadata
            channel = gauge.getChannel();
            title   = gauge.getTitle();
            units   = gauge.getUnits();
            valDig  = gauge.getValueDecimalPlaces();
        }
        if (title == null || title.isEmpty()) title = channel;
        if (units == null) units = "";

        JPanel cell = new JPanel();
        cell.setLayout(new BoxLayout(cell, BoxLayout.Y_AXIS));
        TuningStyle.styleSection(cell, 4, 8);

        JLabel titleLabel = new JLabel(resolveReadoutLabel(title));
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.PLAIN, 10f));
        titleLabel.setForeground(TuningStyle.muted());
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel valueLabel = new JLabel("---");
        valueLabel.setFont(valueLabel.getFont().deriveFont(Font.BOLD, 13f));
        valueLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        cell.add(titleLabel);
        cell.add(valueLabel);

        readoutEntries.add(new ReadoutLabelEntry(titleLabel, valueLabel, channel, title, units, valDig, false));
        return cell;
    }

    private JComponent buildGaugeCell(GaugeModel gaugeModel) {
        // SquareGridLayout sizes the cell; the gauge draws a square of whatever it is given.
        CircularGauge radial = new CircularGauge(gaugeModel, currentIniFileModel, workingImage);
        boolean exprTitle = gaugeModel.getTitleValue().isExpression() && TsStringFunction.containsStringFunction(gaugeModel.getTitle());
        boolean exprUnits = gaugeModel.getUnitsValue().isExpression() && TsStringFunction.containsStringFunction(gaugeModel.getUnits());
        if (exprTitle) {
            radial.setTitle("");
        }
        if (exprUnits) {
            radial.setUnitString("");
        }
        double initialVal = SensorCentral.getInstance().getValue(gaugeModel.getChannel());
        radial.setValue(initialVal);
        gaugeReadoutEntries.add(new GaugeReadoutEntry(radial, gaugeModel.getChannel(), gaugeModel.getName(), exprTitle || exprUnits));
        return radial;
    }

    private void applyExpressionState(ExpressionRow exprRow, IniFileModel iniFileModel, ConfigurationImage ci) {
        // Visibility
        if (exprRow.visibleExpression != null) {
            Boolean visible = evaluateFieldExpression(exprRow.visibleExpression, iniFileModel, ci);
            exprRow.row.setVisible(visible == null || visible);
        }
        // Enabled
        if (exprRow.enableExpression != null) {
            Boolean enabled = evaluateFieldExpression(exprRow.enableExpression, iniFileModel, ci);
            setComponentsEnabled(exprRow.row, enabled == null || enabled);
        }
    }

    //TODO: inline
    private Boolean evaluateFieldExpression(String expression, IniFileModel iniFileModel, ConfigurationImage ci) {
        return ExpressionEvaluator.evaluateBooleanExpression(expression, iniFileModel, ci);
    }

    private static void setComponentsEnabled(Container container, boolean enabled) {
        for (Component comp : container.getComponents()) {
            comp.setEnabled(enabled);
            if (comp instanceof Container) {
                setComponentsEnabled((Container) comp, enabled);
            }
        }
    }

    /**
     * Returns a Runnable that, when invoked on the EDT, looks up the command's binary payload in
     * {@code [ControllerCommands]}, decodes {@code \xNN} escapes, and submits it to the ECU on the
     * IO thread. Falls back to TS_EXECUTE text if the command is not found in the table.
     */
    private Runnable buildCommandExecutor(String commandName) {
        return () -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) {
                return;
            }
            IniFileModel ini = uiContext.iniFileState.getIniFileModel();
            String payloadStr = ini != null ? ini.getControllerCommands().get(commandName) : null;
            if (payloadStr != null) {
                byte[] raw = decodeIniCommandBytes(payloadStr);
                if (raw.length < 1) {
                    log.warn("Empty payload for command: " + commandName);
                    return;
                }
                char opcode = (char) (raw[0] & 0xFF);
                byte[] payload = new byte[raw.length - 1];
                System.arraycopy(raw, 1, payload, 0, payload.length);
                uiContext.getLinkManager().submit(() ->
                    bp.executeCommand(opcode, payload, commandName)
                );
            } else {
                log.warn("Command not found in [ControllerCommands]: " + commandName);
            }
        };
    }

    /**
     * Decodes an INI binary string such as {@code Z\x00\x13\x00\x01} into bytes.
     * Each {@code \xNN} sequence becomes one byte; other characters are taken as-is.
     */
    static byte[] decodeIniCommandBytes(String s) {
        byte[] buf = new byte[s.length()];
        int out = 0;
        int i = 0;
        while (i < s.length()) {
            if (i + 3 < s.length() && s.charAt(i) == '\\' && s.charAt(i + 1) == 'x') {
                buf[out++] = (byte) Integer.parseInt(s.substring(i + 2, i + 4), 16);
                i += 4;
            } else {
                buf[out++] = (byte) s.charAt(i);
                i++;
            }
        }
        return Arrays.copyOf(buf, out);
    }

    public JPanel getContentPane() {
        return contentPane;
    }

    /** Viewport presentation shared by the tuning tab and its visual sandbox. */
    public JPanel getViewportPane() {
        return viewportPane;
    }

    /** The framed form excludes the unused tiled viewport background. */
    JComponent getPanelFrame() {
        return viewportPane.frame;
    }

    int getViewportBackgroundRenderCountForTests() {
        return viewportPane.backgroundRenderCount;
    }

    long getViewportBackgroundCachePixelsForTests() {
        return viewportPane.mosaicCache == null ? 0L
            : (long) viewportPane.mosaicCache.getWidth() * viewportPane.mosaicCache.getHeight();
    }

    long getViewportBackgroundCellRenderCountForTests() {
        return viewportPane.backgroundCellRenderCount;
    }

    private static class DialogViewport extends ScrollablePanel {
        private static final int MARGIN = 8;
        private final JPanel frame = new JPanel(new BorderLayout());
        private int preferredWidth = -1;
        private boolean fillWidth;
        private int backgroundRenderCount;
        private long backgroundCellRenderCount;
        private BufferedImage mosaicCache;
        private int cachedWidth = -1;
        private int cachedHeight = -1;
        private Color cachedBackground;
        private Rectangle cachedFrameBounds;
        private Rectangle cachedVisibleBounds;
        private double cachedScaleX;
        private double cachedScaleY;
        private double cachedPhaseX;
        private double cachedPhaseY;

        DialogViewport(JPanel content) {
            setLayout(null);
            // The dialog sits on one white rounded card; its plain panels let the card show through.
            TuningStyle.styleCard(frame, 10, 10);
            frame.add(content, BorderLayout.CENTER);
            TransparentPanels.install(frame);
            add(frame);
            content.addContainerListener(new java.awt.event.ContainerAdapter() {
                @Override
                public void componentAdded(java.awt.event.ContainerEvent e) {
                    preferredWidth = -1;
                }

                @Override
                public void componentRemoved(java.awt.event.ContainerEvent e) {
                    preferredWidth = -1;
                }
            });
        }

        @Override
        public Dimension getPreferredSize() {
            Dimension size = frame.getPreferredSize();
            return new Dimension(size.width + 2 * MARGIN, size.height + 2 * MARGIN);
        }

        @Override
        public boolean getScrollableTracksViewportHeight() {
            // Paint the unused area below short forms as well as their side margins.
            return getParent() instanceof JViewport && getParent().getHeight() >= getPreferredSize().height;
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Color background = getBackground();
            Rectangle frameBounds = frame.getBounds();
            Rectangle visibleBounds = getVisibleRect();
            AffineTransform transform = ((Graphics2D) g).getTransform();
            double scaleX = transform.getScaleX();
            double scaleY = transform.getScaleY();
            Point2D deviceOrigin = transform.transform(new Point2D.Double(visibleBounds.x, visibleBounds.y), null);
            double phaseX = deviceOrigin.getX() - Math.floor(deviceOrigin.getX());
            double phaseY = deviceOrigin.getY() - Math.floor(deviceOrigin.getY());
            int deviceX = (int) Math.floor(deviceOrigin.getX());
            int deviceY = (int) Math.floor(deviceOrigin.getY());
            double physicalWidth = visibleBounds.width * scaleX + phaseX;
            double physicalHeight = visibleBounds.height * scaleY + phaseY;
            int rasterWidth = physicalWidth > 0 && physicalWidth <= Integer.MAX_VALUE
                ? (int) Math.ceil(physicalWidth) : 0;
            int rasterHeight = physicalHeight > 0 && physicalHeight <= Integer.MAX_VALUE
                ? (int) Math.ceil(physicalHeight) : 0;
            long pixels = (long) rasterWidth * rasterHeight;
            // Cache only decoration: per-hexagon XRender masks are costly on every repaint.
            // The frame and its live editors continue through normal child painting.
            boolean cacheable = background != null && background.getAlpha() == 255
                && isOpaque() && isNormalSrcOver(((Graphics2D) g).getComposite())
                && !(g instanceof PrinterGraphics) && Double.isFinite(scaleX) && Double.isFinite(scaleY)
                && Double.isFinite(deviceOrigin.getX()) && Double.isFinite(deviceOrigin.getY())
                && deviceOrigin.getX() >= Integer.MIN_VALUE && deviceOrigin.getX() <= Integer.MAX_VALUE
                && deviceOrigin.getY() >= Integer.MIN_VALUE && deviceOrigin.getY() <= Integer.MAX_VALUE
                && scaleX > 0 && scaleY > 0 && transform.getShearX() == 0 && transform.getShearY() == 0
                && visibleBounds.width > 0 && visibleBounds.height > 0
                && rasterWidth > 0 && rasterHeight > 0 && pixels <= 16_777_216L;
            if (cacheable) {
                boolean cacheMiss = mosaicCache == null || cachedWidth != rasterWidth || cachedHeight != rasterHeight
                    || !background.equals(cachedBackground) || !frameBounds.equals(cachedFrameBounds)
                    || !visibleBounds.equals(cachedVisibleBounds) || Double.compare(scaleX, cachedScaleX) != 0
                    || Double.compare(scaleY, cachedScaleY) != 0 || Double.compare(phaseX, cachedPhaseX) != 0
                    || Double.compare(phaseY, cachedPhaseY) != 0;
                long visiblePixels = (long) visibleBounds.width * visibleBounds.height;
                if (cacheMiss && isSmallPaintClip(g, visibleBounds, visiblePixels)) {
                    Graphics2D direct = (Graphics2D) g.create();
                    try {
                        paintMosaic(direct, background, frameBounds);
                    } finally {
                        direct.dispose();
                    }
                    return;
                }
                if (cacheMiss) {
                    mosaicCache = new BufferedImage(rasterWidth, rasterHeight, BufferedImage.TYPE_INT_RGB);
                    Graphics2D cacheGraphics = mosaicCache.createGraphics();
                    try {
                        cacheGraphics.setColor(background);
                        cacheGraphics.fillRect(0, 0, rasterWidth, rasterHeight);
                        cacheGraphics.setTransform(new AffineTransform(scaleX, 0, 0, scaleY,
                            transform.getTranslateX() - deviceX, transform.getTranslateY() - deviceY));
                        // getClipBounds() sees the user clip, not the image's implicit device bounds.
                        // Keep polygon generation bounded as well as the allocated raster.
                        cacheGraphics.clip(visibleBounds);
                        paintMosaic(cacheGraphics, background, frameBounds);
                    } finally {
                        cacheGraphics.dispose();
                    }
                    cachedWidth = rasterWidth;
                    cachedHeight = rasterHeight;
                    cachedBackground = background;
                    cachedFrameBounds = new Rectangle(frameBounds);
                    cachedVisibleBounds = new Rectangle(visibleBounds);
                    cachedScaleX = scaleX;
                    cachedScaleY = scaleY;
                    cachedPhaseX = phaseX;
                    cachedPhaseY = phaseY;
                }
                Graphics2D cached = (Graphics2D) g.create();
                try {
                    Area unused = new Area(new Rectangle(0, 0, getWidth(), getHeight()));
                    unused.subtract(new Area(frameBounds));
                    cached.clip(unused);
                    AffineTransform alignToDevicePixels = transform.createInverse();
                    alignToDevicePixels.translate(deviceX, deviceY);
                    cached.transform(alignToDevicePixels);
                    cached.drawImage(mosaicCache, 0, 0, null);
                } catch (java.awt.geom.NoninvertibleTransformException impossibleForPositiveScale) {
                    Graphics2D direct = (Graphics2D) g.create();
                    try {
                        paintMosaic(direct, background, frameBounds);
                    } finally {
                        direct.dispose();
                    }
                } finally {
                    cached.dispose();
                }
                return;
            }
            Graphics2D mosaic = (Graphics2D) g.create();
            try {
                paintMosaic(mosaic, background, frameBounds);
            } finally {
                mosaic.dispose();
            }
        }

        private boolean isNormalSrcOver(Composite composite) {
            return composite instanceof AlphaComposite
                && ((AlphaComposite) composite).getRule() == AlphaComposite.SRC_OVER
                && ((AlphaComposite) composite).getAlpha() == 1.0f;
        }

        private boolean isSmallPaintClip(Graphics g, Rectangle visibleBounds, long viewportPixels) {
            Rectangle clip = g.getClipBounds();
            if (clip == null) {
                return false;
            }
            Rectangle dirty = clip.intersection(visibleBounds);
            return (long) dirty.width * dirty.height < viewportPixels / 4;
        }

        private void paintMosaic(Graphics2D mosaic, Color background, Rectangle frameBounds) {
            backgroundRenderCount++;
            Area unused = new Area(new Rectangle(0, 0, getWidth(), getHeight()));
            unused.subtract(new Area(frameBounds));
            mosaic.clip(unused);
            mosaic.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            boolean dark = background.getRed() + background.getGreen() + background.getBlue() < 384;
            mosaic.setColor(dark ? Color.WHITE : new Color(45, 58, 68));
            final double radius = 22;
            final double rowHeight = Math.sqrt(3) * radius;
            Rectangle clip = mosaic.getClipBounds();
            int firstColumn = (int) Math.floor(clip.x / (1.5 * radius)) - 1;
            int lastColumn = (int) Math.ceil((clip.x + clip.width) / (1.5 * radius)) + 1;
            int firstRow = (int) Math.floor(clip.y / rowHeight) - 1;
            int lastRow = (int) Math.ceil((clip.y + clip.height) / rowHeight) + 1;
            for (int column = firstColumn; column <= lastColumn; column++) {
                for (int row = firstRow; row <= lastRow; row++) {
                    backgroundCellRenderCount++;
                    double centerX = column * 1.5 * radius;
                    double centerY = (row + ((column & 1) == 0 ? 0 : 0.5)) * rowHeight;
                    Path2D hexagon = new Path2D.Double();
                    for (int corner = 0; corner < 6; corner++) {
                        double angle = corner * Math.PI / 3;
                        double x = centerX + radius * Math.cos(angle);
                        double y = centerY + radius * Math.sin(angle);
                        if (corner == 0) {
                            hexagon.moveTo(x, y);
                        } else {
                            hexagon.lineTo(x, y);
                        }
                    }
                    hexagon.closePath();
                    int shade = Math.floorMod(column * 31 + row * 17, 7);
                    mosaic.setComposite(AlphaComposite.SrcOver.derive(0.018f + shade * 0.006f));
                    mosaic.fill(hexagon);
                    mosaic.setComposite(AlphaComposite.SrcOver.derive(0.07f));
                    mosaic.draw(hexagon);
                }
            }
        }

        @Override
        public void doLayout() {
            if (preferredWidth < 0) {
                preferredWidth = frame.getPreferredSize().width;
            }
            int availableWidth = Math.max(0, getWidth() - 2 * MARGIN);
            int width = fillWidth ? availableWidth : Math.min(preferredWidth, availableWidth);
            frame.setBounds((getWidth() - width) / 2, MARGIN, width,
                frame.getPreferredSize().height);
            frame.doLayout();
        }
    }
}
