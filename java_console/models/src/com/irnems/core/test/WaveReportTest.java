package com.irnems.core.test;

import com.rusefi.waves.WaveReport;
import com.irnems.waves.ZoomProvider;
import org.junit.Test;

import static junit.framework.Assert.assertEquals;

/**
 * Date: 6/23/13
 * (c) Andrey Belomutskiy
 */
public class WaveReportTest {
    public static final String report = "up!14679!down!15991!up!16823!down!18134!up!18965!down!20278!up!21108!down!22420!up!23251!down!24563!up!25394!down!26706!up!27536!down!28850!up!29678!down!30991!up!31822!down!33134!up!33965!down!35277!up!36108!down!37420!up!38251!down!39563!up!40394!down!41706!up!42537!down!43849!";

    @Test
    public void testParse() {
        WaveReport wr = new WaveReport(report);
        assertEquals(14, wr.getList().size());

        assertEquals(14679, wr.getMinTime());
        assertEquals(43849, wr.getMaxTime());

        assertEquals(59, wr.timeToScreen(18134, 500, ZoomProvider.DEFAULT));
    }
}
