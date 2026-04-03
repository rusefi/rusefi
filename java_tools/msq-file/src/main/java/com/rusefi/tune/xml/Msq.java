package com.rusefi.tune.xml;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.rusefi.tune.ConfigurationImageGetterSetter2;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.UiVersion;
import com.rusefi.xml.XmlUtil;
import org.jetbrains.annotations.NotNull;

import jakarta.xml.bind.JAXBException;
import jakarta.xml.bind.annotation.*;
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
        versionInfo = new VersionInfo(Integer.toString(UiVersion.CONSOLE_VERSION));
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
            if (field == null) {
                log.info("asImage: skipping unknown field " + constant.getName());
                continue;
            }
            log.debug("Setting " + field);
            ConfigurationImageGetterSetter2.setValue(field, ci, constant);
        }
        return ci;
    }

    /**
     * Applies constants from this MSQ onto an existing image instead of a blank one.
     * Fields present in the MSQ but absent from {@code instance} are skipped.
     * Fields present in {@code base} but absent from the MSQ retain their current value.
     * This is preferable to {@link #asImage} when loading into a live ECU because it
     * preserves the correct image size and handles firmware-version differences gracefully.
     */
    public ConfigurationImage applyOnto(ConfigurationImage base, IniFileModel instance) {
        Objects.requireNonNull(instance, "ini model");
        ConfigurationImage ci = base.clone();
        Page page = findPage();
        if (page == null) return ci;
        for (Constant constant : page.constant) {
            if (constant.getName().startsWith("UNALLOCATED_SPACE")) continue;
            IniField field = instance.getAllIniFields().get(constant.getName());
            if (field == null) {
                log.info("applyOnto: skipping unknown field " + constant.getName());
                continue;
            }
            try {
                ConfigurationImageGetterSetter2.setValue(field, ci, constant);
            } catch (IllegalArgumentException e) {
                log.info("applyOnto: skipping incompatible value for " + constant.getName() + ": " + e.getMessage());
            }
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
        String value = ConfigurationImageGetterSetter.getStringValue(field, image);
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

