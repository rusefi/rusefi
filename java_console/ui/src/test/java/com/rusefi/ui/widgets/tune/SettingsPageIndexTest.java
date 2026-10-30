package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfo;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.ui.widgets.tune.SettingsPageIndex.Page;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class SettingsPageIndexTest {
    private static IniFileModel ini() throws Throwable {
        String text = "[Constants]\npage = 1\n"
            + "idleMode = bits, U08, 0, [0:0], \"Open loop\", \"Closed loop\"\n"
            + "x = array, U08, 1, [2], \"\", 1, 0, 0, 100, 0\n"
            + "z = array, U08, 3, [2x2], \"\", 1, 0, 0, 100, 0\n"
            + "[Menu]\n"
            + "menu = \"&Idle\"\n"
            + "subMenu = idleSettings, \"Idle settings\"\n"
            + "subMenu = std_separator\n"
            + "subMenu = idlePid, \"Idle PID\", 0, { idleMode }\n"
            + "subMenu = idleTable, \"Idle target table\"\n"
            + "groupMenu = \"Advanced idle\"\n"
            + "groupChildMenu = idleTiming, \"Idle timing\"\n"
            + "menu = \"Fuel\"\n"
            + "subMenu = injectors, \"Injectors\"\n"
            + "subMenu = fuelPid, \"PID fuel trim\"\n"
            + "subMenu = fuelIdle, \"Idle\"\n"
            + "subMenu = idleSettings, \"Idle settings again\"\n"
            + "subMenu = std_ms3Rtc, \"Real-time clock\"\n"
            + "subMenu = missingDialog, \"Not in this INI\"\n"
            + "[UserDefined]\n"
            + "dialog = idleSettings, \"Idle settings\"\nfield = \"Mode\", idleMode\n"
            + "dialog = idlePid, \"Idle PID\"\nfield = \"Mode\", idleMode\n"
            + "dialog = idleTiming, \"Idle timing\"\nfield = \"Mode\", idleMode\n"
            + "dialog = injectors, \"Injectors\"\nfield = \"Mode\", idleMode\n"
            + "dialog = fuelPid, \"PID fuel trim\"\nfield = \"Mode\", idleMode\n"
            + "dialog = fuelIdle, \"Idle\"\nfield = \"Mode\", idleMode\n"
            + "[TableEditor]\ntable = idleTable, idleMap, \"Idle target table\", 1\nxBins = x\nyBins = x\nzBins = z\n";
        IniFileMetaInfo meta = mock(IniFileMetaInfo.class);
        when(meta.getnPages()).thenReturn(1);
        when(meta.getPageSize(0)).thenReturn(8);
        when(meta.getSignature()).thenReturn("search-test");
        return IniFileReaderUtil.readIniFile(IniFileReaderUtil.read(
            new ByteArrayInputStream(text.getBytes(StandardCharsets.US_ASCII))), "search-test.ini", meta);
    }

    private static ConfigurationImage closedLoop(boolean on) {
        ConfigurationImage image = new ConfigurationImage(8);
        image.getContent()[0] = (byte) (on ? 1 : 0);
        return image;
    }

    private static List<String> keys(List<Page> pages) {
        List<String> keys = new ArrayList<>();
        for (Page page : pages) {
            keys.add(page.getKey());
        }
        return keys;
    }

    @Test
    void offersThePagesThatOpenAsWindowsInMenuOrder() throws Throwable {
        SettingsPageIndex index = SettingsPageIndex.build(ini(), closedLoop(true));
        assertEquals(Arrays.asList("idleSettings", "idlePid", "idleTable", "idleTiming", "injectors", "fuelPid", "fuelIdle"),
            keys(index.getPages()), "no separators, std_ pages, pages missing from the INI or repeats");
        Page timing = index.getPages().get(3);
        assertEquals(Arrays.asList("Idle", "Advanced idle"), timing.getMenuPath(), "the & mnemonic is gone");
        assertEquals("Idle › Advanced idle › Idle timing", timing.getLabel());
        assertEquals("Idle settings", index.getPages().get(0).getName(), "the first place in the menu wins");
    }

    @Test
    void disabledPagesAreLeftOutAsInTheTree() throws Throwable {
        assertFalse(keys(SettingsPageIndex.build(ini(), closedLoop(false)).getPages()).contains("idlePid"));
        assertTrue(keys(SettingsPageIndex.build(ini(), null).getPages()).contains("idlePid"),
            "without a tune nothing can be judged disabled");
    }

    @Test
    void everyWordMustMatchTheMenuPathBestNamesFirst() throws Throwable {
        SettingsPageIndex index = SettingsPageIndex.build(ini(), closedLoop(true));
        assertEquals(keys(index.getPages()), keys(index.search("  ")), "an empty query lists everything");
        assertEquals(Arrays.asList("fuelIdle", "idleSettings", "idlePid", "idleTable", "idleTiming"),
            keys(index.search("IDLE")), "the exact name first, then names starting with it, in menu order");
        assertEquals(Arrays.asList("fuelPid", "idlePid"), keys(index.search("pid")),
            "a name starting with the query before one only containing it");
        assertEquals(Arrays.asList("fuelPid", "injectors", "fuelIdle"), keys(index.search("fuel")),
            "names containing the words before pages matched only by their menu");
        assertEquals(Arrays.asList("idleTable", "idleTiming", "idleSettings"), keys(index.search("idle t")),
            "every word must match: Idle PID has no t");
        assertEquals(Collections.singletonList("idleTable"), keys(index.search("table idle")), "words in any order");
        assertEquals(Collections.singletonList("idleTiming"), keys(index.search("advanced timing")),
            "group names count too");
        assertTrue(index.search("boost").isEmpty());
    }

    @Test
    void realIniFindsTheUsualPages() throws Throwable {
        IniFileModel ini;
        try (InputStream input = SettingsPageIndexTest.class.getResourceAsStream("/26october.ini")) {
            RawIniFile raw = IniFileReaderUtil.read(input, "26october.ini");
            ini = IniFileReaderUtil.readIniFile(raw, "26october.ini", new IniFileMetaInfoImpl(raw));
        }
        SettingsPageIndex index = SettingsPageIndex.build(ini, null);
        assertTrue(index.getPages().size() > 250, "nearly the whole menu: " + index.getPages().size());
        assertTrue(keys(index.getPages()).contains("veTableDialog"));
        assertFalse(keys(index.getPages()).contains("veTableMap"), "a 3D view, not a settings page");
        assertEquals("Advanced › VVT outputs and PID", index.search("vvt").get(0).getLabel());
        assertEquals("Idle › Idle settings", index.search("idle").get(0).getLabel());
    }

    @Test
    void noIniNoPages() {
        assertTrue(SettingsPageIndex.build(null, null).getPages().isEmpty());
        assertTrue(SettingsPageIndex.build(null, null).search("idle").isEmpty());
    }
}
