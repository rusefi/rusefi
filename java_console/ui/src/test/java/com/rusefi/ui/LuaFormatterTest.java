package com.rusefi.ui;

import neoe.formatter.lua.LuaFormatter;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class LuaFormatterTest {
    @Test
    public void test() throws Exception {
        String code = "function onCanRx(bus, id, dlc, data)\n" +
                "  --print('got CAN id=' .. id .. ' dlc='  .. dlc)\n" +
                "  id11=id%2048\n" +
                "\n" +
                "if id11 == 0x500 then --Check can state of BCM\n" +
                "    canState = data[1]\n" +
                "if canState == 01 then\n" +
                "    packet502[1] = 0x01\n" +
                "else\n" +
                "    packet502[1] = 0x00\n" +
                "end\n" +
                "if id11 == 0x570 then\n" +
                "    curState = data[1]\n" +
                "\n" +
                "if curState == 06 then -- Cranking TODO: MUST ONLY DO THIS ON RPM TILL STARt\n" +
                "packet542[2] =  0x82\n" +
                "end\n" +
                "\n" +
                "if curState == 04 then -- Kill off\n" +
                "packet542[2] =  0x82\n" +
                "end\n" +
                "\n" +
                "if curState == 01 then -- Kill\n" +
                "packet542[2] =  0xA2\n" +
                "end\n" +
                "end\n" +
                "end\n" +
                "\t\n" +
                "\n" +
                "\n" +
                "end";

        String formatted = new LuaFormatter().format(code, new LuaFormatter.Env());
        assertEquals("function onCanRx(bus, id, dlc, data)\n" +
                "\t-- print('got CAN id=' .. id .. ' dlc='  .. dlc)\n" +
                "\tid11 = id % 2048\n" +
                "\n" +
                "\tif id11 == 0x500 then\n" +
                "\t\t-- Check can state of BCM\n" +
                "\t\tcanState = data[1]\n" +
                "\t\tif canState == 01 then\n" +
                "\t\t\tpacket502[1] = 0x01\n" +
                "\t\telse\n" +
                "\t\t\tpacket502[1] = 0x00\n" +
                "\t\tend\n" +
                "\t\tif id11 == 0x570 then\n" +
                "\t\t\tcurState = data[1]\n" +
                "\n" +
                "\t\t\tif curState == 06 then\n" +
                "\t\t\t\t-- Cranking TODO: MUST ONLY DO THIS ON RPM TILL STARt\n" +
                "\t\t\t\tpacket542[2] = 0x82\n" +
                "\t\t\tend\n" +
                "\n" +
                "\t\t\tif curState == 04 then\n" +
                "\t\t\t\t-- Kill off\n" +
                "\t\t\t\tpacket542[2] = 0x82\n" +
                "\t\t\tend\n" +
                "\n" +
                "\t\t\tif curState == 01 then\n" +
                "\t\t\t\t-- Kill\n" +
                "\t\t\t\tpacket542[2] = 0xA2\n" +
                "\t\t\tend\n" +
                "\t\tend\n" +
                "\tend\n" +
                "\n" +
                "\n" +
                "\n" +
                "end\n" +
                "\n", formatted);
    }
}
