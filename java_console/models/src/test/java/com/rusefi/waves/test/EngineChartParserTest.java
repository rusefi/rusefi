package com.rusefi.waves.test;

import com.rusefi.config.generated.Fields;
import com.rusefi.waves.EngineChart;
import com.rusefi.waves.EngineReport;
import com.rusefi.waves.RevolutionLog;
import com.rusefi.waves.EngineChartParser;
import org.junit.Test;

import java.util.List;

import static com.rusefi.waves.EngineReport.isCloseEnough;
import static junit.framework.Assert.*;

/**
 * 1/26/14
 * Andrey Belomutskiy (c) 2012-2014
 */
public class EngineChartParserTest {
    @Test
    public void testMultiParser() {
        EngineChart result = EngineChartParser.unpackToMap("");
        assertTrue(result.getMap().isEmpty());

        result = EngineChartParser.unpackToMap("wave_chart,c1!u_154!0!c1!d_155!11!c1!u_156!24!spa5!u!36!c1!d_157!39!c1!u_158!49!c1!d_159!61!c1!u_160!74!c1!d_161!86!c1!u_162!99!c1!d_163!111!c1!u_164!124!");
        assertEquals(3, result.getMap().size());

        String crankReport = result.get("c1").toString();

        List<EngineReport.UpDown> list = EngineReport.parse(crankReport);
        assertEquals(4, list.size());

        EngineReport.UpDown upDown = list.get(0);
        assertEquals(24, upDown.upTime);
        assertEquals(156, upDown.upTriggerCycleIndex);

        assertEquals(39, upDown.downTime);
        assertEquals(157, upDown.downTriggerCycleIndex);
    }

    @Test
    public void testDutyCycle() {
        EngineChart result = EngineChartParser.unpackToMap("r!1199!64224414!crank2!u!64225149_3!Injector 2!u!64225149!Spark 1!u!64225249!Injector 2!d!64225303!Spark 1!d!64225649!crank2!d!64226105_4!crank!d!64226980_5!crank2!u!64227730_6!Injector 1!u!64227730!Spark 1!u!64227830!Injector 1!d!64227884!Spark 1!d!64228230!crank2!d!64228678_7!crank2!u!64230212_8!Injector 3!u!64230212!Spark 1!u!64230312!Injector 3!d!64230366!Spark 1!d!64230712!crank2!d!64231156_9!crank!u!64231982_0!crank2!u!64232672_1!Injector 4!u!64232672!Spark 1!u!64232772!Injector 4!d!64232826!Spark 1!d!64233172!crank2!d!64233626_2!r!1200!64234412!crank2!u!64235150_3!Injector 2!u!64235150!Spark 1!u!64235250!Injector 2!d!64235304!Spark 1!d!64235650!crank2!d!64236106_4!crank!d!64236981_5!crank2!u!64237730_6!Injector 1!u!64237730!Spark 1!u!64237830!Injector 1!d!64237884!Spark 1!d!64238230!crank2!d!64238677_7!crank2!u!64240213_8!Injector 3!u!64240213!Spark 1!u!64240313!Injector 3!d!64240367!Spark 1!d!64240713!crank2!d!64241158_9!crank!u!64241982_0!crank2!u!64242674_1!Injector 4!u!64242674!Spark 1!u!64242774!Injector 4!d!64242828!Spark 1!d!64243174!crank2!d!64243625_2!r!1200!64244412!crank2!u!64245149_3!Injector 2!u!64245149!Spark 1!u!64245249!Injector 2!d!64245303!Spark 1!d!64245649!crank2!d!64246106_4!crank!d!64246980_5!crank2!u!64247728_6!Injector 1!u!64247728!Spark 1!u!64247828!Injector 1!d!64247882!Spark 1!d!64248228!crank2!d!64248679_7!crank2!u!64250212_8!Injector 3!u!64250212!Spark 1!u!64250312!Injector 3!d!64250366!Spark 1!d!64250712!crank2!d!64251158_9!crank!u!64251982_0!crank2!u!64252674_1!Injector 4!u!64252674!Spark 1!u!64252774!Injector 4!d!64252828!Spark 1!d!64253174!crank2!d!64253625_2!r!1200!64254412!crank2!u!64255150_3!Injector 2!u!64255150!Spark 1!u!64255250!Injector 2!d!64255304!Spark 1!d!64255650!crank2!d!64256106_4!crank!d!64256982_5!crank2!u!64257728_6!Injector 1!u!64257728!Spark 1!u!64257828!Injector 1!d!64257882!Spark 1!d!64258228!crank2!d!64258678_7!crank2!u!64260214_8!Injector 3!u!64260214!Spark 1!u!64260314!Injector 3!d!64260368!Spark 1!d!64260714!,");
        assertFalse(result.getMap().isEmpty());

        StringBuilder revolutions = result.get(Fields.TOP_DEAD_CENTER_MESSAGE);

        RevolutionLog rl = RevolutionLog.parseRevolutions(revolutions);

        StringBuilder inj1 = result.get("Injector 1");
        EngineReport wr = new EngineReport(inj1.toString());

        for (EngineReport.UpDown ud : wr.getList()) {
            assertTrue(isCloseEnough(238.75, rl.getCrankAngleByTime(ud.upTime)));

            assertTrue(ud.getDutyCycle(rl) + "", isCloseEnough(0.0307, ud.getDutyCycle(rl)));
        }
    }

    @Test
    public void testUpOnly() {
        List<EngineReport.UpDown> list = EngineReport.parse("up!15500!u!25500!u!35500!u!45500!u!55500!u!65500!u!75500!u!85500!");
        assertEquals(0, list.size());
    }
}
