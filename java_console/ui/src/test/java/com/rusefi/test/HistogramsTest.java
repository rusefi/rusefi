package com.rusefi.test;

import com.rusefi.Histograms;
import org.junit.jupiter.api.Test;

import java.util.Random;

import static junit.framework.Assert.assertEquals;

/**
 * 12/18/13
 */
public class HistogramsTest {

    @Test
    public void testHistogram() {
        Histograms h = new Histograms();


        String key = "hello";
        Histograms.ValueType type = Histograms.ValueType.INVOCATION;
        h.addValue(type, key, 30);


        Histograms.LocalStats ls = h.local_stats.get();

        Histograms.StatisticsGroup sg = ls.stats.get(type);

        Histograms.Statistics data = sg.data.get(key);

        assertEquals(40, data.histogram.length);


        assertEquals(80, h.getIndex(239));
        assertEquals(223, h.getIndex(239239));
        assertEquals(364, h.getIndex(239239239));


        Random r = new Random();
        for (int i = 0; i < 2342334; i++)
            h.addValue(type, key, r.nextInt());


        assertEquals(640, data.histogram.length);


        System.out.println(h.dumpStats());

    }

}
