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

import com.devexperts.util.TimeUtil;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Arrays;
import java.util.Calendar;
import java.util.TimeZone;
import java.util.function.BiConsumer;
import java.util.logging.Formatter;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import javax.annotation.concurrent.ThreadSafe;

/**
 * Thread-safe formatter for log messages.
 * It is used for formatting log4j, log4j2 and {@link java.util.logging} log messages.
 * Performs conversion of thread names according to patterns specified in configuration file.
 * <p>
 * If the system property {@code logformatter.properties} is specified, then it should contain
 * an URL to the configuration file. Otherwise, configuration is loaded from classpath, using
 * <i>/META-INF/logformatter.properties</i> file.
 * <p>
 * The format of the file is:
 * <ul>
 * <li>pattern=replacement
 * <li>"Pattern" uses regular expression syntax.
 * You can escape "=" in pattern with "\=" syntax.
 * <li>"Replacement" string can refer to capturing groups defined in pattern using usual
 * regular expression syntax "$n", where "n" stands for the number of the group.
 * <li>ISO 8859-1 encoding is used.
 * <li>Empty lines and lines starting with # or ! are ignored.
 * Lines containing wrong patterns are ignored.
 * </ul>
 * Configuration file is loaded during class loading.
 * Any errors which occur in this class are printed in {@code System.err}.
 * <p>
 * Sample configuration file can be found in <i>etc/logformatter.properties</i>.
 * <p>
 * This class is not intended to be used standalone.
 * It is a part of implementation of {@link com.devexperts.logging} package.
 *
 * @see DetailedLogLayout
 * @see DxFeedPatternLayout
 */
@ThreadSafe
public class LogFormatter extends Formatter {
	public static final String CONFIG_FILE_PROPERTY = "logformatter.properties";
	public static final String DEFAULT_CONFIG_FILE = "/META-INF/logformatter.properties";

	private static final String LINE_SEP = DefaultLogging.getProperty("line.separator", "\n");
	private static final BiConsumer<Object, StringBuilder> STRING_FORMAT_CONSUMER = (s, sb) -> sb.append(s);

	// ============== Instance ================
	private final ThreadLocal<LocalFormatter> formatter;

	public LogFormatter() {
		this(TimeZone.getDefault());
	}

	public LogFormatter(TimeZone zone) {
		formatter = ThreadLocal.withInitial(() -> new LocalFormatter(zone));
	}

	/**
	 * Used by {@link java.util.logging} logging.
	 * Formats messages with the same format as for log4j.
	 */
	@Override
	public String format(LogRecord record) {
		String s = format(getLevelChar(record.getLevel()),
			record.getMillis(), Thread.currentThread().getName(),
			record.getLoggerName(), formatMessage(record));
		if (record.getThrown() != null) {
			StringWriter sw = new StringWriter();
			sw.write(s);
			record.getThrown().printStackTrace(new PrintWriter(sw));
			s = sw.toString();
		}
		return s;
	}

	/**
	 * Formats log message.
	 *
	 * @return Formatted message.
	 * @throws NullPointerException when threadName, loggerName, or msg are {@code null}.
	 */
	public String format(char levelChar, long time, String threadName, String loggerName, String msg) {
		StringBuilder out = formatter.get().appendTo;
		out.setLength(0);
		try {
			format(levelChar, time, threadName, loggerName, STRING_FORMAT_CONSUMER, msg, out);
			return out.toString();
		} finally {
			boolean trim = out.length() > 1000;
			out.setLength(0);
			if (trim)
				out.trimToSize();
		}
	}

	void format(char levelChar, long time, String threadName, String loggerName,
		BiConsumer<Object, StringBuilder> msgConsumer, Object msg, StringBuilder out)
	{
		out.append(levelChar).append(" ");
		formatter.get().appendTime(time, out);
		out.append(" ");
		int threadPosition = out.length();
		out.append("[");
		out.append(ThreadNameFormatter.formatThreadName(time, threadName));
		out.append("] ");
		out.append(loggerName, loggerName.lastIndexOf('.') + 1, loggerName.length());
		out.append(" - ");
		int messagePosition = out.length();
		msgConsumer.accept(msg, out);
		out.append(LINE_SEP);
		if (out.length() > messagePosition && out.charAt(messagePosition) == '\b')
			out.delete(threadPosition, messagePosition + 1);
	}

	static char getLevelChar(Level level) {
		int levelInt = level.intValue();
		if (levelInt <= Level.FINEST.intValue())
			return 'T';
		if (levelInt <= Level.FINE.intValue())
			return 'D';
		if (levelInt <= Level.INFO.intValue())
			return 'I';
		if (levelInt <= Level.WARNING.intValue())
			return 'W';
		return 'E';
	}

	private static class LocalFormatter {
		private final Calendar calendar;
		private final char[] timeBuffer = new char[17]; // fixed-size buffer for time data "yyMMdd HHmmss.SSS"
		private final StringBuilder appendTo = new StringBuilder();

		private long translatedMinute;

		private LocalFormatter(TimeZone zone) {
			calendar = Calendar.getInstance(zone);
			Arrays.fill(timeBuffer, 0, 17, ' ');
			timeBuffer[13] = '.';
		}

		private void appendTime(long time, StringBuilder out) {
			if (time < translatedMinute || time >= translatedMinute + TimeUtil.MINUTE) {
				// set year, month, day, hour and minute
				calendar.setTimeInMillis(time);
				translatedMinute = calendar.getTime().getTime() - calendar.get(Calendar.SECOND) * 1000 - calendar.get(Calendar.MILLISECOND);
				print2(0, calendar.get(Calendar.YEAR));
				print2(2, calendar.get(Calendar.MONTH) + 1);
				print2(4, calendar.get(Calendar.DAY_OF_MONTH));
				print2(7, calendar.get(Calendar.HOUR_OF_DAY));
				print2(9, calendar.get(Calendar.MINUTE));
			}

			// set seconds and milliseconds
			int millis = (int)(time - translatedMinute);
			print2(11, millis / 1000);
			print2(14, millis / 10);
			timeBuffer[16] = (char)('0' + millis % 10);
			out.append(timeBuffer);
		}

		private void print2(int offset, int value) {
			timeBuffer[offset] = (char)('0' + (value / 10) % 10);
			timeBuffer[offset + 1] = (char)('0' + value % 10);
		}
	}
}
