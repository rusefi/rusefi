package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.util.Map;

class CalibrationsTestData {
    static CalibrationsTestData load() throws JAXBException {
        return new CalibrationsTestData(
            Msq.readTune("src/test/java/com/rusefi/maintenance/test_data/prev_calibrations.msq"),
            IniFileModelImpl.readIniFile("src/test/java/com/rusefi/maintenance/test_data/prev_calibrations.ini"),
            Msq.readTune("src/test/java/com/rusefi/maintenance/test_data/updated_calibrations.msq"),
            IniFileModelImpl.readIniFile("src/test/java/com/rusefi/maintenance/test_data/updated_calibrations.ini")
        );
    }

    Map<String, Constant> getPrevMsq() {
        return prevMsq.getConstantsAsMap();
    }

    IniFileModel getPrevIni() {
        return prevIni;
    }

    Map<String, Constant> getUpdatedMsq() {
        return updatedMsq.getConstantsAsMap();
    }

    IniFileModel getUpdatedIni() {
        return updatedIni;
    }

    CalibrationsInfo getPrevCalibrationsInfo() {
        return getCalibrationsInfo(prevMsq, prevIni);
    }

    CalibrationsInfo getUpdatedCalibrationsInfo() {
        return getCalibrationsInfo(updatedMsq, updatedIni);
    }

    Constant getPrevValue(final String fieldName) {
        return getValue(prevMsq, fieldName);
    }

    Constant getUpdatedValue(final String fieldName) {
        return getValue(updatedMsq, fieldName);
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

    private CalibrationsTestData(
        final Msq prevMsq,
        final IniFileModel prevIni,
        final Msq updatedMsq,
        final IniFileModel updatedIni
    ) {
        this.prevMsq = prevMsq;
        this.prevIni = prevIni;
        this.updatedMsq = updatedMsq;
        this.updatedIni = updatedIni;
    }

    private final Msq prevMsq;
    private final IniFileModel prevIni;
    private final Msq updatedMsq;
    private final IniFileModel updatedIni;
}
