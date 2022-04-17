package com.rusefi.test;

import com.rusefi.ReaderState;
import com.rusefi.output.FragmentDialogConsumer;
import org.junit.Ignore;
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
                        "indicator = {sd_present}, \"No\", \"Yes\"\n" +
                        "indicator = {sd_present2}, \"No\", \"Yes\"\n" +
                        "indicator = {sd_present3}, \"No\", \"Yes\"\n",
                fragmentDialogConsumer.getContent());

    }
}
