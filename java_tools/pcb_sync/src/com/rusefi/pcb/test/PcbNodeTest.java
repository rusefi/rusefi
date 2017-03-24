package com.rusefi.pcb.test;

import com.rusefi.misc.RemoveUnneededTraces;
import com.rusefi.pcb.ModuleNode;
import com.rusefi.pcb.PcbNode;
import com.rusefi.pcb.SegmentNode;
import org.junit.Test;

import java.util.Arrays;
import java.util.Collections;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

/**
 * (c) Andrey Belomutskiy
 * 12/8/13
 */
public class PcbNodeTest {
    @Test
    public void testParser() {
        PcbNode n = PcbNode.parse("(version 3\r\n)");
        assertEquals("version", n.nodeName);
        assertEquals(1, n.getChildren().size());

        assertEquals("(version 3)\r\n", n.pack());


        n = PcbNode.parse("(host pcbnew \"(2013-07-07 BZR 4022)-stable\")");
        assertEquals(2, n.getChildren().size());
        assertEquals("pcbnew", n.getChildren().get(0));
        assertEquals("\"(2013-07-07 BZR 4022)-stable\"", n.getChildren().get(1));


        n = PcbNode.parse("(area 68.835001 116.9924 170.180001 180.5)");
        assertEquals(4, n.getChildren().size());

        n = PcbNode.parse("(kicad_pcb\n (version 3)\r\n (host pcbnew \"(2013-07-07 BZR 4022)-stable\")\n)");
        assertEquals(2, n.getChildren().size());
        assertEquals("(kicad_pcb\r\n" +
                " (version 3)\r\n" +
                "\r\n" +
                " (host pcbnew \"(2013-07-07 BZR 4022)-stable\")\r\n" +
                ")\r\n", n.pack());
    }

    @Test
    public void testParseModule() {
        ModuleNode module = (ModuleNode) PcbNode.parse("(module SM0805 (layer F.Cu) (tedit 52BF06CE) (tstamp 52A5B5B5)\n" +
                "    (at 205.359 170.688 180)\n" +
                "    (path /52A59B3A)\n" +
                "    (attr smd)\n" +
                "    (fp_text reference R351 (at -0.127 0 270) (layer F.SilkS)\n" +
                "      (effects (font (size 0.50038 0.50038) (thickness 0.1016)))\n" +
                "    )\n" +
                "    (fp_text value 1M (at 0 -1.143 180) (layer F.SilkS) hide\n" +
                "      (effects (font (size 0.50038 0.50038) (thickness 0.10922)))\n" +
                "    )\n" +
                "    (fp_circle (center -1.651 0.762) (end -1.651 0.635) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start -0.508 0.762) (end -1.524 0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start -1.524 0.762) (end -1.524 -0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start -1.524 -0.762) (end -0.508 -0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start 0.508 -0.762) (end 1.524 -0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start 1.524 -0.762) (end 1.524 0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start 1.524 0.762) (end 0.508 0.762) (layer F.SilkS) (width 0.09906))\n" +
                "    (pad 1 smd rect (at -0.9525 0 180) (size 0.889 1.397)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 18 N-000002)\n" +
                "    )\n" +
                "    (pad 2 smd rect (at 0.9525 0 180) (size 0.889 1.397)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 14 GND)\n" +
                "    )\n" +
                "    (model smd/chip_cms.wrl\n" +
                "      (at (xyz 0 0 0))\n" +
                "      (scale (xyz 0.1 0.1 0.1))\n" +
                "      (rotate (xyz 0 0 0))\n" +
                "    )\n" +
                "  )");
        assertEquals("R351", module.getReference());
    }


