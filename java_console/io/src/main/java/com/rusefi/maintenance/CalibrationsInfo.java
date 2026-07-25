package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.tune.xml.Msq;

import java.util.*;

public class CalibrationsInfo {
    private final IniFileModel iniFile;
    private final Map<Integer, ConfigurationImageWithMeta> pages;
    private final Set<Integer> pagesToWrite;

    public CalibrationsInfo(final IniFileModel iniFile, final ConfigurationImageWithMeta image) {
        this(iniFile, Collections.singletonMap(0, image), Collections.emptySet());
    }

    public CalibrationsInfo(
        final IniFileModel iniFile,
        final Map<Integer, ConfigurationImageWithMeta> pages,
        final Set<Integer> pagesToWrite
    ) {
        this.iniFile = Objects.requireNonNull(iniFile, "iniFile");
        this.pages = Collections.unmodifiableMap(new TreeMap<>(pages));
        this.pagesToWrite = Collections.unmodifiableSet(new TreeSet<>(pagesToWrite));
        if (!this.pages.containsKey(0)) {
            throw new IllegalArgumentException("Main calibration page is missing");
        }
    }

    public IniFileModel getIniFile() {
        return iniFile;
    }

    public ConfigurationImageWithMeta getImage() {
        return pages.get(0);
    }

    public ConfigurationImageWithMeta getPage(int pageIdentifier) {
        return pages.get(pageIdentifier);
    }

    public Map<Integer, ConfigurationImageWithMeta> getPages() {
        return pages;
    }

    public Set<Integer> getPagesToWrite() {
        return pagesToWrite;
    }

    public CalibrationsInfo withAllPagesToWrite() {
        final Set<Integer> burnablePages = new TreeSet<>();
        for (int pageIndex = 0; pageIndex < iniFile.getMetaInfo().getnPages(); pageIndex++) {
            int pageIdentifier = iniFile.getMetaInfo().getPageIdentifier(pageIndex);
            if (pages.containsKey(pageIdentifier) && !iniFile.getMetaInfo().getBurnCommand(pageIndex).isEmpty()) {
                int actualSize = pages.get(pageIdentifier).getConfigurationImage().getSize();
                int expectedSize = iniFile.getMetaInfo().getPageSize(pageIndex);
                if (actualSize != expectedSize) {
                    throw new IllegalArgumentException(String.format(
                        "Calibration page 0x%04X has %d bytes, expected %d",
                        pageIdentifier,
                        actualSize,
                        expectedSize
                    ));
                }
                burnablePages.add(pageIdentifier);
            }
        }
        return new CalibrationsInfo(iniFile, pages, burnablePages);
    }

    public Msq generateMsq() {
        final Map<Integer, ConfigurationImage> images = new TreeMap<>();
        pages.forEach((page, image) -> images.put(page, image.getConfigurationImage()));
        return MsqFactory.valueOf(images, getIniFile());
    }
}
