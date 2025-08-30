package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.FileNotFoundException;
import java.util.Objects;

public class TestTuneMigrationContext extends TuneMigrationContext {
    public static TestTuneMigrationContext load(final String testDataFolder) throws JAXBException {
        final TestTuneMigrationContext result;
        try {
            result = new TestTuneMigrationContext(
                Msq.readTune(String.format("%s/prev_calibrations.msq", testDataFolder)),
                IniFileModelImpl.readIniFile(String.format("%s/prev_calibrations.ini", testDataFolder)),
                Msq.readTune(String.format("%s/updated_calibrations.msq", testDataFolder)),
                IniFileModelImpl.readIniFile(String.format("%s/updated_calibrations.ini", testDataFolder)),
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
        return getValue(getPrevTune(), fieldName);
    }

    public Constant getUpdatedValue(final String fieldName) {
        return getValue(getUpdatedTune(), fieldName);
    }

    private static CalibrationsInfo getCalibrationsInfo(final Msq msq, final IniFileModel ini) {
        Objects.requireNonNull(ini);
        final ConfigurationImage image = msq.asImage(ini);
        final ConfigurationImageMetaVersion0_0 meta = new ConfigurationImageMetaVersion0_0(
            image.getSize(),
            ini.getSignature()
        );
        final ConfigurationImageWithMeta imageWithMeta = new ConfigurationImageWithMeta(meta, image.getContent());
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
        super(prevIni, prevMsq, updatedIni, updatedMsq, callbacks);
    }
}
