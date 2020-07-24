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

import java.util.Comparator;
import java.util.List;

/**
 * This class implements modified version of <b>Quick Sort</b> algorithm.
 * This implementation offers <b>O(n*log(n))</b> performance on many data sets
 * that cause other quick sort algorithms to degrade to quadratic performance.
 *
 * <p>The notable differences of this <b>Quick Sort</b> from other sorting algorithms are:
 * <ul>
 * <li> it is <b>unstable</b> - it can re-arrange equal elements in any order;
 * <li> it is <b>robust</b> - it can withstand <b>unstable ordering</b> of elements,
 *      for example if ordering changes during sorting;
 * <li> it is <b>garbage-free</b> - it does not allocate any temporary objects.
 * </ul>
 *
 * <p>In the case of <b>unstable ordering</b> the result of this algorithm is not necessarily fully sorted,
 * but it is guaranteed to complete in a finite amount of time and without exceptions.
 * The result in this case would be partially sorted to the best of algorithm's ability.
 */
public class QuickSort {

	/**
	 * Sorts the specified list into ascending order according
	 * to the {@linkplain Comparable natural ordering} of its elements.
	 * All elements in the list must implement the {@link Comparable} interface.
	 * Furthermore, all elements in the list must be <i>mutually comparable</i>.
	 *
	 * @param <T> the class of the objects in the list.
	 * @param list the list to be sorted.
	 * @throws ClassCastException if the list contains elements that are not <i>mutually comparable</i>.
	 */
	public static <T extends Comparable<? super T>> void sort(List<T> list) {
		quickSort(list, 0, list.size() - 1, null);
	}

	/**
	 * Sorts the specified range of the specified list into ascending order according to the <i>natural ordering</i> of its elements.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in this range must implement the {@link Comparable} interface.
	 * Furthermore, all elements in this range must be <i>mutually comparable</i>.
	 *
	 * @param <T> the class of the objects in the list.
	 * @param list the list to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 * @throws ClassCastException if the list contains elements that are not <i>mutually comparable</i>.
	 */
	public static <T extends Comparable<? super T>> void sort(List<T> list, int fromIndex, int toIndex) {
		rangeCheck(list.size(), fromIndex, toIndex);
		quickSort(list, fromIndex, toIndex - 1, null);
	}

	/**
	 * Sorts the specified list according to the order induced by the specified comparator.
	 * All elements in the list must be <i>mutually comparable</i> using the specified comparator.
	 *
	 * @param <T> the class of the objects in the list.
	 * @param list the list to be sorted.
	 * @param c the comparator to determine the order of the list. A {@code null} value indicates
	 *          that the elements' <i>natural ordering</i> should be used.
	 * @throws ClassCastException if the list contains elements that are not <i>mutually comparable</i>
	 *         using the specified comparator.
	 */
	public static <T> void sort(List<T> list, Comparator<? super T> c) {
		quickSort(list, 0, list.size() - 1, c);
	}

	/**
	 * Sorts the specified range of the specified list according to the order induced by the specified comparator.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in the range must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param <T> the class of the objects in the list.
	 * @param list the list to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @param c the comparator to determine the order of the list. A {@code null} value indicates
	 *          that the elements' <i>natural ordering</i> should be used.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 * @throws ClassCastException if the list contains elements that are not <i>mutually comparable</i>
	 *         using the specified comparator.
	 */
	public static <T> void sort(List<T> list, int fromIndex, int toIndex, Comparator<? super T> c) {
		rangeCheck(list.size(), fromIndex, toIndex);
		quickSort(list, fromIndex, toIndex - 1, c);
	}

	/**
	 * Sorts the specified array of objects into ascending order according
	 * to the {@linkplain Comparable natural ordering} of its elements.
	 * All elements in the array must implement the {@link Comparable} interface.
	 * Furthermore, all elements in the array must be <i>mutually comparable</i>.
	 *
	 * @param a the array to be sorted.
	 * @throws ClassCastException if the array contains elements that are not <i>mutually comparable</i>.
	 */
	public static void sort(Object[] a) {
		quickSort(a, 0, a.length - 1, null);
	}

