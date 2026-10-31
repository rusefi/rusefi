package com.rusefi.maintenance.migration;

import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;
import java.nio.file.Path;
import java.util.Collections;
import java.util.Map;
import java.util.TreeMap;

/** harness for migrating one MSQ between two explicitly supplied INI layouts. */
public final class TuneMigrationSandbox {

    private TuneMigrationSandbox() {
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(TuneMigrationSandbox::show);
    }

    private static void show() {
        final JFrame frame = new JFrame("Tune Migration Sandbox");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        final JTextField tune = new JTextField(70);
        final JTextField sourceIni = new JTextField(70);
        final JTextField targetIni = new JTextField(70);
        final JTextField defaultTune = new JTextField(70);
        final JTextArea output = new JTextArea(9, 90);
        output.setEditable(false);
        output.setLineWrap(true);
        output.setWrapStyleWord(true);

        final JPanel inputs = new JPanel(new GridBagLayout());
        addFileRow(inputs, 0, "Tune MSQ", tune, "msq");
        addFileRow(inputs, 1, "Source INI", sourceIni, "ini");
        addFileRow(inputs, 2, "Target INI", targetIni, "ini");
        addFileRow(inputs, 3, "Target default MSQ", defaultTune, "msq");

        final JButton migrate = new JButton("Migrate");
        migrate.addActionListener(event -> {
            migrate.setEnabled(false);
            output.setText("Migrating...\n");
            final String tuneFile = tune.getText().trim();
            final String sourceIniFile = sourceIni.getText().trim();
            final String targetIniFile = targetIni.getText().trim();
            final String defaultTuneFile = defaultTune.getText().trim();
            new Thread(() -> {
                try {
                    final String result = migrate(
                        Path.of(tuneFile),
                        Path.of(sourceIniFile),
                        Path.of(targetIniFile),
                        Path.of(defaultTuneFile)
                    );
                    SwingUtilities.invokeLater(() -> output.setText(result));
                } catch (Exception e) {
                    SwingUtilities.invokeLater(() -> output.setText("Migration failed: " + e.getMessage()));
                } finally {
                    SwingUtilities.invokeLater(() -> migrate.setEnabled(true));
                }
            }, "tune migration").start();
        });

        final JPanel content = new JPanel(new BorderLayout(8, 8));
        content.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        content.add(inputs, BorderLayout.NORTH);
        content.add(new JScrollPane(output), BorderLayout.CENTER);
        content.add(migrate, BorderLayout.SOUTH);
        frame.setContentPane(content);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    private static void addFileRow(JPanel panel, int row, String label, JTextField field, String extension) {
        final GridBagConstraints constraints = new GridBagConstraints();
        constraints.gridy = row;
        constraints.insets = new Insets(3, 3, 3, 3);
        constraints.anchor = GridBagConstraints.WEST;
        panel.add(new JLabel(label), constraints);

        constraints.gridx = 1;
        constraints.weightx = 1;
        constraints.fill = GridBagConstraints.HORIZONTAL;
        panel.add(field, constraints);

        final JButton browse = new JButton("Browse...");
        browse.addActionListener(event -> {
            final JFileChooser chooser = new JFileChooser(new File(field.getText()).getAbsoluteFile().getParentFile());
            chooser.setFileFilter(new FileNameExtensionFilter(extension.toUpperCase() + " files", extension));
            if (chooser.showOpenDialog(panel) == JFileChooser.APPROVE_OPTION) {
                field.setText(chooser.getSelectedFile().getAbsolutePath());
            }
        });
        constraints.gridx = 2;
        constraints.weightx = 0;
        constraints.fill = GridBagConstraints.NONE;
        panel.add(browse, constraints);
    }

    static String migrate(Path tunePath, Path sourceIniPath, Path targetIniPath, Path defaultTunePath) throws Exception {
        if (tunePath.toString().isEmpty() || sourceIniPath.toString().isEmpty() ||
            targetIniPath.toString().isEmpty() || defaultTunePath.toString().isEmpty()) {
            throw new IllegalArgumentException("Select all four input files");
        }

        final Msq sourceTune = Msq.readTune(tunePath.toString());
        final Msq defaultTune = Msq.readTune(defaultTunePath.toString());
        final IniFileModel sourceIni = IniFileReaderUtil.readIniFile(sourceIniPath.toString());
        final IniFileModel nextIni = IniFileReaderUtil.readIniFile(targetIniPath.toString());
        if (!sourceTune.versionInfo.getSignature().equals(sourceIni.getSignature())) {
            throw new IllegalArgumentException("Tune and source INI signatures do not match");
        }
        if (!defaultTune.versionInfo.getSignature().equals(nextIni.getSignature())) {
            throw new IllegalArgumentException("Default tune and target INI signatures do not match");
        }

        final Map<Integer, ConfigurationImageWithMeta> pages = new TreeMap<>();
        for (int page = 0; page < nextIni.getMetaInfo().getnPages(); page++) {
            final int identifier = nextIni.getMetaInfo().getPageIdentifier(page);
            final int size = nextIni.getMetaInfo().getPageSize(page);
            pages.put(identifier, new ConfigurationImageWithMeta(
                new ConfigurationImageMetaVersion0_0(size, nextIni.getSignature()),
                new byte[size]
            ));
        }
        for (Constant constant : defaultTune.getConstantsAsMap().values()) {
            final IniField field = nextIni.findIniField(constant.getName()).orElseThrow(
                () -> new IllegalArgumentException("Default tune field is missing from target INI: " + constant.getName())
            );
            ConfigurationImageGetterSetter2.setValue(
                field,
                pages.get(field.getPageIndex()).getConfigurationImage(),
                constant
            );
        }

        final CalibrationsHelper.MergeResult result = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
            sourceIni,
            sourceTune,
            new CalibrationsInfo(nextIni, pages, Collections.emptySet()),
            UpdateOperationCallbacks.DUMMY,
            Collections.emptySet()
        );
        final CalibrationsInfo migrated = result.mergedCalibrations.orElseThrow(
            () -> new IllegalStateException("Migration produced no changes")
        );
        final Path output = tunePath.resolveSibling(tunePath.getFileName() + ".migrated.msq");
        migrated.generateMsq().writeXmlFile(output.toString());
        final int defaultPageCount = Integer.parseInt(defaultTune.versionInfo.getNPages());
        final int targetPageCount = nextIni.getMetaInfo().getnPages();

        return "Source signature: " + sourceIni.getSignature() + '\n' +
            "Target signature: " + nextIni.getSignature() + '\n' +
            "Default tune pages: " + defaultPageCount + " of " + targetPageCount + '\n' +
            "Pages to write: " + migrated.getPagesToWrite() + '\n' +
            "Failed fields: " + result.failedFields + '\n' +
            "Migrated tune: " + output.toAbsolutePath() +
            (defaultPageCount < targetPageCount
                ? "\n\nWarning: target pages absent from the default MSQ start zero-filled."
                : "");
    }
}
