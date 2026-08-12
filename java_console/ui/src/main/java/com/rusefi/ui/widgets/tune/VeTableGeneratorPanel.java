package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.tune.ve.ArchetypeBaseVeV1;
import com.rusefi.tune.ve.ArchetypeBaseVeV1Supercharged;
import com.rusefi.tune.ve.ArchetypeBaseVeV1Turbo;
import com.rusefi.tune.ve.VeGenerator;
import com.rusefi.tune.ve.VeTableBinding;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.util.List;
import java.util.Locale;
import java.util.Optional;
import java.util.function.Consumer;

/**
 * Embedded panel for generating an initial VE table using {@code archetype-base-ve-v1}.
 * Replaces the CalibrationDialogWidget content area; Apply/Discard restores the previous view.
 */
public class VeTableGeneratorPanel extends JPanel {

    private final IniFileModel ini;
    private final ConfigurationImage sourceImage;
    private final Consumer<ConfigurationImage> onApply;
    private final Runnable onClose;
    private final String applyLabel;
    private final String closeLabel;

    private VeTableBinding.BoundTable boundTable;

    private JComboBox<ArchetypeBaseVeV1.HeadArchetype> headCombo;
    private JComboBox<ArchetypeBaseVeV1.CamBehavior> camCombo;
    private JCheckBox vvtCheck;
    private JComboBox<ArchetypeBaseVeV1.Aspiration> aspirationCombo;
    private JSpinner idleRpmSpinner;
    private JSpinner maxRpmSpinner;
    private JButton genBtn;

    private JTabbedPane previewTabs;
    private JLabel statusLabel;
    private JButton applyButton;

    private double[][] proposedVe;

    public VeTableGeneratorPanel(IniFileModel ini, ConfigurationImage sourceImage,
                                 Consumer<ConfigurationImage> onApply, Runnable onClose) {
        this(ini, sourceImage, onApply, onClose, "Apply to working tune", "Discard");
    }

    public VeTableGeneratorPanel(IniFileModel ini, ConfigurationImage sourceImage,
                                 Consumer<ConfigurationImage> onApply, Runnable onClose,
                                 String applyLabel, String closeLabel) {
        super(new BorderLayout(6, 6));
        this.ini = ini;
        this.sourceImage = sourceImage.clone();
        this.onApply = onApply;
        this.onClose = onClose;
        this.applyLabel = applyLabel;
        this.closeLabel = closeLabel;

        try {
            boundTable = VeTableBinding.bind(ini, this.sourceImage);
        } catch (VeTableBinding.BindingError e) {
            boundTable = null;
        }

        setBorder(BorderFactory.createEmptyBorder(8, 8, 4, 8));

        add(buildQuestionnairePanel(), BorderLayout.WEST);
        add(buildPreviewPanel(),       BorderLayout.CENTER);
        add(buildSouthPanel(),         BorderLayout.SOUTH);

        if (boundTable == null) {
            statusLabel.setText("Cannot generate: Speed Density (LM_SPEED_DENSITY) with VE_None or VE_MAP required.");
            applyButton.setEnabled(false);
        }
    }

    // ---- questionnaire ----

