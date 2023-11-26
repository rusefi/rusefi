package com.rusefi.ui.test;

import com.rusefi.config.generated.Fields;
import com.rusefi.core.preferences.storage.PersistentConfiguration;
import com.rusefi.ui.FrameHelper;
import com.rusefi.ui.engine.EngineSnifferPanel;
import com.rusefi.waves.EngineChartParser;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;

/**
 * 7/25/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class WavePanelSandbox {

    public WavePanelSandbox() {
        EngineSnifferPanel wp = new EngineSnifferPanel(null, PersistentConfiguration.getConfig().getRoot());

        String report = "t1!u_9504!31000!t2!d_9505!31096!t2!u_9506!31532!t2!d_9507!31951!t2!u_9508!32349!t2!d_9509!32809!t2!u_9510!33185!t2!d_9511!33683!t2!u_9512!34087!t1!d_9513!34091!t2!d_9514!36081!t2!u_9515!36401!t2!d_9516!36775!t2!u_9517!37086!t2!d_9518!37447!t2!u_9519!37749!t2!d_9520!38114!t2!u_9521!38419!t2!d_9522!38787!t2!u_9523!39085!t2!d_9524!39476!t2!u_9525!39836!t2!d_9526!40191!t2!u_9527!40517!t2!d_9528!40941!t2!u_9529!41289!t2!d_9530!41747!t2!u_9531!42152!t2!d_9532!42599!t2!u_9533!42956!t2!d_9534!43386!t2!u_9535!43750!t2!d_9536!44203!t2!u_9537!44572!t1!u_9538!44764!t2!d_9539!45040!t2!u_9540!45404!t2!d_9541!45875!t2!u_9542!46253!t2!d_9543!46746!t2!u_9544!47127!t2!d_9545!47602!t2!u_9546!47986!t1!d_9547!48321!t2!d_9548!48388!t2!u_9549!50288!t2!d_9550!50739!t1!u_9551!50785!t2!u_9552!51035!t2!d_9553!51418!t2!u_9554!51759!t2!d_9555!52090!t2!u_9556!52400!t2!d_9557!52769!t2!u_9558!53090!t2!d_9559!53464!t1!d_9560!53565!t2!u_9561!53773!t2!d_9562!54187!t2!u_9563!54529!t2!d_9564!54946!t2!u_9565!55284!t2!d_9566!55755!t1!u_9567!56041!t2!u_9568!56158!t2!d_9569!56599!t2!u_9570!56965!t2!d_9571!57377!t2!u_9572!57753!t2!d_9573!58201!t2!u_9574!58589!t2!d_9575!59025!t1!d_9576!59229!t2!u_9577!59388!t2!d_9578!59866!t2!u_9579!60244!t2!d_9580!60723!t2!u_9581!61085!t2!d_9582!61585!t2!u_9583!61980!t1!u_9584!62170!t2!d_9585!63992!t2!u_9586!64329!t2!d_9587!64694!t2!u_9588!64993!t2!d_9589!65376!t2!u_9590!65707!t2!d_9591!66055!t2!u_9592!66378!t2!d_9593!66745!t2!u_9594!67049!t2!d_9595!67452!t2!u_9596!67799!t2!d_9597!68190!t2!u_9598!68515!t2!d_9599!68967!t2!u_9600!69412!t2!d_9601!69803!t2!u_9602!70196!t2!d_9603!70629!t2!u_9604!70963!t2!d_9605!71415!t2!u_9606!71862!t2!d_9607!72252!t2!u_9608!72636!t2!d_9609!73068!t2!u_9610!73423!t1!d_9611!73453!t2!d_9612!73923!t2!u_9613!74303!t2!d_9614!74780!t2!u_9615!75160!t2!d_9616!75646!t2!u_9617!76090!t2!d_9618!76424!t1!u_9619!76624!t2!u_9620!78352!t2!d_9621!78744!t2!u_9622!79047!t1!d_9623!79265!t2!d_9624!79422!t2!u_9625!79752!t2!d_9626!80096!t2!u_9627!80393!t2!d_9628!80781!t2!u_9629!81089!t2!d_9630!81483!t1!u_9631!81634!t2!u_9632!81861!t2!d_9633!82216!t2!u_9634!82544!t2!d_9635!82988!t2!u_9636!83335!t2!d_9637!83814!t2!u_9638!84194!t1!d_9639!84534!t2!d_9640!84646!t2!u_9641!84990!t2!d_9642!85429!t2!u_9643!85801!t2!d_9644!86260!t2!u_9645!86621!t1!u_9646!87022!t2!d_9647!87080!t2!u_9648!87435!t2!d_9649!87935!t2!u_9650!88312!t2!d_9651!88789!t2!u_9652!89183!t2!d_9653!89663!t2!u_9654!90043!t1!d_9655!90125!";

        printAngles(report, 34092, 90125);


        wp.displayChart(report);

        new FrameHelper().showFrame(wp.getPanel());
    }

    private void printAngles(String value, int from, int to) {
        String[] array = value.split(EngineChartParser.DELI);

        int index = 0;
        while (index + 2 < array.length) {
            String a = array[index];
            String b = array[index + 1];
            int time = Integer.parseInt(array[index + 2]);
            index += 3;
            if (time < from || time > to)
                continue;

            boolean isUp = b.charAt(0) == 'u';

            double angle = 720.0 * (time - from) / (to - from);

            boolean isPrimary = Fields.PROTOCOL_CRANK1.equals(a);
//            if (!isPrimary)
//                continue;

            String trigger = isPrimary ? "T_PRIMARY" : "T_SECONDARY";

            System.out.println("s->addEvent(" + angle + ", " + trigger + ", " + (isUp ? "TV_HIGH" : "TV_LOW") + ");");


        }
    }

    public static void main(String[] args) throws InvocationTargetException, InterruptedException {
        SwingUtilities.invokeAndWait(WavePanelSandbox::new);
    }
}
