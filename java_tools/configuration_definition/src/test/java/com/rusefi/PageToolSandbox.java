package com.rusefi;

import java.io.IOException;

public class PageToolSandbox {

    public static void main(String[] args) throws IOException {
        RootHolder.ROOT = "../firmware/";

        doRun("integration/config_page_1.txt");
        doRun("integration/config_page_2.txt");
    }

    private static void doRun(String definitionInputFile) throws IOException {
        PlainConfigHandler plainConfigHandler = new PlainConfigHandler(definitionInputFile);

        plainConfigHandler.doJob();
        System.out.println(plainConfigHandler.tsProjectConsumer.getContent());
    }
}
