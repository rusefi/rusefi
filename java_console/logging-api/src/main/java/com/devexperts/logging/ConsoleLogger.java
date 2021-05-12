package com.devexperts.logging;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.logging.ConsoleHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ConsoleLogger extends DefaultLogging {
    @Override
    Map<String, Exception> configureLogFile(String log_file) {
        Logger root = Logger.getLogger("");
        Handler handler = new ConsoleHandler();
        handler.setFormatter(new LogFormatter());
        handler.setLevel(Level.ALL);
        root.addHandler(handler);

        return new LinkedHashMap<>();
    }
}
