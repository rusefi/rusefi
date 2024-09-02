package com.opensr5;

public class ConfigurationImageWithMeta extends ConfigurationImage {
    private final ConfigurationImageMeta meta;

    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta) {
        super(meta.getImageSize());
        this.meta = meta;
    }

    public ConfigurationImageWithMeta(final ConfigurationImageMeta meta, final byte[] content) {
        super(content);
        this.meta = meta;
    }

    public ConfigurationImageMeta getMeta() {
        return meta;
    }
}
