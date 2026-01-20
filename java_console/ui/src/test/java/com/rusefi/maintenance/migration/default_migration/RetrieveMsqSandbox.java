package com.rusefi.maintenance.migration.default_migration;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.IOException;

public class RetrieveMsqSandbox {
    public static void main(String[] args) throws IOException, JAXBException {
        final ConfigurationImageWithMeta image = ConfigurationImageFile.readFromFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/prev_calibrations.zip"
        );
        final IniFileModel ini = IniFileReaderUtil.readIniFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/prev_calibrations.ini"
        );
        final Msq tune = MsqFactory.valueOf(image.getConfigurationImage(), ini);
        tune.writeXmlFile(
            "src/test/java/com/rusefi/maintenance/migration/sandbox/test_data/retrieved_prev_calibrations.msq"
        );
    }
}
