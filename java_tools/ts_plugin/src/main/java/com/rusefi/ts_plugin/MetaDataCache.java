package com.rusefi.ts_plugin;

import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.RawIniFile;
import com.rusefi.TsTuneReader;
import org.jetbrains.annotations.Nullable;

import java.io.FileNotFoundException;

public class MetaDataCache {

    private static String cachedProjectName;
    private static IniFileMetaInfo cache;

    @Nullable
    public synchronized static IniFileMetaInfo getModel(String projectName) {
        if (projectName == null)
            return null;
        if (!projectName.equals(cachedProjectName)) {
            cache = null;
        }
        if (cache == null) {
            System.out.println("Reading meta " + projectName);
            String modeFileName = TsTuneReader.getProjectModeFileName(projectName);
            try {
                cache = new IniFileMetaInfoImpl(RawIniFile.read(modeFileName));
            } catch (FileNotFoundException e) {
                System.out.println("No luck reading " + modeFileName + ": " + e);
                return null;
            }
            cachedProjectName = projectName;
        }
        return cache;
    }

}