	/**
	 * Sorts the specified range of the specified array of objects into ascending order
	 * according to the {@linkplain Comparable natural ordering} of its elements.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in this range must implement the {@link Comparable} interface.
	 * Furthermore, all elements in this range must be <i>mutually comparable</i>.
	 *
	 * @param a the array to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 * @throws ClassCastException if the array contains elements that are not <i>mutually comparable</i>.
	 */
	public static void sort(Object[] a, int fromIndex, int toIndex) {
		rangeCheck(a.length, fromIndex, toIndex);
		quickSort(a, fromIndex, toIndex - 1, null);
	}

	/**
	 * Sorts the specified array of objects according to the order induced by the specified comparator.
	 * All elements in the array must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param <T> the class of the objects to be sorted.
	 * @param a the array to be sorted.
	 * @param c the comparator to determine the order of the array. A {@code null} value indicates
	 *          that the elements' {@linkplain Comparable natural ordering} should be used.
	 * @throws ClassCastException if the array contains elements that are not <i>mutually comparable</i>
	 *         using the specified comparator.
	 */
	public static <T> void sort(T[] a, Comparator<? super T> c) {
		quickSort(a, 0, a.length - 1, c);
	}

	/**
	 * Sorts the specified range of the specified array of objects according to the order induced by the specified comparator.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in the range must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param <T> the class of the objects to be sorted.
	 * @param a the array to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @param c the comparator to determine the order of the array. A {@code null} value indicates
	 *          that the elements' {@linkplain Comparable natural ordering} should be used.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 * @throws ClassCastException if the array contains elements that are not <i>mutually comparable</i>
	 *         using the specified comparator.
	 */
	public static <T> void sort(T[] a, int fromIndex, int toIndex, Comparator<? super T> c) {
		rangeCheck(a.length, fromIndex, toIndex);
		quickSort(a, fromIndex, toIndex - 1, c);
	}

	/**
	 * Sorts the specified array of ints according to the order induced by the specified comparator.
	 * All elements in the array must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param a the array to be sorted.
	 * @param c the comparator to determine the order of the array.
	 */
	public static void sort(int[] a, IntComparator c) {
		quickSort(a, 0, a.length - 1, c);
	}

	/**
	 * Sorts the specified range of the specified array of ints according to the order induced by the specified comparator.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in the range must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param a the array to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @param c the comparator to determine the order of the array.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 */
	public static void sort(int[] a, int fromIndex, int toIndex, IntComparator c) {
		rangeCheck(a.length, fromIndex, toIndex);
		quickSort(a, fromIndex, toIndex - 1, c);
	}

	/**
	 * Sorts the specified array of longs according to the order induced by the specified comparator.
	 * All elements in the array must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param a the array to be sorted.
	 * @param c the comparator to determine the order of the array.
	 */
	public static void sort(long[] a, LongComparator c) {
		quickSort(a, 0, a.length - 1, c);
	}

	/**
	 * Sorts the specified range of the specified array of longs according to the order induced by the specified comparator.
	 * The range to be sorted extends from index {@code fromIndex}, inclusive, to index {@code toIndex}, exclusive.
	 * All elements in the range must be <i>mutually comparable</i> by the specified comparator.
	 *
	 * @param a the array to be sorted.
	 * @param fromIndex the index of the first element (inclusive) to be sorted.
	 * @param toIndex the index of the last element (exclusive) to be sorted.
	 * @param c the comparator to determine the order of the array.
	 * @throws IllegalArgumentException if {@code fromIndex > toIndex}.
	 * @throws IndexOutOfBoundsException if {@code fromIndex < 0} or {@code toIndex > a.length}.
	 */
	public static void sort(long[] a, int fromIndex, int toIndex, LongComparator c) {
		rangeCheck(a.length, fromIndex, toIndex);
		quickSort(a, fromIndex, toIndex - 1, c);
	}

