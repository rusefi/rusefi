package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.tune.CustomIndicator.Tone;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.io.IOException;
import java.util.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

class CustomIndicatorTest {
    private static final String CHANNEL = "customIndicatorTestClt";

    private static CustomIndicator hot() {
        return new CustomIndicator("hot1", CHANNEL + " > 100", "CLT normal", "CLT hot", Tone.NEUTRAL, Tone.RED);
    }

    /** An INI with indicators "a" and "b" and the given output channels. */
    private static IniFileModel ini(String... channels) {
        IniFileModel ini = IndicatorStripTest.iniWithIndicators("a", "b");
        Map<String, IniField> outputs = new HashMap<>();
        for (String channel : channels) {
            outputs.put(channel, mock(IniField.class));
        }
        when(ini.getAllOutputChannels()).thenReturn(outputs);
        return ini;
    }

    private static List<String> shownKeys(IndicatorStrip strip) {
        List<String> keys = new ArrayList<>();
        for (IndicatorStrip.Entry entry : strip.getShown()) {
            keys.add(entry.key);
        }
        return keys;
    }

    @Test
    void storedLikeTheRestOfTheStrip() {
        Node config = new Node();
        IndicatorStripSettings settings = new IndicatorStripSettings(config);
        settings.putCustomIndicator(hot());
        CustomIndicator fan = CustomIndicator.create("fan3 != 0", "Fan 3 off", "Fan 3 on", Tone.NEUTRAL, Tone.GREEN);
        settings.putCustomIndicator(fan);
        assertEquals(Arrays.asList(hot(), fan), new IndicatorStripSettings(config).getCustomIndicators(),
            "kept in the preferences");

        CustomIndicator hotter = new CustomIndicator("hot1", CHANNEL + " > 110", "CLT normal", "CLT hot!", Tone.NEUTRAL, Tone.YELLOW);
        settings.putCustomIndicator(hotter);
        assertEquals(Arrays.asList(hotter, fan), settings.getCustomIndicators(), "an edit keeps its place");

        settings.setOrder(Arrays.asList("custom:hot1", "a"));
        settings.setHidden("custom:hot1", true);
        settings.removeCustomIndicator("hot1");
        assertEquals(Collections.singletonList(fan), settings.getCustomIndicators());
        assertEquals(Collections.singletonList("a"), settings.getOrder(), "its place is forgotten");
        assertTrue(settings.getHidden().isEmpty());
        assertFalse(config.getConfig().containsKey("custom_1"), "no stale entry left behind");
    }

