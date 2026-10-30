package com.rusefi.ui.widgets.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.rusefi.config.FieldType;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.widgets.tune.CustomIndicator.Tone;
import com.rusefi.ui.widgets.tune.CustomIndicatorDialog.Operator;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.Dimension;
import java.util.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

class CustomIndicatorDialogTest {
    private static final String CLT = "dialogTestClt";
    private static final String RPM = "dialogTestRpm";
    private static final String FAN = "dialogTestFan";

    private static IniFileModel ini() {
        IniFileModel ini = IniFileModelMocks.empty();
        Map<String, IniField> outputs = new HashMap<>();
        outputs.put(CLT, new ScalarIniField(CLT, 0, "C", FieldType.INT16, 0.01, "1", 0));
        outputs.put(RPM, new ScalarIniField(RPM, 2, "RPM", FieldType.UINT16, 1, "0", 0));
        outputs.put(FAN, new ScalarIniField(FAN, 4, "", FieldType.UINT8, 1, "0", 0));
        when(ini.getAllOutputChannels()).thenReturn(outputs);
        return ini;
    }

    private static void onEdt(Runnable test) throws Exception {
        SwingUtilities.invokeAndWait(test);
    }

    private static CustomIndicator valid(CustomIndicatorDialog dialog) {
        CustomIndicatorDialog.Parsed parsed = dialog.parse();
        assertNull(parsed.error);
        return parsed.indicator;
    }

    private static String error(CustomIndicatorDialog dialog) {
        CustomIndicatorDialog.Parsed parsed = dialog.parse();
        assertNull(parsed.indicator);
        return parsed.error;
    }

    @Test
    void channelIsOnWithDefaultLabelsAndColours() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            assertTrue(dialog.isOn.isSelected(), "the simplest condition first");
            assertEquals("Choose a channel.", error(dialog));
            assertNull(dialog.getPreview());

            dialog.selectChannel(FAN);
            CustomIndicator indicator = valid(dialog);
            assertEquals(FAN, indicator.getExpression());
            assertEquals(FAN, indicator.getOnLabel(), "without labels the chip shows the condition");
            assertEquals(FAN, indicator.getOffLabel());
            assertEquals(Tone.NEUTRAL, indicator.getOffTone());
            assertEquals(Tone.RED, indicator.getOnTone());
            assertNotNull(dialog.getPreview());

