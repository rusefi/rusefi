package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.ConnectionStatusLogic;

import javax.swing.*;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.nio.ByteBuffer;
import java.util.Optional;

/**
 * Lambda-driven VE-table autotune pane for the rusEFI console.
 *
 * <p>Accumulates closed-loop (or steady-state) samples into the VE table's RPM × load
 * grid and, on request, writes a damped correction back to the ECU. The load axis is
 * resolved automatically from {@code veOverrideMode} + {@code fuelAlgorithm}; the lambda
 * source and target table follow {@code useLambdaOnInterface}.</p>
 *
 * <p>No firmware changes required: it uses existing output channels ({@code RPMValue},
 * {@code MAPValue}/{@code TPSValue}, {@code lambdaValue}/{@code AFRValue}, {@code Gego}).</p>
 */
public class VeAnalyzePane {

    private static final String STATUS_NOT_CONNECTED = "Not connected";
    private static final String STATUS_UNSUPPORTED = "Unsupported VE load mode (only MAP / TPS axes are supported)";

    private static final double MIN_RPM = 500.0;
    private static final double MAX_EGO_DEVIATION_PERCENT = 20.0; // |Gego - 100| <= this -> near target / not a big transient

    private final UIContext uiContext;

    private final JPanel content = new JPanel(new BorderLayout());
    private final JLabel statusLabel = new JLabel(STATUS_NOT_CONNECTED);
    private final JButton buttonStartStop = new JButton("Start");
    private final JButton buttonApply = new JButton("Apply Corrections");
    private final JButton buttonReset = new JButton("Reset");
    private final JLabel summaryLabel = new JLabel(" ");
    private final JTable table = new JTable();
    private final JScrollPane tableScroll = new JScrollPane(table);

    private boolean started = false;

    private VeAnalyzeModel model;
    private ConfigurationImage sourceImage;
    private ArrayIniField veZField;
    private int nLoad;
    private int nRpm;

    // Live-data wiring resolved at connect time.
    private String loadChannel;
    private String lambdaChannel;
    private String targetTableName;
    private boolean useLambda;

    private double[] targetRpmAxis;
    private double[] targetLoadAxis;
    private double[][] targetZ;

