package com.rusefi.panama;

import com.opensr5.ini.field.IniField;
import com.rusefi.config.generated.TsOutputs;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;

import java.nio.ByteBuffer;

public class PanamaHelper {
    public static IniField getIniField(LinkManager linkManager) {
        return linkManager.getBinaryProtocol().getIniFile().getOutputChannel(TsOutputs.MCUSERIAL.getName());
    }

    public static int getMcuSerial(IniField mcuSerialField) {
        ByteBuffer bb = ISensorHolder.getByteBuffer(SensorCentral.getInstance().getResponse(), "error", mcuSerialField.getOffset());
        return bb.getInt();
    }
}
