package com.rusefi.tools.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.config.generated.Fields;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Objects;

public class WriteSimulatorConfiguration {
    // f407-discovery is historically the most inclusive .ini file
    public static final String INI_FILE_FOR_SIMULATOR = "../firmware/tunerstudio/generated/rusefi_f407-discovery.ini";

    public static void main(String[] args) throws IOException, InterruptedException, JAXBException {
        try {
            writeTune();
        } catch (Throwable e) {
            System.err.println("Unfortunately " + e);
        } finally {
            // No way to set Process.exec to be a daemon, we need explicit exit
            System.exit(0);
        }
    }

    private static void writeTune() throws JAXBException, IOException {
        byte[] fileContent = Files.readAllBytes(new File(Fields.SIMULATOR_TUNE_BIN_FILE_NAME).toPath());
        System.out.println("Got " + fileContent.length + " from " + Fields.SIMULATOR_TUNE_BIN_FILE_NAME + " while expecting " + Fields.TOTAL_CONFIG_SIZE);
        if (fileContent.length != Fields.TOTAL_CONFIG_SIZE)
            throw new IllegalStateException("Unexpected image size " + fileContent.length);
        ConfigurationImage configuration = new ConfigurationImage(fileContent);
        System.out.println("Got " + Objects.requireNonNull(configuration, "configuration"));
        IniFileModel ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
        if (ini == null)
            throw new IllegalStateException("Not found " + INI_FILE_FOR_SIMULATOR);
        Msq m = MsqFactory.valueOf(configuration, ini);
        m.writeXmlFile(TuneCanTool.DEFAULT_TUNE);

        Msq newTuneJustToValidate = Msq.readTune(TuneCanTool.DEFAULT_TUNE);
        System.out.println("Looks valid " + newTuneJustToValidate);
    }
}
