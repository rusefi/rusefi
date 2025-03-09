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

import static org.junit.jupiter.api.Assertions.assertEquals;

class CalibrationsTestData {
    static final String IGNITION_TABLE_FIELD_NAME = "ignitionTable";

    static final String PREV_IGNITION_TABLE_VALUE = "\n" +
        "         12.1 14.2 18.1 22.0 25.9 29.9 33.8 37.7 41.6 42.9 42.9 42.9 42.9 42.9 42.9 43.0\n" +
        "         12.0 14.0 17.9 21.7 25.6 29.5 33.3 37.2 41.0 42.3 42.3 42.3 42.3 42.3 42.3 42.4\n" +
        "         11.9 13.9 17.7 21.5 25.3 29.1 32.8 36.6 40.4 41.7 41.7 41.7 41.7 41.7 41.7 41.8\n" +
        "         11.7 13.7 17.4 21.2 24.9 28.7 32.4 36.1 39.9 41.1 41.1 41.1 41.1 41.1 41.1 41.2\n" +
        "         11.5 13.5 17.1 20.8 24.4 28.1 31.7 35.3 39.0 40.2 40.2 40.2 40.2 40.2 40.2 40.3\n" +
        "         11.4 13.3 16.9 20.5 24.1 27.7 31.2 34.8 38.4 39.6 39.6 39.6 39.6 39.6 39.6 39.7\n" +
        "         11.3 13.2 16.7 20.2 23.7 27.3 30.8 34.3 37.8 39.0 39.0 39.0 39.0 39.0 39.0 39.1\n" +
        "         11.2 13.0 16.5 19.9 23.4 26.9 30.3 33.9 37.2 38.4 38.4 38.4 38.4 38.4 38.4 38.5\n" +
        "         11.0 12.8 16.2 19.6 23.0 26.5 29.9 33.3 36.7 37.8 37.8 37.8 37.8 37.8 37.8 37.9\n" +
        "         10.8 12.6 15.9 19.2 22.5 25.9 29.2 32.5 35.8 36.9 36.9 36.9 36.9 36.9 36.9 37.0\n" +
        "         10.7 12.4 15.7 18.9 22.2 25.5 28.7 32.0 35.2 36.3 36.3 36.3 36.3 36.3 36.3 36.4\n" +
        "         10.6 12.3 15.5 18.7 21.9 25.1 28.2 31.4 34.6 35.7 35.7 35.7 35.7 35.7 35.7 35.8\n" +
        "         10.4 12.1 15.2 18.4 21.5 24.7 27.8 30.9 34.1 35.1 35.1 35.1 35.1 35.1 35.1 35.2\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n";

    static final String UPDATED_IGNITION_TABLE_VALUE = "\n" +
        "         12.2 14.2 18.1 22.0 25.9 29.9 33.8 37.7 41.6 42.9 42.9 42.9 42.9 42.9 42.9 42.9\n" +
        "         12.1 14.0 17.9 21.7 25.6 29.5 33.3 37.2 41.0 42.3 42.3 42.3 42.3 42.3 42.3 42.3\n" +
        "         12.0 13.9 17.7 21.5 25.3 29.1 32.8 36.6 40.4 41.7 41.7 41.7 41.7 41.7 41.7 41.7\n" +
        "         11.8 13.7 17.4 21.2 24.9 28.7 32.4 36.1 39.9 41.1 41.1 41.1 41.1 41.1 41.1 41.1\n" +
        "         11.6 13.5 17.1 20.8 24.4 28.1 31.7 35.3 39.0 40.2 40.2 40.2 40.2 40.2 40.2 40.2\n" +
        "         11.5 13.3 16.9 20.5 24.1 27.7 31.2 34.8 38.4 39.6 39.6 39.6 39.6 39.6 39.6 39.6\n" +
        "         11.4 13.2 16.7 20.2 23.7 27.3 30.8 34.3 37.8 39.0 39.0 39.0 39.0 39.0 39.0 39.0\n" +
        "         11.3 13.0 16.5 19.9 23.4 26.9 30.3 33.8 37.2 38.4 38.4 38.4 38.4 38.4 38.4 38.4\n" +
        "         11.1 12.8 16.2 19.6 23.0 26.5 29.9 33.3 36.7 37.8 37.8 37.8 37.8 37.8 37.8 37.8\n" +
        "         10.9 12.6 15.9 19.2 22.5 25.9 29.2 32.5 35.8 36.9 36.9 36.9 36.9 36.9 36.9 36.9\n" +
        "         10.8 12.4 15.7 18.9 22.2 25.5 28.7 32.0 35.2 36.3 36.3 36.3 36.3 36.3 36.3 36.3\n" +
        "         10.7 12.3 15.5 18.7 21.9 25.1 28.2 31.4 34.6 35.7 35.7 35.7 35.7 35.7 35.7 35.7\n" +
        "         10.5 12.1 15.2 18.4 21.5 24.7 27.8 30.9 34.1 35.1 35.1 35.1 35.1 35.1 35.1 35.1\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n";

    static final String ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME = "enableKnockSpectrogram";

    static final String PREV_ENABLE_KNOCK_SPECTROGRAM_VALUE = "\"false\"";

    static final String UPDATED_ENABLE_KNOCK_SPECTROGRAM_VALUE = "\"no\"";

    static CalibrationsTestData load() throws JAXBException {
        final CalibrationsTestData result = new CalibrationsTestData(
            Msq.readTune("src/test/java/com/rusefi/maintenance/test_data/prev_calibrations.msq"),
            IniFileModelImpl.readIniFile("src/test/java/com/rusefi/maintenance/test_data/prev_calibrations.ini"),
            Msq.readTune("src/test/java/com/rusefi/maintenance/test_data/updated_calibrations.msq"),
            IniFileModelImpl.readIniFile("src/test/java/com/rusefi/maintenance/test_data/updated_calibrations.ini")
        );
        assertEquals(
            PREV_IGNITION_TABLE_VALUE,
            result.getPrevValue(IGNITION_TABLE_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_IGNITION_TABLE_VALUE,
            result.getUpdatedValue(IGNITION_TABLE_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_ENABLE_KNOCK_SPECTROGRAM_VALUE,
            result.getPrevValue(ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_ENABLE_KNOCK_SPECTROGRAM_VALUE,
            result.getUpdatedValue(ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME).getValue()
        );

        return result;
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
