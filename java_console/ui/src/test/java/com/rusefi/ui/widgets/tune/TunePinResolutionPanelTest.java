package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.IniFileModel;
import com.rusefi.ini.reader.IniFileReaderUtil;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import org.junit.jupiter.api.Test;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class TunePinResolutionPanelTest {
    private static final String CURRENT_INI = "../../firmware/tunerstudio/generated/rusefi_alphax-4chan.ini";

    @Test
    public void usesWizardCardWidth() {
        TunePinResolutionPanel panel = new TunePinResolutionPanel(() -> { }, () -> { });

        assertEquals(TunePinResolutionPanel.CARD_WIDTH, panel.getCardForTests().getPreferredSize().width);
        assertEquals(TunePinResolutionPanel.CARD_WIDTH, panel.getCardForTests().getMaximumSize().width);
    }

    @Test
    public void requiresEverySelectionAndAppliesResolvedPins() throws Exception {
        IniFileModel ini = IniFileReaderUtil.readIniFile(CURRENT_INI);
        Msq.ApplyResult result = unresolvedVvtPins(ini);
        AtomicInteger applyStarted = new AtomicInteger();
        TunePinResolutionPanel panel = new TunePinResolutionPanel(() -> { }, applyStarted::incrementAndGet);
        CompletableFuture<TunePinResolutionPanel.Resolution> future = new CompletableFuture<>();

        panel.showRequest(result, ini, true, future);

        assertEquals(2, panel.getIssueCountForUnitTest());
        assertFalse(panel.isApplyEnabledForUnitTest());
        panel.selectForUnitTest(0, "Aux9 - High1 legacy F2");
        assertFalse(panel.isApplyEnabledForUnitTest());
        panel.selectForUnitTest(1, "Aux10 - High2 legacy F4");
        assertTrue(panel.isApplyEnabledForUnitTest());

        panel.applyForUnitTest();
        TunePinResolutionPanel.Resolution resolution = future.get();
        ConfigurationImage image = result.resolvePins(resolution.getSelections());

        assertEquals(1, applyStarted.get());
        assertEquals("\"Aux9 - High1 legacy F2\"", ConfigurationImageGetterSetter.getStringValue(
            ini.findIniField("vvtPins1").orElseThrow(), image));
        assertEquals("\"Aux10 - High2 legacy F4\"", ConfigurationImageGetterSetter.getStringValue(
            ini.findIniField("vvtPins2").orElseThrow(), image));

    }

    @Test
    public void cancelLeavesNoResolution() throws Exception {
        IniFileModel ini = IniFileReaderUtil.readIniFile(CURRENT_INI);
        AtomicInteger cancelled = new AtomicInteger();
        TunePinResolutionPanel panel = new TunePinResolutionPanel(cancelled::incrementAndGet, () -> { });
        CompletableFuture<TunePinResolutionPanel.Resolution> future = new CompletableFuture<>();

        panel.showRequest(unresolvedVvtPins(ini), ini, false, future);
        panel.cancelForUnitTest();

        assertNull(future.get());
        assertEquals(1, cancelled.get());
    }

    private static Msq.ApplyResult unresolvedVvtPins(IniFileModel ini) {
        Msq tune = Msq.create(ini.getMetaInfo().getPageSize(0), "test");
        tune.findPage().constant.add(new Constant("vvtPins1", null, "\"F2 - VVT#1\"", null));
        tune.findPage().constant.add(new Constant("vvtPins2", null, "\"F4 - VVT#2\"", null));
        return tune.applyOntoWithReport(
            new ConfigurationImage(ini.getMetaInfo().getPageSize(0)), ini, null);
    }
}