    @Test
    public void testConnected() {
        ModuleNode p353 = (ModuleNode) PcbNode.parse("(module PIN_ARRAY_10X2 (layer F.Cu) (tedit 52BF05FA) (tstamp 52B9B18D)\n" +
                "    (at 179.07 166.37 270)\n" +
                "    (path /52B479E1)\n" +
                "    (fp_text reference P353 (at 2.667 -3.048 360) (layer F.SilkS)\n" +
                "      (effects (font (size 0.50038 0.50038) (thickness 0.1016)))\n" +
                "    )\n" +
                "    (fp_text value CONN_10X2 (at 0.254 4.318 270) (layer F.SilkS) hide\n" +
                "      (effects (font (size 1.016 1.016) (thickness 0.2032)))\n" +
                "    )\n" +
                "    (fp_line (start 12.49934 2.49936) (end 12.49934 -2.49936) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start 12.49934 -2.49936) (end -12.7508 -2.49936) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start -12.7508 -2.49936) (end -12.7508 2.49936) (layer F.SilkS) (width 0.09906))\n" +
                "    (fp_line (start -12.7508 2.49936) (end 12.49934 2.49936) (layer F.SilkS) (width 0.09906))\n" +
                "    (pad 1 thru_hole rect (at -11.47064 1.27 270) (size 1.524 1.524) (drill 0.8128)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 3 /CAN_TX)\n" +
                "    )\n" +
                "    (pad 2 thru_hole circle (at -11.47064 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 5 /CS2)\n" +
                "    )\n" +
                "    (pad 3 thru_hole circle (at -8.93064 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 9 /SPI_MISO)\n" +
                "    )\n" +
                "    (pad 4 thru_hole circle (at -8.93064 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 10 /SPI_MOSI)\n" +
                "    )\n" +
                "    (pad 5 thru_hole circle (at -6.39064 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 4 /CS1)\n" +
                "    )\n" +
                "    (pad 6 thru_hole circle (at -6.39064 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 11 /SPI_SCK)\n" +
                "    )\n" +
                "    (pad 7 thru_hole circle (at -3.85064 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 6 /CS3)\n" +
                "    )\n" +
                "    (pad 8 thru_hole circle (at -3.85064 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 9 thru_hole circle (at -1.31064 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 7 /CS4)\n" +
                "    )\n" +
                "    (pad 10 thru_hole circle (at -1.31064 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 8 /CS_SD_MODULE)\n" +
                "    )\n" +
                "    (pad 11 thru_hole circle (at 1.22936 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 12 thru_hole circle (at 1.22936 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 13 thru_hole circle (at 3.76936 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 14 thru_hole circle (at 3.76936 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 15 thru_hole circle (at 6.30936 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 13 /USART_TX)\n" +
                "    )\n" +
                "    (pad 16 thru_hole circle (at 6.30936 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "      (net 12 /USART_RX)\n" +
                "    )\n" +
                "    (pad 17 thru_hole circle (at 8.84936 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 18 thru_hole circle (at 8.84936 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 19 thru_hole circle (at 11.38936 1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad 20 thru_hole circle (at 11.38936 -1.27 270) (size 1.524 1.524) (drill 1.016)\n" +
                "      (layers *.Cu *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (model lib/3d/M_header_10x2.wrl\n" +
                "      (at (xyz 0 0 0))\n" +
                "      (scale (xyz 1 1 1))\n" +
                "      (rotate (xyz -90 0 0))\n" +
                "    )\n" +
                "  )\n");

        SegmentNode segment = (SegmentNode) PcbNode.parse("(segment (start 221.32036 156.21) (end 222.631 154.89936) (width 0.254) (layer B.Cu) (net 5) (tstamp 52BE7B1B))");

        assertFalse(p353.isConnected(segment.end));
        assertFalse(p353.isConnected(segment.start));

    }


    @Test
    public void testSegmentConnected() {
        SegmentNode s1 = (SegmentNode) PcbNode.parse("(segment (start 219.075 162.941) (end 219.075 163.322) (width 0.254) (layer F.Cu) (net 7) (tstamp 52BC5D9B))");

        SegmentNode s0 = (SegmentNode) PcbNode.parse("(segment (start 216.662 161.544) (end 217.678 161.544) (width 0.254) (layer B.Cu) (net 7) (tstamp 52BC5D95))");
        SegmentNode s2 = (SegmentNode) PcbNode.parse("(segment (start 219.075 162.941) (end 219.075 163.322) (width 0.254) (layer F.Cu) (net 7) (tstamp 52BC5D9B))");

        assertFalse(RemoveUnneededTraces.isUnused(Arrays.asList(s0, s2), s1, Collections.<PcbNode>emptyList()));
    }


