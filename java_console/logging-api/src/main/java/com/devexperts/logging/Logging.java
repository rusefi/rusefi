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

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.logging.Level;

/**
 * Main logging class.
 * It supports use of both log4j and {@link java.util.logging} logging facilities.
 * <p>First it tries to use log4j logging. If this attempt fails, it uses {@link java.util.logging} logging,
 * so you'll always have some logging running.
 * <p>Usage pattern:
 * <br><code>public class SomeClass {
 * <br>private static final Logging log = Logging.getLogging(SomeClass.class);
 * <br>}
 * </code>
 *
 * @see Log4jLogging
 * @see DefaultLogging
 * @see LogFormatter
 */
public class Logging {
	private static final boolean TRACE_LOGGING = Logging.class.desiredAssertionStatus();

	private static final int FINEST_INT = Level.FINEST.intValue();
	private static final int FINE_INT = Level.FINE.intValue();

	public static final String LOG_CLASS_NAME = "log.className";
	public static final String LOG_FILE_PROPERTY = "log.file";
	public static final String ERR_FILE_PROPERTY = "err.file";
	public static final String LOG_MAX_FILE_SIZE_PROPERTY = "log.maxFileSize";
	public static final String ERR_MAX_FILE_SIZE_PROPERTY = "err.maxFileSize";
	public static final String DEFAULT_MAX_FILE_SIZE = "900M";

	private static final ConcurrentMap<String, Logging> INSTANCES = new ConcurrentHashMap<>();
	private static final DefaultLogging IMPL = configure(DefaultLogging.getProperty(LOG_CLASS_NAME, "com.devexperts.logging.Log4j2Logging"));

	public static Logging getLogging(Class<?> clazz) {
		return getLogging(clazz.getName());
	}

	public static Logging getLogging(String name) {
		Logging logging = INSTANCES.get(name);
		if (logging != null)
			return logging;
		INSTANCES.putIfAbsent(name, new Logging(name));
		return INSTANCES.get(name);
	}

	/**
	 * Programmatically reconfigures logging to a specified file. This method
	 * overrides the value of {@link #LOG_FILE_PROPERTY} system property.
	 */
	public static void configureLogFile(String log_file) {
		reportErrors(IMPL, IMPL.configureLogFileAndConsole(log_file));
	}

	// ========== Instance =========

	private final Object peer;

	/**
	 * This constructor is designed for abstract framework classes like BeanBase or
	 * DAOBase that extend Logging to decorate messages by
	 * overriding {@link #decorateLogMessage(String)} method.
	 */
	protected Logging() {
		peer = IMPL.getPeer(getClass().getName());
	}

	protected Logging(String name) {
		peer = IMPL.getPeer(name);
	}

	/**
	 * Returns category name of this logging.
	 */
	public final String getName() {
		return IMPL.getName(peer);
	}

	/**
	 * Changes default {@link #debugEnabled()} behaviour for this logging instance.
	 * Use this method to turn off debugging information for classes that do not
	 * need to print their debugging information in production environment.
	 */
	public final void configureDebugEnabled(boolean defaultDebugEnabled) {
		IMPL.setDebugEnabled(peer, Boolean.parseBoolean(DefaultLogging.getProperty(getName() + ".debug",
			String.valueOf(defaultDebugEnabled))));
	}

	public final boolean debugEnabled() {
		return IMPL.debugEnabled(peer);
	}

	public final void trace(String message) {
		log(Level.FINEST, message, null);
	}

	public final void debug(String message) {
		log(Level.FINE, message, null);
	}

	public final void debug(String message, Throwable t) {
		log(Level.FINE, message, t);
	}

	public final void info(String message) {
		log(Level.INFO, message, null);
	}

	public final void info(String message, Throwable t) {
		log(Level.INFO, message, t);
	}

	public final void warn(String message) {
		log(Level.WARNING, message, null);
	}

	public final void warn(String message, Throwable t) {
		log(Level.WARNING, message, t);
	}

	public final void error(String message) {
		log(Level.SEVERE, message, null);
	}

	public final void error(String message, Throwable t) {
		log(Level.SEVERE, message, t);
	}

	public final RuntimeException log(RuntimeException e) {
		log(Level.SEVERE, e.getMessage(), e);
		return e;
	}

	/**
	 * Decorates log message (reformatting, auditing, etc).
	 * This method is invoked one time for each logging event.
	 */
	protected String decorateLogMessage(String msg) {
		return msg;
	}

	// ========== Internal ==========

	private void log(Level level, String msg, Throwable t) {
		if (TRACE_LOGGING)
			TraceLogging.log(getName(), level, decorateLogMessage(msg), t);
		int levelInt = level.intValue();
		if (levelInt <= FINEST_INT)
			return; // trace never goes to regular log
		if (levelInt <= FINE_INT && !IMPL.debugEnabled(peer))
			return;
		try {
			msg = decorateLogMessage(msg == null ? "" : msg);
		} catch (Throwable tt) {
			IMPL.log(peer, Level.SEVERE, "Failed to decorate log message", tt);
		}
		IMPL.log(peer, level, msg, t);
	}

	/**
	 * At first tries to use logging from passed class name. If this attempt fails, tries to use log4j logging.
	 * If this attempt fails, it uses log4j2 logging. If this attempt fails, it uses {@link java.util.logging} logging.
	 *
	 * @return Logging implementation
	 */
	private static DefaultLogging configure(String className) {
		DefaultLogging impl = null;
		Map<String, Exception> errors = new LinkedHashMap<>();
		if (!className.isEmpty()) {
			try {
				impl = (DefaultLogging)Class.forName(className).newInstance();
				errors.putAll(impl.configure());
			} catch (Throwable t) {
				// failed to configure with passed class name
				impl = null;
				if (!(t instanceof LinkageError) && !(t.getCause() instanceof LinkageError)) {
//					errors.put(className + " link", new IllegalStateException(t));
				}
			}
		}
/*
		if (impl == null) {
			try {
				impl = (DefaultLogging)Class.forName("com.devexperts.logging.Log4jLogging").newInstance();
				errors.putAll(impl.configure());
			} catch (Throwable t) {
				// failed to configure log4j
				impl = null;
				// LinkageError means that log4j is not found at all, otherwise it was found but our config is wrong
				if (!(t instanceof LinkageError) && !(t.getCause() instanceof LinkageError)) {
					errors.put("log4j link", new IllegalStateException(t));
				}
			}
		}
 */
		if (impl == null) {
			try {
				impl = (DefaultLogging)Class.forName("com.devexperts.logging.Log4j2Logging").newInstance();
				errors.putAll(impl.configure());
			} catch (Throwable t) {
				// failed to configure log4j2
				impl = null;
				if (!(t instanceof LinkageError) && !(t.getCause() instanceof LinkageError)) {
					//errors.put("log4j2 link", new IllegalStateException(t));
				}
			}
		}
		if (impl == null) {
			impl = new DefaultLogging();
			errors.putAll(impl.configure());
		}

		reportErrors(impl, errors);
		return impl;
	}

	private static void reportErrors(DefaultLogging impl, Map<String, Exception> errors) {
		for (Map.Entry<String, Exception> entry : errors.entrySet())
			impl.log(impl.getPeer("config"), Level.SEVERE, entry.getKey(), entry.getValue());
	}
}
