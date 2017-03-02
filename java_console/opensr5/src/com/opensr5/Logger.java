package com.opensr5;

/**
 * (c) Andrey Belomutskiy
 * 3/7/2015
 */
public interface Logger {
    Logger CONSOLE = new Logger() {
        @Override
        public void trace(String msg) {
            System.out.println(msg);
        }

        @Override
        public void info(String msg) {
            System.out.println(msg);
        }

        @Override
        public void error(String msg) {
            System.err.println(msg);
        }
    };

    void trace(String msg);

    void info(String msg);

    void error(String msg);
}
