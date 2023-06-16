package com.rusefi.tools;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.IoUtil;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.io.LinkManager;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

public class WriteSimulatorConfiguration {
    // f407-discovery is historically the most inclusive .ini file
    private static final String INI_FILE_FOR_SIMULATOR = "../firmware/tunerstudio/generated/rusefi_f407-discovery.ini";
    private static final String FOLDER = "generated";

    public static void main(String[] args) throws IOException, InterruptedException, JAXBException {
        // SimulatorExecHelper
        LinkManager linkManager = new LinkManager();
        IoUtil.connectToSimulator(linkManager, true);
        BinaryProtocol bp = Objects.requireNonNull(linkManager.getBinaryProtocol(), "getBinaryProtocol");
        ConfigurationImage configuration = bp.getControllerConfiguration();
        System.out.println("Got " + Objects.requireNonNull(configuration, "configuration"));
        IniFileModel ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
        if (ini == null)
            throw new IllegalStateException("Not found " + INI_FILE_FOR_SIMULATOR);
        Msq m = MsqFactory.valueOf(configuration, ini);
        new File(FOLDER).mkdirs();
        m.writeXmlFile(FOLDER + File.separator + "simulator_tune.xml");
        System.exit(0);
    }
}
