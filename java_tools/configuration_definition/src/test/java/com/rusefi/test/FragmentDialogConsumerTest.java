package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.FragmentDialogConsumer;
import org.junit.Test;

import java.io.IOException;

import static org.junit.Assert.assertEquals;

public class FragmentDialogConsumerTest {
    @Test
    public void generateFragmentDialog() throws IOException {
        ReaderState state = new ReaderState();
        String outputChannels = "" +
                "\n" +
                "" +
                "struct_no_prefix output_channels_s\n" +
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
                        "\tindicator = {sd_present}, \"No\", \"Yes\"\n" +
                        "\tindicator = {sd_present2}, \"No\", \"Yes\"\n" +
                        "\tindicator = {sd_present3}, \"No\", \"Yes\"\n" +
                        "\n" +
                        "liveGraph = ac_state_1_Graph, \"Graph\", South\n" +
                        "\tgraphLine = RPMValue\n" +
                        "\tgraphLine = rpmAcceleration\n" +
                        "\tgraphLine = speedToRpmRatio\n" +
                        "\tgraphLine = alignmentFill_at_10\n" +
                        "liveGraph = ac_state_2_Graph, \"Graph\", South\n" +
                        "\tgraphLine = luaTimingMult\n" +
                        "\tgraphLine = vehicleSpeedKph\n" +
                        "\tgraphLine = internalMcuTemperature\n" +
                        "\tgraphLine = alignmentFill_at_18\n" +
                        "\n" +
                        "dialog = ac_stateDialog, \"ac_state\"\n" +
                        "\tpanel = ac_stateIndicatorPanel\n" +
                        "\tpanel = ac_state_1_Graph\n" +
                        "\tpanel = ac_state_2_Graph\n" +
                        "\n",
                fragmentDialogConsumer.getContent());

    }
}
