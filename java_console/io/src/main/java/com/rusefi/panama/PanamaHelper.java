package com.rusefi.panama;

import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;

import java.nio.ByteBuffer;

public class PanamaHelper {
    public static final String MCUSERIAL = "MCUSERIAL";

    public static IniField getIniField(final BinaryProtocol bp) throws IniMemberNotFound {
        return bp.getIniFile().getOutputChannel(MCUSERIAL);
    }

    public static IniField getIniField(LinkManager linkManager) throws IniMemberNotFound {
        return getIniField(linkManager.getBinaryProtocol());
    }

    public static int getMcuSerial(IniField mcuSerialField) {
        ByteBuffer bb = ISensorHolder.getByteBuffer(SensorCentral.getInstance().getResponse(), "error", mcuSerialField.getOffset());
        return bb.getInt();
    }
}
