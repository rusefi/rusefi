package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.rusefi.core.preferences.storage.Node;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

class IndicatorStripTest {
    static IniFileModel iniWithIndicators(String... channels) {
        IniFileModel ini = IniFileModelMocks.empty();
        FrontPageModel frontPage = new FrontPageModel();
        for (String channel : channels) {
            frontPage.getIndicators().add(new IndicatorModel("{ " + channel + " }",
                channel + " off", channel + " on", "white", "black", "red", "black"));
        }
        when(ini.getFrontPage()).thenReturn(frontPage);
        return ini;
    }

    private static List<String> shownKeys(IndicatorStrip strip) {
        List<String> keys = new ArrayList<>();
        for (IndicatorStrip.Entry entry : strip.getShown()) {
            keys.add(entry.key);
        }
        return keys;
    }

    private static void onEdt(Runnable test) throws Exception {
        SwingUtilities.invokeAndWait(test);
    }

    @Test
    void followsTheIniAndHidesWithoutIndicators() throws Exception {
        onEdt(() -> {
            AtomicReference<IniFileModel> ini = new AtomicReference<>();
            IndicatorStrip strip = new IndicatorStrip(ini::get, new IndicatorStripSettings(new Node()));
            try {
                assertFalse(strip.getContent().isVisible(), "no INI yet");
                assertNull(strip.getIndicatorPanel());

                ini.set(iniWithIndicators("fanOn", "pumpOn"));
                strip.refresh();
                assertTrue(strip.getContent().isVisible());
                IndicatorPanel first = strip.getIndicatorPanel();
                assertEquals(2, first.getPanel().getComponentCount());
                String tooltip = ((JLabel) first.getPanel().getComponent(0)).getToolTipText();
                assertTrue(tooltip.contains("fanOn off") && tooltip.contains(IndicatorStrip.CHIP_HINT),
                    "the chip's text, then how to rearrange: " + tooltip);

                strip.refresh();
                assertSame(first, strip.getIndicatorPanel(), "same INI: the chips are kept");

                // another ECU connects with different firmware
                ini.set(iniWithIndicators("fanOn", "pumpOn", "triggerError"));
                strip.refresh();
                assertNotSame(first, strip.getIndicatorPanel());
                assertEquals(3, strip.getIndicatorPanel().getPanel().getComponentCount());

                ini.set(iniWithIndicators());
                strip.refresh();
                assertFalse(strip.getContent().isVisible(), "an INI without indicators hides the strip");
                assertEquals(0, strip.getContent().getComponentCount());
            } finally {
                strip.destroy();
            }
        });
    }

    /** The Gauges and Tuning strips share one settings object: a change in one shows in the other. */
    @Test
    void stripsShareOrderAndHiddenChips() throws Exception {
        onEdt(() -> {
            IniFileModel ini = iniWithIndicators("a", "b", "c", "d");
            Node config = new Node();
            IndicatorStripSettings settings = new IndicatorStripSettings(config);
            IndicatorStrip gauges = new IndicatorStrip(() -> ini, settings);
            IndicatorStrip tuning = new IndicatorStrip(() -> ini, settings);
            try {
                gauges.move(3, 0);
                assertEquals(Arrays.asList("d", "a", "b", "c"), shownKeys(gauges));
                assertEquals(Arrays.asList("d", "a", "b", "c"), shownKeys(tuning));

                settings.setHidden("b", true);
                assertEquals(Arrays.asList("d", "a", "c"), shownKeys(gauges));
                assertEquals(Arrays.asList("d", "a", "c"), shownKeys(tuning));
                assertEquals(3, tuning.getIndicatorPanel().getPanel().getComponentCount());

                // a new strip (the next console run) reads the same preferences
                IndicatorStrip restarted = new IndicatorStrip(() -> ini, new IndicatorStripSettings(config));
                assertEquals(Arrays.asList("d", "a", "c"), shownKeys(restarted));
                restarted.destroy();

                settings.showAll();
                settings.resetOrder();
                assertEquals(Arrays.asList("a", "b", "c", "d"), shownKeys(tuning));
            } finally {
                gauges.destroy();
                tuning.destroy();
            }
        });
    }

