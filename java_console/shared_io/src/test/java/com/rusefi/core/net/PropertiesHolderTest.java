package com.rusefi.core.net;

import org.junit.jupiter.api.Test;

import java.util.Properties;

import static org.junit.jupiter.api.Assertions.assertEquals;

class PropertiesHolderTest {
    @Test
    void rollbackRootIsOptional() {
        Properties properties = new Properties();
        properties.setProperty("auto_update_root_url", "https://example.test/");
        assertEquals("https://example.test/",
            PropertiesHolder.getFirmwareRollbackRootUrl(properties));

        properties.setProperty("firmware_rollback_root_url", "https://example.test/build_server/lts");
        assertEquals("https://example.test/build_server/lts",
            PropertiesHolder.getFirmwareRollbackRootUrl(properties));
    }

    @Test
    void updateHelpUrlCanBeWhiteLabelSpecific() {
        Properties properties = new Properties();
        assertEquals(ConnectionAndMeta.RUSEFI_WIKI_DOWNLOAD_PAGE,
            PropertiesHolder.getUpdateHelpUrl(properties, ConnectionAndMeta.RUSEFI_WIKI_DOWNLOAD_PAGE));

        properties.setProperty("UPDATE_FW_HELP_URL", "https://example.test/controller-downloads");
        assertEquals("https://example.test/controller-downloads",
            PropertiesHolder.getUpdateHelpUrl(properties, ConnectionAndMeta.RUSEFI_WIKI_DOWNLOAD_PAGE));
    }
}
