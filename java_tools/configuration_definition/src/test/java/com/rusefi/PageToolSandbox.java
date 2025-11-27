package com.rusefi;

import java.io.IOException;
import java.util.Collections;

public class PageToolSandbox {

    public static void main(String[] args) throws IOException {
        RootHolder.ROOT = "../firmware/";

        doRun("integration/config_page_1.txt", 1);
        doRun("integration/config_page_2.txt", 2);
        doRun("integration/config_page_3.txt", 3);
    }

    private static void doRun(String definitionInputFile, int pageIndex) throws IOException {
        PlainConfigHandler plainConfigHandler = new PlainConfigHandler(definitionInputFile, pageIndex, Collections.emptyList());

        plainConfigHandler.doJob();
        System.out.println(plainConfigHandler.tsProjectConsumer.getContent());
    }
}
