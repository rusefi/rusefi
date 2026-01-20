package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.opensr5.ini.field.IniField;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.FileNotFoundException;
import java.util.Collections;
import java.util.Objects;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TestTuneMigrationContext extends TuneMigrationContext {
    public static TestTuneMigrationContext load(final String testDataFolder) throws JAXBException {
        final TestTuneMigrationContext result;
        try {
            result = new TestTuneMigrationContext(
                Msq.readTune(String.format("%s/prev_calibrations.msq", testDataFolder)),
                IniFileReaderUtil.readIniFile(String.format("%s/prev_calibrations.ini", testDataFolder)),
                Msq.readTune(String.format("%s/updated_calibrations.msq", testDataFolder)),
                IniFileReaderUtil.readIniFile(String.format("%s/updated_calibrations.ini", testDataFolder)),
                new TestCallbacks()
            );
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        return result;
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

    public Constant getPrevValue(final String fieldName) {
        Constant value = getValue(getPrevTune(), fieldName);
        if (value == null)
            throw new NullPointerException("Failed to locate " + fieldName);
        return value;
    }

    public Constant getUpdatedValue(final String fieldName) {
        return getValue(getUpdatedTune(), fieldName);
    }

    public Constant getMigratedValue(final String fieldName) {
        return getMigratedConstants().get(fieldName);
    }

    public void checkPrevAndUpdatedIniFields(
        final String iniFieldName,
        final IniField prevIniField,
        final IniField updatedIniField
    ) {
        assertEquals(prevIniField, getPrevIniFile().getIniField(iniFieldName));
        assertEquals(updatedIniField, getUpdatedIniFile().getIniField(iniFieldName));
    }

    public void checkValueMigration(
        final String iniFieldName,
        final Constant prevValue,
        final Constant updatedValue,
        final Constant migratedValue
    ) {
        assertEquals(prevValue, getPrevValue(iniFieldName));
        assertEquals(updatedValue, getUpdatedValue(iniFieldName));
        assertEquals(migratedValue, getMigratedConstants().get(iniFieldName));
    }

    private static CalibrationsInfo getCalibrationsInfo(final Msq msq, final IniFileModel ini) {
        Objects.requireNonNull(ini);
        final ConfigurationImage image = msq.asImage(ini);
        ConfigurationImageWithMeta imageWithMeta = ConfigurationImageWithMeta.valueOf(ini, image);
        return new CalibrationsInfo(ini, imageWithMeta);
    }

    private Constant getValue(final Msq msq, final String fieldName) {
        return msq.getConstantsAsMap().get(fieldName);
    }

    public TestTuneMigrationContext(
        final Msq prevMsq,
        final IniFileModel prevIni,
        final Msq updatedMsq,
        final IniFileModel updatedIni,
        final UpdateOperationCallbacks callbacks
    ) {
        super(prevIni, prevMsq, updatedIni, updatedMsq, callbacks, Collections.emptySet());
    }
}
