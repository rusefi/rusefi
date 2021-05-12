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

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.Arrays;
import java.util.Collection;
import java.util.concurrent.ConcurrentMap;
import java.util.stream.Collector;

/**
 * A synchronized thread-safe version of {@link IndexedSet} class.
 * It provides following benefits over standard {@link IndexedSet}:
 *
 * <ul>
 * <li> concurrent asynchronous read access
 * <li> synchronized thread-safe write access
 * <li> all iterators are concurrent
 * </ul>
 *
 * <p>Note that <b>SynchronizedIndexedSet</b> can be wrapped by {@link IndexedMap}
 * to create what can be considered a <b>SynchronizedIndexedMap</b>.
 */
public class SynchronizedIndexedSet<K, V> extends IndexedSet<K, V> {
	private static final long serialVersionUID = 0;

	// ========== static factory methods ===========

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT}.
	 */
	public static <V> SynchronizedIndexedSet<V, V> create() {
		return new SynchronizedIndexedSet<>();
	}

	/**
	 * Creates new empty set with default identity indexer.
	 */
	public static <V> SynchronizedIndexedSet<V, V> createIdentity() {
		return new SynchronizedIndexedSet<>((IndexerFunction.IdentityKey<V, V>)(v -> v));
	}

	/**
	 * Creates new empty set with specified indexer.
	 */
	public static <K, V> SynchronizedIndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified identity indexer.
	 */
	public static <K, V> SynchronizedIndexedSet<K, V> createIdentity(IndexerFunction.IdentityKey<K, ? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified int indexer.
	 */
	public static <V> SynchronizedIndexedSet<Integer, V> createInt(IndexerFunction.IntKey<? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified long indexer.
	 */
	public static <V> SynchronizedIndexedSet<Long, V> createLong(IndexerFunction.LongKey<? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}
	 */
	@Deprecated
	public static <V> SynchronizedIndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #createLong(IndexerFunction.LongKey) createLong(indexer)}
	 */
	@Deprecated
	public static <V> SynchronizedIndexedSet<Long, V> create(IndexerFunction.LongKey<? super V> indexer) {
		return new SynchronizedIndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <K, V> SynchronizedIndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer, int initialCapacity) {
		return new SynchronizedIndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <V> SynchronizedIndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer, int initialCapacity) {
		return new SynchronizedIndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #createLong(IndexerFunction.LongKey) createLong(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <V> SynchronizedIndexedSet<Long, V> create(IndexerFunction.LongKey<? super V> indexer, int initialCapacity) {
		return new SynchronizedIndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public static <K, V> SynchronizedIndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer, Collection<? extends V> c) {
		return new SynchronizedIndexedSet<>(indexer, c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public static <V> SynchronizedIndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer, Collection<? extends V> c) {
		return new SynchronizedIndexedSet<>(indexer, c);
	}

	/**
	 * Creates a new set with default indexer containing specified elements.
	 */
	@SafeVarargs
	public static <V> SynchronizedIndexedSet<V, V> of(V... objs) {
		return new SynchronizedIndexedSet<>(Arrays.asList(objs));
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with default indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	@SuppressWarnings("unchecked")
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<V, V>> collector() {
		return collector((IndexerFunction<V, ? super V>)IndexerFunction.DEFAULT);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with default identity indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<V, V>> collectorIdentity() {
		return collector((IndexerFunction.IdentityKey<V, V>)(v -> v));
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <K, V> Collector<V, ?, ? extends SynchronizedIndexedSet<K, V>> collector(IndexerFunction<K, ? super V> indexer) {
		return Collector.of(() -> create(indexer), IndexedSet::add,
			(left, right) -> { left.addAll(right); return left; },
			Collector.Characteristics.CONCURRENT, Collector.Characteristics.UNORDERED, Collector.Characteristics.IDENTITY_FINISH);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified identity indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <K, V> Collector<V, ?, ? extends SynchronizedIndexedSet<K, V>> collectorIdentity(IndexerFunction.IdentityKey<K, ? super V> indexer) {
		return collector((IndexerFunction<K, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified int indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<Integer, V>> collectorInt(IndexerFunction.IntKey<? super V> indexer) {
		return collector((IndexerFunction<Integer, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified long indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<Long, V>> collectorLong(IndexerFunction.LongKey<? super V> indexer) {
		return collector((IndexerFunction<Long, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 *
	 * @deprecated Use {@link #collectorInt(IndexerFunction.IntKey) collectorInt(indexer)}
	 */
	@Deprecated
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<Integer, V>> collector(IndexerFunction.IntKey<? super V> indexer) {
		return collector((IndexerFunction<Integer, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code SynchronizedIndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#CONCURRENT concurrent} and {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 *
	 * @deprecated Use {@link #collectorLong(IndexerFunction.LongKey) collectorLong(indexer)}
	 */
	@Deprecated
	public static <V> Collector<V, ?, ? extends SynchronizedIndexedSet<Long, V>> collector(IndexerFunction.LongKey<? super V> indexer) {
		return collector((IndexerFunction<Long, ? super V>)indexer);
	}

	// ========== Construction and Sizing Operations ==========

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT}.
	 */
	public SynchronizedIndexedSet() {
		super();
	}

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT} and specified initial capacity.
	 */
	public SynchronizedIndexedSet(int initialCapacity) {
		super(initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer.
	 */
	protected SynchronizedIndexedSet(IndexerFunction<K, ? super V> indexer) {
		super(indexer);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}
	 */
	@Deprecated
	public SynchronizedIndexedSet(Indexer<K, ? super V> indexer) {
		super(indexer);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 */
	protected SynchronizedIndexedSet(IndexerFunction<K, ? super V> indexer, int initialCapacity) {
		super(indexer, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public SynchronizedIndexedSet(Indexer<K, ? super V> indexer, int initialCapacity) {
		super(indexer, initialCapacity);
	}

	/**
	 * Creates a new set containing the elements in the specified collection.
	 * If specified collection is an {@link IndexedSet}, then new indexed set uses same indexer,
	 * otherwise it uses default indexer {@link IndexerFunction#DEFAULT}.
	 */
	public SynchronizedIndexedSet(Collection<V> c) {
		super(c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 */
	protected SynchronizedIndexedSet(IndexerFunction<K, ? super V> indexer, Collection<? extends V> c) {
		super(indexer, c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public SynchronizedIndexedSet(Indexer<K, ? super V> indexer, Collection<? extends V> c) {
		super(indexer, c);
	}

	/**
	 * Returns a shallow copy of this set - the values themselves are not cloned.
	 */
	@Override
	public synchronized SynchronizedIndexedSet<K, V> clone() {
		return (SynchronizedIndexedSet<K, V>)super.clone();
	}

	/**
	 * Increases the capacity of this set instance, if necessary, to ensure that it
	 * can hold at least the number of elements specified by the capacity argument.
	 * <p>
	 * Returns <b>this</b> set instance for convenience.
	 */
	@Override
	public synchronized SynchronizedIndexedSet<K, V> withCapacity(int capacity) {
		return (SynchronizedIndexedSet<K, V>)super.withCapacity(capacity);
	}

	/**
	 * Adds all of the elements in the specified collection into this set.
	 * <p>
	 * Returns <b>this</b> set instance for convenience.
	 */
	@Override
	public synchronized SynchronizedIndexedSet<K, V> withElements(Collection<? extends V> c) {
		return (SynchronizedIndexedSet<K, V>)super.withElements(c);
	}

	/**
	 * Increases the capacity of this set instance, if necessary, to ensure that it
	 * can hold at least the number of elements specified by the capacity argument.
	 */
	@Override
	public synchronized void ensureCapacity(int capacity) {
		super.ensureCapacity(capacity);
	}

	/**
	 * Trims the capacity of this set instance to be the set's current size.
	 * An application can use this operation to minimize the storage of this set instance.
	 */
	@Override
	public synchronized void trimToSize() {
		super.trimToSize();
	}

	/**
	 * Removes all elements from this set.
	 */
	@Override
	public synchronized void clear() {
		super.clear();
	}

	// ========== Query Operations ==========

	/**
	 * Returns static structure statistics of this set.
	 */
	@Override
	public synchronized IndexedSetStats getStats() {
		// This method is synchronized to provide consistent view of several cross-linked variables.
		// It should not pose any contention risk anyway.
		return super.getStats();
	}

	// ========== Modification Operations ==========

	/**
	 * Puts specified element into this set and returns previous element that matches specified one.
	 */
	@Override
	public synchronized V put(V value) {
		return super.put(value);
	}

	/**
	 * Puts specified element into this set if it is absent and
	 * returns current element in the set that matches specified one.
	 * This is equivalent to
	 * <pre>
	 *   if (set.containsValue(value)) {
	 *     return set.getByValue(value);
	 *   } else {
	 *     set.put(value);
	 *     return value;
	 *   }
	 * </pre>
	 * except that the action is performed atomically if it is properly synchronized.
	 * <p>
	 * Note, that unlike {@link ConcurrentMap#putIfAbsent},
	 * this method returns specified value (not <b>null</b>) if the value was absent.
	 */
	@Override
	public synchronized V putIfAbsentAndGet(V value) {
		return super.putIfAbsentAndGet(value);
	}

	/**
	 * Removes the element from this set which matches specified value if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	@Override
	public synchronized V removeValue(V value) {
		return super.removeValue(value);
	}

	/**
	 * Removes the element from this set which matches specified key if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	@Override
	public synchronized V removeKey(K key) {
		return super.removeKey(key);
	}

	/**
	 * Removes the element from this set which matches specified key if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	@Override
	public synchronized V removeKey(long key) {
		return super.removeKey(key);
	}

	// ========== Internal Implementation - Helper Instance Methods ==========

	@Override
	void checkModification(Object checkCore, long checkModCount) {
		// Do nothing - all iterators are concurrent.
	}

	@Override
	synchronized void removeIterated(Object checkCore, long checkModCount, boolean concurrent, V lastValue, int lastIndex) {
		super.removeIterated(checkCore, checkModCount, true, lastValue, lastIndex);
	}

	@Override
	synchronized void writeCore(ObjectOutputStream out) throws IOException {
		// This method is synchronized to provide consistent serialization.
		// It should not pose any contention risk anyway.
		super.writeCore(out);
	}
}