    @Test
    void movingAChipKeepsHiddenChipsInPlace() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a", "b", "c", "d"), settings);
            try {
                settings.setHidden("b", true);
                // shown: a c d; move d before c, then a to the end
                strip.move(2, 1);
                assertEquals(Arrays.asList("a", "d", "c"), shownKeys(strip));
                strip.move(0, 3);
                assertEquals(Arrays.asList("d", "c", "a"), shownKeys(strip));
                settings.showAll();
                assertEquals(Arrays.asList("b", "d", "c", "a"), shownKeys(strip), "b kept its place before d");
            } finally {
                strip.destroy();
            }
        });
    }

    /** Indicators that firmware adds later follow the stored order; ones only another ECU has keep their place. */
    @Test
    void storedOrderSurvivesOtherFirmware() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            settings.setOrder(Arrays.asList("c", "gone", "a"));
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a", "b", "c", "new"), settings);
            try {
                assertEquals(Arrays.asList("c", "a", "b", "new"), shownKeys(strip));
                strip.move(1, 0);
                assertTrue(settings.getOrder().contains("gone"));
            } finally {
                strip.destroy();
            }
        });
    }

    @Test
    void dragMovesOnlyPastTheThresholdAndAfterRelease() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a", "b", "c"), settings);
            try {
                JPanel panel = strip.getIndicatorPanel().getPanel();
                panel.setSize(1000, 100);
                panel.doLayout();
                Rectangle first = panel.getComponent(0).getBounds();
                Rectangle last = panel.getComponent(2).getBounds();
                Point start = new Point(first.x + 5, first.y + first.height / 2);

                // a twitch during a click moves nothing
                strip.pressed(0, start, true);
                strip.dragged(new Point(start.x + 2, start.y));
                assertFalse(strip.isDragging());
                strip.released();
                assertEquals(Arrays.asList("a", "b", "c"), shownKeys(strip));
                assertFalse(settings.hasCustomOrder());

                // drag the first chip past the right half of the last one: it goes to the end
                strip.pressed(0, start, true);
                strip.dragged(new Point(last.x + last.width - 2, start.y));
                assertTrue(strip.isDragging());
                strip.released();
                assertEquals(Arrays.asList("b", "c", "a"), shownKeys(strip));
                assertFalse(strip.isDragging());

                // the right button never drags
                strip.pressed(0, start, false);
                strip.dragged(new Point(last.x + last.width - 2, start.y));
                assertFalse(strip.isDragging());
            } finally {
                strip.destroy();
            }
        });
    }

    @Test
    void menuHidesAChipAndBringsHiddenOnesBack() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a", "b"), settings);
            try {
                JPopupMenu menu = new JPopupMenu();
                strip.fillMenu(menu, strip.getShown().get(0));
                JMenuItem hide = (JMenuItem) menu.getComponent(0);
                assertEquals("Hide “a off / a on”", hide.getText());
                hide.doClick();
                assertEquals(Arrays.asList("b"), shownKeys(strip));

                settings.setHidden("b", true);
                assertTrue(strip.getContent().isVisible(), "all hidden: still there to right-click");
                assertNull(strip.getIndicatorPanel());

                JPopupMenu stripMenu = new JPopupMenu();
                strip.fillMenu(stripMenu, null);
                JMenu hidden = findMenu(stripMenu, IndicatorStrip.HIDDEN_MENU);
                assertEquals(2, hidden.getItemCount());
                hidden.getItem(1).doClick();
                assertEquals(Arrays.asList("b"), shownKeys(strip));
            } finally {
                strip.destroy();
            }
        });
    }

    /** New, Edit and Delete in the strip's menu, with the dialogs answered by the test. */
    @Test
    void menuCreatesEditsAndDeletesCustomIndicators() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a"), settings);
            CustomIndicator hot = new CustomIndicator("hot1", "clt > 100", "CLT ok", "CLT hot",
                CustomIndicator.Tone.NEUTRAL, CustomIndicator.Tone.RED);
            CustomIndicator hotter = new CustomIndicator("hot1", "clt > 110", "CLT ok", "CLT hot",
                CustomIndicator.Tone.NEUTRAL, CustomIndicator.Tone.RED);
            List<CustomIndicator> answers = new ArrayList<>(Arrays.asList(hot, null, hotter));
            List<CustomIndicator> opened = new ArrayList<>();
            boolean[] confirm = {false};
            strip.setPromptsForTests(new IndicatorStrip.Prompts() {
                @Override
                CustomIndicator edit(Component parent, IniFileModel ini, CustomIndicator existing) {
                    opened.add(existing);
                    return answers.remove(0);
                }

                @Override
                boolean confirmDelete(Component parent, String name) {
                    return confirm[0];
                }
            });
            try {
                item(menuFor(strip, null), IndicatorStrip.NEW_INDICATOR).doClick();
                assertEquals(Arrays.asList("a", "custom:hot1"), shownKeys(strip));

                // an INI chip offers no Edit or Delete
                JPopupMenu iniMenu = menuFor(strip, strip.getShown().get(0));
                assertNull(findItem(iniMenu, IndicatorStrip.EDIT_INDICATOR));

                // cancelled: nothing changes
                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.EDIT_INDICATOR).doClick();
                assertEquals(Collections.singletonList(hot), settings.getCustomIndicators());
                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.EDIT_INDICATOR).doClick();
                assertEquals(Collections.singletonList(hotter), settings.getCustomIndicators());
                assertEquals(Arrays.asList(null, hot, hot), opened, "Edit opens the indicator");

                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.DELETE_INDICATOR).doClick();
                assertEquals(1, settings.getCustomIndicators().size(), "not confirmed");
                confirm[0] = true;
                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.DELETE_INDICATOR).doClick();
                assertTrue(settings.getCustomIndicators().isEmpty());
                assertEquals(Collections.singletonList("a"), shownKeys(strip));

                for (int i = 0; i < IndicatorStripSettings.MAX_CUSTOM; i++) {
                    settings.putCustomIndicator(CustomIndicator.create("x" + i, "off", "on",
                        CustomIndicator.Tone.NEUTRAL, CustomIndicator.Tone.RED));
                }
                assertFalse(item(menuFor(strip, null), IndicatorStrip.NEW_INDICATOR).isEnabled(), "full");
            } finally {
                strip.destroy();
            }
        });
    }

    /** A name replaces the labels wherever the strip refers to the indicator; an empty one brings them back. */
    @Test
    void renamedIndicatorIsCalledByItsName() throws Exception {
        onEdt(() -> {
            IndicatorStripSettings settings = new IndicatorStripSettings(new Node());
            settings.putCustomIndicator(new CustomIndicator("hot1", "clt > 100", "CLT ok", "CLT hot",
                CustomIndicator.Tone.NEUTRAL, CustomIndicator.Tone.RED));
            IndicatorStrip strip = new IndicatorStrip(() -> iniWithIndicators("a"), settings);
            List<String> names = new ArrayList<>(Arrays.asList("Overheat", null, ""));
            strip.setPromptsForTests(new IndicatorStrip.Prompts() {
                @Override
                String rename(Component parent, String current) {
                    return names.remove(0);
                }
            });
            try {
                assertEquals("CLT ok / CLT hot", strip.getShown().get(1).getName());
                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.RENAME_INDICATOR).doClick();
                assertEquals("Overheat", settings.getCustomIndicators().get(0).getName());
                assertEquals("Overheat", strip.getShown().get(1).getName());
                assertNotNull(findItem(menuFor(strip, strip.getShown().get(1)), "Hide “Overheat”"));
                String tooltip = ((JLabel) strip.getIndicatorPanel().getPanel().getComponent(1)).getToolTipText();
                assertTrue(tooltip.contains("Overheat: CLT ok"), tooltip);

                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.RENAME_INDICATOR).doClick();
                assertEquals("Overheat", settings.getCustomIndicators().get(0).getName(), "cancelled");

                item(menuFor(strip, strip.getShown().get(1)), IndicatorStrip.RENAME_INDICATOR).doClick();
                assertEquals("CLT ok / CLT hot", strip.getShown().get(1).getName(), "empty: the labels again");
                assertEquals("clt > 100", settings.getCustomIndicators().get(0).getExpression(), "only the name changed");
            } finally {
                strip.destroy();
            }
        });
    }

    private static JPopupMenu menuFor(IndicatorStrip strip, IndicatorStrip.Entry chip) {
        JPopupMenu menu = new JPopupMenu();
        strip.fillMenu(menu, chip);
        return menu;
    }

    private static JMenuItem findItem(JPopupMenu menu, String text) {
        for (Component item : menu.getComponents()) {
            if (item instanceof JMenuItem && text.equals(((JMenuItem) item).getText())) {
                return (JMenuItem) item;
            }
        }
        return null;
    }

    private static JMenuItem item(JPopupMenu menu, String text) {
        JMenuItem item = findItem(menu, text);
        assertNotNull(item, text);
        return item;
    }

    @Test
    void repeatedExpressionsGetTheirOwnKeys() {
        java.util.Set<String> taken = new java.util.HashSet<>();
        assertEquals("a > 1", IndicatorStripSettings.keyOf("{  a   > 1 }", taken));
        assertEquals("a > 1#2", IndicatorStripSettings.keyOf("{a > 1}", taken));
        assertEquals("x || y", IndicatorStripSettings.keyOf("{ x || y }", taken));
    }

    private static JMenu findMenu(JPopupMenu menu, String text) {
        for (Component item : menu.getComponents()) {
            if (item instanceof JMenu && text.equals(((JMenu) item).getText())) {
                return (JMenu) item;
            }
        }
        throw new AssertionError("no " + text);
    }
}
