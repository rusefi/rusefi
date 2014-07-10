package com.irnems.ui.test;

import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.WavePanel;
import org.junit.Test;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.Set;
import java.util.TreeSet;

import static junit.framework.Assert.assertEquals;

/**
 * 6/25/14
 * (c) Andrey Belomutskiy
 */

public class WavePanelTest {
    @Test
    public void testInsertIndex() {

        Set<String> names = new TreeSet<String>();
        names.add(WavePanel.CRANK1);

        String name;

        name = "inj2";
        assertEquals(-1, WavePanel.getInsertIndex(name, names));
        names.add(name);

        name = "inj1";
        assertEquals(1, WavePanel.getInsertIndex(name, names));
        names.add(name);


        assertEquals(1, WavePanel.getInsertIndex("spa1", names));

    }
}

