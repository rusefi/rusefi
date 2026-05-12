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
 * A comparison function, which imposes a <i>total ordering</i> on some collection of longs.
 * Comparators can be passed to a sort method
 * to allow precise control over the sort order.
 *
 * <p>The purpose of this function is to allow non-trivial ordering of longs which depend on some external data.
 * For example when longs are some identifiers (pseudo-references) of actual data.
 */
@SuppressWarnings("UnusedDeclaration")
public interface LongComparator {
	/**
	 * Compares its two arguments for order. Returns a negative integer, zero, or a positive integer
	 * as the first argument is less than, equal to, or greater than the second.
	 *
	 * @param i1 the first long to be compared.
	 * @param i2 the second long to be compared.
	 * @return a negative integer, zero, or a positive integer as the first argument is
	 *         less than, equal to, or greater than the second.
	 */
    int compare(long i1, long i2);
}
