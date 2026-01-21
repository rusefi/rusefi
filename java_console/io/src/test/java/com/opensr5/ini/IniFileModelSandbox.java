package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.rusefi.ini.reader.IniFileReader;

public class IniFileModelSandbox {
    private static final Logging log = Logging.getLogging(IniFileModelSandbox.class);

    private static void prettyPrintGauge(GaugeModel gauge) {
        if (gauge != null) {
            log.info("gauge title: " + gauge.getTitle());
            log.info("gauge units: " + gauge.getUnits());
            log.info("gauge channel: " + gauge.getChannel());
            log.info("gauge lowValue: " + gauge.getLowValue());
            log.info("gauge highValue: " + gauge.getHighValue());

            log.info("-----------------------");
        }
    }

    public static void main(String[] args) {
        IniFileModel iniFile = IniFileModelTestHelper.findAndReadIniFile(IniFileReader.INI_FILE_PATH);
        log.info("Dialogs: " + iniFile.getDialogs());
        GaugeModel gauge = iniFile.getGauge("idleAirValvePositionGauge");
        prettyPrintGauge(gauge);

        GaugeCategoryModel gaugeCategoryModel = iniFile.getGaugeCategories().get("Idle");
        if (gaugeCategoryModel != null) {
            log.info("gauge category title: " + gaugeCategoryModel.getName());
            log.info("gauge category gauges: ");
            for (GaugeModel categoryGauge : gaugeCategoryModel.getGauges()) {
                prettyPrintGauge(categoryGauge);
            }

        } else {
            log.warn("Gauge category 'Idle' not found in the INI file");
        }
    }
}