	// ========== Quick Sort of x[lo..hi] (inclusive) ==========

	private static final int BINARY_INSERT_LIST = 20;
	private static final int BINARY_INSERT_ARRAY = 40;
	private static final int MOM_START = 400;
	private static final int MOM_BASE = 15;
	static {
		//noinspection ConstantConditions,PointlessBooleanExpression
		if (BINARY_INSERT_LIST < 4 || BINARY_INSERT_ARRAY < 4 || MOM_START < 25 || MOM_BASE < 5)
			throw new AssertionError("invalid sort constants");
	}

	private static <T> void quickSort(List<T> x, int lo, int hi, Comparator<? super T> comparator) {
		// Quick sort large ranges in a loop to retain stack depth at log(n).
		while (hi - lo > BINARY_INSERT_LIST) {
			T pivot;
			int loScan;
			int hiScan;
			if (hi - lo > MOM_START) {
				// Range is large - perform median-of-medians search of good pivot.
				pivot = x.get(medianOfMedians(comparator, x, momStep(lo, hi), lo, hi));
				loScan = lo;
				hiScan = hi;
			} else {
				// Range is small - perform median-of-five search of good pivot.
				pivot = x.get(medianOfFive(comparator, x, lo, lo + 1, (lo + hi) >>> 1, hi - 1, hi));
				// Median-of-five rearranges elements around pivot - skip comparisons of 4 outer elements.
				loScan = lo + 2;
				hiScan = hi - 2;
			}
			// Excessive checks (loScan <= hiScan) protect from IndexOutOfBoundsException due to unstable ordering.
			while (loScan <= hiScan) {
				while (loScan <= hiScan && compare(x.get(loScan), pivot, comparator) < 0)
					loScan++;
				while (loScan <= hiScan && compare(x.get(hiScan), pivot, comparator) > 0)
					hiScan--;
				if (loScan > hiScan)
					break;
				T tmp = x.get(loScan);
				x.set(loScan, x.get(hiScan));
				x.set(hiScan, tmp);
				loScan++;
				hiScan--;
			}
			// Do recursion into smaller range, then do larger range ourselves.
			if (hiScan - lo < hi - loScan) {
				quickSort(x, lo, hiScan, comparator);
				// Protect from degenerate partition when (loScan == lo) due to unstable ordering.
				lo = Math.max(loScan, lo + 1);
			} else {
				quickSort(x, loScan, hi, comparator);
				// Protect from degenerate partition when (hiScan == hi) due to unstable ordering.
				hi = Math.min(hiScan, hi - 1);
			}
		}
		// Binary insertion sort the remaining small range.
		binaryInsertionSort(x, lo, hi, comparator);
	}

	private static <T> void quickSort(T[] x, int lo, int hi, Comparator<? super T> comparator) {
		while (hi - lo > BINARY_INSERT_ARRAY) {
			T pivot;
			int loScan;
			int hiScan;
			if (hi - lo > MOM_START) {
				pivot = x[medianOfMedians(comparator, x, momStep(lo, hi), lo, hi)];
				loScan = lo;
				hiScan = hi;
			} else {
				pivot = x[medianOfFive(comparator, x, lo, lo + 1, (lo + hi) >>> 1, hi - 1, hi)];
				loScan = lo + 2;
				hiScan = hi - 2;
			}
			while (loScan <= hiScan) {
				while (loScan <= hiScan && compare(x[loScan], pivot, comparator) < 0)
					loScan++;
				while (loScan <= hiScan && compare(x[hiScan], pivot, comparator) > 0)
					hiScan--;
				if (loScan > hiScan)
					break;
				T tmp = x[loScan];
				x[loScan] = x[hiScan];
				x[hiScan] = tmp;
				loScan++;
				hiScan--;
			}
			if (hiScan - lo < hi - loScan) {
				quickSort(x, lo, hiScan, comparator);
				lo = Math.max(loScan, lo + 1);
			} else {
				quickSort(x, loScan, hi, comparator);
				hi = Math.min(hiScan, hi - 1);
			}
		}
		binaryInsertionSort(x, lo, hi, comparator);
	}