    @Test
    public void testSegmentConnected2() {
        SegmentNode s = (SegmentNode) PcbNode.parse("(segment (start 213.106 171.069) (end 213.106 166.3065) (width 0.254) (layer F.Cu) (net 22))");


        ModuleNode mn = (ModuleNode) PcbNode.parse("(module MINI-USB-5P-3400020P1 (layer F.Cu) (tedit 52C0C373) (tstamp 52BAC27E)\n" +
                "    (at 213.106 175.26)\n" +
                "    (descr OPL)\n" +
                "    (tags \"USB MINI 5 SMD-1\")\n" +
                "    (path /52C034E1)\n" +
                "    (attr smd)\n" +
                "    (fp_text reference J351 (at 0 -1.905) (layer F.SilkS)\n" +
                "      (effects (font (size 0.50038 0.50038) (thickness 0.1016)))\n" +
                "    )\n" +
                "    (fp_text value MINI-USB-5P-3400020P1 (at 0.762 -0.254) (layer F.SilkS) hide\n" +
                "      (effects (font (size 0.4318 0.4318) (thickness 0.0508)))\n" +
                "    )\n" +
                "    (fp_line (start 3.81 4.572) (end 3.81 3.103) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start -3.81 4.572) (end -3.81 3.103) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start 2.032 -4.572) (end 3.302 -4.572) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start 3.81 -2.297) (end 3.81 0.103) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start -3.81 4.572) (end 3.81 4.572) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start -3.81 -2.297) (end -3.81 0.103) (layer F.SilkS) (width 0.127))\n" +
                "    (fp_line (start -3.3 -4.572) (end -2.1 -4.572) (layer F.SilkS) (width 0.127))\n" +
                "    (pad 5 smd rect (at -1.6 -4.191) (size 0.508 2.54)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 14 GND)\n" +
                "    )\n" +
                "    (pad 4 smd rect (at -0.8 -4.191) (size 0.508 2.54)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "    )\n" +
                "    (pad 3 smd rect (at 0 -4.191) (size 0.508 2.54)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 22 N-0000056)\n" +
                "    )\n" +
                "    (pad 2 smd rect (at 0.8 -4.191) (size 0.508 2.54)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 19 N-000003)\n" +
                "    )\n" +
                "    (pad 1 smd rect (at 1.6 -4.191) (size 0.508 2.54)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 15 N-000001)\n" +
                "    )\n" +
                "    (pad 6 smd rect (at -4.5 1.603 90) (size 2.794 2)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 18 N-000002)\n" +
                "    )\n" +
                "    (pad 6 smd rect (at 4.5 1.603 90) (size 2.794 2)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 18 N-000002)\n" +
                "    )\n" +
                "    (pad 6 smd rect (at -4.5 -3.897 90) (size 2.794 2)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 18 N-000002)\n" +
                "    )\n" +
                "    (pad 6 smd rect (at 4.5 -3.897 90) (size 2.794 2)\n" +
                "      (layers F.Cu F.Paste F.Mask)\n" +
                "      (net 18 N-000002)\n" +
                "    )\n" +
                "    (pad \"\" np_thru_hole circle (at -2.159 -1.397 90) (size 1.016 1.016) (drill 0.762)\n" +
                "      (layers *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (pad \"\" np_thru_hole circle (at 2.159 -1.397 90) (size 1.016 1.016) (drill 0.762)\n" +
                "      (layers *.Mask F.SilkS)\n" +
                "    )\n" +
                "    (model lib/3d/usb-2.wrl\n" +
                "      (at (xyz 0 0 0))\n" +
                "      (scale (xyz 1 1 1))\n" +
                "      (rotate (xyz -90 0 -90))\n" +
                "    )\n" +
                "  )");

        assertTrue(mn.isConnected(s.start));
    }
}
