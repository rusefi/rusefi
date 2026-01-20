package com.rusefi.tune.xml;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;

public class MsqFactory {
    public static Msq valueOf(ConfigurationImage image, IniFileModel ini) {
        return Msq.valueOf(image, ini.getMetaInfo().getPageSize(0), ini.getSignature(), ini);
    }
}