	private static void quickSort(int[] x, int lo, int hi, IntComparator comparator) {
		while (hi - lo > BINARY_INSERT_ARRAY) {
			int pivot;
			int loScan;
			int hiScan;
			if (hi - lo > MOM_START) {
				pivot = x[medianOfMedians(comparator, x, momStep(lo, hi), lo, hi)];
				loScan = lo;
				hiScan = hi;
			} else {
				pivot = x[medianOfFive(comparator, x, lo, lo + 1, (lo + hi) >>> 1, hi - 1, hi)];
				loScan = lo + 2;
				hiScan = hi - 2;
			}
			while (loScan <= hiScan) {
				while (loScan <= hiScan && compare(x[loScan], pivot, comparator) < 0)
					loScan++;
				while (loScan <= hiScan && compare(x[hiScan], pivot, comparator) > 0)
					hiScan--;
				if (loScan > hiScan)
					break;
				int tmp = x[loScan];
				x[loScan] = x[hiScan];
				x[hiScan] = tmp;
				loScan++;
				hiScan--;
			}
			if (hiScan - lo < hi - loScan) {
				quickSort(x, lo, hiScan, comparator);
				lo = Math.max(loScan, lo + 1);
			} else {
				quickSort(x, loScan, hi, comparator);
				hi = Math.min(hiScan, hi - 1);
			}
		}
		binaryInsertionSort(x, lo, hi, comparator);
	}

	private static void quickSort(long[] x, int lo, int hi, LongComparator comparator) {
		while (hi - lo > BINARY_INSERT_ARRAY) {
			long pivot;
			int loScan;
			int hiScan;
			if (hi - lo > MOM_START) {
				pivot = x[medianOfMedians(comparator, x, momStep(lo, hi), lo, hi)];
				loScan = lo;
				hiScan = hi;
			} else {
				pivot = x[medianOfFive(comparator, x, lo, lo + 1, (lo + hi) >>> 1, hi - 1, hi)];
				loScan = lo + 2;
				hiScan = hi - 2;
			}
			while (loScan <= hiScan) {
				while (loScan <= hiScan && compare(x[loScan], pivot, comparator) < 0)
					loScan++;
				while (loScan <= hiScan && compare(x[hiScan], pivot, comparator) > 0)
					hiScan--;
				if (loScan > hiScan)
					break;
				long tmp = x[loScan];
				x[loScan] = x[hiScan];
				x[hiScan] = tmp;
				loScan++;
				hiScan--;
			}
			if (hiScan - lo < hi - loScan) {
				quickSort(x, lo, hiScan, comparator);
				lo = Math.max(loScan, lo + 1);
			} else {
				quickSort(x, loScan, hi, comparator);
				hi = Math.min(hiScan, hi - 1);
			}
		}
		binaryInsertionSort(x, lo, hi, comparator);
	}

	// ========== Binary Insertion Sort of x[lo..hi] (inclusive) ==========

	private static <T> void binaryInsertionSort(List<T> x, int lo, int hi, Comparator<? super T> comparator) {
		for (int i = lo; ++i <= hi;) {
			T pivot = x.get(i);
			int left = lo;
			for (int right = i; left < right;) {
				int mid = (left + right) >>> 1;
				if (compare(pivot, x.get(mid), comparator) < 0)
					right = mid;
				else
					left = mid + 1;
			}
			if (left < i) {
				for (int k = i; k > left; k--)
					x.set(k, x.get(k - 1));
				x.set(left, pivot);
			}
		}
	}

