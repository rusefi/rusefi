package com.opensr5;

import java.util.Date;

/**
 * (c) Andrey Belomutskiy
 * 3/7/2015
 */
public interface Logger {
    Logger CONSOLE = new Logger() {
        @Override
        public void trace(String msg) {
            System.out.println(new Date() + " " + msg);
        }

        @Override
        public void info(String msg) {
            System.out.println(new Date() + " " + msg);
        }

        @Override
        public void error(String msg) {
            System.err.println(new Date() + " " + msg);
        }
    };

    void trace(String msg);

    void info(String msg);

    void error(String msg);
}
