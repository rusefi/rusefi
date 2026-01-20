package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.tune.xml.Msq;

public class MsqFactory {
    public static Msq valueOf(ConfigurationImage image, IniFileModel ini) {
        return Msq.valueOf(image, ini.getMetaInfo().getPageSize(0), ini.getSignature(), ini);
    }
}
