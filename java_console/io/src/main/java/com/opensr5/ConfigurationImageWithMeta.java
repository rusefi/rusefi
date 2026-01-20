package com.opensr5;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * calibration binary snapshot with signature info
 */
public class ConfigurationImageWithMeta {
    public static ConfigurationImageWithMeta VOID = new ConfigurationImageWithMeta(null, new byte[0]);

    private final ConfigurationImageMeta meta;
    @NotNull
    private final ConfigurationImage image;

    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta) {
        image = new ConfigurationImage(meta.getImageSize());
        this.meta = meta;
    }

    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta, final byte[] content) {
        image = new ConfigurationImage(content);
        this.meta = meta;
    }

    public static @NotNull ConfigurationImageWithMeta valueOf(IniFileModel ini, ConfigurationImage image) {
        ConfigurationImageMetaVersion0_0 meta = new ConfigurationImageMetaVersion0_0(
            image.getSize(),
            ini.getSignature()
        );
        return new ConfigurationImageWithMeta(meta, image.getContent());
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
