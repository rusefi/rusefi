package com.opensr5;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileReader;
import com.opensr5.ini.RawIniFile;

import java.io.File;
import java.io.FileNotFoundException;

public class Main {

    public static void main(String[] args) throws FileNotFoundException {
        System.out.println("OpenSR5 - load/program tune via serial port utility");
        System.out.println("  Andrey Belomutskiy, (c) 2012-2020\n");
        System.out.println("  https://github.com/rusefi/opensr5_flash");
        System.out.flush();

        if (args.length < 1) {
            System.err.println("Usage:");
            System.err.println("opensr5 DEFINITION_FILE.INI");
            System.exit(-1);
        }

        String projectIniFileName = args[0];

        RawIniFile content = IniFileReader.read(new File(projectIniFileName));

        IniFileMetaInfo meta = new IniFileMetaInfoImpl(content);

        System.out.println("nPages = " + meta.getnPages());
        System.out.println("blockingFactor = " + meta.getBlockingFactor());

    }
}
