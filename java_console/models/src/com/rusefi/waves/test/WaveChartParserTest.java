package com.rusefi.waves.test;

import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.WaveChart;
import com.rusefi.waves.WaveChartParser;
import com.rusefi.waves.WaveReport;
import org.junit.Test;

import java.util.List;

import static com.rusefi.waves.WaveReport.isCloseEnough;
import static junit.framework.Assert.*;

/**
 * 1/26/14
 * Andrey Belomutskiy (c) 2012-2014
 */
public class WaveChartParserTest {
    @Test
    public void testMultiParser() {
        WaveChart result = WaveChartParser.unpackToMap("");
        assertTrue(result.map.isEmpty());

        result = WaveChartParser.unpackToMap("wave_chart,input1 A8!up!185080!r!0!2670996!crank!up!2670996_2!crank2!up!2674512!Injector 4!up!2674512!Spark 1!up!2674512!Injector 4!down!2674665!Spark 1!down!2674912!crank2!down!2680055!crank2!up!2687963!Injector 2!up!2687963!Spark 1!up!2687963!Injector 2!down!2688116!Spark 1!down!2688363!crank2!down!2693093!crank!down!2697428!crank2!up!2700454!Injector 1!up!2700454!Spark 1!up!2700454!Injector 1!down!2700607!Spark 1!down!2700854!crank2!down!2705329!crank2!up!2712449!Injector 3!up!2712449!Spark 1!up!2712449!Injector 3!down!2712681!Spark 1!down!2712849!crank2!down!2717385!r!0!2721629!crank!up!2721629!crank2!up!2724641!Injector 1!up!2724641!Injector 2!up!2724641!Injector 3!up!2724641!Injector 4!up!2724641!Spark 1!up!2724641!Injector 4!down!2726241!Injector 3!down!2726241!Injector 2!down!2726241!Injector 1!down!2726241!crank2!down!2729677!Spark 1!down!2730008!crank2!up!2736851!Injector 1!up!2736851!Injector 2!up!2736851!Injector 3!up!2736851!Injector 4!up!2736851!Spark 1!up!2736851!Injector 4!down!2738451!Injector 3!down!2738451!Injector 2!down!2738451!Injector 1!down!2738451!crank2!down!2741922!Spark 1!down!2742218!crank!down!2746104!crank2!up!2749010!Injector 1!up!2749010!Injector 2!up!2749010!Injector 3!up!2749010!Injector 4!up!2749010!Spark 1!up!2749010!Injector 4!down!2750601!Injector 3!down!2750601!Injector 2!down!2750601!Injector 1!down!2750601!crank2!down!2753919!Spark 1!down!2754377!crank2!up!2760922!Injector 1!up!2760922!Injector 2!up!2760922!Injector 3!up!2760922!Injector 4!up!2760922!Spark 1!up!2760922!Injector 4!down!2762522!Injector 3!down!2762522!Injector 2!down!2762522!Injector 1!down!2762522!crank2!down!2765882!Spark 1!down!2766289!r!236!2769990!crank!up!2769990!crank2!up!2773003!Injector 1!up!2773003!Injector 2!up!2773003!Injector 3!up!2773003!Injector 4!up!2773003!Spark 1!up!2773003!Injector 4!down!2774603!Injector 3!down!2774603!Injector 2!down!2774603!Injector 1!down!2774603!Spark 1!down!2778110!crank2!down!2778143!crank2!up!2785215!Injector 1!up!2785215!Injector 2!up!2785215!Injector 3!up!2785215!Injector 4!up!2785215!Spark 1!up!2785215!Injector 4!down!2786815!,");
        assertEquals(9, result.map.size());

        String crankReport = result.get("crank").toString();

        List<WaveReport.UpDown> list = WaveReport.parse(crankReport);
        assertEquals(2, list.size());

        WaveReport.UpDown upDown = list.get(0);
        assertEquals(2670996, upDown.upTime);
        assertEquals(2, upDown.upIndex);

        assertEquals(2697428, upDown.downTime);
        assertEquals(-1, upDown.downIndex);
    }

