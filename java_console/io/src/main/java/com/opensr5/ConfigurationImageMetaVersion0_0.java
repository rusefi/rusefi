package com.opensr5;

import com.opensr5.ini.IniFileModel;
import org.jetbrains.annotations.NotNull;

public class ConfigurationImageMetaVersion0_0 extends ConfigurationImageMeta {
    private int imageSize = 0;
    private String ecuSignature;

    @SuppressWarnings("unused") // used by org.yaml.snakeyaml.Yaml.load method
    public ConfigurationImageMetaVersion0_0() {
    }

    public ConfigurationImageMetaVersion0_0(final int imageSize, final String ecuSignature) {
        this.imageSize = imageSize;
        this.ecuSignature = ecuSignature;
    }

    public static @NotNull ConfigurationImageMetaVersion0_0 getMeta(IniFileModel iniFile) {
        return new ConfigurationImageMetaVersion0_0(iniFile.getMetaInfo().getPageSize(0), iniFile.getMetaInfo().getSignature());
    }

    @Override
    public int getImageSize() {
        return imageSize;
    }

    @SuppressWarnings("unused") // used by org.yaml.snakeyaml.Yaml.load method
    public void setImageSize(int imageSize) {
        this.imageSize = imageSize;
    }

    @SuppressWarnings("unused") // used by org.yaml.snakeyaml.Yaml.dump method
    public String getEcuSignature() {
        return ecuSignature;
    }

    @SuppressWarnings("unused") // used by org.yaml.snakeyaml.Yaml.load method
    public void setEcuSignature(String ecuSignature) {
        this.ecuSignature = ecuSignature;
    }
}
