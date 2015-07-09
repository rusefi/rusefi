package com.rusefi.ui.engine.test;

import com.rusefi.waves.EngineReport;
import com.rusefi.waves.test.EngineReportTest;
import com.rusefi.ui.util.FrameHelper;
import com.rusefi.ui.engine.UpDownImage;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

/**
 * Date: 6/23/13
 * (c) Andrey Belomutskiy
 */
public class UpDownSandbox extends FrameHelper {

    public UpDownSandbox() {

        EngineReport wr = new EngineReport(EngineReportTest.report);

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