    @Test
    void chipFollowsItsChannelWithTheChosenColours() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            settings.putCustomIndicator(hot());
            IndicatorStrip strip = new IndicatorStrip(() -> ini(CHANNEL), settings);
            try {
                assertEquals(Arrays.asList("a", "b", "custom:hot1"), shownKeys(strip), "after the INI indicators");
                IndicatorPanel panel = strip.getIndicatorPanel();
                JLabel chip = (JLabel) panel.getPanel().getComponent(2);

                SensorCentral.getInstance().setValue(90, CHANNEL);
                panel.refresh(null);
                assertEquals("CLT normal", chip.getText());

                SensorCentral.getInstance().setValue(105, CHANNEL);
                panel.refresh(null);
                assertEquals("CLT hot", chip.getText());
                assertEquals(TuningStyle.indicatorColors("red", "black")[0], chip.getBackground());

                // it can be moved and hidden like an INI indicator
                strip.move(2, 0);
                assertEquals(Arrays.asList("custom:hot1", "a", "b"), shownKeys(strip));
                settings.setHidden("custom:hot1", true);
                assertEquals(Arrays.asList("a", "b"), shownKeys(strip));
            } finally {
                strip.destroy();
            }
        });
    }

    @Test
    void missingChannelShowsAGreyChipThatSaysWhy() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            settings.putCustomIndicator(hot());
            IndicatorStrip strip = new IndicatorStrip(() -> ini("somethingElse"), settings);
            try {
                assertEquals(Arrays.asList("a", "b", "custom:hot1"), shownKeys(strip), "kept, not dropped");
                IndicatorPanel panel = strip.getIndicatorPanel();
                JLabel chip = (JLabel) panel.getPanel().getComponent(2);
                SensorCentral.getInstance().setValue(105, CHANNEL);
                panel.refresh(null);
                assertEquals("CLT normal", chip.getText(), "never on");
                assertEquals(TuningStyle.indicatorColors("white", "black")[0], chip.getBackground());
                assertTrue(chip.getToolTipText().contains(CHANNEL + " is not available"), chip.getToolTipText());
            } finally {
                strip.destroy();
            }
        });
    }

    @Test
    void customChipsShowWithoutAnyIni() throws Exception {
        SwingUtilities.invokeAndWait(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            settings.putCustomIndicator(hot());
            IndicatorStrip strip = new IndicatorStrip(() -> null, settings);
            try {
                assertTrue(strip.getContent().isVisible());
                assertEquals(Collections.singletonList("custom:hot1"), shownKeys(strip));
            } finally {
                strip.destroy();
            }
        });
    }

    /** Renaming changes the chip text only where it showed the default; typed labels stay. */
    @Test
    void renameFollowsDefaultLabelsOnly() {
        CustomIndicator byCondition = new CustomIndicator("p", "pedal != 0", "pedal ≠ 0", "pedal ≠ 0",
            Tone.NEUTRAL, Tone.RED);
        CustomIndicator named = byCondition.renamed("Pedal Split");
        assertEquals("Pedal Split", named.getOnLabel());
        assertEquals("Pedal Split", named.getOffLabel());
        CustomIndicator renamed = named.renamed("Split");
        assertEquals("Split", renamed.getOnLabel());
        assertEquals(byCondition, renamed.renamed(""), "no name: the condition again");

        CustomIndicator typed = new CustomIndicator("q", "pedal != 0", "Pedal ok", "Pedal split", Tone.NEUTRAL, Tone.RED);
        assertEquals(typed.withName("Split"), typed.renamed("Split"), "typed labels are kept");
        CustomIndicator onlyOn = new CustomIndicator("r", "pedal", "Pedal", "Pedal split", Tone.NEUTRAL, Tone.RED);
        assertEquals("Pedal split", onlyOn.renamed("X").getOnLabel());
        assertEquals("Pedal", onlyOn.renamed("X").getOffLabel(), "\"Pedal\" differs from the default \"pedal\", so it was typed and stays");
    }

    @Test
    void nameIsStoredAndOptionalInFiles() throws IOException {
        Node config = new Node();
        IndicatorStripSettings settings = new IndicatorStripSettings(config);
        settings.putCustomIndicator(hot().withName("Overheat"));
        settings.putCustomIndicator(CustomIndicator.create("fan3", "off", "on", Tone.NEUTRAL, Tone.GREEN));
        assertEquals("Overheat", new IndicatorStripSettings(config).getCustomIndicators().get(0).getName());

        Properties file = new Properties();
        settings.toFile(file, "indicators.");
        assertEquals("Overheat", file.getProperty("indicators.custom_0.name"));
        assertNull(file.getProperty("indicators.custom_1.name"), "an empty name is not written");

        // a file from before names existed
        file.remove("indicators.custom_0.name");
        IndicatorStripSettings loaded = new IndicatorStripSettings(new Node());
        loaded.replaceWith(IndicatorStripSettings.fromFile(file, "indicators."));
        assertEquals("", loaded.getCustomIndicators().get(0).getName());
    }

    @Test
    void fileRoundTripAndValidation() throws IOException {
        IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
        settings.putCustomIndicator(hot());
        settings.setOrder(Arrays.asList("custom:hot1", "a || b"));
        settings.setHidden("b", true);
        Properties file = new Properties();
        settings.toFile(file, "indicators.");

        IndicatorStripSettings other = new IndicatorStripSettings(new Node());
        other.putCustomIndicator(CustomIndicator.create("x", "X off", "X on", Tone.NEUTRAL, Tone.BLUE));
        other.replaceWith(IndicatorStripSettings.fromFile(file, "indicators."));
        assertEquals(Collections.singletonList(hot()), other.getCustomIndicators(), "the file's indicators replace the old ones");
        assertEquals(Arrays.asList("custom:hot1", "a || b"), other.getOrder());
        assertEquals(Collections.singleton("b"), other.getHidden());

        assertNull(IndicatorStripSettings.fromFile(new Properties(), "indicators."), "older files have no strip");

        Properties badTone = (Properties) file.clone();
        badTone.setProperty("indicators.custom_0.on_tone", "purple");
        assertThrows(IOException.class, () -> IndicatorStripSettings.fromFile(badTone, "indicators."));
        Properties tooMany = (Properties) file.clone();
        tooMany.setProperty("indicators.custom_count", "65");
        assertThrows(IOException.class, () -> IndicatorStripSettings.fromFile(tooMany, "indicators."));
        Properties repeated = (Properties) file.clone();
        repeated.setProperty("indicators.custom_count", "2");
        for (String key : CustomIndicator.PROPERTIES) {
            String value = file.getProperty("indicators.custom_0." + key);
            if (value != null) {
                repeated.setProperty("indicators.custom_1." + key, value);
            }
        }
        assertThrows(IOException.class, () -> IndicatorStripSettings.fromFile(repeated, "indicators."));
    }
}
