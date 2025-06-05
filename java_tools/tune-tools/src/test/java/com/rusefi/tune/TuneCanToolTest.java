package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.*;
import com.rusefi.tools.tune.TuneCanTool;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.jupiter.api.Test;

import static com.rusefi.ConfigFieldImpl.unquote;
import static org.junit.jupiter.api.Assertions.*;

public class TuneCanToolTest {
    @Test
    public void testGPPWMTuneParse() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(TuneCanToolTest.class.getResource("/simulator_tune-2023-06.xml").getFile());

        IniFileModel ini = IniFileModelImpl.readIniFile(TuneReadWriteTest.TEST_INI);
        assertEquals(256, ini.getBlockingFactor());
        assertFalse(ini.getFieldsInUiOrder().isEmpty());

        RootHolder.ROOT = "../../firmware/";

        StringBuilder sb = TuneCanTool.getTunePatch(lessOldDefaultTune, customOldTune, ini, TuneReadWriteTest.TUNE_NAME, new StringBuilder(), null, "");

        // tests GPPWM table bins
        assertTrue(sb.indexOf("engineConfiguration->gppwm[0].loadAxis = GPPWM_Tps;") > 0);
        assertTrue(sb.indexOf("engineConfiguration->gppwm[1].loadAxis = GPPWM_Tps;") > 0);
        assertTrue(sb.indexOf("engineConfiguration->gppwm[2].loadAxis = GPPWM_Tps;") > 0);
        assertTrue(sb.indexOf("engineConfiguration->gppwm[3].loadAxis = GPPWM_Tps;") > 0);
    }

    @Test
    public void TuneCanToolGetParentTest() throws Exception {
    	 RootHolder.ROOT = "../../firmware/";

    	 ConfigField gppwmTable = TuneCanTool.getReaderState("gppwm1_table"); // engineConfiguration->gppwm[0].table
    	 ConfigField cltRevLimitRpmBins = TuneCanTool.getReaderState("cltRevLimitRpmBins"); // config->cltRevLimitRpmBins
    	 ConfigField rpmSoftLimitTimingRetard = TuneCanTool.getReaderState("rpmSoftLimitTimingRetard"); // engineConfiguration->rpmSoftLimitTimingRetard

    	 String gppwmTableParentReference = TuneCanTool.getParentReference(gppwmTable, new StringBuffer("gppwm[0].table"));
    	 String cltRevLimitRpmBinsTableParentReference = TuneCanTool.getParentReference(cltRevLimitRpmBins, null);
    	 String rpmSoftLimitTimingRetardParentReference = TuneCanTool.getParentReference(rpmSoftLimitTimingRetard, null);

    	 assertEquals("engineConfiguration->gppwm[0].table", gppwmTableParentReference);
    	 assertEquals("config->", cltRevLimitRpmBinsTableParentReference);
    	 assertEquals("engineConfiguration->", rpmSoftLimitTimingRetardParentReference);
    }

    @Test
    public void TuneCanToolBooleanTest() throws Exception {
    	RootHolder.ROOT = "../../firmware/";
    	Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
    	String fieldName = "enableCanWideband";// config->enableCanWideband
    	ConfigField cf = TuneCanTool.getReaderState(fieldName);
    	Constant customValue = customOldTune.getConstantsAsMap().get(fieldName);

    	// this logic is used on getTunePatch, maybe extract to function?
    	Boolean configFieldState = unquote(cf.getTrueName()).equals(unquote(customValue.getValue()));

    	assertFalse(configFieldState); // as config->enableCanWideband default is false
    }
}
