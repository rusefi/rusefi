package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.MsqFactory;
import com.rusefi.tune.xml.Msq;

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

    public Msq generateMsq() {
        return MsqFactory.valueOf(getImage().getConfigurationImage(), getIniFile());
    }
}