    private JPanel buildQuestionnairePanel() {
        JPanel p = new JPanel(new GridBagLayout());
        p.setBorder(BorderFactory.createCompoundBorder(
            BorderFactory.createTitledBorder("Engine Profile"),
            BorderFactory.createEmptyBorder(4, 4, 4, 4)));

        GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(3, 6, 3, 6);
        c.anchor = GridBagConstraints.WEST;

        headCombo = new JComboBox<>(ArchetypeBaseVeV1.HeadArchetype.values());
        headCombo.setRenderer(enumRenderer());
        headCombo.setSelectedItem(ArchetypeBaseVeV1.HeadArchetype.STOCK_4V);

        camCombo = new JComboBox<>(ArchetypeBaseVeV1.CamBehavior.values());
        camCombo.setRenderer(enumRenderer());

        aspirationCombo = new JComboBox<>(ArchetypeBaseVeV1.Aspiration.values());
        aspirationCombo.setRenderer(enumRenderer());

        vvtCheck = new JCheckBox("VVT present");

        double idleDefault = prefillIdleRpm(ini, sourceImage);
        double maxDefault  = prefillMaxRpm(ini, sourceImage);
        idleRpmSpinner = new JSpinner(new SpinnerNumberModel(idleDefault, 400.0, 2000.0, 50.0));
        maxRpmSpinner  = new JSpinner(new SpinnerNumberModel(maxDefault,  1500.0, 20000.0, 100.0));

        int row = 0;
        addRow(p, c, row++, "Head archetype:", headCombo);
        addRow(p, c, row++, "Cam profile:",    camCombo);
        addRow(p, c, row++, "Aspiration:",     aspirationCombo);
        addRow(p, c, row++, "Idle RPM:",       idleRpmSpinner);
        addRow(p, c, row++, "Max RPM:",        maxRpmSpinner);

        c.gridx = 0; c.gridy = row++; c.gridwidth = 2;
        p.add(vvtCheck, c);
        c.gridwidth = 1;

        JLabel siNote = new JLabel(
            "<html>Note: version 1 supports four-stroke spark-ignition engines only.<br>" +
            "Two-stroke, rotary, and other types are not supported.</html>");
        siNote.setForeground(new Color(120, 80, 0));
        siNote.setFont(siNote.getFont().deriveFont(siNote.getFont().getSize() - 1f));

        c.insets = new Insets(8, 6, 4, 6);
        c.gridx = 0; c.gridy = row++; c.gridwidth = 2;
        p.add(siNote, c);
        c.gridwidth = 1;
        c.insets = new Insets(3, 6, 3, 6);

        genBtn = new JButton("Generate Preview");
        genBtn.setEnabled(boundTable != null);
        genBtn.addActionListener(e -> onGenerate());

        c.gridx = 0; c.gridy = row; c.gridwidth = 2;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.insets = new Insets(6, 6, 3, 6);
        p.add(genBtn, c);

        c.gridx = 0; c.gridy = row + 1; c.gridwidth = 2;
        c.weighty = 1.0; c.fill = GridBagConstraints.VERTICAL;
        p.add(Box.createVerticalGlue(), c);

        return p;
    }

    private static double prefillMaxRpm(IniFileModel ini, ConfigurationImage image) {
        try {
            Optional<IniField> opt = ini.findIniField("rpmHardLimit");
            if (!opt.isPresent()) return 7000;
            String str = ConfigurationImageGetterSetter.getStringValue(opt.get(), image).trim();
            double val = Double.parseDouble(str);
            if (Double.isFinite(val) && val >= 1500 && val <= 20000) return val;
        } catch (Exception ignored) {}
        return 7000;
    }

    private static double prefillIdleRpm(IniFileModel ini, ConfigurationImage image) {
        try {
            Optional<IniField> opt = ini.findIniField("cltIdleRpm");
            if (!opt.isPresent() || !(opt.get() instanceof ArrayIniField)) return 900;
            Double[][] vals = ConfigurationImageGetterSetter.getArrayValues((ArrayIniField) opt.get(), image);
            if (vals.length > 0 && vals[0].length > 0) {
                double hot = vals[0][vals[0].length - 1];
                if (Double.isFinite(hot) && hot >= 400 && hot <= 2000) return hot;
            }
        } catch (Exception ignored) {}
        return 900;
    }

    private void addRow(JPanel p, GridBagConstraints c, int row, String label, JComponent comp) {
        c.gridx = 0; c.gridy = row; c.fill = GridBagConstraints.NONE; c.weightx = 0; c.weighty = 0;
        p.add(new JLabel(label), c);
        c.gridx = 1; c.fill = GridBagConstraints.HORIZONTAL; c.weightx = 1.0;
        p.add(comp, c);
        c.weightx = 0;
    }

    // ---- preview panel ----

    private JPanel buildPreviewPanel() {
        previewTabs = new JTabbedPane();
        previewTabs.addTab("Current",  buildCurrentTab());
        previewTabs.addTab("Proposed", placeholder("Click 'Generate Preview' to compute."));
        previewTabs.addTab("Delta",    placeholder("Click 'Generate Preview' to compute."));

        JPanel p = new JPanel(new BorderLayout());
        p.add(previewTabs, BorderLayout.CENTER);
        return p;
    }

    private JComponent buildCurrentTab() {
        if (boundTable == null) {
            return placeholder("Binding failed — check fuel algorithm and load mode.");
        }
        return buildVeTable(boundTable.currentVe, boundTable.rpmAxis, boundTable.mapAxis, false);
    }

    // ---- south bar ----

