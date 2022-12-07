package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.FragmentDialogConsumer;
import org.junit.Test;

import java.io.IOException;

import static org.junit.Assert.assertEquals;

public class FragmentDialogConsumerTest {
    @Test
    public void generateFragmentDialog() {
        ReaderState state = new ReaderState();
        String outputChannels = "" +
                "\n" +
                "" +
                "struct_no_prefix output_channels_s\n" +
                "struct speed_density_s\n" +
                "\n" +
                "float Tcharge_coff\n" +
                "bit bit_of_sd\n" +
                "\n" +
                "! speed_density_s\n" +
                "end_struct\n" +
                "\n" +
                "speed_density_s sd;\n" +
                "bit sd_present\n" +
                "bit sd_present2\n" +
                "bit sd_present3\n" +
                "uint16_t autoscale RPMValue;;\"RPM\",1, 0, 0, 8000, 0\n" +
                "uint16_t rpmAcceleration;dRPM;\"RPM/s\",1, 0, 0, 5, 0\n" +
                "\tuint16_t autoscale speedToRpmRatio;;\"value\",{1/1}, 0, 0, 0, 0\n" +
                "    float luaTimingMult;\n" +
                "\tuint8_t vehicleSpeedKph\n" +
                "\tint8_t autoscale internalMcuTemperature;mcu;\"deg C\",1, 0, 0, 0, 0\n" +
                "end_struct\n";

        FragmentDialogConsumer fragmentDialogConsumer = new FragmentDialogConsumer("ac_state");

        state.readBufferedReader(outputChannels, fragmentDialogConsumer);

        assertEquals("indicatorPanel = ac_stateIndicatorPanel, 2\n" +
                        "\tindicator = {sd_bit_of_sd}, \"bit_of_sd No\", \"bit_of_sd Yes\"\n" +
                        "\tindicator = {sd_present}, \"sd_present No\", \"sd_present Yes\"\n" +
                        "\tindicator = {sd_present2}, \"sd_present2 No\", \"sd_present2 Yes\"\n" +
                        "\tindicator = {sd_present3}, \"sd_present3 No\", \"sd_present3 Yes\"\n" +
                        "\n" +
                        "dialog = ac_stateDialog, \"ac_state\"\n" +
                        "\tpanel = ac_stateIndicatorPanel\n" +
                        "\tliveGraph = ac_state_1_Graph, \"Graph\", South\n" +
                        "\t\tgraphLine = sd_Tcharge_coff\n" +
                        "\t\tgraphLine = RPMValue\n" +
                        "\t\tgraphLine = rpmAcceleration\n" +
                        "\t\tgraphLine = speedToRpmRatio\n" +
                        "\tliveGraph = ac_state_2_Graph, \"Graph\", South\n" +
                        "\t\tgraphLine = luaTimingMult\n" +
                        "\t\tgraphLine = vehicleSpeedKph\n" +
                        "\t\tgraphLine = internalMcuTemperature\n" +
                        "\n",
                fragmentDialogConsumer.getContent());
    }
}