	private static <T> void binaryInsertionSort(T[] x, int lo, int hi, Comparator<? super T> comparator) {
		for (int i = lo; ++i <= hi;) {
			T pivot = x[i];
			int left = lo;
			for (int right = i; left < right;) {
				int mid = (left + right) >>> 1;
				if (compare(pivot, x[mid], comparator) < 0)
					right = mid;
				else
					left = mid + 1;
			}
			if (left < i) {
				System.arraycopy(x, left, x, left + 1, i - left);
				x[left] = pivot;
			}
		}
	}

	private static void binaryInsertionSort(int[] x, int lo, int hi, IntComparator comparator) {
		for (int i = lo; ++i <= hi;) {
			int pivot = x[i];
			int left = lo;
			for (int right = i; left < right;) {
				int mid = (left + right) >>> 1;
				if (compare(pivot, x[mid], comparator) < 0)
					right = mid;
				else
					left = mid + 1;
			}
			if (left < i) {
				System.arraycopy(x, left, x, left + 1, i - left);
				x[left] = pivot;
			}
		}
	}

	private static void binaryInsertionSort(long[] x, int lo, int hi, LongComparator comparator) {
		for (int i = lo; ++i <= hi;) {
			long pivot = x[i];
			int left = lo;
			for (int right = i; left < right;) {
				int mid = (left + right) >>> 1;
				if (compare(pivot, x[mid], comparator) < 0)
					right = mid;
				else
					left = mid + 1;
			}
			if (left < i) {
				System.arraycopy(x, left, x, left + 1, i - left);
				x[left] = pivot;
			}
		}
	}

	// ========== Median Of Medians ==========
	// Finds median of medians using quinary tree and median of five in each node.
	// Expected number of used elements is pow(5, 1 + ceil(log(1 + size / MOM_START, MOM_BASE))).
	// All used elements are spaced evenly (as much as possible) using "step" step.

	private static int momStep(int lo, int hi) {
		int mult = 5;
		for (int k = (int)((hi - lo + 1L) / MOM_START); k > 0; k /= MOM_BASE)
			mult *= 5;
		while (hi - lo < mult - 1 && mult > 5)
			mult /= 5;
		return (hi - lo) / (mult - 1);
	}

	private static <T> int medianOfMedians(Comparator<? super T> comparator, List<T> x, int step, int lo, int hi) {
		int ns = (hi - lo - step * 4) / 5;
		if (ns < step * 4)
			return medianOfFive(comparator, x, lo, lo + step, (lo + hi) >>> 1, hi - step, hi);
		int bs = ns + step;
		return medianOfFive(comparator, x,
			medianOfMedians(comparator, x, step, lo, lo + ns),
			medianOfMedians(comparator, x, step, lo + bs, lo + bs + ns),
			medianOfMedians(comparator, x, step, lo + bs + bs, hi - bs - bs),
			medianOfMedians(comparator, x, step, hi - bs - ns, hi - bs),
			medianOfMedians(comparator, x, step, hi - ns, hi)
		);
	}

	private static <T> int medianOfMedians(Comparator<? super T> comparator, T[] x, int step, int lo, int hi) {
		int ns = (hi - lo - step * 4) / 5;
		if (ns < step * 4)
			return medianOfFive(comparator, x, lo, lo + step, (lo + hi) >>> 1, hi - step, hi);
		int bs = ns + step;
		return medianOfFive(comparator, x,
			medianOfMedians(comparator, x, step, lo, lo + ns),
			medianOfMedians(comparator, x, step, lo + bs, lo + bs + ns),
			medianOfMedians(comparator, x, step, lo + bs + bs, hi - bs - bs),
			medianOfMedians(comparator, x, step, hi - bs - ns, hi - bs),
			medianOfMedians(comparator, x, step, hi - ns, hi)
		);
	}

