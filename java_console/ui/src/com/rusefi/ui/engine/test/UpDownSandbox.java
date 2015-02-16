package com.rusefi.ui.engine.test;

import com.rusefi.waves.WaveReport;
import com.rusefi.waves.test.WaveReportTest;
import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.engine.UpDownImage;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

/**
 * Date: 6/23/13
 * (c) Andrey Belomutskiy
 */
public class UpDownSandbox extends FrameHelper {

    public UpDownSandbox() {

        WaveReport wr = new WaveReport(WaveReportTest.report);

        showFrame(new UpDownImage(wr, "test"));

    }

    public static void main(String[] args) throws InvocationTargetException, InterruptedException {
        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                new UpDownSandbox();
            }
        });
    }
}
