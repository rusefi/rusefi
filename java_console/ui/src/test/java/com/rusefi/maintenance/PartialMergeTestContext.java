package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import jakarta.xml.bind.JAXBException;
import java.io.FileNotFoundException;
import java.util.Collections;
import java.util.Map;
import java.util.Set;

public class PartialMergeTestContext extends TuneMigrationContext {
    public static final String TEST_DATA_FOLDER = "src/test/java/com/rusefi/maintenance/migration/default_migration/test_data";

    public static PartialMergeTestContext load() throws JAXBException {
        try {
            return new PartialMergeTestContext(
                Msq.readTune(TEST_DATA_FOLDER + "/prev_calibrations.msq"),
                IniFileReaderUtil.readIniFile(TEST_DATA_FOLDER + "/prev_calibrations.ini"),
                Msq.readTune(TEST_DATA_FOLDER + "/updated_calibrations.msq"),
                IniFileReaderUtil.readIniFile(TEST_DATA_FOLDER + "/updated_calibrations.ini"),
                new TestCallbacks()
            );
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }

    public PartialMergeTestContext(
        final Msq prevMsq,
        final IniFileModel prevIni,
        final Msq updatedMsq,
        final IniFileModel updatedIni,
        final UpdateOperationCallbacks callbacks
    ) {
        super(prevIni, prevMsq, updatedIni, updatedMsq, callbacks, Collections.emptySet());
    }

    public TestCallbacks getTestCallbacks() {
        return (TestCallbacks) getCallbacks();
    }

    public CalibrationsInfo getPrevCalibrationsInfo() {
        return getCalibrationsInfo(getPrevTune(), getPrevIniFile());
    }

    public CalibrationsInfo getUpdatedCalibrationsInfo() {
        return getCalibrationsInfo(getUpdatedTune(), getUpdatedIniFile());
    }

    private static CalibrationsInfo getCalibrationsInfo(final Msq msq, final IniFileModel ini) {
        final ConfigurationImage image = msq.asImage(ini);
        ConfigurationImageWithMeta imageWithMeta = ConfigurationImageWithMeta.valueOf(ini, image);
        return new CalibrationsInfo(ini, imageWithMeta);
    }

    public CalibrationsHelper.MergeResult mergeWithSimulatedFailures(Set<String> simulatedFailures) {
        TestCallbacks callbacks = getTestCallbacks();
        IniFileModel prevIniFile = getPrevIniFile();
        Msq prevMsq = getPrevTune();
        CalibrationsInfo newCalibrations = getUpdatedCalibrationsInfo();
        Set<String> additionalIniFieldsToIgnore = Collections.emptySet();

        TuneMigrationContext context = new TuneMigrationContext(
            prevIniFile,
            prevMsq,
            newCalibrations.getIniFile(),
            newCalibrations.generateMsq(),
            callbacks,
            additionalIniFieldsToIgnore
        );
        com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator.INSTANCE.migrateTune(context);
        Set<Map.Entry<String, Constant>> valuesToUpdate = context.getMigratedConstants().entrySet();

        if (valuesToUpdate.isEmpty()) {
            return new CalibrationsHelper.MergeResult(java.util.Optional.empty(), new java.util.ArrayList<>());
        }

        java.util.List<String> failedFields = new java.util.ArrayList<>();
        ConfigurationImage mergedImage = newCalibrations.getImage().getConfigurationImage().clone();
        for (Map.Entry<String, Constant> valueToUpdate : valuesToUpdate) {
            String migratedFieldName = valueToUpdate.getKey();
            Constant migratedValue = valueToUpdate.getValue();
            java.util.Optional<IniField> fieldToUpdate = newCalibrations.getIniFile().findIniField(migratedFieldName);

            if (fieldToUpdate.isPresent()) {
                if (simulatedFailures.contains(migratedFieldName)) {
                    callbacks.log("WARNING: Failed to migrate field `" + migratedFieldName + "`, skipping: Simulated failure", true, false);
                    failedFields.add(migratedFieldName);
                    continue;
                }

                try {
                    ConfigurationImageGetterSetter2.setValue(fieldToUpdate.get(), mergedImage, migratedValue);
                } catch (Throwable e) {
                    callbacks.log("WARNING: Failed to migrate field `" + migratedFieldName + "`, skipping: " + e.getMessage(), true, false);
                    failedFields.add(migratedFieldName);
                }
            }
        }

        return new CalibrationsHelper.MergeResult(
            java.util.Optional.of(new CalibrationsInfo(
                newCalibrations.getIniFile(),
                new ConfigurationImageWithMeta(newCalibrations.getImage().getMeta(), mergedImage.getContent())
            )),
            failedFields
        );
    }
}
