package com.opensr5;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Andrey Belomutskiy, (c) 2013-2020
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

        @Override
        public boolean isTradeEnabled() {
            return false;
        }
    };
    String DIR = "logs/";
    String DATE_PATTERN = "yyyy-MM-dd_HH_mm_ss_SSS";
    String END_OF_TIMESTAND_TAG = "<EOT>: ";

    static String getDate() {
        return new SimpleDateFormat(DATE_PATTERN).format(new Date());
    }

    void trace(String msg);

    void info(String msg);

    void error(String msg);

    boolean isTradeEnabled();
}
