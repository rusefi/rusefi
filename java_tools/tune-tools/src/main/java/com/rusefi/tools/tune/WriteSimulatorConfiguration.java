package com.rusefi.tools.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.*;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IniFileProvider;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.config.generated.Fields;
import com.rusefi.enums.engine_type_e;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.List;
import java.util.Map;
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
        if (args.length != 1)
            throw new IllegalArgumentException("One argument expected: .ini file name");
        String iniFileName = args[0];
        IniFileModelImpl ini = new IniFileModelImpl().readIniFile(iniFileName);
        BinaryProtocol.iniFileProvider = signature -> ini;

        System.out.println("ROOT_FOLDER=" + ROOT_FOLDER);
        try {
            try {
                readBinaryWriteXmlTune(iniFileName, Fields.SIMULATOR_TUNE_BIN_FILE_NAME, TuneCanTool.DEFAULT_TUNE, ini);
            } catch (Throwable e) {
                throw new IllegalStateException("White default tune", e);
            }
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
                writeSpecificEngineType(iniFileName, type, ini);
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

    private static void writeSpecificEngineType(String iniFileName, engine_type_e engineType, IniFileModelImpl ini) {
        try {
            String in = Fields.SIMULATOR_TUNE_BIN_FILE_NAME_PREFIX + "_" + engineType.ordinal() + Fields.SIMULATOR_TUNE_BIN_FILE_NAME_SUFFIX;
            readBinaryWriteXmlTune(iniFileName, in,
                    TuneCanTool.getDefaultTuneName(engineType), ini);
        } catch (Throwable e) {
            throw new IllegalStateException("With " + engineType, e);
        }
    }

    private static void readBinaryWriteXmlTune(String iniFileName, String inputBinaryTuneFileName, String outputXmlFileName, IniFileModel ini) throws JAXBException, IOException {
        // we have to use board-specific .ini to account for all the board-specific offsets
        // INI_FILE_FOR_SIMULATOR is just not universal enough
        if (ini == null)
            throw new IllegalStateException("Not found " + iniFileName);
        byte[] fileContent = Files.readAllBytes(new File(ROOT_FOLDER + inputBinaryTuneFileName).toPath());
        int pageSize = ini.getMetaInfo().getTotalSize();
        log.info("Got " + fileContent.length + " from " + inputBinaryTuneFileName + " while expecting " + pageSize);
        if (fileContent.length != pageSize)
            throw new IllegalStateException("Unexpected image size " + fileContent.length + " while expecting " + pageSize);
        ConfigurationImage configuration = new ConfigurationImage(fileContent);
        log.info("Got " + Objects.requireNonNull(configuration, "configuration"));
        Msq m = MsqFactory.valueOf(configuration, ini);
        String name = Fields.KNOCKNOISERPMBINS.getName();
        Constant noiseRpmBins = m.page.get(1).getConstantsAsMap().get(name);
        if (!noiseRpmBins.getValue().contains(Fields.DEFAULT_RPM_AXIS_HIGH_VALUE + ".0"))
            throw new IllegalStateException(name + " canary wonders if everything is fine?");
        m.writeXmlFile(ROOT_FOLDER + outputXmlFileName);

        Msq newTuneJustToValidate = Msq.readTune(ROOT_FOLDER + outputXmlFileName);
        log.info("Looks valid " + newTuneJustToValidate);
    }
}
