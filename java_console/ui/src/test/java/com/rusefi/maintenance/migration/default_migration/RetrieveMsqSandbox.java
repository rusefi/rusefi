package com.rusefi.maintenance.migration.default_migration;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;
import java.io.IOException;

/* Sandbox test for debugging msq-file retrieving: */
public class RetrieveMsqSandbox {
    //@Test
    void retrievePrevCalibrationsMsq() throws IOException, JAXBException {
        final ConfigurationImageWithMeta image = ConfigurationImageFile.readFromFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/prev_calibrations.zip"
        );
        final IniFileModel ini = IniFileModelImpl.readIniFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/prev_calibrations.ini"
        );
        final Msq tune = MsqFactory.valueOf(image.getConfigurationImage(), ini);
        tune.writeXmlFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/retrieved_prev_calibrations.msq"
        );
    }
}
