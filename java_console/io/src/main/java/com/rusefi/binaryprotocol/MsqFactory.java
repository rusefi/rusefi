package com.rusefi.binaryprotocol;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.config.generated.Fields;
import com.rusefi.tune.xml.Msq;

public class MsqFactory {
    public static Msq valueOf(ConfigurationImage image, IniFileModel ini) {
        return Msq.valueOf(image, ini.getMetaInfo().getTotalSize(), Fields.TS_SIGNATURE, ini);
    }
}
