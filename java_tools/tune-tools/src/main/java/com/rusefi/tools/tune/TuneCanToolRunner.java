package com.rusefi.tools.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.RootHolder;
import com.rusefi.enums.engine_type_e;

import javax.xml.bind.JAXBException;
import java.io.IOException;

import static com.rusefi.tools.tune.WriteSimulatorConfiguration.INI_FILE_FOR_SIMULATOR;

public class TuneCanToolRunner extends TuneCanTool {
    static {
        initialize(INI_FILE_FOR_SIMULATOR);
    }

    protected static void initialize(String iniFileForSimulator) {
        ini = new IniFileModelImpl().readIniFile(iniFileForSimulator);
        if (ini == null)
            throw new IllegalStateException("Not found " + iniFileForSimulator);
/*
        Set<String> allFields = new TreeSet<>();
        allFields.addAll(ini.allIniFields.keySet());
        allFields.removeAll(ini.fieldsInUiOrder.keySet());
        if (!allFields.isEmpty())
            throw new IllegalStateException("What about all these fields not mentioned on the UI: " + allFields);
*/
        RootHolder.ROOT = "../firmware/";
    }

    public static void main(String[] args) throws JAXBException, IOException {
        runPopular();
    }

    public static void runPopular() throws JAXBException, IOException {
        // while adding a line here make sure corresponding line is at rusEfiFunctionalTest.cpp
        // https://github.com/rusefi/rusefi/issues/4038
        processREOtune(1621, engine_type_e.HONDA_OBD1, "Honda-OBD1", "");
        processREOtune(985, engine_type_e.MAZDA_MIATA_NB2, "MazdaMiataNB2", "nb2");
        processREOtune(1508, engine_type_e.HELLEN_154_HYUNDAI_COUPE_BK1, "COUPE-BK1", "coupleBK1");
        processREOtune(1507, engine_type_e.HELLEN_154_HYUNDAI_COUPE_BK2, "COUPE-BK2", "coupleBK2");
        processREOtune(1626, engine_type_e.HYUNDAI_PB, "PB", "pb");
        processREOtune(1591, engine_type_e.BMW_M52, "M52", "");
        processREOtune(1641, engine_type_e.HELLEN_121_NISSAN_6_CYL, "VQ", "");
        processREOtune(1622, engine_type_e.MERCEDES_M111, "m111-alex", "");
    }
}
