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

import java.io.Serializable;
import java.util.Comparator;
import java.util.Objects;
import java.util.function.IntFunction;
import java.util.function.IntToDoubleFunction;
import java.util.function.IntToLongFunction;
import java.util.function.IntUnaryOperator;

/**
 * A comparison function, which imposes a <i>total ordering</i> on some collection of ints.
 * Comparators can be passed to a sort method (such as {@link QuickSort#sort(int[], IntComparator) QuickSort.sort})
 * to allow precise control over the sort order.
 *
 * <p>The purpose of this function is to allow non-trivial ordering of ints which depend on some external data.
 * For example when ints are some identifiers (pseudo-references) of actual data.
 */
@SuppressWarnings("UnusedDeclaration")
public interface IntComparator {
	/**
	 * Compares its two arguments for order. Returns a negative integer, zero, or a positive integer
	 * as the first argument is less than, equal to, or greater than the second.
	 *
	 * @param i1 the first int to be compared.
	 * @param i2 the second int to be compared.
	 * @return a negative integer, zero, or a positive integer as the first argument is
	 *         less than, equal to, or greater than the second.
	 */
	public int compare(int i1, int i2);

	/**
	 * Returns a comparator that imposes the reverse ordering of this comparator.
	 *
	 * @return a comparator that imposes the reverse ordering of this comparator.
	 */
	public default IntComparator reversed() {
		return (IntComparator & Serializable) (i1, i2) -> compare(i2, i1);
	}

	/**
	 * Returns a lexicographic-order comparator with another comparator.
	 * If this comparator considers two elements equal, i.e. {@code compare(i1, i2) == 0},
	 * then other comparator is used to determine the order.
	 *
	 * <p>The returned comparator is serializable if the specified comparator is also serializable.
	 *
	 * @param other the other comparator to be used when this comparator compares two ints that are equal.
	 * @return a lexicographic-order comparator composed of this comparator and then the other comparator.
	 * @throws NullPointerException if the argument is null.
	 */
	public default IntComparator thenComparing(IntComparator other) {
		Objects.requireNonNull(other);
		return (IntComparator & Serializable) (i1, i2) -> {
			int res = compare(i1, i2);
			return res != 0 ? res : other.compare(i1, i2);
		};
	}

	/**
	 * Returns a lexicographic-order comparator with a function that extracts
	 * a sort key to be compared with the given sort key comparator.
	 *
	 * <p>This default implementation delegates to
	 * <code>{@link #thenComparing(IntComparator) thenComparing}({@link #comparing(IntFunction, Comparator) comparing}(keyExtractor,&nbsp;keyComparator))</code> expression.
	 *
	 * @param <K> the type of the sort key.
	 * @param keyExtractor the function used to extract the sort key.
	 * @param keyComparator the comparator used to compare the sort key.
	 * @return a lexicographic-order comparator composed of this comparator
	 *         and then comparing an extracted sort key using the specified sort key comparator.
	 * @throws NullPointerException if either argument is null.
	 */
	public default <K> IntComparator thenComparing(IntFunction<? extends K> keyExtractor, Comparator<? super K> keyComparator) {
		return thenComparing(comparing(keyExtractor, keyComparator));
	}

