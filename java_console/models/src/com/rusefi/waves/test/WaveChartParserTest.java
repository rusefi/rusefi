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

        result = WaveChartParser.unpackToMap("wave_chart,input1 A8!u!185080!r!0!2670996!crank!u!2670996_2!crank2!u!2674512!Injector 4!u!2674512!Spark 1!u!2674512!Injector 4!d!2674665!Spark 1!d!2674912!crank2!d!2680055!crank2!u!2687963!Injector 2!u!2687963!Spark 1!u!2687963!Injector 2!d!2688116!Spark 1!d!2688363!crank2!d!2693093!crank!d!2697428!crank2!u!2700454!Injector 1!u!2700454!Spark 1!u!2700454!Injector 1!d!2700607!Spark 1!d!2700854!crank2!d!2705329!crank2!u!2712449!Injector 3!u!2712449!Spark 1!u!2712449!Injector 3!d!2712681!Spark 1!d!2712849!crank2!d!2717385!r!0!2721629!crank!u!2721629!crank2!u!2724641!Injector 1!u!2724641!Injector 2!u!2724641!Injector 3!u!2724641!Injector 4!u!2724641!Spark 1!u!2724641!Injector 4!d!2726241!Injector 3!d!2726241!Injector 2!d!2726241!Injector 1!d!2726241!crank2!d!2729677!Spark 1!d!2730008!crank2!u!2736851!Injector 1!u!2736851!Injector 2!u!2736851!Injector 3!u!2736851!Injector 4!u!2736851!Spark 1!u!2736851!Injector 4!d!2738451!Injector 3!d!2738451!Injector 2!d!2738451!Injector 1!d!2738451!crank2!d!2741922!Spark 1!d!2742218!crank!d!2746104!crank2!u!2749010!Injector 1!u!2749010!Injector 2!u!2749010!Injector 3!u!2749010!Injector 4!u!2749010!Spark 1!u!2749010!Injector 4!d!2750601!Injector 3!d!2750601!Injector 2!d!2750601!Injector 1!d!2750601!crank2!d!2753919!Spark 1!d!2754377!crank2!u!2760922!Injector 1!u!2760922!Injector 2!u!2760922!Injector 3!u!2760922!Injector 4!u!2760922!Spark 1!u!2760922!Injector 4!d!2762522!Injector 3!d!2762522!Injector 2!d!2762522!Injector 1!d!2762522!crank2!d!2765882!Spark 1!d!2766289!r!236!2769990!crank!u!2769990!crank2!u!2773003!Injector 1!u!2773003!Injector 2!u!2773003!Injector 3!u!2773003!Injector 4!u!2773003!Spark 1!u!2773003!Injector 4!d!2774603!Injector 3!d!2774603!Injector 2!d!2774603!Injector 1!d!2774603!Spark 1!d!2778110!crank2!d!2778143!crank2!u!2785215!Injector 1!u!2785215!Injector 2!u!2785215!Injector 3!u!2785215!Injector 4!u!2785215!Spark 1!u!2785215!Injector 4!d!2786815!,");
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
        WaveChart result = WaveChartParser.unpackToMap("r!1199!64224414!crank2!u!64225149_3!Injector 2!u!64225149!Spark 1!u!64225249!Injector 2!d!64225303!Spark 1!d!64225649!crank2!d!64226105_4!crank!d!64226980_5!crank2!u!64227730_6!Injector 1!u!64227730!Spark 1!u!64227830!Injector 1!d!64227884!Spark 1!d!64228230!crank2!d!64228678_7!crank2!u!64230212_8!Injector 3!u!64230212!Spark 1!u!64230312!Injector 3!d!64230366!Spark 1!d!64230712!crank2!d!64231156_9!crank!u!64231982_0!crank2!u!64232672_1!Injector 4!u!64232672!Spark 1!u!64232772!Injector 4!d!64232826!Spark 1!d!64233172!crank2!d!64233626_2!r!1200!64234412!crank2!u!64235150_3!Injector 2!u!64235150!Spark 1!u!64235250!Injector 2!d!64235304!Spark 1!d!64235650!crank2!d!64236106_4!crank!d!64236981_5!crank2!u!64237730_6!Injector 1!u!64237730!Spark 1!u!64237830!Injector 1!d!64237884!Spark 1!d!64238230!crank2!d!64238677_7!crank2!u!64240213_8!Injector 3!u!64240213!Spark 1!u!64240313!Injector 3!d!64240367!Spark 1!d!64240713!crank2!d!64241158_9!crank!u!64241982_0!crank2!u!64242674_1!Injector 4!u!64242674!Spark 1!u!64242774!Injector 4!d!64242828!Spark 1!d!64243174!crank2!d!64243625_2!r!1200!64244412!crank2!u!64245149_3!Injector 2!u!64245149!Spark 1!u!64245249!Injector 2!d!64245303!Spark 1!d!64245649!crank2!d!64246106_4!crank!d!64246980_5!crank2!u!64247728_6!Injector 1!u!64247728!Spark 1!u!64247828!Injector 1!d!64247882!Spark 1!d!64248228!crank2!d!64248679_7!crank2!u!64250212_8!Injector 3!u!64250212!Spark 1!u!64250312!Injector 3!d!64250366!Spark 1!d!64250712!crank2!d!64251158_9!crank!u!64251982_0!crank2!u!64252674_1!Injector 4!u!64252674!Spark 1!u!64252774!Injector 4!d!64252828!Spark 1!d!64253174!crank2!d!64253625_2!r!1200!64254412!crank2!u!64255150_3!Injector 2!u!64255150!Spark 1!u!64255250!Injector 2!d!64255304!Spark 1!d!64255650!crank2!d!64256106_4!crank!d!64256982_5!crank2!u!64257728_6!Injector 1!u!64257728!Spark 1!u!64257828!Injector 1!d!64257882!Spark 1!d!64258228!crank2!d!64258678_7!crank2!u!64260214_8!Injector 3!u!64260214!Spark 1!u!64260314!Injector 3!d!64260368!Spark 1!d!64260714!,");
        assertFalse(result.map.isEmpty());

        StringBuilder revolutions = result.get(RevolutionLog.TOP_DEAD_CENTER_MESSAGE);

        RevolutionLog rl = RevolutionLog.parseRevolutions(revolutions);

        StringBuilder inj1 = result.get("Injector 1");
        WaveReport wr = new WaveReport(inj1.toString());

        for (WaveReport.UpDown ud : wr.getList()) {
            assertTrue(isCloseEnough(238.75, rl.getCrankAngleByTime(ud.upTime)));

            assertTrue(ud.getDutyCycle(rl) + "", isCloseEnough(0.0307, ud.getDutyCycle(rl)));
        }
    }

    @Test
    public void testUpOnly() {
        List<WaveReport.UpDown> list = WaveReport.parse("up!15500!u!25500!u!35500!u!45500!u!55500!u!65500!u!75500!u!85500!");
        assertEquals(0, list.size());
    }
}
