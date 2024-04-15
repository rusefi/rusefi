package com.rusefi.tools.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.config.generated.Fields;
import com.rusefi.enums.engine_type_e;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

/**
 * see <a href="https://github.com/rusefi/rusefi/wiki/Canned-Tune-Process">...</a>
 */
public class WriteSimulatorConfiguration {
    private static final Logging log = getLogging(WriteSimulatorConfiguration.class);
    // f407-discovery is historically the most inclusive .ini file
    public static final String INI_FILE_FOR_SIMULATOR = "../firmware/tunerstudio/generated/rusefi_f407-discovery.ini";

    public static String ROOT_FOLDER = System.getProperty("ROOT_FOLDER", "../simulator/");

    public static void main(String[] args) throws IOException, InterruptedException, JAXBException {
        System.out.println("ROOT_FOLDER=" + ROOT_FOLDER);
        try {
            readBinaryWriteXmlTune(Fields.SIMULATOR_TUNE_BIN_FILE_NAME, TuneCanTool.DEFAULT_TUNE);
            for (engine_type_e type : new engine_type_e[]{
                    // [CannedTunes] see 'rusEfiFunctionalTest.cpp' which exports default tunes into binary files for us
                // [CannedTunes] TuneCanToolRunner for last third step
                    engine_type_e.MERCEDES_M111,
                    engine_type_e.BMW_M52,
                    engine_type_e.MAZDA_MIATA_NA6,
                    engine_type_e.MAZDA_MIATA_NA94,
                    engine_type_e.MAZDA_MIATA_NA96,
                    engine_type_e.MAZDA_MIATA_NB1,
                    engine_type_e.MAZDA_MIATA_NB2,
                    engine_type_e.HONDA_K,
                    engine_type_e.HONDA_OBD1,
                    engine_type_e.HELLEN_121_NISSAN_6_CYL,
                    engine_type_e.HELLEN_154_HYUNDAI_COUPE_BK1,
                    engine_type_e.HELLEN_154_HYUNDAI_COUPE_BK2,
                    engine_type_e.HYUNDAI_PB,
                    engine_type_e.MAVERICK_X3,
                    engine_type_e.POLARIS_RZR,
                    engine_type_e.HARLEY,
            }) {
                writeSpecificEngineType(type);
            }
            TuneCanToolRunner.runPopular();
        } catch (Throwable e) {
            log.error("Unfortunately", e);
            System.exit(-1);
        } finally {
            // No way to set Process.exec to be a daemon, we need explicit exit
            System.exit(0);
        }
    }

    private static void writeSpecificEngineType(engine_type_e engineType) {
        try {
            String in = Fields.SIMULATOR_TUNE_BIN_FILE_NAME_PREFIX + "_" + engineType.ordinal() + Fields.SIMULATOR_TUNE_BIN_FILE_NAME_SUFFIX;
            readBinaryWriteXmlTune(in,
                    TuneCanTool.getDefaultTuneName(engineType));
        } catch (Throwable e) {
            throw new IllegalStateException("With " + engineType, e);
        }
    }

    private static void readBinaryWriteXmlTune(String inputBinaryTuneFileName, String outputXmlFileName) throws JAXBException, IOException {
        byte[] fileContent = Files.readAllBytes(new File(ROOT_FOLDER + inputBinaryTuneFileName).toPath());
        System.out.println("Got " + fileContent.length + " from " + inputBinaryTuneFileName + " while expecting " + Fields.TOTAL_CONFIG_SIZE);
        if (fileContent.length != Fields.TOTAL_CONFIG_SIZE)
            throw new IllegalStateException("Unexpected image size " + fileContent.length + " while expecting " + Fields.TOTAL_CONFIG_SIZE);
        ConfigurationImage configuration = new ConfigurationImage(fileContent);
        System.out.println("Got " + Objects.requireNonNull(configuration, "configuration"));
        IniFileModel ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
        if (ini == null)
            throw new IllegalStateException("Not found " + INI_FILE_FOR_SIMULATOR);
        Msq m = MsqFactory.valueOf(configuration, ini);
        m.writeXmlFile(ROOT_FOLDER + outputXmlFileName);

        Msq newTuneJustToValidate = Msq.readTune(ROOT_FOLDER + outputXmlFileName);
        System.out.println("Looks valid " + newTuneJustToValidate);
    }
}