	/**
	 * Returns a lexicographic-order comparator with a function that extracts a comparable sort key.
	 *
	 * <p>This default implementation delegates to
	 * <code>{@link #thenComparing(IntComparator) thenComparing}({@link #comparing(IntFunction) comparing}(keyExtractor))</code> expression.
	 *
	 * @param <K> the type of the comparable sort key.
	 * @param keyExtractor the function used to extract the comparable sort key.
	 * @return a lexicographic-order comparator composed of this comparator
	 *         and then comparing an extracted comparable sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public default <K extends Comparable<? super K>> IntComparator thenComparing(IntFunction<? extends K> keyExtractor) {
		return thenComparing(comparing(keyExtractor));
	}

	/**
	 * Returns a lexicographic-order comparator with a function that extracts an int sort key.
	 *
	 * <p>This default implementation delegates to
	 * <code>{@link #thenComparing(IntComparator) thenComparing}({@link #comparingInt(IntUnaryOperator) comparing}(keyExtractor))</code> expression.
	 *
	 * @param keyExtractor the function used to extract the int sort key.
	 * @return a lexicographic-order comparator composed of this comparator
	 *         and then comparing an extracted int sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public default IntComparator thenComparingInt(IntUnaryOperator keyExtractor) {
		return thenComparing(comparingInt(keyExtractor));
	}

	/**
	 * Returns a lexicographic-order comparator with a function that extracts a long sort key.
	 *
	 * <p>This default implementation delegates to
	 * <code>{@link #thenComparing(IntComparator) thenComparing}({@link #comparingLong(IntToLongFunction) comparing}(keyExtractor))</code> expression.
	 *
	 * @param keyExtractor the function used to extract the long sort key.
	 * @return a lexicographic-order comparator composed of this comparator
	 *         and then comparing an extracted long sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public default IntComparator thenComparingLong(IntToLongFunction keyExtractor) {
		return thenComparing(comparingLong(keyExtractor));
	}

	/**
	 * Returns a lexicographic-order comparator with a function that extracts a double sort key.
	 *
	 * <p>This default implementation delegates to
	 * <code>{@link #thenComparing(IntComparator) thenComparing}({@link #comparingDouble(IntToDoubleFunction) comparing}(keyExtractor))</code> expression.
	 *
	 * @param keyExtractor the function used to extract the double sort key.
	 * @return a lexicographic-order comparator composed of this comparator
	 *         and then comparing and extracted double sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public default IntComparator thenComparingDouble(IntToDoubleFunction keyExtractor) {
		return thenComparing(comparingDouble(keyExtractor));
	}

	/**
	 * Accepts a function that extracts a sort key and a sort key comparator, and returns
	 * a comparator that compares by an extracted sort key using the specified sort key comparator.
	 *
	 * <p>The returned comparator is serializable if the specified function and comparator are both serializable.
	 *
	 * @param <K> the type of the sort key.
	 * @param keyExtractor the function used to extract the sort key.
	 * @param keyComparator the comparator used to compare the sort key.
	 * @return a comparator that compares by an extracted sort key using the specified sort key comparator.
	 * @throws NullPointerException if either argument is null.
	 */
	public static <K> IntComparator comparing(IntFunction<? extends K> keyExtractor, Comparator<? super K> keyComparator) {
		Objects.requireNonNull(keyExtractor);
		Objects.requireNonNull(keyComparator);
		return (IntComparator & Serializable)
			(i1, i2) -> keyComparator.compare(keyExtractor.apply(i1), keyExtractor.apply(i2));
	}

	/**
	 * Accepts a function that extracts a comparable sort, and returns
	 * a comparator that compares by an extracted comparable sort key.
	 *
	 * <p>The returned comparator is serializable if the specified function is also serializable.
	 *
	 * @param <K> the type of the comparable sort key.
	 * @param keyExtractor the function used to extract the comparable sort key.
	 * @return a comparator that compares by an extracted comparable sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public static <K extends Comparable<? super K>> IntComparator comparing(IntFunction<? extends K> keyExtractor) {
		Objects.requireNonNull(keyExtractor);
		return (IntComparator & Serializable)
			(i1, i2) -> keyExtractor.apply(i1).compareTo(keyExtractor.apply(i2));
	}

	/**
	 * Accepts a function that extracts an int sort key, and returns
	 * a comparator that compares by an extracted int sort key.
	 *
	 * <p>The returned comparator is serializable if the specified function is also serializable.
	 *
	 * @param keyExtractor the function used to extract the int sort key.
	 * @return a comparator that compares by an extracted int sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public static IntComparator comparingInt(IntUnaryOperator keyExtractor) {
		Objects.requireNonNull(keyExtractor);
		return (IntComparator & Serializable)
			(i1, i2) -> Integer.compare(keyExtractor.applyAsInt(i1), keyExtractor.applyAsInt(i2));
	}

	/**
	 * Accepts a function that extracts a long sort key, and returns
	 * a comparator that compares by an extracted long sort key.
	 *
	 * <p>The returned comparator is serializable if the specified function is also serializable.
	 *
	 * @param keyExtractor the function used to extract the long sort key.
	 * @return a comparator that compares by an extracted long sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public static IntComparator comparingLong(IntToLongFunction keyExtractor) {
		Objects.requireNonNull(keyExtractor);
		return (IntComparator & Serializable)
			(i1, i2) -> Long.compare(keyExtractor.applyAsLong(i1), keyExtractor.applyAsLong(i2));
	}

	/**
	 * Accepts a function that extracts a double sort key, and returns
	 * a comparator that compares by an extracted double sort key.
	 *
	 * <p>The returned comparator is serializable if the specified function is also serializable.
	 *
	 * @param keyExtractor the function used to extract the double sort key.
	 * @return a comparator that compares by an extracted double sort key.
	 * @throws NullPointerException if the argument is null.
	 */
	public static IntComparator comparingDouble(IntToDoubleFunction keyExtractor) {
		Objects.requireNonNull(keyExtractor);
		return (IntComparator & Serializable)
			(i1, i2) -> Double.compare(keyExtractor.applyAsDouble(i1), keyExtractor.applyAsDouble(i2));
	}
}
