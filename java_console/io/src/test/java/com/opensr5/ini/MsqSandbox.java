package com.opensr5.ini;

import com.opensr5.ConfigurationImage;
import com.opensr5.io.ConfigurationImageFile;
import com.rusefi.binaryprotocol.IniNotFoundException;
import com.rusefi.binaryprotocol.RealIniFileProvider;
import com.rusefi.tune.xml.Msq;

import javax.xml.bind.JAXBException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;

public class MsqSandbox {
    public static void main(String[] args) throws JAXBException, IOException {
        if (args.length != 1) {
            System.out.println("Exactly one fileName argument expected");
            System.exit(-1);
        }
        String fileName = args[0];
        Msq msq = Msq.readTune(fileName);

        String signature = msq.versionInfo.getSignature();


        final IniFileModel iniFile;
        try {
            iniFile = new RealIniFileProvider().provide(signature);
        } catch (IniNotFoundException e) {
            System.err.println("error: " + e);
            System.exit(-2);
            return;
        }
        ConfigurationImage ci = msq.asImage(iniFile);


        try (FileOutputStream fos = new FileOutputStream("output.bin")) {
            fos.write(ci.getContent());
        }
    }
}
