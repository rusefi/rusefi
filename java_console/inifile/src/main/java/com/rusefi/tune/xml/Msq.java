package com.rusefi.tune.xml;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.Version;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import javax.xml.bind.JAXBException;
import javax.xml.bind.annotation.*;
import java.io.File;
import java.io.IOException;
import java.util.*;

@XmlRootElement
public class Msq {
    public static final Locale TS_INTEGRATION_LOCALE = Locale.US;
    private static final Logging log = Logging.getLogging(Msq.class);

    public List<Page> page = new ArrayList<>();

    public final VersionInfo versionInfo;

    public Bibliography bibliography = new Bibliography();

    public Msq() {
        versionInfo = new VersionInfo(Integer.toString(Version.CONSOLE_VERSION));
    }

    static {
        log.info("java=" + System.getProperty("java.version"));
    }

    /**
     * @see #asImage
     */
    @NotNull
    public static Msq valueOf(ConfigurationImage image, int totalConfigSize, String tsSignature, IniFileModel ini) {
        Objects.requireNonNull(image, "image valueOf");
        Msq tune = create(totalConfigSize, tsSignature);
        for (String key : ini.getAllIniFields().keySet())
            tune.loadConstant(ini, key, image);
        return tune;
    }

    @NotNull
    public static Msq create(int totalConfigSize, String tsSignature) {
        Msq tune = new Msq();
        tune.versionInfo.setSignature(tsSignature);
        // TODO: document what on earth is this null/null page about?!
        tune.page.add(new Page(null, null));
        tune.page.add(new Page(0, totalConfigSize));
        return tune;
    }

    /**
     * @see #valueOf for opposite operation
     */
    public ConfigurationImage asImage(IniFileModel instance) {
        Objects.requireNonNull(instance, "ini model");
        ConfigurationImage ci = new ConfigurationImage(instance.getMetaInfo().getPageSize(0));

        Page page = findPage();
        if (page == null)
            return ci;
        for (Constant constant : page.constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) {
                continue;
            }
            IniField field = instance.getAllIniFields().get(constant.getName());
            Objects.requireNonNull(field, "Field for " + constant.getName());
            log.debug("Setting " + field);
            IniField.setValue(field, ci, constant);
        }
        return ci;
    }

    public static Msq readTune(String fileName) throws JAXBException {
        return XmlUtil.readModel(Msq.class, new File(fileName));
    }

    public void writeXmlFile(String outputXmlFileName) throws JAXBException, IOException {
        Objects.requireNonNull(versionInfo, "versionInfo");
        versionInfo.validate();
        Page page = findPage();
        if (page.constant.isEmpty())
            throw new IllegalStateException("Empty Msq file " + page);
        XmlUtil.writeXml(this, Msq.class, outputXmlFileName);
    }

    public void loadConstant(IniFileModel ini, String key, ConfigurationImage image) {
        IniField field = ini.getAllIniFields().get(key);
        String value = IniField.getValue(field, image);
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

    public Map<String, Constant> getConstantsAsMap() {
        return findPage().getConstantsAsMap();
    }
}
