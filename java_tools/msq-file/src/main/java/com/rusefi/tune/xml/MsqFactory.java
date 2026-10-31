package com.rusefi.tune.xml;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;

import java.util.Map;

public class MsqFactory {
    public static Msq valueOf(ConfigurationImage image, IniFileModel ini) {
        return Msq.valueOf(image, ini.getMetaInfo().getPageSize(0), ini.getSignature(), ini);
    }

    public static Msq valueOf(Map<Integer, ConfigurationImage> pages, IniFileModel ini) {
        return Msq.valueOf(pages, ini.getSignature(), ini);
    }
}
