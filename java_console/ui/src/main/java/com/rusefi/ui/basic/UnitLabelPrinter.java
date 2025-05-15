package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;

import java.awt.*;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

public enum UnitLabelPrinter {
    INSTANCE;

    private static final Logging log = getLogging(UnitLabelPrinter.class);

    public boolean printUnitLabel(
        final JComponent parent,
        final SerialPortScanner.PortResult ecuPort,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        final Optional<CalibrationsInfo> currentCalibrations = CalibrationsHelper.readCurrentCalibrations(
            ecuPort,
            callbacks
        );

        if (currentCalibrations.isPresent()) {
            final UnitIdentifiers unitIdentifiers = new UnitIdentifiers(currentCalibrations.get(), callbacks);
            if (!unitIdentifiers.isEmpty()) {
                if (JOptionPane.showConfirmDialog(
                    parent,
                    String.format("Connected unit has: \n\n%s\nWould you like to print unit label?", unitIdentifiers),
                    "Unit identification",
                    JOptionPane.YES_NO_OPTION
                ) == JOptionPane.YES_OPTION) {
                    try {
                        Desktop.getDesktop().print(unitIdentifiers.writeToTempFile().toFile());
                        result = true;
                    } catch (final IOException e) {
                        log.error("Failed to form temporary file:", e);
                        callbacks.logLine("Failed to form temporary file");
                    }
                } else {
                    callbacks.logLine("User selected not to print unit label...");
                    result = true;
                }
            } else {
                callbacks.logLine("No unit identifier ini-fields found in current calibrations...");
            }
        } else {
            callbacks.logLine("Failed to read current calibrations...");
        }
        return result;
    }

    private static class UnitIdentifiers {
        private static final String UID_SUM_FIELD_NAME = "uidSum";
        private static final String SHORT_UID_FIELD_NAME = "shortUid";
        private static final String HW_REVISION_FIELD_NAME = "hwRevision";

        private final LinkedHashMap<String, String> identifiers = new LinkedHashMap<>();

        UnitIdentifiers(final CalibrationsInfo calibrationsInfo, final UpdateOperationCallbacks callbacks) {
            final Optional<String> uidSum = readFieldValue(UID_SUM_FIELD_NAME, calibrationsInfo, callbacks);
            uidSum.ifPresent(value -> {
                // `IniField.getValue` method returns value as string representation of a double.
                // Let's parse it back into `Double` to display them to users as integers:
                final double doubleValue = Double.parseDouble(value);
                identifiers.put("Long UID", String.format("%.0f", doubleValue));
            });
            final Optional<String> shortUid = readFieldValue(SHORT_UID_FIELD_NAME, calibrationsInfo, callbacks);
            shortUid.ifPresent(value -> {
                // `IniField.getValue` method returns value as string representation of a double.
                // Let's parse it back into `Double` to display them to users as integers:
                final double doubleValue = Double.parseDouble(value);
                identifiers.put("Short UID", String.format("%.0f", doubleValue));
            });
            final Optional<String> hwRevision = readFieldValue(HW_REVISION_FIELD_NAME, calibrationsInfo, callbacks);
            hwRevision.ifPresent(value -> identifiers.put("Hardware", value));
        }

        boolean isEmpty() {
            return identifiers.isEmpty();
        }

        public Path writeToTempFile() throws IOException {
            final Path result = Files.createTempFile("unit_id_", ".txt");
            try (final FileWriter fileWriter = new FileWriter(result.toString(), false);
                 final BufferedWriter bufferedWriter = new BufferedWriter(fileWriter)
            ) {
                for (final Map.Entry<String, String> identifier : identifiers.entrySet()) {
                    bufferedWriter.write(String.format("%s:\t%s", identifier.getKey(), identifier.getValue()));
                    bufferedWriter.newLine();
                }
            }
            return result;
        }

        @Override
        public String toString() {
            final StringBuilder result = new StringBuilder();
            for (final Map.Entry<String, String> identifier : identifiers.entrySet()) {
                result.append(String.format("%s: %s\n", identifier.getKey(), identifier.getValue()));
            }
            return result.toString();
        }

        private static Optional<String> readFieldValue(
            final String fieldName,
            final CalibrationsInfo calibrationsInfo,
            final UpdateOperationCallbacks callbacks
        ) {
            final Optional<IniField> iniField = calibrationsInfo.getIniFile().findIniField(fieldName);
            if (!iniField.isPresent()) {
                callbacks.logLine(String.format("Calibrations don't contain `%s` field", fieldName));
            }
            return iniField.map(field -> field.getValue(calibrationsInfo.getImage().getConfigurationImage()));
        }
    }
}
