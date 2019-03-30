package rusefi;

import org.junit.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class GccMapReaderTest {
    @Test
    public void testSingleLine() {
        List<GccMapReader.Record> r = GccMapReader.process(Collections.singletonList(" .bss.PWMD1     0x1fff9a0c       0x18 build_kinetis/obj/hal_pwm_lld.o"));
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x18, r.get(0).getSize());
    }

    @Test
    public void testMultiLine() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                " .bss._ZL12turnOffEvent",
                "0x1fff9db8       0x60 build_kinetis/obj/aux_valves.o"));
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x60, r.get(0).getSize());
    }

    @Test
    public void testThreeLine() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                " .bss.ch_idle_thread_wa",
                "                0x1fff8d10      0x610 build_kinetis/obj/chsys.o",
                "                0x1fff8d10                ch_idle_thread_wa"));
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x610, r.get(0).getSize());
    }

    @Test
    public void testIssue() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                ".bss            0x1fff8d10     0xf2e4 load address 0x0002b1bc",
                "                0x1fff8d10                . = ALIGN (0x4)",
                "                0x1fff8d10                _bss_start = .",
                " *(.bss)",
                " *(.bss.*)",
                " .bss.ch_idle_thread_wa",
                "                0x1fff8d10      0x610 build_kinetis/obj/chsys.o",
                "                0x1fff8d10                ch_idle_thread_wa",
                " .bss.ch        0x1fff9320       0x80 build_kinetis/obj/chschd.o",
                "                0x1fff9320                ch"));
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x80, r.get(0).getSize());
    }
}
