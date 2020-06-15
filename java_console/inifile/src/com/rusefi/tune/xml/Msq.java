package com.rusefi.tune.xml;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.generated.Fields;
import com.rusefi.ui.storage.PersistentConfiguration;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import javax.xml.bind.annotation.*;
import java.io.File;
import java.io.IOException;
import java.util.Objects;

@XmlRootElement
public class Msq {
    public static final String outputXmlFileName = PersistentConfiguration.RUSEFI_SETTINGS_FOLDER + File.separator + "output.msq";

    private Page page = new Page();

    private final VersionInfo versionInfo;

    public Msq() {
        versionInfo = new VersionInfo("rusEFI+2020");
    }

    @NotNull
    public static Msq valueOf(ConfigurationImage image) {
        IniFileModel ini = IniFileModel.getInstance();
        Msq tune = new Msq();
        for (String key : ini.allIniFields.keySet())
            tune.loadConstant(ini, key, image);
        return tune;
    }

    public ConfigurationImage asImage(IniFileModel instance) {
        ConfigurationImage ci = new ConfigurationImage(Fields.TOTAL_CONFIG_SIZE);

        for (Constant constant : getPage().constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            IniField field = instance.allIniFields.get(constant.getName());
            Objects.requireNonNull(field, "Field for " + constant.getName());
            System.out.println("Setting " + field);
            field.setValue(ci, constant);
        }
        return ci;
    }

    public static Msq readTune(String fileName) throws Exception {
        return XmlUtil.readModel(Msq.class, fileName);
    }

    public void writeXmlFile(String outputXmlFileName) throws JAXBException, IOException {
        XmlUtil.writeXml(this, Msq.class, outputXmlFileName);
    }

    public void loadConstant(IniFileModel ini, String key, ConfigurationImage image) {
        IniField field = ini.allIniFields.get(key);
        String value = field.getValue(image);
        getPage().constant.add(new Constant(field.getName(), field.getUnits(), value));
    }

    @XmlElement
    public Bibliography getBibliography() {
        return new Bibliography();
    }

    @XmlElement
    public VersionInfo getVersionInfo() {
        return versionInfo;
    }

    @XmlElement
    public Page getPage() {
        return page;
    }

    public void setPage(Page page) {
        this.page = page;
    }

    @XmlElement
    public Settings getSettings() {
        return new Settings();
    }

    @XmlElement
    public UserComments getUserComments() {
        return new UserComments();
    }
}
