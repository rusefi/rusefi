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
package com.devexperts.util;

/**
 * A collection of static utility methods for manipulation of Java long time.
 * @see System#currentTimeMillis()
 */
public class TimeUtil {
	private TimeUtil() {} // do not create this class

	/**
	 * Number of milliseconds in a second.
	 */
	public static final long SECOND = 1000;

	/**
	 * Number of milliseconds in a minute.
	 */
	public static final long MINUTE = 60 * SECOND;

	/**
	 * Number of milliseconds in an hour.
	 */
	public static final long HOUR = 60 * MINUTE;

	/**
	 * Number of milliseconds in an day.
	 */
	public static final long DAY = 24 * HOUR;

	/**
	 * Returns correct number of seconds with proper handling negative values and overflows.
	 * Idea is that number of milliseconds shall be within [0..999] interval
	 * so that the following equation always holds
	 * {@code getSecondsFromTime(timeMillis) * 1000L + getMillisFromTime(timeMillis) == timeMillis}
	 * as as long the time in seconds fits into <b>int</b>.
	 * @see #getMillisFromTime(long)
	 */
	public static int getSecondsFromTime(long timeMillis) {
		return timeMillis >= 0 ? (int)Math.min(timeMillis / SECOND, Integer.MAX_VALUE) :
			(int)Math.max((timeMillis + 1) / SECOND - 1, Integer.MIN_VALUE);
	}

	/**
	 * Returns correct number of milliseconds with proper handling negative values.
	 * Idea is that number of milliseconds shall be within [0..999] interval
	 * so that the following equation always holds
	 * {@code getSecondsFromTime(timeMillis) * 1000L + getMillisFromTime(timeMillis) == timeMillis}
	 * as as long the time in seconds fits into <b>int</b>.
	 * @see #getSecondsFromTime(long)
	 */
	public static int getMillisFromTime(long timeMillis) {
		return (int)Math.floorMod(timeMillis, SECOND);
	}
}