    public VeAnalyzePane(UIContext uiContext) {
        this.uiContext = uiContext;

        buttonStartStop.setEnabled(false);
        buttonApply.setEnabled(false);
        buttonReset.setEnabled(false);

        buttonStartStop.addActionListener(e -> toggleStartStop());
        buttonApply.addActionListener(e -> applyCorrections());
        buttonReset.addActionListener(e -> {
            if (model != null) {
                model.reset();
                refreshTable();
                updateSummary();
            }
        });

        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5));
        buttons.add(statusLabel);
        buttons.add(buttonStartStop);
        buttons.add(buttonApply);
        buttons.add(buttonReset);
        content.add(buttons, BorderLayout.NORTH);

        JPanel south = new JPanel(new BorderLayout());
        south.add(summaryLabel, BorderLayout.CENTER);
        content.add(south, BorderLayout.SOUTH);

        table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        table.setDefaultRenderer(Object.class, new ConfidenceRenderer());
        content.add(tableScroll, BorderLayout.CENTER);

        SensorCentral.getInstance().addListener((SensorCentral.ResponseListener) this::onFrame);

        ConnectionStatusLogic.INSTANCE.addAndFireListener(isConnected -> SwingUtilities.invokeLater(() -> {
            if (isConnected) {
                initFromEcu();
            } else {
                setStartState(false);
                buttonStartStop.setEnabled(false);
                buttonApply.setEnabled(false);
                buttonReset.setEnabled(false);
                statusLabel.setText(STATUS_NOT_CONNECTED);
            }
        }));
    }

    // ---- ECU init -----------------------------------------------------------

    private void initFromEcu() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null) {
            return;
        }
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage image = bp.getControllerConfiguration();
        if (ini == null || image == null) {
            return;
        }

        try {
            resolveAxisAndLambda(ini, image);
        } catch (RuntimeException e) {
            statusLabel.setText(STATUS_UNSUPPORTED);
            buttonStartStop.setEnabled(false);
            return;
        }

        try {
            if (!bindVeTable(ini, image)) {
                statusLabel.setText(STATUS_UNSUPPORTED);
                buttonStartStop.setEnabled(false);
                return;
            }
            if (!bindTargetTable(ini, image)) {
                statusLabel.setText("Cannot bind target lambda/AFR table");
                buttonStartStop.setEnabled(false);
                return;
            }
        } catch (Exception e) {
            statusLabel.setText("Bind failed: " + e.getMessage());
            buttonStartStop.setEnabled(false);
            return;
        }

        statusLabel.setText("");
        buttonStartStop.setEnabled(true);
        buttonApply.setEnabled(true);
        buttonReset.setEnabled(true);
        refreshTable();
        updateSummary();
    }

    private boolean bindVeTable(IniFileModel ini, ConfigurationImage image) {
        TableModel t = ini.getTable("veTableTbl");
        if (t == null) {
            return false;
        }
        ArrayIniField rpmField = requireArray(ini, t.getXBinsConstant());
        ArrayIniField loadField = requireArray(ini, t.getYBinsConstant());
        ArrayIniField veField = requireArray(ini, t.getZBinsConstant());
        if (rpmField == null || loadField == null || veField == null) {
            return false;
        }
        if (veField.getRows() != loadField.getRows() || veField.getCols() != rpmField.getRows()) {
            return false;
        }

        double[] rpmAxis = readAxis1d(image, rpmField);
        double[] loadAxis = readAxis1d(image, loadField);
        double[][] currentVe = readZ(image, veField, loadField.getRows(), rpmField.getRows());

        nLoad = loadAxis.length;
        nRpm = rpmAxis.length;
        veZField = veField;
        sourceImage = image;
        model = new VeAnalyzeModel(rpmAxis, loadAxis, currentVe);
        return true;
    }

    private void resolveAxisAndLambda(IniFileModel ini, ConfigurationImage image) {
        int veOverride = readOrdinal(ini, image, "veOverrideMode");
        int fuelAlgo = readOrdinal(ini, image, "fuelAlgorithm");

        if (veOverride == VariableRegistryValues.ve_override_e_VE_MAP) {
            loadChannel = "MAPValue";
        } else if (veOverride == VariableRegistryValues.ve_override_e_VE_TPS) {
            loadChannel = "TPSValue";
        } else {
            // VE_None: the natural load axis follows the air-mass model.
            if (fuelAlgo == VariableRegistryValues.engine_load_mode_e_LM_SPEED_DENSITY) {
                loadChannel = "MAPValue";
            } else if (fuelAlgo == VariableRegistryValues.engine_load_mode_e_LM_ALPHA_N) {
                loadChannel = "TPSValue";
            } else {
                throw new IllegalStateException("MAF/Lua VE load axis has no live channel");
            }
        }

        useLambda = readOrdinal(ini, image, "useLambdaOnInterface") != 0;
        lambdaChannel = useLambda ? "lambdaValue" : "AFRValue";
        targetTableName = useLambda ? "lambdaTableTbl" : "afrTableTbl";
    }

    private boolean bindTargetTable(IniFileModel ini, ConfigurationImage image) {
        TableModel t = ini.getTable(targetTableName);
        if (t == null) {
            return false;
        }
        ArrayIniField xField = requireArray(ini, t.getXBinsConstant());
        ArrayIniField yField = requireArray(ini, t.getYBinsConstant());
        ArrayIniField zField = requireArray(ini, t.getZBinsConstant());
        if (xField == null || yField == null || zField == null) {
            return false;
        }
        int rows = yField.getRows();
        int cols = xField.getRows();
        targetRpmAxis = readAxis1d(image, xField);
        targetLoadAxis = readAxis1d(image, yField);
        targetZ = readZ(image, zField, rows, cols);
        return true;
    }

    // ---- Live data ----------------------------------------------------------

    private void onFrame() {
        if (!started || model == null) {
            return;
        }
        SensorCentral sc = SensorCentral.getInstance();
        double rpm = sc.getValue("RPMValue");
        double load = sc.getValue(loadChannel);
        double lambda = sc.getValue(lambdaChannel);
        double gego = sc.getValue("Gego");

        if (!Double.isFinite(rpm) || !Double.isFinite(load) || rpm < MIN_RPM
                || !Double.isFinite(lambda) || lambda <= 0) {
            return;
        }
        if (!lambdaInRange(lambda)) {
            return;
        }
        double target = targetFor(rpm, load);
        if (!Double.isFinite(target) || target <= 0) {
            return;
        }

        boolean egoActive = Math.abs(gego - 100.0) <= MAX_EGO_DEVIATION_PERCENT;
        model.addSample(rpm, load, lambda, target, egoActive);
    }

    private boolean lambdaInRange(double lambda) {
        double lo = useLambda ? 0.5 : 7.0;
        double hi = useLambda ? 1.5 : 25.0;
        return lambda >= lo && lambda <= hi;
    }

    private double targetFor(double rpm, double load) {
        if (targetRpmAxis == null || targetLoadAxis == null || targetZ == null) {
            return Double.NaN;
        }
        return VeAnalyzeModel.interpolate(rpm, load, targetRpmAxis, targetLoadAxis, targetZ);
    }

    // ---- Actions ------------------------------------------------------------

    private void toggleStartStop() {
        setStartState(!started);
    }

    private void setStartState(boolean enabled) {
        started = enabled;
        buttonStartStop.setText(started ? "Stop" : "Start");
    }

    private void applyCorrections() {
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        if (bp == null || model == null || sourceImage == null) {
            return;
        }
        double[][] suggested = model.getSuggestedVe();
        ConfigurationImage clone = sourceImage.clone();
        double multiplier = veZField.getMultiplier();
        for (int c = 0; c < nLoad; c++) {
            for (int r = 0; r < nRpm; r++) {
                int byteOffset = veZField.getOffset(c, r);
                int storageSize = veZField.getType().getStorageSize();
                ByteBuffer bb = clone.getByteBuffer(byteOffset, storageSize);
                int raw = (int) Math.round(suggested[c][r] / multiplier);
                veZField.getType().writeRawValue(bb, raw);
            }
        }
        ConfigurationImage toWrite = clone;
        uiContext.getLinkManager().submit(() -> bp.uploadChanges(toWrite));
        statusLabel.setText("Corrections applied.");
    }

    // ---- UI helpers ---------------------------------------------------------

    private void updateSummary() {
        if (model == null) {
            summaryLabel.setText(" ");
            return;
        }
        summaryLabel.setText(String.format("Cells with data: %d / %d (%.0f%%)",
            model.getCellsWithData(), model.getTotalCells(), model.getCoverage() * 100.0));
    }

    private void refreshTable() {
        if (model == null) {
            return;
        }
        table.setModel(new VeTableModel(model));
        table.createDefaultColumnsFromModel();
    }

    private static final class VeTableModel extends AbstractTableModel {
        private final VeAnalyzeModel model;
        private final double[] rpm;
        private final double[] load;

        VeTableModel(VeAnalyzeModel model) {
            this.model = model;
            this.rpm = model.getRpmAxis();
            this.load = model.getLoadAxis();
        }

        @Override
        public int getRowCount() {
            return load.length;
        }

        @Override
        public int getColumnCount() {
            return rpm.length;
        }

        @Override
        public String getColumnName(int column) {
            return String.format("%.0f", rpm[column]);
        }

        @Override
        public Object getValueAt(int rowIndex, int columnIndex) {
            return model.getSuggestedVe()[rowIndex][columnIndex];
        }
    }

    private static final class ConfidenceRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
                                                       boolean hasFocus, int row, int column) {
            Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            VeTableModel m = (VeTableModel) table.getModel();
            setText(String.format("%.1f", (Double) value));
            setHorizontalAlignment(SwingConstants.RIGHT);
            if (!isSelected) {
                switch (m.model.getConfidence(row, column)) {
                    case HIGH:   c.setBackground(new Color(0x90, 0xee, 0x90)); break;
                    case MEDIUM: c.setBackground(new Color(0xff, 0xe6, 0x66)); break;
                    case LOW:    c.setBackground(new Color(0xff, 0xb3, 0x66)); break;
                    default:     c.setBackground(new Color(0xe0, 0xe0, 0xe0)); break;
                }
            }
            setToolTipText(String.format("load=%.0f rpm=%.0f samples=%d",
                m.load[row], m.rpm[column], m.model.getSampleCounts()[row][column]));
            return c;
        }
    }

    // ---- Static helpers -----------------------------------------------------

    private static int readOrdinal(IniFileModel ini, ConfigurationImage image, String fieldName) {
        Optional<IniField> opt = ini.findIniField(fieldName);
        if (!opt.isPresent()) {
            return 0;
        }
        Double v = image.readNumericValue(opt.get());
        if (v == null) {
            return 0;
        }
        return (int) Math.round(v);
    }

    private static ArrayIniField requireArray(IniFileModel ini, String name) {
        if (name == null || name.isEmpty()) {
            return null;
        }
        Optional<IniField> opt = ini.findIniField(name);
        if (!opt.isPresent() || !(opt.get() instanceof ArrayIniField)) {
            return null;
        }
        return (ArrayIniField) opt.get();
    }

    private static double[] readAxis1d(ConfigurationImage image, ArrayIniField field) {
        int n = field.getRows();
        double[] axis = new double[n];
        double multiplier = field.getMultiplier();
        for (int i = 0; i < n; i++) {
            int byteOffset = field.getOffset(i, 0);
            int storageSize = field.getType().getStorageSize();
            ByteBuffer bb = image.getByteBuffer(byteOffset, storageSize);
            axis[i] = field.getType().readRawValue(bb) * multiplier;
        }
        return axis;
    }

    private static double[][] readZ(ConfigurationImage image, ArrayIniField field, int rows, int cols) {
        double[][] z = new double[rows][cols];
        double multiplier = field.getMultiplier();
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int byteOffset = field.getOffset(r, c);
                int storageSize = field.getType().getStorageSize();
                ByteBuffer bb = image.getByteBuffer(byteOffset, storageSize);
                z[r][c] = field.getType().readRawValue(bb) * multiplier;
            }
        }
        return z;
    }

    // ---- Public API ----------------------------------------------------------

    public JPanel getContent() {
        return content;
    }
}
