package com.rusefi.maintenance;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.nio.ByteOrder;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * [tag:image_byte_order] An image built from a tune file is uploaded to the controller as is, so
 * it must be encoded in the byte order declared by the ini.
 */
public class MsqAsImageByteOrderTest {
    @Test
    public void imageOfABigEndianController() {
        IniField word = new ScalarIniField("word", 0, "", FieldType.UINT16, 1, "0", 0);
        IniFileModel ini = mock(IniFileModel.class);
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(ini.getMetaInfo()).thenReturn(meta);
        when(meta.getPageSize(0)).thenReturn(2);
        when(ini.getByteOrder()).thenReturn(ByteOrder.BIG_ENDIAN);
        when(ini.getAllIniFields()).thenReturn(Collections.singletonMap("word", word));
        Msq msq = Msq.create(2, "big");
        msq.findPage().constant.add(new Constant("word", "", "258", "0"));

        ConfigurationImage image = msq.asImage(ini);

        assertEquals(ByteOrder.BIG_ENDIAN, image.getByteOrder());
        assertArrayEquals(new byte[]{1, 2}, image.getContent());
    }
}
