package com.rusefi.tune;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileReader;
import com.rusefi.*;
import com.rusefi.tools.tune.TuneCanTool;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.io.File;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.ConfigFieldImpl.unquote;
import static org.junit.jupiter.api.Assertions.*;

public class TuneCanToolTest {
    private static final Logging log = getLogging(TuneCanToolTest.class);

    @BeforeAll
    public static void before() {
        RootHolder.ROOT = "../../firmware/";
        // somewhere deep we have append prefix is not absolute path, so let's make path absolute
        //TODO: check if there exists more elegant way to initialize `TuneCanTool.boardPath` properly
        TuneCanTool.boardPath = new File(RootHolder.ROOT + "config/boards/hellen/uaefi/").getAbsolutePath() + File.separator;
    }

    @Test
    public void testGPPWMTuneParse() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(TuneCanToolTest.class.getResource("/simulator_tune-2023-06.xml").getFile());

        IniFileModel ini = IniFileReader.readIniFile(TuneReadWriteTest.TEST_INI);
        assertEquals(256, ini.getBlockingFactor());
        assertFalse(ini.getFieldsInUiOrder().isEmpty());

        try {
            StringBuilder sb = TuneCanTool.getTunePatch(lessOldDefaultTune, customOldTune, ini, TuneReadWriteTest.TUNE_NAME, new StringBuilder(), null, "");

            // tests GPPWM table bins
            assertTrue(sb.indexOf("engineConfiguration->gppwm[0].loadAxis = GPPWM_Tps;") > 0);
            assertTrue(sb.indexOf("engineConfiguration->gppwm[1].loadAxis = GPPWM_Tps;") > 0);
            assertTrue(sb.indexOf("engineConfiguration->gppwm[2].loadAxis = GPPWM_Tps;") > 0);
            assertTrue(sb.indexOf("engineConfiguration->gppwm[3].loadAxis = GPPWM_Tps;") > 0);
        } catch (Exception e) {
            System.err.print(String.format("TuneCanToolTest.testGPPWMTuneParse: Exception: %s", e.getMessage()));
            e.printStackTrace();
            throw e;
        }
    }

    @Test
    public void TuneCanToolGetParentTest() throws Exception {
         try {
             ConfigField gppwmTable = TuneCanTool.getReaderState("gppwm1_table"); // engineConfiguration->gppwm[0].table
             ConfigField cltRevLimitRpmBins = TuneCanTool.getReaderState("cltRevLimitRpmBins"); // config->cltRevLimitRpmBins
             ConfigField rpmSoftLimitTimingRetard = TuneCanTool.getReaderState("rpmSoftLimitTimingRetard"); // engineConfiguration->rpmSoftLimitTimingRetard

             String gppwmTableParentReference = TuneCanTool.getParentReference(gppwmTable, new StringBuffer("gppwm[0].table"));
             String cltRevLimitRpmBinsTableParentReference = TuneCanTool.getParentReference(cltRevLimitRpmBins, null);
             String rpmSoftLimitTimingRetardParentReference = TuneCanTool.getParentReference(rpmSoftLimitTimingRetard, null);

             assertEquals("engineConfiguration->gppwm[0].table", gppwmTableParentReference);
             assertEquals("config->", cltRevLimitRpmBinsTableParentReference);
             assertEquals("engineConfiguration->", rpmSoftLimitTimingRetardParentReference);
         } catch (final Exception e) {
             System.err.print(String.format("TuneCanToolTest.TuneCanToolGetParentTest: Exception: %s", e.getMessage()));
             e.printStackTrace();
             throw e;
         }
    }

    @Test
    public void TuneCanToolBooleanTest() throws Exception {
    	Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
    	String fieldName = "enableAemXSeries";// config->enableAemXSeries
        try {
            ConfigField cf = TuneCanTool.getReaderState(fieldName);
            Constant customValue = customOldTune.getConstantsAsMap().get(fieldName);

            // this logic is used on getTunePatch, maybe extract to function?
            Boolean configFieldState = unquote(cf.getTrueName()).equals(unquote(customValue.getValue()));

            assertFalse(configFieldState); // as config->enableAemXSeries default is false
        } catch (final Exception e) {
            System.err.print(String.format("TuneCanToolTest.TuneCanToolBooleanTest: Exception: %s", e.getMessage()));
            e.printStackTrace();
            throw e;
        }
    }
}