	private static int medianOfMedians(IntComparator comparator, int[] x, int step, int lo, int hi) {
		int ns = (hi - lo - step * 4) / 5;
		if (ns < step * 4)
			return medianOfFive(comparator, x, lo, lo + step, (lo + hi) >>> 1, hi - step, hi);
		int bs = ns + step;
		return medianOfFive(comparator, x,
			medianOfMedians(comparator, x, step, lo, lo + ns),
			medianOfMedians(comparator, x, step, lo + bs, lo + bs + ns),
			medianOfMedians(comparator, x, step, lo + bs + bs, hi - bs - bs),
			medianOfMedians(comparator, x, step, hi - bs - ns, hi - bs),
			medianOfMedians(comparator, x, step, hi - ns, hi)
		);
	}

	private static int medianOfMedians(LongComparator comparator, long[] x, int step, int lo, int hi) {
		int ns = (hi - lo - step * 4) / 5;
		if (ns < step * 4)
			return medianOfFive(comparator, x, lo, lo + step, (lo + hi) >>> 1, hi - step, hi);
		int bs = ns + step;
		return medianOfFive(comparator, x,
			medianOfMedians(comparator, x, step, lo, lo + ns),
			medianOfMedians(comparator, x, step, lo + bs, lo + bs + ns),
			medianOfMedians(comparator, x, step, lo + bs + bs, hi - bs - bs),
			medianOfMedians(comparator, x, step, hi - bs - ns, hi - bs),
			medianOfMedians(comparator, x, step, hi - ns, hi)
		);
	}

	// ========== Median Of Five ==========
	// Finds median of 5 elements using 6 comparisons. See first method for algorithm explanation.
	// All methods do reorder their input around median, thus performing partial sorting.
	// This side effect is used by quick sort algorithms to skip comparisons of 4 outer elements.
	// This side effect is useless for median of medians algorithms, but by using same methods we save on bytecode.

	private static <T> int medianOfFive(Comparator<? super T> comparator, List<T> x, int ai, int bi, int ci, int di, int ei) {
		T a = x.get(ai);
		T b = x.get(bi);
		T c = x.get(ci);
		T d = x.get(di);
		T e = x.get(ei);
		T t;
		// (a, b, c, d, e) - sort (a, b)
		if (compare(a, b, comparator) > 0) { t = a; a = b; b = t; }
		// (a < b, c, d, e) - sort (d, e)
		if (compare(d, e, comparator) > 0) { t = d; d = e; e = t; }
		// (a < b, c, d < e) - sort pairs (a < b, d < e) by lowest of (a, d)
		if (compare(a, d, comparator) > 0) { t = a; a = d; d = t; t = b; b = e; e = t; }
		// (a < b, c, a < d < e) - now [a] < [b, d, e], put it aside
		// [a] < [b, d, e] (b, c, d < e) - sort (b, c)
		if (compare(b, c, comparator) > 0) { t = b; b = c; c = t; }
		// [a] < [c, d, e] (b < c, d < e) - sort pairs (b < c, d < e) by lowest of (b, d)
		if (compare(b, d, comparator) > 0) { t = b; b = d; d = t; t = c; c = e; e = t; }
		// [a] < [c, d, e] (b < c, b < d < e) - now [b] < [c, d, e], put it aside
		// [a, b] < [c, d, e] (c, d < e) - sort (c, d)
		if (compare(c, d, comparator) > 0) { t = c; c = d; d = t; }
		// [a, b] < [c, d, e] (c < d, c < e) - now [c] < [d, e], rewrite
		// [a, b] < [c] < [d, e] - [c] is a median
		x.set(ai, a);
		x.set(bi, b);
		x.set(ci, c);
		x.set(di, d);
		x.set(ei, e);
		return ci;
	}

	private static <T> int medianOfFive(Comparator<? super T> comparator, T[] x, int ai, int bi, int ci, int di, int ei) {
		T a = x[ai];
		T b = x[bi];
		T c = x[ci];
		T d = x[di];
		T e = x[ei];
		T t;
		if (compare(a, b, comparator) > 0) { t = a; a = b; b = t; }
		if (compare(d, e, comparator) > 0) { t = d; d = e; e = t; }
		if (compare(a, d, comparator) > 0) { t = a; a = d; d = t; t = b; b = e; e = t; }
		if (compare(b, c, comparator) > 0) { t = b; b = c; c = t; }
		if (compare(b, d, comparator) > 0) { t = b; b = d; d = t; t = c; c = e; e = t; }
		if (compare(c, d, comparator) > 0) { t = c; c = d; d = t; }
		x[ai] = a;
		x[bi] = b;
		x[ci] = c;
		x[di] = d;
		x[ei] = e;
		return ci;
	}

