package com.rusefi.maintenance;

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
