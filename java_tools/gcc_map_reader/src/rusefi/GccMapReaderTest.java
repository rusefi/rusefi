package rusefi;

import org.junit.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class GccMapReaderTest {
    private static final String BSS = "bss";

    @Test
    public void testSimpleLinesWithTagInTheMiddle() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                " .bss.PWMD1     0x1fff9a0c       0x18 build_kinetis/obj/hal_pwm_lld.o",
                GccMapReader.START_OF_DATA_TAG,
                " .bss.PWMD2     0x1fff9a0c       0x18 build_kinetis/obj/hal_pwm_lld.o"

        ), BSS);
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x18, r.get(0).getSize());
    }

    @Test
    public void testMultiLine() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                GccMapReader.START_OF_DATA_TAG,
                " .bss._ZL12turnOffEvent",
                "0x1fff9db8       0x60 build_kinetis/obj/aux_valves.o"), BSS);
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x60, r.get(0).getSize());
    }

    @Test
    public void testThreeLine() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                GccMapReader.START_OF_DATA_TAG,
                " .bss.ch_idle_thread_wa",
                "                0x1fff8d10      0x610 build_kinetis/obj/chsys.o",
                "                0x1fff8d10                ch_idle_thread_wa"), BSS);
        assertNotNull(r);
        assertEquals(1, r.size());
        assertEquals(0x610, r.get(0).getSize());
    }

    @Test
    public void testIssue() {
        List<GccMapReader.Record> r = GccMapReader.process(Arrays.asList(
                GccMapReader.START_OF_DATA_TAG,
                ".bss            0x1fff8d10     0xf2e4 load address 0x0002b1bc",
                "                0x1fff8d10                . = ALIGN (0x4)",
                "                0x1fff8d10                _bss_start = .",
                " *(.bss)",
                " *(.bss.*)",
                " .bss.ch_idle_thread_wa",
                "                0x1fff8d10      0x610 build_kinetis/obj/chsys.o",
                "                0x1fff8d10                ch_idle_thread_wa",
                " .bss.ch        0x1fff9320       0x80 build_kinetis/obj/chschd.o",
                "                0x1fff9320                ch"), BSS);
        assertNotNull(r);
        assertEquals(2, r.size());
        assertEquals(0x610, r.get(0).getSize());
        assertEquals(0x80, r.get(1).getSize());
    }

    @Test
    public void testData() {
        String[] strings = {
                GccMapReader.START_OF_DATA_TAG,
                " .text._dbg_check_suspend",
                "                0x00000000       0x1c build_kinetis/obj/chdebug.o",
                " .rodata._dbg_check_disable.str1.4",
                "                0x00000000        0x5 build_kinetis/obj/chdebug.o",
                " .rodata._dbg_check_suspend.str1.4",
                "                0x00000000        0x5 build_kinetis/obj/chdebug.o",
                " .text          0x00000000        0x0 build_kinetis/obj/chtrace.o",
                " .data          0x00000000        0x0 build_kinetis/obj/chtrace.o",
                " .bss.f         0x00000000        0x7 build_kinetis/obj/chtrace.o",
                " .comment       0x00000000       0x80 build_kinetis/obj/chtrace.o"};
        List<GccMapReader.Record> rodata = GccMapReader.process(Arrays.asList(strings), "rodata");
        assertNotNull(rodata);
        assertEquals(2, rodata.size());
        assertEquals(5, rodata.get(0).getSize());

        List<GccMapReader.Record> bss = GccMapReader.process(Arrays.asList(strings), BSS);
        assertNotNull(bss);
        assertEquals(1, bss.size());

    }
}
