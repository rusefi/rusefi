package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.IniField;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;

import javax.swing.*;

import java.awt.*;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;
import java.util.stream.Collectors;

import static com.devexperts.logging.Logging.getLogging;

public enum UnitLabelPrinter {
    INSTANCE;

    private static final Logging log = getLogging(UnitLabelPrinter.class);

    public static final Set<String> UNIT_IDENTIFIER_FIELD_NAMES = Arrays.stream(new String[] {
        UnitIdentifiers.UID_SUM_FIELD_NAME,
        UnitIdentifiers.SHORT_UID_FIELD_NAME,
        UnitIdentifiers.HW_REVISION_FIELD_NAME
    }).collect(Collectors.toSet());

    public boolean printUnitLabel(
        final JComponent parent,
        final PortResult ecuPort,
        final UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext
    ) {
        boolean result = false;
        final Optional<CalibrationsInfo> currentCalibrations = CalibrationsHelper.readCurrentCalibrations(
            ecuPort.port,
            callbacks, connectivityContext
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
                        final PrinterJob job = PrinterJob.getPrinterJob();
                        job.setPrintable(unitIdentifiers);
                        if (job.printDialog()) {
                            job.print();
                        } else {
                            callbacks.logLine("User closed `Print` dialog...");
                        }
                        result = true;
                    } catch (final PrinterException e) {
                        log.error("Failed to print unit identifiers:", e);
                        callbacks.logLine("Failed to print unit identifiers");
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

    private static class UnitIdentifiers implements Printable {
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
                identifiers.put("", String.format("%.0f", doubleValue));
            });
            final Optional<String> shortUid = readFieldValue(SHORT_UID_FIELD_NAME, calibrationsInfo, callbacks);
            shortUid.ifPresent(value -> {
                // `IniField.getValue` method returns value as string representation of a double.
                // Let's parse it back into `Double` to display them to users as integers:
                final double doubleValue = Double.parseDouble(value);
                identifiers.put("Short", String.format("%.0f", doubleValue));
            });
            final Optional<String> hwRevision = readFieldValue(HW_REVISION_FIELD_NAME, calibrationsInfo, callbacks);
            hwRevision.ifPresent(value -> identifiers.put("HW", value));
        }

        boolean isEmpty() {
            return identifiers.isEmpty();
        }

        @Override
        public String toString() {
            final StringBuilder result = new StringBuilder();
            write(result::append, () -> result.append("\n"));
            return result.toString();
        }

        private void write(final Consumer<String> stringWriter, final Runnable endOfLineWriter) {
            for (final Map.Entry<String, String> identifier : identifiers.entrySet()) {
                final String idKey = identifier.getKey();
                final String idValue = identifier.getValue();
                if (idKey.isEmpty()) {
                    stringWriter.accept(idValue);
                } else {
                    stringWriter.accept(String.format("%s: %s", idKey, idValue));
                }
                endOfLineWriter.run();
            }
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
            return iniField.map(field -> IniField.getValue(field, calibrationsInfo.getImage().getConfigurationImage()));
        }

        @Override
        public int print(final Graphics graphics, final PageFormat pageFormat, final int pageIndex) {
            if (pageIndex > 0) {
                return NO_SUCH_PAGE;
            }
            final Graphics2D g2d = (Graphics2D)graphics;
            g2d.translate(pageFormat.getImageableX(), pageFormat.getImageableY());
            final AtomicInteger intPosY = new AtomicInteger(10);
            write(
                line -> graphics.drawString(line, 0, intPosY.get()),
                () -> intPosY.addAndGet(10)
            );
            return PAGE_EXISTS;
        }
    }
}
