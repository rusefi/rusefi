package com.opensr5;

public abstract class ConfigurationImageMeta {
    public String getZipEntryName() {
        return getZipEntryName(getClass());
    }

    public abstract int getImageSize();

    public abstract String getEcuSignature();

    public static <MetaType extends ConfigurationImageMeta> String getZipEntryName(final Class<MetaType> clazz) {
        return String.format("%s.yaml", clazz.getSimpleName());
    }
}
