package com.rusefi.tune.xml;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.rusEFIVersion;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import javax.xml.bind.annotation.*;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

@XmlRootElement
public class Msq {
    private static final Logging log = Logging.getLogging(Msq.class);

    public List<Page> page = new ArrayList<>();

    public final VersionInfo versionInfo;

    public Bibliography bibliography = new Bibliography();

    public Msq() {
        versionInfo = new VersionInfo(Integer.toString(rusEFIVersion.CONSOLE_VERSION));
    }

    @NotNull
    public static Msq valueOf(ConfigurationImage image, int totalConfigSize, String tsSignature) {
        IniFileModel ini = IniFileModel.getInstance();
        Msq tune = create(totalConfigSize, tsSignature);
        for (String key : ini.allIniFields.keySet())
            tune.loadConstant(ini, key, image);
        return tune;
    }

    @NotNull
    public static Msq create(int totalConfigSize, String tsSignature) {
        Msq tune = new Msq();
        tune.versionInfo.setSignature(tsSignature);
        tune.page.add(new Page(null, null));
        tune.page.add(new Page(0, totalConfigSize));
        return tune;
    }

    public ConfigurationImage asImage(IniFileModel instance, int totalConfigSize) {
        ConfigurationImage ci = new ConfigurationImage(totalConfigSize);

        Page page = findPage();
        if (page == null)
            return ci;
        for (Constant constant : page.constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            IniField field = instance.allIniFields.get(constant.getName());
            Objects.requireNonNull(field, "Field for " + constant.getName());
            log.debug("Setting " + field);
            field.setValue(ci, constant);
        }
        return ci;
    }

    public static Msq readTune(String fileName) throws Exception {
        return XmlUtil.readModel(Msq.class, fileName);
    }

    public void writeXmlFile(String outputXmlFileName) throws JAXBException, IOException {
        Objects.requireNonNull(versionInfo, "versionInfo");
        versionInfo.validate();
        Page page = findPage();
        if (page.constant.isEmpty())
            throw new IllegalStateException("No data?");
        XmlUtil.writeXml(this, Msq.class, outputXmlFileName);
    }

    public void loadConstant(IniFileModel ini, String key, ConfigurationImage image) {
        IniField field = ini.allIniFields.get(key);
        String value = field.getValue(image);
        Page page = findPage();
        if (page == null) {
            log.error("Msq: No page");
            return;
        }
        if (value.isEmpty()) {
            log.debug("Skipping " + field.getName());
            return;
        }
        Constant constant = new Constant(field.getName(), field.getUnits(), value, field.getDigits());
        page.constant.add(constant);
        if (field instanceof ArrayIniField) {
            ArrayIniField arrayIniField = (ArrayIniField) field;
            constant.setCols(Integer.toString(arrayIniField.getCols()));
            constant.setRows(Integer.toString(arrayIniField.getRows()));
        }
    }

    public Page findPage() {
        for (Page p : page) {
            Integer size = p.getSize();
            if (size == null)
                continue;
            return p;
        }
        return null;
    }

    @XmlElement
    public Settings getSettings() {
        return new Settings();
    }

    @XmlElement
    public UserComments getUserComments() {
        return new UserComments();
    }

    public VersionInfo getVersionInfo() {
        return versionInfo;
    }
}
