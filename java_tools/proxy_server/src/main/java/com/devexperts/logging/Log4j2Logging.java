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

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.Appender;
import org.apache.logging.log4j.core.Logger;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.appender.ConsoleAppender;
import org.apache.logging.log4j.core.appender.RollingFileAppender;
import org.apache.logging.log4j.core.appender.rolling.SizeBasedTriggeringPolicy;
import org.apache.logging.log4j.core.config.AppenderRef;
import org.apache.logging.log4j.core.config.Configuration;
import org.apache.logging.log4j.core.config.LoggerConfig;
import org.apache.logging.log4j.core.config.NullConfiguration;
import org.apache.logging.log4j.core.filter.ThresholdFilter;
import org.apache.logging.log4j.core.layout.AbstractStringLayout;
import org.apache.logging.log4j.message.SimpleMessage;
import org.apache.logging.log4j.status.StatusLogger;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.logging.Level;

import static org.apache.logging.log4j.Level.DEBUG;
import static org.apache.logging.log4j.Level.ERROR;
import static org.apache.logging.log4j.Level.INFO;
import static org.apache.logging.log4j.Level.OFF;
import static org.apache.logging.log4j.Level.WARN;
import static org.apache.logging.log4j.core.Filter.Result.ACCEPT;
import static org.apache.logging.log4j.core.Filter.Result.DENY;
import static org.apache.logging.log4j.core.config.ConfigurationSource.NULL_SOURCE;

/**
 * Logging implementation that uses log4j2 logging facilities.
 */
class Log4j2Logging extends DefaultLogging {
	private static final String FQCN = Logging.class.getName() + ".";

	static {
		StatusLogger.getLogger().setLevel(OFF);
	}

	@Override
	Map<String, Exception> configure() {
		LoggerContext ctx = (LoggerContext)LogManager.getContext(false);
		if (ctx.getConfiguration().getConfigurationSource() != NULL_SOURCE)
			return Collections.emptyMap(); // do nothing since log4j2 was already configured
		return configureLogFileAndConsole(getProperty(Logging.LOG_FILE_PROPERTY, null));
	}

	private static Map<String, Exception> reconfigure(String logFile) {
		LoggerContext ctx = (LoggerContext)LogManager.getContext(false);
		Configuration config = ctx.getConfiguration();

		Map<String, Exception> errors = new LinkedHashMap<>();
		config.getRootLogger().setLevel(DEBUG);
		String errFile = getProperty(Logging.ERR_FILE_PROPERTY, null);
		for (Map.Entry<String, Appender> entry : config.getRootLogger().getAppenders().entrySet()) {
			entry.getValue().stop();
			// Safe to delete here since config.getRootLogger().getAppenders() returns new map
			config.getRootLogger().removeAppender(entry.getKey());
		}

		Appender appender = null;
		if (logFile != null) {
			try {
				appender = createFileAppender("common-file", logFile, Logging.LOG_MAX_FILE_SIZE_PROPERTY, errors);
			} catch (Exception e) {
				errors.put(logFile, e);
			}
		}

		if (appender == null)
			appender = ConsoleAppender.newBuilder()
				.withName("common")
				.withLayout(getDetailedLayout())
				.setTarget(ConsoleAppender.Target.SYSTEM_OUT)
				.build();

		config.getRootLogger().addAppender(appender, DEBUG,
			errFile == null ? null : ThresholdFilter.createFilter(WARN, DENY, ACCEPT));

		if (errFile != null) {
			try {
				Appender errAppender = createFileAppender("error", errFile, Logging.ERR_MAX_FILE_SIZE_PROPERTY, errors);
				config.getRootLogger().addAppender(errAppender, WARN, ThresholdFilter.createFilter(WARN, ACCEPT, DENY));
			} catch (Exception e) {
				errors.put(errFile, e);
			}
		}
		ctx.updateLoggers();
		return errors;
	}

	private static AbstractStringLayout getDetailedLayout() {
		return DxFeedPatternLayout.createDefaultLayout(null);
	}

