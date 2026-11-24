package com.opensr5;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.nio.ByteOrder;

/**
 * calibration binary snapshot with signature info
 */
public class ConfigurationImageWithMeta {
    public static ConfigurationImageWithMeta VOID = new ConfigurationImageWithMeta(null, new byte[0], ByteOrder.LITTLE_ENDIAN);

    private final ConfigurationImageMeta meta;
    @NotNull
    private final ConfigurationImage image;

    /**
     * [tag:image_byte_order] There is deliberately no constructor without a byte order: the
     * content is the controller's raw bytes, see IniFileModel#getByteOrder().
     */
    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta, final ByteOrder byteOrder) {
        image = new ConfigurationImage(meta.getImageSize(), byteOrder);
        this.meta = meta;
    }

    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta, final byte[] content, final ByteOrder byteOrder) {
        image = new ConfigurationImage(content, byteOrder);
        this.meta = meta;
    }

    public static @NotNull ConfigurationImageWithMeta valueOf(IniFileModel ini, ConfigurationImage image) {
        ConfigurationImageMetaVersion0_0 meta = new ConfigurationImageMetaVersion0_0(
            image.getSize(),
            ini.getSignature()
        );
        return new ConfigurationImageWithMeta(meta, image.getContent(), ini.getByteOrder());
    }

    public ConfigurationImageMeta getMeta() {
        return meta;
    }

    // todo: migrate API to NotNull one day?
    @Nullable
    public ConfigurationImage getConfigurationImage() {
        return image.isEmpty() ? null : image;
    }

    public boolean isEmpty() {
        return image.isEmpty();
    }
}
