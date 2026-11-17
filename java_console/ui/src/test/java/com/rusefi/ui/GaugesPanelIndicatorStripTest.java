package com.rusefi.ui;

import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import com.rusefi.ui.widgets.tune.CustomIndicator;
import com.rusefi.ui.widgets.tune.IndicatorStrip;
import com.rusefi.ui.widgets.tune.IndicatorStripSettings;

import javax.swing.*;
import javax.swing.event.MenuEvent;
import javax.swing.event.MenuListener;
import java.awt.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

class GaugesPanelIndicatorStripTest {
    private static final String CHANNEL = "gaugesStripFanChannel";

    @Test
    void indicatorsSitUnderTheCommandBarAndPollOnlyWhileActive() throws Exception {
        IniFileModel ini = IniFileModelMocks.empty();
        FrontPageModel frontPage = new FrontPageModel();
        frontPage.getIndicators().add(new IndicatorModel("{ " + CHANNEL + " }",
            "Fan off", "Fan on", "white", "black", "green", "black"));
        when(ini.getFrontPage()).thenReturn(frontPage);
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(ini);
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 1);

        GaugesPanel[] holder = new GaugesPanel[1];
        SwingUtilities.invokeAndWait(() -> holder[0] = new GaugesPanel(context, config));
        GaugesPanel panel = holder[0];
        try {
            SwingUtilities.invokeAndWait(() -> {
                JComponent strip = panel.getIndicatorStrip().getContent();
                assertTrue(strip.isVisible());
                Container toolbar = strip.getParent();
                JComponent content = panel.getContent();
                assertSame(toolbar, ((BorderLayout) content.getLayout()).getLayoutComponent(BorderLayout.NORTH),
                    "the strip is part of the toolbar above the gauges");
                assertSame(strip, toolbar.getComponent(toolbar.getComponentCount() - 1),
                    "below the command bar");

                // Edit dashboard > Indicators is the strip's right-click menu, filled when opened
                JMenu indicators = null;
                for (Component item : panel.getEditMenu().getComponents()) {
                    if (item instanceof JMenu && GaugesPanel.INDICATORS.equals(((JMenu) item).getText())) {
                        indicators = (JMenu) item;
                    }
                }
                assertNotNull(indicators);
                for (MenuListener listener : indicators.getMenuListeners()) {
                    listener.menuSelected(new MenuEvent(indicators));
                }
                assertTrue(indicators.getItemCount() > 0);
                assertEquals(IndicatorStrip.NEW_INDICATOR, indicators.getItem(0).getText());
            });

            assertFalse(hasDemand());
            SwingUtilities.invokeAndWait(() -> panel.setActive(true));
            assertTrue(hasDemand());
            SwingUtilities.invokeAndWait(() -> panel.setActive(false));
            assertFalse(hasDemand());
            SwingUtilities.invokeAndWait(() -> panel.setActive(true));
        } finally {
            SwingUtilities.invokeAndWait(panel::destroy);
        }
        assertFalse(hasDemand(), "destroy releases the indicator channels");
    }

    /** Save layout… / Load layout… carry the indicator strip; a file without one leaves it alone. */
    @Test
    void layoutFilesCarryTheIndicatorStrip(@TempDir Path directory) throws Exception {
        UIContext context = new UIContext();
        context.iniFileState.setIniFileModelForTest(IniFileModelMocks.empty());
        Node config = new Node();
        config.setProperty("gauges_rows", 1);
        config.setProperty("gauges_cols", 1);
        IndicatorStripSettings strip = context.indicatorStripSettings;
        CustomIndicator hot = new CustomIndicator("hot1", "coolant > 100", "CLT normal", "CLT hot",
            CustomIndicator.Tone.NEUTRAL, CustomIndicator.Tone.RED);

        GaugesPanel[] holder = new GaugesPanel[1];
        SwingUtilities.invokeAndWait(() -> holder[0] = new GaugesPanel(context, config));
        GaugesPanel panel = holder[0];
        try {
            Path withStrip = directory.resolve("with-strip.gauges");
            Path older = directory.resolve("older.gauges");
            SwingUtilities.invokeAndWait(() -> {
                try {
                    GaugeLayout.save(older, config);
                    strip.putCustomIndicator(hot);
                    strip.setHidden("a", true);
                    panel.saveLayout(withStrip);
                    assertTrue(new String(Files.readAllBytes(withStrip)).contains("indicators.custom_0.expression"));

                    strip.removeCustomIndicator("hot1");
                    strip.showAll();
                    panel.loadLayout(withStrip);
                    assertEquals(Collections.singletonList(hot), strip.getCustomIndicators());
                    assertEquals(Collections.singleton("a"), strip.getHidden());
                    assertFalse(config.getConfig().containsKey(IndicatorStripSettings.NODE),
                        "the strip is not stored with the gauges");

                    panel.loadLayout(older);
                    assertEquals(Collections.singletonList(hot), strip.getCustomIndicators(), "an older file keeps the strip");
                } catch (java.io.IOException e) {
                    throw new AssertionError(e);
                }
            });
        } finally {
            SwingUtilities.invokeAndWait(panel::destroy);
        }
    }

    private static boolean hasDemand() {
        return SensorCentral.getInstance().getOutputChannelDemand().getChannels()
            .contains(CHANNEL.toLowerCase());
    }
}