	private static RollingFileAppender createFileAppender(String name, String logFile, String maxSizeKey,
		Map<String, Exception> errors)
	{
		RollingFileAppender.Builder builder = RollingFileAppender.newBuilder();
		builder.setConfiguration(new NullConfiguration());
		builder.withName(name);
		builder.withLayout(getDetailedLayout());
		builder.withFileName(logFile);
		builder.withFilePattern(logFile);
		builder.withAppend(true);
		builder.withImmediateFlush(true);

		int limit = getLimit(maxSizeKey, errors);
		if (limit == 0)
			limit = 900 * 1024 * 1024; // Default in Logging.DEFAULT_MAX_FILE_SIZE
		builder.withPolicy(SizeBasedTriggeringPolicy.createPolicy(Integer.toString(limit)));

		return builder.build();
	}

	@Override
	Map<String, Exception> configureLogFileAndConsole(String logFile) {
		Map<String, Exception> result = reconfigure(logFile);

		LoggerContext ctx = (LoggerContext)LogManager.getContext(false);
		Configuration config = ctx.getConfiguration();

		Appender appender = ConsoleAppender.newBuilder()
				.withName("common-console")
				.withLayout(getDetailedLayout())
				.setTarget(ConsoleAppender.Target.SYSTEM_OUT)
				.build();
		appender.start();
		// broken stuff :( while this adds console appender this also kills file appender :( todo: find a way to have both
		config.addAppender(appender);
		AppenderRef[] refs = new AppenderRef[] { AppenderRef.createAppenderRef(appender.getName(), null, null) };
		LoggerConfig loggerConfig = LoggerConfig.createLogger("true", org.apache.logging.log4j.Level.ALL, "logger-console", "true", refs, null, config, null);
		loggerConfig.addAppender(appender, null, null);
		config.addLogger(LogManager.ROOT_LOGGER_NAME, loggerConfig);
		config.getRootLogger().addAppender(appender, DEBUG, null);
		ctx.updateLoggers();
		return result;
	}

	@Override
	Map<String, Exception> configureLogFile(String logFile) {
		return reconfigure(logFile);
	}

	@Override
	Object getPeer(String name) {
		return LogManager.getLogger(name);
	}

	@Override
	String getName(Object peer) {
		return ((Logger)peer).getName();
	}

	@Override
	boolean debugEnabled(Object peer) {
		return ((Logger)peer).isDebugEnabled();
	}

	@Override
	void setDebugEnabled(Object peer, boolean debugEnabled) {
		((Logger)peer).setLevel(debugEnabled ? DEBUG : INFO);
	}

	@Override
	void log(Object peer, Level level, String msg, Throwable t) {
		org.apache.logging.log4j.Level priority;
		if (level.intValue() <= Level.FINE.intValue())
			priority = DEBUG;
		else if (level.intValue() <= Level.INFO.intValue())
			priority = INFO;
		else if (level.intValue() <= Level.WARNING.intValue())
			priority = WARN;
		else
			priority = ERROR;

		if (!((Logger)peer).isEnabled(priority))
			return;

		// Before calling log4j logger we must clear "interrupted" flag from current thread.
		// If this flag is "true", log4j will log error in 1 appender only (and probably clear the flag).
		// We will re-establish "interrupted" flag later.
		boolean interrupted = Thread.interrupted();
		try {
			((Logger)peer).logMessage(FQCN, priority, null, new SimpleMessage(msg == null ? "" : msg), t);
		} catch (Exception e) {
			System.err.println(new LogFormatter().format('E', System.currentTimeMillis(),
				Thread.currentThread().getName(), "Log4j", e + " during logging of " + msg));
			if (!(e instanceof IllegalStateException) || e.getMessage() == null ||
				!e.getMessage().equals("Current state = FLUSHED, new state = CODING"))
			{
				e.printStackTrace(System.err);
			}
		} finally {
			if (interrupted)
				Thread.currentThread().interrupt();
		}
	}
}
