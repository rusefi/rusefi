package rusefi;

import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

public class GccMapReaderTest {
    private static final String BSS = "bss";

    private static List<String> mapRecord(String columns, boolean multiline) {
        return multiline
                ? Arrays.asList(GccMapReader.START_OF_DATA_TAG, " .bss.ch_idle_thread_wa", columns)
                : Arrays.asList(GccMapReader.START_OF_DATA_TAG, " .bss.ch_idle_thread_wa " + columns);
    }

    @Test
    public void testHexPrefixInObjectPath() {
        for (boolean multiline : new boolean[]{false, true}) {
            for (String path : new String[]{
                    "/tmp/ccAB0xCD.ltrans0.ltrans.o",
                    "C:/Users/Runner Admin/AppData/Local/Temp/ccAB0xCD.ltrans0.ltrans.o",
                    "C:\\Users\\Runner Admin\\AppData\\Local\\Temp\\ccAB0xCD.ltrans0.ltrans.o"}) {
                List<String> lines = mapRecord("  0x20000000   0x610 " + path, multiline);
                List<GccMapReader.Record> records = GccMapReader.process(lines, BSS);
                assertEquals(1, records.size());
                assertEquals(0x610, records.get(0).getSize());
                assertTrue(records.get(0).toString().contains(path));
            }
        }
    }

    @Test
    public void testHexadecimalFilenameSuffix() {
        for (boolean multiline : new boolean[]{false, true}) {
            List<GccMapReader.Record> records = GccMapReader.process(
                    mapRecord("0x20000000 0x610 /tmp/object0x20", multiline), BSS);
            assertEquals(1, records.size());
            assertEquals(0x610, records.get(0).getSize());
        }
    }

    @Test
    public void testOversizedSectionDiagnostic() {
        for (boolean multiline : new boolean[]{false, true}) {
            List<String> lines = mapRecord("0x20000000 0x80000000 build/obj/large.o", multiline);
            IllegalStateException error = assertThrows(IllegalStateException.class,
                    () -> GccMapReader.process(lines, BSS));
            assertEquals("Invalid size at line " + lines.size() + ": " + lines.get(lines.size() - 1),
                    error.getMessage());
            assertInstanceOf(NumberFormatException.class, error.getCause());
        }
    }

    @Test
    public void testTruncatedMultilineRecord() {
        List<String> lines = Arrays.asList(GccMapReader.START_OF_DATA_TAG, " .bss.truncated");
        IllegalStateException error = assertThrows(IllegalStateException.class,
                () -> GccMapReader.process(lines, BSS));
        assertEquals("Missing address and size after line 2:  .bss.truncated", error.getMessage());
    }

    @Test
    public void testMalformedColumns() {
        for (boolean multiline : new boolean[]{false, true}) {
            for (String columns : new String[]{
                    "0x20000000 0xNOPE file.o", "0xWRONG 0x610 file.o", "0x20000000 file.o"}) {
                List<String> lines = mapRecord(columns, multiline);
                IllegalStateException error = assertThrows(IllegalStateException.class,
                        () -> GccMapReader.process(lines, BSS));
                assertEquals("Invalid address or size at line " + lines.size() + ": " + lines.get(lines.size() - 1),
                        error.getMessage());
            }
        }
    }

    @Test
    public void testIgnoresScriptAndSymbolRows() {
        List<GccMapReader.Record> records = GccMapReader.process(Arrays.asList(
                GccMapReader.START_OF_DATA_TAG,
                " *(.bss.*)",
                "                0x20000000 . = ALIGN (0x4)",
                "                0x20000000 .bss.symbol",
                " .text.other 0x08000000 0x20 build/other.o",
                " .bss.real 0x20000000 0x18 build/ALIGN.o"), BSS);
        assertEquals(1, records.size());
        assertEquals(0x18, records.get(0).getSize());
    }

    @Test
    public void testColumnsWithoutObjectPath() {
        for (boolean multiline : new boolean[]{false, true}) {
            List<GccMapReader.Record> records = GccMapReader.process(
                    mapRecord("\t0x200000AB\t0xAB\t", multiline), BSS);
            assertEquals(1, records.size());
            assertEquals(0xAB, records.get(0).getSize());
        }
    }

    @Test
    public void testTextTotalsExcludeUnwindAndFill() {
        List<GccMapReader.Record> records = GccMapReader.process(Arrays.asList(
                GccMapReader.START_OF_DATA_TAG,
                " *(SORT_BY_ALIGNMENT(.text.*))",
                " *fill*         0x08000000 0x6",
                " .ARM.exidx.text.handler",
                "                0x08000006 0x8 build/unwind.o",
                " .ARM.extab.text.handler 0x0800000e 0xc build/unwind.o",
                " .text.handler 0x0800001a 0x20 build/handler.o"), "text");
        assertEquals(1, records.size());
        assertEquals(0x20, records.get(0).getSize());
    }

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
