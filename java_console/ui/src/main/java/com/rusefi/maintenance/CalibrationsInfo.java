package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;

public class CalibrationsInfo {
    private final IniFileModel iniFile;
    private final ConfigurationImageWithMeta image;

    public CalibrationsInfo(final IniFileModel iniFile, final ConfigurationImageWithMeta image) {
        this.iniFile = iniFile;
        this.image = image;
    }

    public IniFileModel getIniFile() {
        return iniFile;
    }

    public ConfigurationImageWithMeta getImage() {
        return image;
    }
}