            dialog.offLabel.setText("Fan 3 off");
            dialog.onLabel.setText("Fan 3 on");
            dialog.onTone.setSelectedItem(Tone.GREEN);
            indicator = valid(dialog);
            assertEquals("Fan 3 off", indicator.getOffLabel());
            assertEquals("Fan 3 on", indicator.getOnLabel());
            assertEquals(Tone.GREEN, indicator.getOnTone());
        });
    }

    @Test
    void nameIsOptionalAndKeptWhenEditing() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            dialog.selectChannel(FAN);
            assertEquals("", valid(dialog).getName(), "no name: menus use the labels");
            dialog.name.setText("  Fan 3  ");
            assertEquals("Fan 3", valid(dialog).getName());
            assertEquals("Fan 3", valid(dialog).getOnLabel(), "without labels the chip shows the name");
            assertEquals("Fan 3", valid(dialog).getOffLabel());
            JLabel chip = (JLabel) dialog.getPreview().getPanel().getComponent(0);
            assertEquals("Fan 3", chip.getText(), "the preview follows the name as it is typed");

            // reopened, the labels are still empty, so they keep following the name
            CustomIndicatorDialog reopened = new CustomIndicatorDialog(ini(), valid(dialog));
            assertEquals("", reopened.onLabel.getText());
            assertEquals("", reopened.offLabel.getText());
            reopened.name.setText("Radiator fan");
            assertEquals("Radiator fan", valid(reopened).getOnLabel());

            // named, but saved while the chip still showed the condition
            CustomIndicator stuck = new CustomIndicator("id8", FAN, FAN, FAN, Tone.NEUTRAL, Tone.RED, "Fan 3");
            CustomIndicatorDialog fixed = new CustomIndicatorDialog(ini(), stuck);
            assertEquals("", fixed.onLabel.getText(), "the condition was never typed");
            assertEquals("Fan 3", valid(fixed).getOnLabel());
            dialog.name.setText(String.join("", Collections.nCopies(CustomIndicatorDialog.MAX_LABEL + 1, "n")));
            assertTrue(error(dialog).startsWith("A name can be at most"));

            CustomIndicator named = new CustomIndicator("id9", FAN, "Fan off", "Fan on", Tone.NEUTRAL, Tone.GREEN, "Fan 3");
            dialog = new CustomIndicatorDialog(ini(), named);
            assertEquals("Fan 3", dialog.name.getText());
            dialog.name.setText("Radiator fan");
            assertEquals(named.withName("Radiator fan"), valid(dialog), "renamed, nothing else changed");
        });
    }

    @Test
    void thresholdShowsUnitsAndNeedsANumber() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            dialog.threshold.doClick();
            dialog.selectChannel(CLT);
            assertEquals("C", dialog.units.getText());
            dialog.operator.setSelectedItem(Operator.AT_LEAST);
            assertEquals("Enter a number to compare the channel with.", error(dialog));
            dialog.value.setText("abc");
            assertEquals("Enter a number to compare the channel with.", error(dialog));

            dialog.value.setText("100.5");
            CustomIndicator indicator = valid(dialog);
            assertEquals(CLT + " >= 100.5", indicator.getExpression());
            assertEquals(CLT + " ≥ 100.5", indicator.getOnLabel());
        });
    }

    @Test
    void expressionsAreChecked() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            dialog.selectChannel(CLT);
            dialog.advanced.doClick();
            assertEquals(CLT, dialog.expression.getText(), "starts from the condition chosen before");

            dialog.expression.setText(CLT + " > 100 && oilPress < 1");
            assertEquals("Unknown channel: oilPress.", error(dialog));
            dialog.expression.setText(CLT + " >");
            assertEquals("The condition is not a valid expression.", error(dialog));
            dialog.expression.setText("1 > 0");
            assertEquals("The condition must use at least one channel.", error(dialog));
            dialog.expression.setText("{ " + CLT + " }");
            assertEquals("The condition cannot contain { or }.", error(dialog));

            dialog.expression.setText(CLT + " > 100 && " + RPM + " < 1000");
            assertEquals(CLT + " > 100 && " + RPM + " < 1000", valid(dialog).getExpression());
            String shortened = valid(dialog).getOnLabel();
            assertTrue(shortened.endsWith("…") && shortened.length() <= CustomIndicatorDialog.MAX_LABEL,
                "a long condition is shortened for the chip: " + shortened);

            dialog.onLabel.setText("Hot {idle}");
            assertEquals("Labels cannot contain { or }.", error(dialog));
            dialog.onLabel.setText(String.join("", Collections.nCopies(CustomIndicatorDialog.MAX_LABEL + 1, "x")));
            assertTrue(error(dialog).startsWith("Labels can be at most"));
        });
    }

    @Test
    void editingOpensTheSimplestModeAndKeepsTheId() throws Exception {
        onEdt(() -> {
            CustomIndicator onOff = new CustomIndicator("id1", FAN, "Fan off", "Fan on", Tone.NEUTRAL, Tone.GREEN);
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), onOff);
            assertTrue(dialog.isOn.isSelected());
            assertEquals(FAN, dialog.channelList.getSelectedValue());
            assertEquals(onOff, valid(dialog), "opening and saving changes nothing");

            CustomIndicator hot = new CustomIndicator("id2", CLT + " >= 100", CLT + " ≥ 100", CLT + " ≥ 100",
                Tone.DARK, Tone.YELLOW);
            dialog = new CustomIndicatorDialog(ini(), hot);
            assertTrue(dialog.threshold.isSelected());
            assertEquals(Operator.AT_LEAST, dialog.operator.getSelectedItem());
            assertEquals("100", dialog.value.getText());
            assertEquals("", dialog.onLabel.getText(), "a default label stays a default");
            assertEquals(hot, valid(dialog));

            CustomIndicator both = new CustomIndicator("id3", CLT + " > 100 && " + RPM + " < 1000", "OK", "Hot idle",
                Tone.NEUTRAL, Tone.RED);
            dialog = new CustomIndicatorDialog(ini(), both);
            assertTrue(dialog.advanced.isSelected());
            assertEquals(both, valid(dialog));
        });
    }

    /**
     * The option pane does not grow once shown, and a GridBagLayout short of its preferred size drops
     * every row to its minimum: the channel list then shrank to one cut row when a channel was picked.
     */
    @Test
    void contentKeepsItsSizeWhileEditing() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            Dimension initial = dialog.getContent().getPreferredSize();
            dialog.selectChannel(CLT);
            assertNotNull(dialog.getPreview(), "a chip instead of the hint");
            assertEquals(initial, dialog.getContent().getPreferredSize(), "after picking a channel");
            for (JRadioButton mode : new JRadioButton[]{dialog.threshold, dialog.advanced, dialog.isOn}) {
                mode.doClick();
                assertEquals(initial, dialog.getContent().getPreferredSize(), "after choosing " + mode.getText());
            }
            JScrollPane list = (JScrollPane) SwingUtilities.getAncestorOfClass(JScrollPane.class, dialog.channelList);
            assertEquals(list.getPreferredSize().height, list.getMinimumSize().height,
                "the list never gives up rows for lack of space");
        });
    }

    @Test
    void searchFiltersTheChannels() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            assertEquals(3, dialog.channelModel.size());
            dialog.search.setText("rpm");
            assertEquals(1, dialog.channelModel.size());
            assertEquals(RPM, dialog.channelList.getSelectedValue(), "the only match is chosen");
            dialog.search.setText("");
            assertEquals(3, dialog.channelModel.size());
            assertEquals(RPM, dialog.channelList.getSelectedValue(), "clearing the search keeps the choice");
        });
    }

    @Test
    void previewFollowsTheLiveChannel() throws Exception {
        onEdt(() -> {
            CustomIndicatorDialog dialog = new CustomIndicatorDialog(ini(), null);
            dialog.threshold.doClick();
            dialog.selectChannel(CLT);
            dialog.value.setText("100");
            dialog.offLabel.setText("CLT ok");
            dialog.onLabel.setText("CLT hot");
            IndicatorPanel preview = dialog.getPreview();
            JLabel chip = (JLabel) preview.getPanel().getComponent(0);
            SensorCentral.getInstance().setValue(90, CLT);
            preview.refresh(null);
            assertEquals("CLT ok", chip.getText());
            SensorCentral.getInstance().setValue(120, CLT);
            preview.refresh(null);
            assertEquals("CLT hot", chip.getText());
        });
    }
}
