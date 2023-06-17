package com.rusefi.tune;

import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.Assert;
import org.junit.Test;

import java.util.Map;

import static com.rusefi.tune.TuneReadWriteTest.SRC_TEST_RESOURCES;
import static org.junit.Assert.assertFalse;

public class LoadOlderTuneTest {
    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(SRC_TEST_RESOURCES + "simulator_tune-2023-06.xml");

        Map<String, Constant> byName = lessOldDefaultTune.getConstantsAsMap();

        int noLongerPresent = 0;
        int sameValueCounter = 0;
        int notSameValueCounter = 0;

        for (Map.Entry<String, Constant> e : customOldTune.getConstantsAsMap().entrySet()) {
            String name = e.getKey();
            Constant customValue = e.getValue();
            Constant newerDefault = lessOldDefaultTune.getConstantsAsMap().get(name);
            if (newerDefault == null) {
                noLongerPresent++;
                System.out.println("No longer present " + name);
            } else {
                boolean isSameValue = newerDefault.getValue().equals(customValue.getValue());
                if (isSameValue) {
                    System.out.println("Still around " + name);
                    sameValueCounter++;
                } else {
                    notSameValueCounter++;
                    System.out.println("Newer default " + newerDefault + " older value " + customValue);
                }
            }
        }

        Assert.assertTrue(noLongerPresent > 0);
        Assert.assertTrue(sameValueCounter > 0);
        Assert.assertTrue(notSameValueCounter > 0);

        //IniFileModel ini = new IniFileModel().readIniFile(INI_FILE_FOR_SIMULATOR);
    }
}
