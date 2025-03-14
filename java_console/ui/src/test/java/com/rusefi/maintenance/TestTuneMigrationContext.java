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

public class TestTuneMigrationContext extends TuneMigrationContext {
    public static TestTuneMigrationContext load(final String testDataFolderName) throws JAXBException {
        final TestTuneMigrationContext result = new TestTuneMigrationContext(
            Msq.readTune(String.format(
                "src/test/java/com/rusefi/maintenance/%s/prev_calibrations.msq",
                testDataFolderName
            )),
            IniFileModelImpl.readIniFile(String.format(
                "src/test/java/com/rusefi/maintenance/%s/prev_calibrations.ini",
                testDataFolderName
            )),
            Msq.readTune(String.format(
                "src/test/java/com/rusefi/maintenance/%s/updated_calibrations.msq",
                testDataFolderName
            )),
            IniFileModelImpl.readIniFile(String.format(
                "src/test/java/com/rusefi/maintenance/%s/updated_calibrations.ini",
                testDataFolderName
            )),
            new TestCallbacks()
        );
        return result;
    }

    TestCallbacks getTestCallbacks() {
        return (TestCallbacks) getCallbacks();
    }

    CalibrationsInfo getPrevCalibrationsInfo() {
        return getCalibrationsInfo(getPrevTune(), getPrevIniFile());
    }

    CalibrationsInfo getUpdatedCalibrationsInfo() {
        return getCalibrationsInfo(getUpdatedTune(), getUpdatedIniFile());
    }

    Constant getPrevValue(final String fieldName) {
        return getValue(getPrevTune(), fieldName);
    }

    Constant getUpdatedValue(final String fieldName) {
        return getValue(getUpdatedTune(), fieldName);
    }

    private static CalibrationsInfo getCalibrationsInfo(final Msq msq, final IniFileModel ini) {
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

    private TestTuneMigrationContext(
        final Msq prevMsq,
        final IniFileModel prevIni,
        final Msq updatedMsq,
        final IniFileModel updatedIni,
        final UpdateOperationCallbacks callbacks
    ) {
        super(prevIni, prevMsq, updatedIni, updatedMsq, callbacks);
    }
}
