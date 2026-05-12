package com.opensr5.ini.test;

import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.RawIniFile;
import com.rusefi.ini.reader.IniFileReaderUtil;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class FrontPageTest {
    @Test
    public void testFrontPageParsing() throws IOException {
        String iniContent = "[FrontPage]\n" +
                "    gauge1 = RPMGauge\n" +
                "    gauge2 = CLTGauge\n" +
                "    indicator = { hasCriticalError }, \"Config Error\", \"Config Error\", white, black, red, black\n" +
                "    indicator = { needBurn }, \"Settings saved\", \"Unsaved changes\",  white,  black, yellow,  black\n" +
                "    indicator = { fuelCutReasonBlinker  != 0 }, \"Injection OK\", { Fuel cut: bitStringValue(fuelIgnCutCodeList, fuelCutReason)}, white, black, yellow, black\n";

        RawIniFile rawIniFile = IniFileReaderUtil.read(new ByteArrayInputStream(iniContent.getBytes(StandardCharsets.UTF_8)));
        IniFileModel model = IniFileReaderTest.readLines(rawIniFile);

        FrontPageModel frontPage = model.getFrontPage();
        assertEquals(2, frontPage.getGaugeNames().size());
        assertEquals("RPMGauge", frontPage.getGaugeNames().get(0));
        assertEquals("CLTGauge", frontPage.getGaugeNames().get(1));

        assertEquals(3, frontPage.getIndicators().size());
        assertEquals("{ hasCriticalError }", frontPage.getIndicators().get(0).getExpression());
        assertEquals("Config Error", frontPage.getIndicators().get(0).getOffLabel());
        assertEquals("red", frontPage.getIndicators().get(0).getOnBg());

        assertEquals("{ fuelCutReasonBlinker  != 0 }", frontPage.getIndicators().get(2).getExpression());
        assertEquals("Injection OK", frontPage.getIndicators().get(2).getOffLabel());
        assertEquals("{ Fuel cut: bitStringValue(fuelIgnCutCodeList, fuelCutReason)}", frontPage.getIndicators().get(2).getOnLabel());
    }
}
