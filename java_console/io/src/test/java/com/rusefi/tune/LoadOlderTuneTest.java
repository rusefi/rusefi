package com.rusefi.tune;

import com.opensr5.ini.IniFileModel;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.Page;
import org.junit.Assert;
import org.junit.Test;

import java.util.Map;

import static com.rusefi.tune.TuneReadWriteTest.SRC_TEST_RESOURCES;
import static com.rusefi.tune.TuneReadWriteTest.TEST_INI;
import static org.junit.Assert.assertFalse;

public class LoadOlderTuneTest {
    @Test
    public void loadOlderTuneAgainstCurrentIni() throws Exception {
        Msq customOldTune = Msq.readTune(TuneReadWriteTest.TUNE_NAME);
        Page dataPage = customOldTune.findPage();
        assertFalse(dataPage.constant.isEmpty());

        Msq lessOldDefaultTune = Msq.readTune(SRC_TEST_RESOURCES + "simulator_tune-2023-06.xml");

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
                boolean isSameValue = simplerSpaces(newerDefault.getValue()).equals(simplerSpaces(customValue.getValue()));
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

        IniFileModel ini = new IniFileModel().readIniFile(TEST_INI);
        System.out.printf(ini.toString());
    }

    private static Object simplerSpaces(String value) {
        if (value == null)
            return value;
        return value.replaceAll("\\s+", " ").trim();
    }
}