	private static int medianOfFive(IntComparator comparator, int[] x, int ai, int bi, int ci, int di, int ei) {
		int a = x[ai];
		int b = x[bi];
		int c = x[ci];
		int d = x[di];
		int e = x[ei];
		int t;
		if (compare(a, b, comparator) > 0) { t = a; a = b; b = t; }
		if (compare(d, e, comparator) > 0) { t = d; d = e; e = t; }
		if (compare(a, d, comparator) > 0) { t = a; a = d; d = t; t = b; b = e; e = t; }
		if (compare(b, c, comparator) > 0) { t = b; b = c; c = t; }
		if (compare(b, d, comparator) > 0) { t = b; b = d; d = t; t = c; c = e; e = t; }
		if (compare(c, d, comparator) > 0) { t = c; c = d; d = t; }
		x[ai] = a;
		x[bi] = b;
		x[ci] = c;
		x[di] = d;
		x[ei] = e;
		return ci;
	}

	private static int medianOfFive(LongComparator comparator, long[] x, int ai, int bi, int ci, int di, int ei) {
		long a = x[ai];
		long b = x[bi];
		long c = x[ci];
		long d = x[di];
		long e = x[ei];
		long t;
		if (compare(a, b, comparator) > 0) { t = a; a = b; b = t; }
		if (compare(d, e, comparator) > 0) { t = d; d = e; e = t; }
		if (compare(a, d, comparator) > 0) { t = a; a = d; d = t; t = b; b = e; e = t; }
		if (compare(b, c, comparator) > 0) { t = b; b = c; c = t; }
		if (compare(b, d, comparator) > 0) { t = b; b = d; d = t; t = c; c = e; e = t; }
		if (compare(c, d, comparator) > 0) { t = c; c = d; d = t; }
		x[ai] = a;
		x[bi] = b;
		x[ci] = c;
		x[di] = d;
		x[ei] = e;
		return ci;
	}

	// ========== Utility Code ==========

	/**
	 * Compares specified objects using either specified comparator or their natural ordering.
	 */
	@SuppressWarnings("unchecked")
	private static int compare(Object o1, Object o2, Comparator c) {
		// Boost performance and protect from degenerate partition due to unstable ordering.
		if (o1 == o2)
			return 0;
		return c != null ? c.compare(o1, o2) : ((Comparable)o1).compareTo(o2);
	}

	private static int compare(int i1, int i2, IntComparator c) {
		// Boost performance and protect from degenerate partition due to unstable ordering.
		if (i1 == i2)
			return 0;
		return c.compare(i1, i2);
	}

	private static int compare(long i1, long i2, LongComparator c) {
		// Boost performance and protect from degenerate partition due to unstable ordering.
		if (i1 == i2)
			return 0;
		return c.compare(i1, i2);
	}

	/**
	 * Checks that fromIndex and toIndex are in range and throws appropriate exception if they aren't.
	 */
	private static void rangeCheck(int length, int fromIndex, int toIndex) {
		if (fromIndex > toIndex)
			throw new IllegalArgumentException("fromIndex " + fromIndex + " > toIndex " + toIndex);
		if (fromIndex < 0)
			throw new IndexOutOfBoundsException("fromIndex " + fromIndex + " < 0");
		if (toIndex > length)
			throw new IndexOutOfBoundsException("toIndex " + toIndex + " > length " + length);
	}

	/**
	 * Private constructor to prevent instantiation.
	 */
	private QuickSort() {}
}
