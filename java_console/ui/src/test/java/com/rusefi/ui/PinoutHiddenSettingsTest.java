package com.rusefi.ui;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileMetaInfoImpl;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

/**
 * A pin setting Tuning hides or disables: by its own condition, by the panel embedding its dialog, or by the menu
 * entry opening its page. The pickers say which settings hide it and offer to open Tuning where it is turned on.
 */
class PinoutHiddenSettingsTest {
    private static final String B6 = "B6 Low Side output";

    @Test
    void hiddenSettingsNameWhatHidesThemAndWhereToTurnThemOn() throws Throwable {
        IniFileModel ini = readIni();
        // every feature off, no cylinders
        Map<String, PinAssignments.Candidate> off = byKey(PinAssignments.candidates(ini, image(0, 0, 0), B6));

        PinAssignments.Candidate boost = off.get("boostPin");
        assertTrue(boost.hidden, "the panel embedding its dialog is off");
        assertEquals(Collections.singletonList("Enable boost"), boost.hiddenBy);
        assertArrayEquals(new String[]{"boostEnabled", "boostPage"}, boost.openTarget());

        PinAssignments.Candidate nitrous = off.get("nitrousPin");
        assertTrue(nitrous.hidden, "its menu entry is off");
        assertEquals(Collections.singletonList("Enable nitrous"), nitrous.hiddenBy);
        assertArrayEquals(new String[]{"nitrousEnabled", "features"}, nitrous.openTarget());

        PinAssignments.Candidate injector = off.get("injectionPins3");
        assertTrue(injector.hidden, "its own condition is off");
        assertEquals(Collections.singletonList("Cylinders count"), injector.hiddenBy);
        assertArrayEquals(new String[]{"cylindersCount", "features"}, injector.openTarget());

        PinAssignments.Candidate fan = off.get("fanPin");
        assertFalse(fan.hidden);
        assertTrue(fan.hiddenBy.isEmpty());
        assertArrayEquals(new String[]{"fanPin", "fanSettings"}, fan.openTarget(), "opens the setting itself");

        // every feature on, four cylinders
        for (PinAssignments.Candidate candidate : PinAssignments.candidates(ini, image(1, 1, 4), B6)) {
            assertFalse(candidate.hidden, candidate.fieldKey);
        }
    }

    @Test
    void pickerExplainsAndOffersToOpenTuning() throws Throwable {
        IniFileModel ini = readIni();
        PinoutPin b6 = new PinoutPin("B6", "Low side", "outputs", "outputs", B6, "");
        PinAssignPanel panel = new PinAssignPanel(b6, "Connector B",
            PinAssignments.candidates(ini, image(0, 0, 0), B6), ts -> ts.substring(0, 2));
        List<String> assigned = new ArrayList<>();
        List<String[]> opened = new ArrayList<>();
        panel.setOnAssign(candidate -> assigned.add(candidate.fieldKey));
        panel.setOnOpenInTuning(opened::add);

        assertTrue(panel.select("fanPin"));
        assertFalse(panel.assignAndOpenButton.isVisible(), "a shown setting needs no detour");

        assertTrue(panel.select("boostPin"));
        assertTrue(panel.assignAndOpenButton.isVisible());
        assertEquals("Boost output will use B6. Tuning hides or disables it because of Enable boost: "
            + "assigning a pin does not turn it on.", panel.effect.getText());
        assertEquals("Boost output  (off)", PinoutFunctionsPage.settingText(panel.selected()));
        assertEquals("Tuning hides or disables it because of Enable boost",
            PinAssignPanel.hiddenReason(panel.selected()));

        panel.assignAndOpenButton.doClick();
        assertEquals(Collections.singletonList("boostPin"), assigned, "it assigns first");
        assertEquals(1, opened.size());
        assertArrayEquals(new String[]{"boostEnabled", "boostPage"}, opened.get(0), "then opens Enable boost");
    }

    private static Map<String, PinAssignments.Candidate> byKey(List<PinAssignments.Candidate> candidates) {
        return candidates.stream().collect(Collectors.toMap(c -> c.fieldKey, Function.identity()));
    }

    /** Boost and nitrous enabled (0/1), and the cylinder count; no pin chosen. */
    private static ConfigurationImage image(int boost, int nitrous, int cylinders) {
        // padded: the expression evaluator reads 4 bytes for any scalar, U08 included
        return ConfigurationImageTestHelper.newLittleEndian(new byte[]{0, (byte) boost, 0, (byte) nitrous, 0, (byte) cylinders, 0, 0, 0, 0});
    }

    private static IniFileModel readIni() throws Throwable {
        String outputs = "\"NONE\", \"" + B6 + "\"";
        String text = "[Constants]\n"
            + "signature = \"pinout-hidden-test\"\n"
            + "nPages = 1\npageSize = 10\npageReadCommand = \"r\"\nochBlockSize = 0\npage = 1\n"
            + "boostPin = bits, U08, 0, [0:0], " + outputs + "\n"
            + "boostEnabled = bits, U08, 1, [0:0], \"false\", \"true\"\n"
            + "nitrousPin = bits, U08, 2, [0:0], " + outputs + "\n"
            + "nitrousEnabled = bits, U08, 3, [0:0], \"false\", \"true\"\n"
            + "injectionPins3 = bits, U08, 4, [0:0], " + outputs + "\n"
            + "cylindersCount = scalar, U08, 5, \"\", 1, 0, 1, 12, 0\n"
            + "fanPin = bits, U08, 6, [0:0], " + outputs + "\n"
            + "[Menu]\nmenu = \"Setup\"\n"
            + "subMenu = boostPage, \"Boost control\"\n"
            + "subMenu = nitrousPage, \"Nitrous\", { nitrousEnabled }\n"
            + "subMenu = features, \"Features\"\n"
            + "subMenu = injectionOutputs, \"Injection outputs\"\n"
            + "subMenu = fanSettings, \"Fan\"\n"
            + "[UserDefined]\n"
            // the boost output panel shows only once boost is on, like many rusEFI feature pages
            + "dialog = boostOutput, \"Boost output\"\nfield = \"Boost output\", boostPin\n"
            + "dialog = boostPage, \"Boost control\"\nfield = \"Enable boost\", boostEnabled\n"
            + "panel = boostOutput, West, { boostEnabled }\n"
            + "dialog = nitrousPage, \"Nitrous\"\nfield = \"Nitrous output\", nitrousPin\n"
            + "dialog = features, \"Features\"\nfield = \"Enable nitrous\", nitrousEnabled\n"
            + "field = \"Cylinders count\", cylindersCount\n"
            // as in tunerstudio.template.ini: an injector output shows only for the cylinders the engine has
            + "dialog = injectionOutputs, \"Injection outputs\"\n"
            + "field = \"Injection Output 3\", injectionPins3, { cylindersCount > 2 }\n"
            + "dialog = fanSettings, \"Fan\"\nfield = \"Fan output\", fanPin\n";
        RawIniFile raw = IniFileReaderUtil.read(new ByteArrayInputStream(text.getBytes(StandardCharsets.UTF_8)));
        return IniFileReaderUtil.readIniFile(raw, "pinout-hidden-test.ini", new IniFileMetaInfoImpl(raw));
    }
}
