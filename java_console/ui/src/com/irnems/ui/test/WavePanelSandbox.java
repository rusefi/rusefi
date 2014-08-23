package com.irnems.ui.test;

import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.WavePanel;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

/**
 * 7/25/13
 * (c) Andrey Belomutskiy
 */
public class WavePanelSandbox extends FrameHelper {

    public WavePanelSandbox() {


        WavePanel wp = WavePanel.getInstance();

        wp.displayChart("Injector 1!down!55013038!crank!up!55013444!crank!down!55013781!Injector 3!up!55013789!Injector 3!down!55013969!crank2!up!55014065!crank!up!55014358!crank!down!55014706!input1 A8!up!55014730!Injector 4!up!55014734!Injector 4!down!55014933!input1 A8!down!55014941!crank!up!55015298!crank!down!55015638!Injector 2!up!55015644!Injector 2!down!55015823!crank2!down!55015932!crank!up!55016223!crank!down!55016566!Injector 1!up!55016574!Injector 1!down!55016753!crank!up!55017148!crank!down!55017494!Injector 3!up!55017499!Injector 3!down!55017679!crank2!up!55017777!crank!up!55018070!crank!down!55018406!Injector 4!input1 A8!up!55018414!up!55018416!input1 A8!down!55018439!input1 A8!up!55018439!Injector 4!down!55018593!input1 A8!down!55018600!crank!up!55018989!crank!down!55019330!Injector 2!up!55019338!Injector 2!down!55019517!crank2!down!55019618!crank!up!55019909!crank!down!55020246!Injector 1!up!55020251!Injector 1!down!55020430!crank!up!55020827!crank!down!55021171!Injector 3!up!55021179!Injector 3!down!55021358!crank2!up!55021453!crank!up!55021747!crank!down!55022084!Injector 4input1 A8!up!55022089!!up!55022093!Injector 4!down!55022270!input1 A8!down!55022276!input1 A8!up!55022276!input1 A8!down!55022276!crank!up!55022666!crank!down!55023006!Injector 2!up!55023011!Injector 2!down!55023191!crank2!down!55023294!crank!up!55023584!crank!down!55023928!Injector 1!up!55023932!Injector 1!down!55024109!crank!up!55024507!crank!down!55024846!Injector 3!up!55024871!Injector 3!down!55025072!crank2!up!55025138!crank!up!55025440!crank!down!55025780!Injector 4input1 A8!up!55025785!!up!55025791!input1 A8!down!55025923!input1 A8!up!55025923!Injector 4!down!55025970!input1 A8!down!55025976!input1 A8!up!55025976!crank!up!55026355!crank!down!55026690!Injector 2!up!55026696!Injector 2!down!55026874!crank2!down!55026979!crank!up!55027268!crank!down!55027611!Injector 1!up!55027619!Injector 1!down!55027800!crank!up!55028186!crank!down!55028520!,");

        showFrame(wp.getPanel());
    }

    public static void main(String[] args) throws InvocationTargetException, InterruptedException {
        SwingUtilities.invokeAndWait(new Runnable() {
            @Override
            public void run() {
                new WavePanelSandbox();
            }
        });
    }
}
