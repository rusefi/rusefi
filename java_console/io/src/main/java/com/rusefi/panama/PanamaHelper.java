package com.rusefi.panama;

import com.opensr5.ini.IniMemberNotFound;
import com.opensr5.ini.field.IniField;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class PanamaHelper {
    public static final String MCUSERIAL = "MCUSERIAL";

    public static IniField getIniField(final BinaryProtocol bp) throws IniMemberNotFound {
        return bp.getIniFile().getOutputChannel(MCUSERIAL);
    }

    public static IniField getIniField(LinkManager linkManager) throws IniMemberNotFound {
        return getIniField(linkManager.getBinaryProtocol());
    }

    /** @param byteOrder [tag:image_byte_order] byte order of the output channel block */
    public static int getMcuSerial(IniField mcuSerialField, ByteOrder byteOrder) {
        ByteBuffer bb = ISensorHolder.getByteBuffer(SensorCentral.getInstance().getResponse(), "error", mcuSerialField.getOffset(), 4, byteOrder);
        return bb.getInt();
    }
}
