package com.rusefi.tools.tune;

import com.rusefi.RootHolder;

import jakarta.xml.bind.JAXBException;
import java.io.IOException;

import static com.rusefi.tools.tune.TuneCanTool.writeDiffBetweenLocalTuneFileAndDefaultTune;

public class TuneCanToolRunner {
    public static void main(String[] args) throws JAXBException, IOException {
        int count = 8;
        if (args.length != count)
            throw new IllegalArgumentException(count + " arguments expected but " + args.length);
        TuneCanTool.boardPath = args[0];
        System.out.println("boardPath=" + TuneCanTool.boardPath);
        TuneContext.iniFileName = args[1];
        System.out.println("iniFileName=" + TuneContext.iniFileName);
        String name = args[2];
        String defaultTuneFileName = args[3];
        String customTuneFileName = args[4];
        RootHolder.ROOT = args[5];
        String ignoreListFileName = args[6];
        String whiteListFileName = args[7];
        System.out.println("ignoreListFileName=" + ignoreListFileName);
        System.out.println("whiteListFileName=" + whiteListFileName);

        if (!ignoreListFileName.isEmpty()) {
            TuneCanToolHelper.readIgnoreList(ignoreListFileName);
        } else {
            TuneCanToolHelper.readWhiteList(whiteListFileName);
        }

        TuneCanToolHelper.initialize(TuneContext.iniFileName);
        TuneContext.boardPrepend = TuneCanTool.boardPath + "prepend.txt";
        System.out.println("boardPrepend=" + TuneContext.boardPrepend);

        writeDiffBetweenLocalTuneFileAndDefaultTune(name,
            defaultTuneFileName,
            customTuneFileName, "", name);
    }
}

