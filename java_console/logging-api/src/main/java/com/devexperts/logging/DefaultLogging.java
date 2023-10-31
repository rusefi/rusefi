/*
 * !++
 * QDS - Quick Data Signalling Library
 * !-
 * Copyright (C) 2002 - 2020 Devexperts LLC
 * !-
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 * !__
 */
package com.devexperts.logging;

import java.io.IOException;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.logging.ConsoleHandler;
import java.util.logging.FileHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

/**
 * Logging implementation that uses {@link java.util.logging} logging facilities.
 */
class DefaultLogging {

	Map<String, Exception> configure() {
		// Heuristically check if there was an attempt to manually configure logging
		if (getProperty("java.util.logging.config.class", null) != null ||
			getProperty("java.util.logging.config.file", null) != null ||
			!hasDefaultHandlers(Logger.getLogger("")))
		{
			return Collections.emptyMap(); // logging was already manually configured
		}
		return configureLogFile(getProperty(Logging.LOG_FILE_PROPERTY, null));
	}

	private boolean hasDefaultHandlers(Logger root) {
		// Default configuration is 1 ConsoleHandler with SimpleFormatter and INFO level
		Handler[] handlers = root.getHandlers();
		if (handlers.length != 1)
			return false;
		Handler handler = handlers[0];
		return handler.getClass() == ConsoleHandler.class &&
			handler.getFormatter().getClass() == SimpleFormatter.class &&
			handler.getLevel() == Level.INFO;
	}

	Map<String, Exception> configureLogFileAndConsole(String log_file) {
		Map<String, Exception> result = configureLogFile(log_file);
		initAndAdd(new ConsoleHandler(), Level.ALL, getRootLogger());
		return result;
	}

	Map<String, Exception> configureLogFile(String log_file) {
		Logger root = getRootLogger();
		Map<String, Exception> errors = new LinkedHashMap<>();

		try {
			// Don't reset configuration. Retain all manually configured loggers, but
			// reconfigure the root logger, which (as we checked) has a default configuration with
			// 1 ConsoleHandler with SimpleFormatter and INFO level
			for (Handler handler : root.getHandlers())
				root.removeHandler(handler);

			// configure "log" file or console
			Handler handler = null;
			if (log_file != null) {
				try {
					handler = new FileHandler(log_file, getLimit(Logging.LOG_MAX_FILE_SIZE_PROPERTY, errors), 2, true);
				} catch (IOException e) {
					errors.put(log_file, e);
				}
			}
			if (handler == null)
				handler = new ConsoleHandler();
			initAndAdd(handler, Level.ALL, root);

			// configure "err" file
			String err_file = getProperty(Logging.ERR_FILE_PROPERTY, null);
			if (err_file != null) {
				try {
					handler = new FileHandler(err_file, getLimit(Logging.ERR_MAX_FILE_SIZE_PROPERTY, errors), 2, true);
					initAndAdd(handler, Level.WARNING, root);
				} catch (IOException e) {
					errors.put(err_file, e);
				}
			}
		} catch (SecurityException e) {
			// ignore -- does not have persmission to change configuration
		}
		return errors;
	}

	private void initAndAdd(Handler handler, Level all, Logger root) {
		handler.setFormatter(new LogFormatter());
		handler.setLevel(all);
		root.addHandler(handler);
	}

	private Logger getRootLogger() {
		return Logger.getLogger("");
	}

	Object getPeer(String name) {
		return Logger.getLogger(name);
	}

	String getName(Object peer) {
		return ((Logger)peer).getName();
	}

	boolean debugEnabled(Object peer) {
		return ((Logger)peer).isLoggable(Level.FINE);
	}

	void setDebugEnabled(Object peer, boolean debug_enabled) {
		((Logger)peer).setLevel(debug_enabled ? Level.ALL : Level.INFO);
	}

	void log(Object peer, Level level, String msg, Throwable t) {
		((Logger)peer).log(level, msg, t);
	}

	// ========== Utility methods ==========

	/**
	 * Safely, from security point of view, gets system property.
	 */
	static String getProperty(String key, String def) {
		// For applets we need to be ready for security exception in getProperty() call
		try {
			return System.getProperty(key, def);
		} catch (SecurityException e) {
			return def;
		}
	}

	static int getLimit(String key, Map<String, Exception> errors) {
		String value = getProperty(key, Logging.DEFAULT_MAX_FILE_SIZE).trim();
		int multiplier = 1;
		if (value.endsWith("K") || value.endsWith("k")) {
			multiplier = 1024;
			value = value.substring(0, value.length() - 1);
		} else if (value.endsWith("M") || value.endsWith("m")) {
			multiplier = 1024 * 1024;
			value = value.substring(0, value.length() - 1);
		} else if (value.endsWith("G") || value.endsWith("g")) {
			multiplier = 1024 * 1024 * 1024;
			value = value.substring(0, value.length() - 1);
		}
		try {
			return Integer.parseInt(value) * multiplier;
		} catch (NumberFormatException e) {
			errors.put(key, e);
			return 0;
		}
	}
}
