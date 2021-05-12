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

import java.io.PrintStream;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.logging.Level;

/**
 * This is a small in-memory cyclic buffer to keep the log for debugging concurrency problems in order to
 * reconstruct what was going on immediately before tests crashes, without actually writing all debug logging
 * to the log file or console normally. It is used by some tests when assertions are enabled,
 * otherwise this class should not even be loaded.
 */
public class TraceLogging {
	private static final int STOPPED_INDEX = -1;
	private static final int SIZE = Integer.parseInt(System.getProperty("TraceLogging.size", "4096")); // must be power of 2
	private static final int MASK = SIZE - 1;

	private static final int THREAD_OFS = 0;
	private static final int NAME_OFS = 1;
	private static final int LEVEL_OFS = 2;
	private static final int MSG_OFS = 3;
	private static final int THROWABLE_OFS = 4;
	private static final int DATA_CNT = 5;

	private static final long[] timeQueue = new long[SIZE];
	private static final Object[] dataQueue = new Object[SIZE * DATA_CNT];
	private static final AtomicInteger index = new AtomicInteger(STOPPED_INDEX);
	private static int lastIndex = STOPPED_INDEX;

	static {
		if ((SIZE & MASK) != 0)
			throw new RuntimeException("Size must be a power of two");
	}

	/**
	 * Restarts trace logging from scratch (old log entries are cleared).
	 * Use it at the beginning of the test.
	 */
	public static synchronized void restart() {
		Arrays.fill(dataQueue, null);
		lastIndex = STOPPED_INDEX;
		index.compareAndSet(STOPPED_INDEX, 0);
	}

	/**
	 * Stops trace logging.
	 */
	public static void stop() {
		stopIndex(-1);
	}

	/**
	 * Adds log entry. It is invoked from {@link Logging#log(Level, String, Throwable)} method when
	 * assertions are enabled.
	 */
	public static void log(String loggerName, Level level, String msg, Throwable t) {
		append(nextIndex(), loggerName, level, msg, t);
	}

	/**
	 * Adds last entry and stops trace logging.
	 */
	public static void logAndStop(Class<?> where, String msg) {
		logAndStop(where, msg, null);
	}

	/**
	 * Adds last entry with exception and stops trace logging.
	 */
	public static void logAndStop(Class<?> where, String msg, Throwable t) {
		append(stopIndex(0), where.getName(), Level.INFO, msg, t);
	}

	private static void append(int i, String loggerName, Level level, String msg, Throwable t) {
		if (i < 0)
			return;
		timeQueue[i] = System.currentTimeMillis();
		dataQueue[i * DATA_CNT + THREAD_OFS] = Thread.currentThread();
		dataQueue[i * DATA_CNT + NAME_OFS] = loggerName;
		dataQueue[i * DATA_CNT + LEVEL_OFS] = level;
		dataQueue[i * DATA_CNT + MSG_OFS] = msg;
		dataQueue[i * DATA_CNT + THROWABLE_OFS] = t;
	}

	/**
	 * Dumps last entries from this trace log.
	 * It should be called after {@link #stop()} or {@link #logAndStop(Class, String)}.
	 * It does nothing if called more than once after stop or before stop.
	 */
	public static synchronized void dump(PrintStream out, String title) {
		int stop = lastIndex;
		if (stop < 0)
			return;
		lastIndex = STOPPED_INDEX;
		LogFormatter formatter = new LogFormatter();
		out.println("********************** Dump trace log for " + title);
		int i = stop;
		do {
			i = (i + 1) & MASK;
			Thread thread = (Thread)dataQueue[i * DATA_CNT + THREAD_OFS];
			if (thread == null)
				continue;
			String loggerName = (String)dataQueue[i * DATA_CNT + NAME_OFS];
			Level level = (Level)dataQueue[i * DATA_CNT + LEVEL_OFS];
			String msg = (String)dataQueue[i * DATA_CNT + MSG_OFS];
			Throwable t = (Throwable)dataQueue[i * DATA_CNT + THROWABLE_OFS];
			long time = timeQueue[i];
			out.print("* ");
			out.print(formatter.format(LogFormatter.getLevelChar(level), time, thread.getName(), loggerName, msg));
			if (t != null)
				t.printStackTrace(out);
		} while (i != stop);
		out.println("********************** Done trace log for " + title);
	}

	private static int nextIndex() {
		int result;
		do {
			result = index.get();
		} while (result >= 0 && !index.compareAndSet(result, (result + 1) & MASK));
		return result;
	}

	private static synchronized int stopIndex(int lastOffset) {
		int result;
		do {
			result = index.get();
		} while (result >= 0 && !index.compareAndSet(result, STOPPED_INDEX));
		if (result >= 0)
			lastIndex = (result + lastOffset) & MASK;
		return result;
	}

}
