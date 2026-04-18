package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.GaugeModel;
import com.opensr5.ini.IniValue;
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
import com.rusefi.ui.UIContext;
import com.rusefi.ui.widgets.SensorGauge;
import eu.hansolo.steelseries.gauges.Radial;
import eu.hansolo.steelseries.tools.BackgroundColor;
import com.rusefi.ui.laf.GradientTitleBorder;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.SwingUtil;
import com.rusefi.ui.util.WrapLayout;

import com.devexperts.logging.Logging;

import javax.swing.*;
import java.awt.*;
import java.util.*;
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
    private final UIContext uiContext;
    private ConfigurationImage workingImage;
    private IniFileModel currentIniFileModel;
    private final List<ExpressionRow> expressionRows = new ArrayList<>();
    private final List<IndicatorPanel> indicatorPanels = new ArrayList<>();
    private final List<ReadoutLabelEntry> readoutEntries = new ArrayList<>();
    private final List<GaugeReadoutEntry> gaugeReadoutEntries = new ArrayList<>();
    private static final int READOUT_GAUGE_SIZE = 150;
    /** Called after each user edit with the current working image, so listeners can re-evaluate their own expressions. */
    private Consumer<ConfigurationImage> onConfigChange;

    public void setOnConfigChange(Consumer<ConfigurationImage> onConfigChange) {
        this.onConfigChange = onConfigChange;
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
        final JLabel valueLabel;
        final String channel;
        final String units;
        final int valDigits;

        ReadoutLabelEntry(JLabel valueLabel, String channel, String units, int valDigits) {
            this.valueLabel = valueLabel;
            this.channel = channel;
            this.units = units;
            this.valDigits = valDigits;
        }
    }

    /** Tracks a gauge-ref readout rendered as a small Radial gauge for live update. */
    private static class GaugeReadoutEntry {
        final Radial radial;
        final String channel;
        final String gaugeName;
        final boolean hasExpressionLabels;

        GaugeReadoutEntry(Radial radial, String channel, String gaugeName, boolean hasExpressionLabels) {
            this.radial = radial;
            this.channel = channel;
            this.gaugeName = gaugeName;
            this.hasExpressionLabels = hasExpressionLabels;
        }
    }

    public CalibrationDialogWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
        contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
        // Refresh readouts whenever the ECU sends new output-channel data.
        // Indicator panels register their own SensorCentral listeners independently.
        SensorCentral.getInstance().addListener(() -> {
            if (!readoutEntries.isEmpty() || !gaugeReadoutEntries.isEmpty()) {
                SwingUtilities.invokeLater(this::refreshReadouts);
            }
        });
    }

    private static void applyLayout(JPanel panel, String layoutHint) {
        if ("border".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new BorderLayout(4, 4));
        } else if ("xAxis".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
        } else {
            panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        }
    }

    public ConfigurationImage getWorkingImage() {
        return workingImage;
    }

    /**
     * Drops any in-progress edits. Called when the ECU disconnects so stale calibrations
     * from the previous board are not carried over to the next connection.
     */
    public void reset() {
        workingImage = null;
        contentPane.removeAll();
        contentPane.revalidate();
        contentPane.repaint();
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        workingImage = ci != null ? ci.clone() : null;
        currentIniFileModel = iniFileModel;
        expressionRows.clear();
        indicatorPanels.clear();
        readoutEntries.clear();
        gaugeReadoutEntries.clear();
        contentPane.removeAll();
        if (dialogModel != null) {
            applyLayout(contentPane, dialogModel.getLayoutHint());
            contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
        }
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
    }

    public void update(String key) {
        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage ci = uiContext.getBinaryProtocol().getControllerConfiguration();
        update(key, iniFileModel, ci);
    }

    public void update(String key, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (key != null) {
            DialogModel dialog = iniFileModel.getDialogs().get(key);
            if (dialog != null) {
                update(dialog, iniFileModel, ci);
                return;
            }
            workingImage = ci != null ? ci.clone() : null;

            Runnable notifyEdit = () -> { if (onConfigChange != null) onConfigChange.accept(workingImage); };

            TableModel table = iniFileModel.getTable(key);
            if (table != null) {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), workingImage);
                tuningTableView.setOnEdit(notifyEdit);
                contentPane.add(tuningTableView.getContent());
            } else {
                CurveModel curve = iniFileModel.getCurves().get(key);
                if (curve != null) {
                    contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                    CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, workingImage);
                    curveWidget.setOnEdit(notifyEdit);
                    contentPane.add(curveWidget.getContentPane());
                }
            }
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void fillPanel(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
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
                    renderField(container, entry.getAs(DialogModel.Field.class), iniFileModel, ci);
                    break;
                case COMMAND:
                    container.add(CalibrationFieldFactory.createCommandRow(entry.getAs(DialogModel.Command.class)));
                    break;
                case INDICATOR:
                    pendingIndicators.add(entry.getAs(IndicatorModel.class));
                    break;
                case GAUGE:
                    pendingGauges.add(entry.getAs(String.class));
                    break;
                case PANEL:
                    renderPanelEntry(container, entry.getAs(PanelModel.class), iniFileModel, ci,
                            isBorderLayout, horizontalPanelRef, notifyEdit);
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

    private void renderField(JPanel container, DialogModel.Field field, IniFileModel iniFileModel, ConfigurationImage ci) {
        Runnable onChange = this::refreshExpressions;
        Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
        JPanel row = iniField.map(value -> {
            try {
                return CalibrationFieldFactory.createFieldRow(field, value, ci, workingImage, onChange);
            } catch (OrdinalOutOfRangeException e) {
                log.warn("Skipping field " + field.getKey() + " with out-of-range ordinal: " + e.getMessage());
                return CalibrationFieldFactory.createLabelRow(field);
            }
        }).orElseGet(() -> CalibrationFieldFactory.createLabelRow(field));

        boolean hasExpressions = field.getEnableExpression() != null || field.getVisibleExpression() != null;
        if (hasExpressions) {
            ExpressionRow exprRow = new ExpressionRow(row, field.getEnableExpression(), field.getVisibleExpression());
            expressionRows.add(exprRow);
            ConfigurationImage evalImage = workingImage != null ? workingImage : ci;
            if (evalImage != null) {
                applyExpressionState(exprRow, iniFileModel, evalImage);
            }
        }
        container.add(row);
    }

    private void renderIndicatorGroup(JPanel container, List<IndicatorModel> indicators, IniFileModel iniFileModel, ConfigurationImage ci, int cols) {
        IndicatorPanel ip = new IndicatorPanel(indicators, iniFileModel, Math.max(1, cols));
        ip.refresh(workingImage != null ? workingImage : ci);
        indicatorPanels.add(ip);
        container.add(ip.getPanel());
    }

    private void renderGaugeGroup(JPanel container, List<String> gaugeNames, IniFileModel iniFileModel) {
        JPanel gaugePanel = new JPanel(new GridLayout(0, 2, 4, 4));
        gaugePanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        for (String gaugeName : gaugeNames) {
            GaugeModel gaugeModel = iniFileModel.getGauge(gaugeName);
            if (gaugeModel != null) {
                gaugePanel.add(buildGaugeCell(gaugeModel));
            }
        }
        container.add(gaugePanel);
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

    private void renderPanelEntry(JPanel container, PanelModel panel, IniFileModel iniFileModel, ConfigurationImage ci,
                                  boolean isBorderLayout, JPanel[] horizontalPanelRef, Runnable notifyEdit) {
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
            CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, workingImage);
            curveWidget.setOnEdit(notifyEdit);
            JComponent content = curveWidget.getContentPane();
            CalibrationFieldFactory.applyStyle(content);
            content.setAlignmentX(Component.LEFT_ALIGNMENT);
            if (constraint != null) targetContainer.add(content, constraint); else targetContainer.add(content);
            return;
        }

        TableModel table = iniFileModel.getTable(panel.getPanelName());
        if (table != null) {
            TuningTableView tuningTableView = new TuningTableView(table.getTitle());
            tuningTableView.displayTable(iniFileModel, table.getTableId(), workingImage);
            tuningTableView.setOnEdit(notifyEdit);
            JComponent content = tuningTableView.getContent();
            CalibrationFieldFactory.applyStyle(content);
            content.setAlignmentX(Component.LEFT_ALIGNMENT);
            if (constraint != null) targetContainer.add(content, constraint); else targetContainer.add(content);
            return;
        }

        JPanel panelWidget = new JPanel();
        panelWidget.setAlignmentX(Component.LEFT_ALIGNMENT);
        DialogModel subDialog = panel.resolveDialog(iniFileModel);
        String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
        applyLayout(panelWidget, subLayoutHint);

        if (subDialog != null) {
            String uiName = subDialog.getUiName();
            if (uiName == null || uiName.isEmpty()) {
                uiName = subDialog.getKey();
            }
            panelWidget.setName(uiName);
            GradientTitleBorder.installBorder(uiName, panelWidget);
            fillPanel(panelWidget, subDialog, iniFileModel, ci);
        } else {
            panelWidget.setName(panel.getPanelName());
            GradientTitleBorder.installBorder(panel.getPanelName(), panelWidget);
        }
        if (constraint != null) targetContainer.add(panelWidget, constraint); else targetContainer.add(panelWidget);
    }

    /** older render logic, used only for test, TODO: refactor the test and remove */
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

    /** Pushes the current working image to all indicator panels (called when config fields change). */
    private void refreshIndicators() {
        for (IndicatorPanel ip : indicatorPanels) {
            ip.refresh(workingImage);
        }
    }

    private void refreshReadouts() {
        for (ReadoutLabelEntry entry : readoutEntries) {
            double val = SensorCentral.getInstance().getValue(entry.channel);
            String text = Double.isNaN(val) ? "---" :
                    String.format("%." + entry.valDigits + "f", val) +
                    (entry.units != null && !entry.units.isEmpty() ? " " + entry.units : "");
            entry.valueLabel.setText(text);
        }
        for (GaugeReadoutEntry entry : gaugeReadoutEntries) {
            double val = SensorCentral.getInstance().getValue(entry.channel);
            if (!Double.isNaN(val)) entry.radial.setValue(val);
            if (entry.hasExpressionLabels) {
                ISensorHolder.ResolvedGaugeLabels labels = SensorCentral.getInstance().getResolvedLabels(entry.gaugeName);
                if (labels != null) {
                    entry.radial.setTitle(labels.getTitle());
                    entry.radial.setUnitString(labels.getUnits());
                }
            }
        }
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
        cell.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.GRAY, 1),
                BorderFactory.createEmptyBorder(4, 8, 4, 8)));

        JLabel titleLabel = new JLabel(title);
        titleLabel.setFont(titleLabel.getFont().deriveFont(Font.PLAIN, 10f));
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel valueLabel = new JLabel("---");
        valueLabel.setFont(valueLabel.getFont().deriveFont(Font.BOLD, 13f));
        valueLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        cell.add(titleLabel);
        cell.add(valueLabel);

        readoutEntries.add(new ReadoutLabelEntry(valueLabel, channel, units, valDig));
        return cell;
    }

    private double resolveGaugeValue(IniValue v, double fallback) {
        if (v == null) return fallback;
        if (!v.isExpression()) return v.getNumericValue();
        Double evaluated = ExpressionEvaluator.evaluateNumericExpression(v.getRawString(), currentIniFileModel, workingImage);
        return evaluated != null ? evaluated : fallback;
    }

    private JComponent buildGaugeCell(GaugeModel gaugeModel) {
        double lo = resolveGaugeValue(gaugeModel.getLowValueValue(), gaugeModel.getLowValue());
        double hi = resolveGaugeValue(gaugeModel.getHighValueValue(), gaugeModel.getHighValue());
        Radial radial = SensorGauge.createRadial(hi, lo, gaugeModel);
        radial.setBackgroundColor(BackgroundColor.LIGHT_GRAY);
        radial.setLcdDecimals(gaugeModel.getValueDecimalPlaces());
        Dimension size = new Dimension(READOUT_GAUGE_SIZE, READOUT_GAUGE_SIZE);
        radial.setPreferredSize(size);
        radial.setMinimumSize(size);
        radial.setMaximumSize(size);
        boolean exprTitle = gaugeModel.getTitleValue().isExpression() && TsStringFunction.containsStringFunction(gaugeModel.getTitle());
        boolean exprUnits = gaugeModel.getUnitsValue().isExpression() && TsStringFunction.containsStringFunction(gaugeModel.getUnits());
        if (exprTitle) radial.setTitle("");
        if (exprUnits) radial.setUnitString("");
        double initialVal = SensorCentral.getInstance().getValue(gaugeModel.getChannel());
        if (!Double.isNaN(initialVal)) radial.setValue(initialVal);
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

    public JPanel getContentPane() {
        return contentPane;
    }
}