    private JPanel buildSouthPanel() {
        statusLabel = new JLabel(" ");
        statusLabel.setBorder(BorderFactory.createEmptyBorder(2, 4, 2, 4));

        applyButton = new JButton(applyLabel);
        applyButton.setEnabled(false);
        applyButton.addActionListener(e -> onApply());

        JButton discardBtn = new JButton(closeLabel);
        discardBtn.addActionListener(e -> onClose.run());

        JLabel note = new JLabel("Apply schedules ECU RAM upload; use Burn to ECU for flash persistence.");
        note.setFont(note.getFont().deriveFont(Font.ITALIC, note.getFont().getSize() - 1f));

        JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT, 8, 4));
        buttons.add(note);
        buttons.add(applyButton);
        buttons.add(discardBtn);

        JPanel south = new JPanel(new BorderLayout());
        south.add(statusLabel, BorderLayout.NORTH);
        south.add(buttons,     BorderLayout.SOUTH);
        return south;
    }

    // ---- actions ----

    private void onGenerate() {
        if (boundTable == null) return;
        try {
            double idleRpm = ((Number) idleRpmSpinner.getValue()).doubleValue();
            double maxRpm  = ((Number) maxRpmSpinner.getValue()).doubleValue();

            ArchetypeBaseVeV1.Request req = new ArchetypeBaseVeV1.Request(
                true,
                idleRpm, maxRpm,
                (ArchetypeBaseVeV1.HeadArchetype) headCombo.getSelectedItem(),
                (ArchetypeBaseVeV1.CamBehavior)   camCombo.getSelectedItem(),
                vvtCheck.isSelected(),
                (ArchetypeBaseVeV1.Aspiration)     aspirationCombo.getSelectedItem()
            );

            VeGenerator gen;
            ArchetypeBaseVeV1.Aspiration asp = req.aspiration;
            if (asp == ArchetypeBaseVeV1.Aspiration.TURBOCHARGED) {
                gen = new ArchetypeBaseVeV1Turbo(req);
            } else if (asp == ArchetypeBaseVeV1.Aspiration.SUPERCHARGED) {
                gen = new ArchetypeBaseVeV1Supercharged(req);
            } else {
                gen = new ArchetypeBaseVeV1(req);
            }
            VeGenerator.Result result = gen.generate(boundTable.rpmAxis, boundTable.mapAxis);
            ConfigurationImage patched = VeTableBinding.applyToClone(sourceImage, boundTable, result.veTable);
            proposedVe = VeTableBinding.decodeZ(patched, boundTable);

            double[][] delta = new double[boundTable.nLoad][boundTable.nRpm];
            for (int r = 0; r < boundTable.nLoad; r++) {
                for (int col = 0; col < boundTable.nRpm; col++) {
                    delta[r][col] = proposedVe[r][col] - boundTable.currentVe[r][col];
                }
            }

            previewTabs.setComponentAt(1, buildVeTable(proposedVe, boundTable.rpmAxis, boundTable.mapAxis, false));
            previewTabs.setComponentAt(2, buildVeTable(delta,      boundTable.rpmAxis, boundTable.mapAxis, true));
            previewTabs.setSelectedIndex(1);
            applyButton.setEnabled(true);

            int changedCells = 0;
            double minDelta = Double.MAX_VALUE, maxDelta = -Double.MAX_VALUE;
            for (int r = 0; r < boundTable.nLoad; r++) {
                for (int col = 0; col < boundTable.nRpm; col++) {
                    double d = delta[r][col];
                    if (Math.abs(d) > 0.05) changedCells++;
                    if (d < minDelta) minDelta = d;
                    if (d > maxDelta) maxDelta = d;
                }
            }

            StringBuilder sb = new StringBuilder("Profile: ").append(result.profileId);
            appendWarnings(sb, result.warnings);
            appendWarnings(sb, boundTable.effectiveVeWarnings);
            sb.append(String.format(Locale.ROOT, "  |  Changed: %d cells  |  \u0394 [%.1f, %.1f]  |  Max |\u0394|: %.1f",
                changedCells, minDelta, maxDelta, Math.max(Math.abs(minDelta), Math.abs(maxDelta))));
            statusLabel.setText(sb.toString());

        } catch (IllegalArgumentException e) {
            statusLabel.setText("Invalid parameters: " + e.getMessage());
            applyButton.setEnabled(false);
        } catch (VeTableBinding.BindingError e) {
            statusLabel.setText("Encoding error: " + e.getMessage());
            applyButton.setEnabled(false);
        }
    }

    private void onApply() {
        if (proposedVe == null || boundTable == null) return;
        try {
            ConfigurationImage patched = VeTableBinding.applyToClone(sourceImage, boundTable, proposedVe);
            onApply.accept(patched);
            onClose.run();
        } catch (VeTableBinding.BindingError e) {
            JOptionPane.showMessageDialog(SwingUtilities.getWindowAncestor(this),
                "Apply failed: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    // ---- table builders ----

    private static JComponent buildVeTable(double[][] data, double[] rpmAxis, double[] mapAxis, boolean isDelta) {
        double lo = Double.MAX_VALUE, hi = -Double.MAX_VALUE;
        if (!isDelta) {
            for (double[] row : data) for (double v : row) { lo = Math.min(lo, v); hi = Math.max(hi, v); }
        }
        final double min = lo, max = hi;

        JTable table = new JTable(new VePreviewModel(data, rpmAxis, mapAxis)) {
            @Override
            public Component prepareRenderer(TableCellRenderer renderer, int row, int column) {
                Component c = super.prepareRenderer(renderer, row, column);
                if (!isCellSelected(row, column)) {
                    if (column == 0) {
                        c.setBackground(getBackground());
                    } else {
                        try {
                            double val = Double.parseDouble(String.valueOf(getModel().getValueAt(row, column)));
                            if (isDelta) {
                                c.setBackground(val > 0.05 ? new Color(180, 230, 180) : val < -0.05 ? new Color(230, 180, 180) : Color.WHITE);
                            } else {
                                double ratio = (max > min) ? Math.max(0, Math.min(1, (val - min) / (max - min))) : 0.5;
                                c.setBackground(new Color((int)(173 + ratio * 82), (int)(216 - ratio * 34), (int)(230 - ratio * 37)));
                            }
                            c.setForeground(Color.BLACK);
                        } catch (NumberFormatException ignored) {}
                    }
                }
                return c;
            }
        };

        DefaultTableCellRenderer centeredRenderer = new DefaultTableCellRenderer();
        centeredRenderer.setHorizontalAlignment(SwingConstants.CENTER);
        table.setDefaultRenderer(Object.class, centeredRenderer);
        table.getTableHeader().setReorderingAllowed(false);
        table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
        table.setRowHeight(20);
        table.getColumnModel().getColumn(0).setPreferredWidth(62);
        for (int i = 1; i < table.getColumnCount(); i++) {
            table.getColumnModel().getColumn(i).setPreferredWidth(50);
        }
        return new JScrollPane(table);
    }

    private static JLabel placeholder(String text) {
        return new JLabel(text, SwingConstants.CENTER);
    }

    private static void appendWarnings(StringBuilder sb, List<String> warnings) {
        for (String w : warnings) {
            sb.append(" | WARNING: ").append(w);
        }
    }

    // ---- inner model ----

    private static class VePreviewModel extends AbstractTableModel {
        private final double[][] data;
        private final double[] rpmAxis;
        private final double[] mapAxis;

        VePreviewModel(double[][] data, double[] rpmAxis, double[] mapAxis) {
            this.data = data;
            this.rpmAxis = rpmAxis;
            this.mapAxis = mapAxis;
        }

        @Override public int getRowCount()    { return data.length; }
        @Override public int getColumnCount() { return data[0].length + 1; }

        @Override
        public String getColumnName(int col) {
            if (col == 0) return "kPa \\ RPM";
            double v = (col - 1 < rpmAxis.length) ? rpmAxis[col - 1] : Double.NaN;
            return Double.isFinite(v) ? String.format(Locale.ROOT, "%.0f", v) : "?";
        }

        @Override
        public Object getValueAt(int row, int col) {
            int rev = data.length - 1 - row;
            if (col == 0) {
                double v = rev < mapAxis.length ? mapAxis[rev] : Double.NaN;
                return Double.isFinite(v) ? String.format(Locale.ROOT, "%.0f", v) : "?";
            }
            return String.format(Locale.ROOT, "%.1f", data[rev][col - 1]);
        }
    }

    @SuppressWarnings("rawtypes")
    private static DefaultListCellRenderer enumRenderer() {
        return new DefaultListCellRenderer() {
            @Override
            public Component getListCellRendererComponent(JList<?> list, Object value, int index,
                                                          boolean isSelected, boolean hasFocus) {
                String label = value == null ? "" :
                    (value instanceof ArchetypeBaseVeV1.HeadArchetype) ? ((ArchetypeBaseVeV1.HeadArchetype) value).label :
                    (value instanceof ArchetypeBaseVeV1.CamBehavior)   ? ((ArchetypeBaseVeV1.CamBehavior)   value).label :
                    (value instanceof ArchetypeBaseVeV1.Aspiration)     ? ((ArchetypeBaseVeV1.Aspiration)     value).label :
                    value.toString();
                return super.getListCellRendererComponent(list, label, index, isSelected, hasFocus);
            }
        };
    }
}
