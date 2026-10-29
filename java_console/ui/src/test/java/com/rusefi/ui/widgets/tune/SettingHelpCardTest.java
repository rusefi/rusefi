package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.*;

public class SettingHelpCardTest {
    @Test
    public void summaryIsTheFirstLineOrSentence() {
        assertEquals("Injector flow rate at 3 bar.",
            SettingHelpCard.firstSentence("Injector flow rate at 3 bar. Used by the fuel calculation."));
        // INI help writes line breaks as \n
        assertEquals("Determines the fuel strategy.",
            SettingHelpCard.firstSentence("Determines the fuel strategy.\\nSpeed Density: ..."));
        // an abbreviation followed by lower case does not end the sentence
        assertEquals("Use e.g. a long overrun.", SettingHelpCard.firstSentence("Use e.g. a long overrun."));

        assertFalse(new SettingHelpCard.Help("Bore", "mm", "Cylinder bore.").hasMore());
        assertTrue(new SettingHelpCard.Help("Bore", "mm", "Cylinder bore.\\nMeasured at the top.").hasMore());
    }

    @Test
    public void rowHelpIsFoundFromAnyControlInTheRowAndHtmlLabelsArePlain() {
        JPanel row = new JPanel();
        JButton pinout = new JButton();
        row.add(pinout);
        SettingHelpCard.attach(row, "<html><div>Fan on temperature</div></html>", "C", "Turns the fan on.");
        assertSame(row, SettingHelpCard.findRow(pinout));
        SettingHelpCard.Help help = (SettingHelpCard.Help) row.getClientProperty(SettingHelpCard.KEY);
        assertEquals("Fan on temperature", help.name);
        assertEquals("C", help.units);

        JPanel noHelp = new JPanel();
        SettingHelpCard.attach(noHelp, "Name", null, "  ");
        assertNull(SettingHelpCard.findRow(noHelp));
        assertNull(SettingHelpCard.findRow(null));
    }

    @Test
    public void cardGoesRightOfTheControlsOrBelowWhenThereIsNoRoom() {
        Rectangle window = new Rectangle(0, 0, 1500, 900);
        Dimension card = new Dimension(340, 120);

        Rectangle controls = new Rectangle(700, 300, 440, 28);
        assertEquals(new Point(1148, 300), SettingHelpCard.place(controls, card, window));

        // too close to the right edge: below the row, right-aligned with its controls
        Rectangle wide = new Rectangle(400, 300, 1000, 28);
        assertEquals(new Point(1060, 332), SettingHelpCard.place(wide, card, window));

        // never past the bottom of the window
        Rectangle low = new Rectangle(700, 850, 440, 28);
        assertEquals(780, SettingHelpCard.place(low, card, window).y);
    }

    @Test
    public void onlyMovementTowardTheCardDelaysSwitchingRows() {
        // card to the right of the rows, top at the open row
        Rectangle card = new Rectangle(1148, 300, 340, 120);
        Point from = new Point(900, 310);

        // More sits at the card's bottom left, about (1158, 400): a slope of about 0.35
        assertTrue(SettingHelpCard.headingTo(from, new Point(920, 318), card), "down-right toward More");
        assertFalse(SettingHelpCard.headingTo(from, new Point(920, 330), card), "steeply down, toward the next rows");
        assertTrue(SettingHelpCard.headingTo(from, new Point(930, 310), card), "straight right");
        assertFalse(SettingHelpCard.headingTo(from, new Point(900, 340), card), "down the rows");
        assertFalse(SettingHelpCard.headingTo(from, new Point(890, 290), card), "up and away");
        assertFalse(SettingHelpCard.headingTo(from, from, card), "no movement");
        assertFalse(SettingHelpCard.headingTo(null, from, card));

        // card below the row: heading down toward its top edge
        Rectangle below = new Rectangle(800, 340, 340, 120);
        assertTrue(SettingHelpCard.headingTo(new Point(900, 310), new Point(905, 325), below));
        assertFalse(SettingHelpCard.headingTo(new Point(900, 310), new Point(900, 300), below), "up the rows");
    }

    @Test
    public void collapsedCardShowsTheSummaryAndMoreExpandsIt() throws Exception {
        SettingHelpCard.Help help = new SettingHelpCard.Help("Fuel strategy", "",
            "Determines the method used for fuel.\\nSpeed Density: uses MAP and IAT.");
        SwingUtilities.invokeAndWait(() -> {
            boolean[] toggled = new boolean[1];
            JComponent collapsed = SettingHelpCard.createCard(help, false, () -> toggled[0] = true);
            String text = ((JEditorPane) find(collapsed, "settingHelpText")).getText();
            assertTrue(text.contains("Determines the method used for fuel."));
            assertFalse(text.contains("Speed Density"));
            JButton more = (JButton) find(collapsed, "settingHelpMore");
            assertTrue(more.getText().startsWith("More"));
            more.doClick(0);
            assertTrue(toggled[0]);

            JComponent expanded = SettingHelpCard.createCard(help, true, () -> { });
            assertTrue(((JEditorPane) find(expanded, "settingHelpText")).getText().contains("Speed Density"));
            assertTrue(((JButton) find(expanded, "settingHelpMore")).getText().startsWith("Show less"));

            assertNull(find(SettingHelpCard.createCard(new SettingHelpCard.Help("Bore", "mm", "Cylinder bore."),
                false, () -> { }), "settingHelpMore"), "nothing more to show");
        });
    }

    @Test
    public void onlyAPinnedCardHasACloseButton() throws Exception {
        SettingHelpCard.Help help = new SettingHelpCard.Help("Bore", "mm", "Cylinder bore.");
        SwingUtilities.invokeAndWait(() -> {
            assertNull(find(SettingHelpCard.createCard(help, false, () -> { }), "settingHelpClose"));
            boolean[] closed = new boolean[1];
            JButton close = (JButton) find(SettingHelpCard.createCard(help, true, () -> { }, () -> closed[0] = true),
                "settingHelpClose");
            assertNotNull(close);
            close.doClick(0);
            assertTrue(closed[0]);
        });
    }

    private static Component find(Container container, String name) {
        for (Component component : container.getComponents()) {
            if (name.equals(component.getName())) {
                return component;
            }
            if (component instanceof Container) {
                Component nested = find((Container) component, name);
                if (nested != null) {
                    return nested;
                }
            }
        }
        return null;
    }
}
