package com.rusefi.ini;

import com.opensr5.ini.IniFileModel;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertSame;
import static org.mockito.Mockito.mock;

class IniFileStateTest {
    @Test
    void offlineIniIsIsolatedFromLiveIniCacheAndRestoredAfterOfflineMode() {
        UIContext context = new UIContext();
        IniFileModel liveIni = mock(IniFileModel.class);
        IniFileModel offlineIni = mock(IniFileModel.class);
        context.iniFileState.setIniFileModelForTest(liveIni);
        context.setIniFileModelForOffline(offlineIni);

        context.setOfflineMode(true);
        assertSame(offlineIni, context.iniFileState.getIniFileModel());

        context.setOfflineMode(false);
        assertSame(liveIni, context.iniFileState.getIniFileModel());
    }
}