    @Test
    public void testDutyCycle() {
        WaveChart result = WaveChartParser.unpackToMap("r!1199!64224414!crank2!up!64225149_3!Injector 2!up!64225149!Spark 1!up!64225249!Injector 2!down!64225303!Spark 1!down!64225649!crank2!down!64226105_4!crank!down!64226980_5!crank2!up!64227730_6!Injector 1!up!64227730!Spark 1!up!64227830!Injector 1!down!64227884!Spark 1!down!64228230!crank2!down!64228678_7!crank2!up!64230212_8!Injector 3!up!64230212!Spark 1!up!64230312!Injector 3!down!64230366!Spark 1!down!64230712!crank2!down!64231156_9!crank!up!64231982_0!crank2!up!64232672_1!Injector 4!up!64232672!Spark 1!up!64232772!Injector 4!down!64232826!Spark 1!down!64233172!crank2!down!64233626_2!r!1200!64234412!crank2!up!64235150_3!Injector 2!up!64235150!Spark 1!up!64235250!Injector 2!down!64235304!Spark 1!down!64235650!crank2!down!64236106_4!crank!down!64236981_5!crank2!up!64237730_6!Injector 1!up!64237730!Spark 1!up!64237830!Injector 1!down!64237884!Spark 1!down!64238230!crank2!down!64238677_7!crank2!up!64240213_8!Injector 3!up!64240213!Spark 1!up!64240313!Injector 3!down!64240367!Spark 1!down!64240713!crank2!down!64241158_9!crank!up!64241982_0!crank2!up!64242674_1!Injector 4!up!64242674!Spark 1!up!64242774!Injector 4!down!64242828!Spark 1!down!64243174!crank2!down!64243625_2!r!1200!64244412!crank2!up!64245149_3!Injector 2!up!64245149!Spark 1!up!64245249!Injector 2!down!64245303!Spark 1!down!64245649!crank2!down!64246106_4!crank!down!64246980_5!crank2!up!64247728_6!Injector 1!up!64247728!Spark 1!up!64247828!Injector 1!down!64247882!Spark 1!down!64248228!crank2!down!64248679_7!crank2!up!64250212_8!Injector 3!up!64250212!Spark 1!up!64250312!Injector 3!down!64250366!Spark 1!down!64250712!crank2!down!64251158_9!crank!up!64251982_0!crank2!up!64252674_1!Injector 4!up!64252674!Spark 1!up!64252774!Injector 4!down!64252828!Spark 1!down!64253174!crank2!down!64253625_2!r!1200!64254412!crank2!up!64255150_3!Injector 2!up!64255150!Spark 1!up!64255250!Injector 2!down!64255304!Spark 1!down!64255650!crank2!down!64256106_4!crank!down!64256982_5!crank2!up!64257728_6!Injector 1!up!64257728!Spark 1!up!64257828!Injector 1!down!64257882!Spark 1!down!64258228!crank2!down!64258678_7!crank2!up!64260214_8!Injector 3!up!64260214!Spark 1!up!64260314!Injector 3!down!64260368!Spark 1!down!64260714!,");
        assertFalse(result.map.isEmpty());

        StringBuilder revolutions = result.get(RevolutionLog.TOP_DEAD_CENTER_MESSAGE);

        RevolutionLog rl = RevolutionLog.parseRevolutions(revolutions);

        StringBuilder inj1 = result.get(WaveChart.INJECTOR_1);
        WaveReport wr = new WaveReport(inj1.toString());

        for (WaveReport.UpDown ud : wr.getList()) {
            assertTrue(isCloseEnough(238.75, rl.getCrankAngleByTime(ud.upTime)));

            assertTrue(isCloseEnough(0.308, ud.getDutyCycle(rl)));
        }
    }

    @Test
    public void testUpOnly() {
        List<WaveReport.UpDown> list = WaveReport.parse("up!15500!up!25500!up!35500!up!45500!up!55500!up!65500!up!75500!up!85500!");
        assertEquals(0, list.size());
    }
}
