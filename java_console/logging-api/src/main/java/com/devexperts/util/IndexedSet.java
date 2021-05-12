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
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Set;
import java.util.concurrent.ConcurrentMap;
import java.util.stream.Collector;
import javax.annotation.Nonnull;

/**
 * A collection that contains no duplicate elements according to specified identification strategy.
 * The <b>IndexedSet</b> class implements and obeys general contracts of {@link Set Set} interface
 * and provides additional benefits over standard implementations:
 *
 * <ul>
 * <li> delegation of element identification to external strategy
 * <li> concurrent asynchronous read access
 * <li> smaller memory footprint and faster performance
 * </ul>
 *
 * <p>The <b>IndexedSet</b> assumes that identity of an element can be represented by a variable number
 * of attributes, therefore it delegates identification to an external strategy &mdash; the {@link IndexerFunction}.
 * In order to fulfil contracts of {@link Map Map} interface and for convenience, the <b>IndexedSet</b>
 * supports concept of <b>explicit key</b> object and also numeric key representation, but these
 * identification means are optional and need not be supported by all strategies.
 *
 * <p><b>Note that the IndexedSet is not synchronized!</b> Concurrent modifications of <b>IndexedSet</b>
 * from multiple threads must be synchronized externally to preserve data integrity.
 * On the other side, the <b>IndexedSet</b> fully supports concurrent asynchronous read operations,
 * which works during concurrent modification by other thread. In case of concurrent modification
 * each atomic read sees <b>IndexedSet</b> either before or after each atomic write operation.
 *
 * <p>The <b>IndexedSet</b> does not support <b>null</b> values, but it supports <b>null</b> keys
 * if they are supported by corresponding {@link IndexerFunction}. The <b>IndexedSet</b> is serializable.
 */
public class IndexedSet<K, V> extends AbstractConcurrentSet<V> implements Cloneable, Serializable {
	private static final long serialVersionUID = 0L;

	// 'private' fields and methods of IndexedSet class shall be accessed only from within IndexedSet class itself.

	private final IndexerFunction<K, ? super V> indexer;
	private transient volatile Core<K, V> core;

	// ========== static factory methods ===========

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT}.
	 */
	public static <V> IndexedSet<V, V> create() {
		return new IndexedSet<>();
	}

	/**
	 * Creates new empty set with default identity indexer.
	 */
	public static <V> IndexedSet<V, V> createIdentity() {
		return new IndexedSet<>((IndexerFunction.IdentityKey<V, V>)(v -> v));
	}

	/**
	 * Creates new empty set with specified indexer.
	 */
	public static <K, V> IndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified identity indexer.
	 */
	public static <K, V> IndexedSet<K, V> createIdentity(IndexerFunction.IdentityKey<K, ? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified int indexer.
	 */
	public static <V> IndexedSet<Integer, V> createInt(IndexerFunction.IntKey<? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified long indexer.
	 */
	public static <V> IndexedSet<Long, V> createLong(IndexerFunction.LongKey<? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}
	 */
	@Deprecated
	public static <V> IndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #createLong(IndexerFunction.LongKey) createLong(indexer)}
	 */
	@Deprecated
	public static <V> IndexedSet<Long, V> create(IndexerFunction.LongKey<? super V> indexer) {
		return new IndexedSet<>(indexer);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <K, V> IndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer, int initialCapacity) {
		return new IndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <V> IndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer, int initialCapacity) {
		return new IndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #createLong(IndexerFunction.LongKey) createLong(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public static <V> IndexedSet<Long, V> create(IndexerFunction.LongKey<? super V> indexer, int initialCapacity) {
		return new IndexedSet<>(indexer, initialCapacity);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public static <K, V> IndexedSet<K, V> create(IndexerFunction<K, ? super V> indexer, Collection<? extends V> c) {
		return new IndexedSet<>(indexer, c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #createInt(IndexerFunction.IntKey) createInt(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public static <V> IndexedSet<Integer, V> create(IndexerFunction.IntKey<? super V> indexer, Collection<? extends V> c) {
		return new IndexedSet<>(indexer, c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #createLong(IndexerFunction.LongKey) createLong(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public static <V> IndexedSet<Long, V> create(IndexerFunction.LongKey<? super V> indexer, Collection<? extends V> c) {
		return new IndexedSet<>(indexer, c);
	}

	/**
	 * Creates a new set with default indexer containing specified elements.
	 */
	@SafeVarargs
	public static <V> IndexedSet<V, V> of(V... objs) {
		return new IndexedSet<>(Arrays.asList(objs));
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with default indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	@SuppressWarnings("unchecked")
	public static <V> Collector<V, ?, ? extends IndexedSet<V, V>> collector() {
		return collector((IndexerFunction<V, ? super V>)IndexerFunction.DEFAULT);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with default identity indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends IndexedSet<V, V>> collectorIdentity() {
		return collector((IndexerFunction.IdentityKey<V, V>)(v -> v));
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <K, V> Collector<V, ?, ? extends IndexedSet<K, V>> collector(IndexerFunction<K, ? super V> indexer) {
		return Collector.of(() -> create(indexer), IndexedSet::add,
			(left, right) -> { left.addAll(right); return left; },
			Collector.Characteristics.UNORDERED, Collector.Characteristics.IDENTITY_FINISH);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified identity indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <K, V> Collector<V, ?, ? extends IndexedSet<K, V>> collectorIdentity(IndexerFunction.IdentityKey<K, ? super V> indexer) {
		return collector((IndexerFunction<K, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified int indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends IndexedSet<Integer, V>> collectorInt(IndexerFunction.IntKey<? super V> indexer) {
		return collector((IndexerFunction<Integer, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified long indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 */
	public static <V> Collector<V, ?, ? extends IndexedSet<Long, V>> collectorLong(IndexerFunction.LongKey<? super V> indexer) {
		return collector((IndexerFunction<Long, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 *
	 * @deprecated Use {@link #collectorInt(IndexerFunction.IntKey) collectorInt(indexer)}
	 */
	@Deprecated
	public static <V> Collector<V, ?, ? extends IndexedSet<Integer, V>> collector(IndexerFunction.IntKey<? super V> indexer) {
		return collector((IndexerFunction<Integer, ? super V>)indexer);
	}

	/**
	 * Returns a {@code Collector} that accumulates the input elements into a new {@code IndexedSet} with specified indexer.
	 * This is an {@link Collector.Characteristics#UNORDERED unordered} Collector.
	 *
	 * @deprecated Use {@link #collectorLong(IndexerFunction.LongKey) collectorLong(indexer)}
	 */
	@Deprecated
	public static <V> Collector<V, ?, ? extends IndexedSet<Long, V>> collector(IndexerFunction.LongKey<? super V> indexer) {
		return collector((IndexerFunction<Long, ? super V>)indexer);
	}

	// ========== Construction and Sizing Operations ==========

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT}.
	 */
	public IndexedSet() {
		this(0);
	}

	/**
	 * Creates new empty set with default indexer {@link IndexerFunction#DEFAULT} and specified initial capacity.
	 */
	@SuppressWarnings("unchecked")
	public IndexedSet(int initialCapacity) {
		this(IndexerFunction.DEFAULT, initialCapacity);
	}

	/**
	 * Creates new empty set with specified indexer.
	 */
	protected IndexedSet(IndexerFunction<K, ? super V> indexer) {
		this(indexer, 0);
	}

	/**
	 * Creates new empty set with specified indexer.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}
	 */
	@Deprecated
	public IndexedSet(Indexer<K, ? super V> indexer) {
		this((IndexerFunction<K, ? super V>) indexer);
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 */
	@SuppressWarnings("unchecked")
	protected IndexedSet(IndexerFunction<K, ? super V> indexer, int initialCapacity) {
		if (indexer == null)
			throw new NullPointerException("Indexer is null.");
		this.indexer = indexer;
		this.core = initialCapacity <= 0 ? (Core<K, V>)Core.EMPTY_CORE : new Core(indexer, initialCapacity, GOLDEN_RATIO); // Atomic volatile write.
	}

	/**
	 * Creates new empty set with specified indexer and specified initial capacity.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withCapacity(int) withCapacity(initialCapacity)}
	 */
	@Deprecated
	public IndexedSet(Indexer<K, ? super V> indexer, int initialCapacity) {
		this((IndexerFunction<K, ? super V>) indexer, initialCapacity);
	}

	/**
	 * Creates a new set containing the elements in the specified collection.
	 * If specified collection is an {@link IndexedSet}, then new indexed set uses same indexer,
	 * otherwise it uses default indexer {@link IndexerFunction#DEFAULT}.
	 */
	@SuppressWarnings("unchecked")
	public IndexedSet(Collection<V> c) {
		this(c instanceof IndexedSet ? ((IndexedSet<K, V>) c).getIndexerFunction() : IndexerFunction.DEFAULT, c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 */
	protected IndexedSet(IndexerFunction<K, ? super V> indexer, Collection<? extends V> c) {
		this(indexer, c.size());
		addAll(c);
	}

	/**
	 * Creates a new set with specified indexer containing the elements in the specified collection.
	 *
	 * @deprecated Use {@link #create(IndexerFunction) create(indexer)}.{@link #withElements(Collection) withElements(c)}
	 */
	@Deprecated
	public IndexedSet(Indexer<K, ? super V> indexer, Collection<? extends V> c) {
		this((IndexerFunction<K, ? super V>) indexer, c);
	}

	/**
	 * Returns a shallow copy of this set - the values themselves are not cloned.
	 */
	@SuppressWarnings({"unchecked"})
	@Override
	public IndexedSet<K, V> clone() {
		try {
			IndexedSet<K, V> result = (IndexedSet<K, V>)super.clone();
			if (result.core != Core.EMPTY_CORE)
				result.core = new Core<>(result.core);
			return result;
		} catch (CloneNotSupportedException e) {
			throw new InternalError();
		}
	}

	/**
	 * Increases the capacity of this set instance, if necessary, to ensure that it
	 * can hold at least the number of elements specified by the capacity argument.
	 * <p>
	 * Returns <b>this</b> set instance for convenience.
	 */
	public IndexedSet<K, V> withCapacity(int capacity) {
		ensureCapacity(capacity);
		return this;
	}

	/**
	 * Adds all of the elements in the specified collection into this set.
	 * <p>
	 * Returns <b>this</b> set instance for convenience.
	 */
	public IndexedSet<K, V> withElements(Collection<? extends V> c) {
		ensureCapacity(c.size());
		addAll(c);
		return this;
	}

	/**
	 * Increases the capacity of this set instance, if necessary, to ensure that it
	 * can hold at least the number of elements specified by the capacity argument.
	 */
	public void ensureCapacity(int capacity) {
		core = core.ensureCapacity(indexer, capacity); // Atomic volatile read and write.
	}

	/**
	 * Trims the capacity of this set instance to be the set's current size.
	 * An application can use this operation to minimize the storage of this set instance.
	 */
	public void trimToSize() {
		core = core.trimToSize(indexer); // Atomic volatile read and write.
	}

	/**
	 * Removes all elements from this set.
	 */
	@Override
	public void clear() {
		core = core.clear(); // Atomic volatile read and write.
	}

	// ========== Query Operations ==========

	/**
	 * Returns indexer used to distinguish and identify elements in this set.
	 *
	 * @deprecated Use {@link #getIndexerFunction()}
	 */
	@SuppressWarnings({"rawtypes", "unchecked"})
	@Deprecated
	public Indexer<K, ? super V> getIndexer() {
		return indexer instanceof Indexer ? (Indexer)indexer : new Indexer.DelegateIndexer(indexer);
	}

	/**
	 * Returns indexer function used to distinguish and identify elements in this set.
	 */
	public IndexerFunction<K, ? super V> getIndexerFunction() {
		return indexer;
	}

	/**
	 * Returns the number of elements in this set.
	 */
	@Override
	public int size() {
		return core.size(); // Atomic volatile read.
	}

	/**
	 * Returns the element from this set which matches specified value or <b>null</b> if none were found.
	 */
	public V getByValue(V value) {
		return core.getByValue(value); // Atomic volatile read.
	}

	/**
	 * Returns the element from this set which matches specified key or <b>null</b> if none were found.
	 */
	public V getByKey(K key) {
		return core.getByKey(key); // Atomic volatile read.
	}

	/**
	 * Returns the element from this set which matches specified key or <b>null</b> if none were found.
	 */
	public V getByKey(int key) {
		return core.getByKey(key); // Atomic volatile read.
	}

	/**
	 * Returns the element from this set which matches specified key or <b>null</b> if none were found.
	 */
	public V getByKey(long key) {
		return core.getByKey(key); // Atomic volatile read.
	}

	/**
	 * Returns <b>true</b> if this set contains element which matches specified value.
	 * <p>
	 * This implementation delegates to ({@link #getByValue(Object) getByValue(value)}&nbsp;!=&nbsp;null) expression.
	 * <p>
	 * Note, that unlike {@link HashSet#contains},
	 * this method might throw {@link ClassCastException} if value is of the wrong class.
	 *
	 * @deprecated Use {@link #containsValue} to be explicit about type and intent.
	 */
	@Override
	@SuppressWarnings("unchecked")
	public boolean contains(Object value) {
		return getByValue((V)value) != null;
	}

	/**
	 * Returns <b>true</b> if this set contains element which matches specified value.
	 * <p>
	 * This implementation delegates to ({@link #getByValue(Object) getByValue(value)}&nbsp;!=&nbsp;null) expression.
	 */
	public boolean containsValue(V value) {
		return getByValue(value) != null;
	}

	/**
	 * Returns <b>true</b> if this set contains element which matches specified key.
	 * <p>
	 * This implementation delegates to ({@link #getByKey(Object) getByKey(key)}&nbsp;!=&nbsp;null) expression.
	 */
	public boolean containsKey(K key) {
		return getByKey(key) != null;
	}

	/**
	 * Returns <b>true</b> if this set contains element which matches specified key.
	 * <p>
	 * This implementation delegates to ({@link #getByKey(int) getByKey(key)}&nbsp;!=&nbsp;null) expression.
	 */
	public boolean containsKey(int key) {
		return getByKey(key) != null;
	}

	/**
	 * Returns <b>true</b> if this set contains element which matches specified key.
	 * <p>
	 * This implementation delegates to ({@link #getByKey(long) getByKey(key)}&nbsp;!=&nbsp;null) expression.
	 */
	public boolean containsKey(long key) {
		return getByKey(key) != null;
	}

	/**
	 * Returns an iterator over the elements in this set.
	 */
	@Nonnull
	@Override
	@SuppressWarnings("unchecked")
	public Iterator<V> iterator() {
		return (Iterator<V>)iterator(IndexedIterator.VALUE_FAILFAST);
	}

	/**
	 * Returns an iterator over the keys of elements in this set.
	 */
	@SuppressWarnings("unchecked")
	public Iterator<K> keyIterator() {
		return (Iterator<K>)iterator(IndexedIterator.KEY_FAILFAST);
	}

	/**
	 * Returns an iterator over the entries in this set.
	 */
	@SuppressWarnings("unchecked")
	public Iterator<Map.Entry<K, V>> entryIterator() {
		return (Iterator<Map.Entry<K,V>>)iterator(IndexedIterator.ENTRY_FAILFAST);
	}

	/**
	 * Returns concurrent iterator over the elements in this set.
	 */
	@SuppressWarnings("unchecked")
	public Iterator<V> concurrentIterator() {
		return (Iterator<V>)iterator(IndexedIterator.VALUE_CONCURRENT);
	}

	/**
	 * Returns an array containing all of the elements in this set.
	 * Obeys the general contract of the {@link Collection#toArray()} method.
	 */
	@Nonnull
	@Override
	public Object[] toArray() {
		return core.toArray(null); // Atomic volatile read.
	}

	/**
	 * Returns an array containing all of the elements in this set whose runtime type is that of the specified array.
	 * Obeys the general contract of the {@link Collection#toArray(Object[])} method.
	 */
	@Nonnull
	@Override
	public <T> T[] toArray(T[] a) {
		return core.toArray(a); // Atomic volatile read.
	}

	/**
	 * Returns static structure statistics of this set.
	 */
	public IndexedSetStats getStats() {
		return core.getStats();
	}

	// ========== Modification Operations ==========

	/**
	 * Puts specified element into this set and returns previous element that matches specified one.
	 */
	public V put(V value) {
		return putImpl(core, value); // Atomic volatile read.
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
	public V putIfAbsentAndGet(V value) {
		Core<K, V> core = this.core; // Atomic volatile read.
		V oldValue = core.getByValue(value);
		if (oldValue != null)
			return oldValue;
		putImpl(core, value);
		return value;
	}

	/**
	 * Adds specified element into this set and returns <b>true</b>
	 * if this operation has increased the size of this set.
	 * <p>
	 * This implementation adds value using {@link #put(Object) put(value)} method.
	 */
	@Override
	public boolean add(V value) {
		return put(value) == null;
	}

	/**
	 * Removes specified element from this set if it is present and returns
	 * <b>true</b> if this operation has decreased the size of this set.
	 * <p>
	 * This implementation removes value using {@link #removeValue(Object) removeValue(value)} method.
	 * <p>
	 * Note, that unlike {@link HashSet#remove},
	 * this method might throw {@link ClassCastException} if value is of the wrong class.
	 */
	@Override
	@SuppressWarnings("unchecked")
	public boolean remove(Object value) {
		return removeValue((V) value) != null;
	}

	/**
	 * Removes the element from this set which matches specified value if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	public V removeValue(V value) {
		Core<K, V> core = this.core; // Atomic volatile read.
		V oldValue = core.removeValue(value);
		this.core = core; // Atomic volatile write.
		return oldValue;
	}

	/**
	 * Removes the element from this set which matches specified key if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	public V removeKey(K key) {
		Core<K, V> core = this.core; // Atomic volatile read.
		V oldValue = core.removeKey(key);
		this.core = core; // Atomic volatile write.
		return oldValue;
	}

	/**
	 * Removes the element from this set which matches specified key if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	public V removeKey(int key) {
		return removeKey((long)key);
	}

	/**
	 * Removes the element from this set which matches specified key if it is present
	 * and returns removed element or <b>null</b> if none were found.
	 */
	public V removeKey(long key) {
		Core<K, V> core = this.core; // Atomic volatile read.
		V oldValue = core.removeKey(key);
		this.core = core; // Atomic volatile write.
		return oldValue;
	}

	// ========== Internal Implementation - Helper Instance Methods ==========

	private V putImpl(Core<K, V> core, V value) {
		V oldValue;
		if (core.needRehash()) {
			// Rehash shall be done before put in order to move away from EMPTY_CORE and protect from bad magic.
			// However in situ replacement of existing value shall keep old modCount and avoid rehash.
			if (core == Core.EMPTY_CORE || (oldValue = core.put(value, true)) == null) {
				core = core.rehash(indexer, 0);
				oldValue = core.put(value, false);
			}
		} else
			oldValue = core.put(value, false);
		this.core = core; // Atomic volatile write.
		return oldValue;
	}

	private Iterator<?> iterator(int type) {
		Core<K, V> core = this.core; // Atomic volatile read.
		return core.size() == 0 ? IndexedIterator.EMPTY_ITERATOR : new IndexedIterator<>(this, core, type);
	}

	void checkModification(Object checkCore, long checkModCount) {
		Core<K, V> core = this.core; // Atomic volatile read.
		if (checkCore != core || checkModCount != core.getModCount())
			throw new ConcurrentModificationException();
	}

	void removeIterated(Object checkCore, long checkModCount, boolean concurrent, V lastValue, int lastIndex) {
		Core<K, V> core = this.core; // Atomic volatile read.
		if (!concurrent && (checkCore != core || checkModCount != core.getModCount()))
			throw new ConcurrentModificationException();
		if (core.getAt(lastIndex) == lastValue) // Atomic read.
			core.removeAt(lastIndex, core.getInitialIndexByValue(lastValue));
		else if (concurrent)
			core.removeValue(lastValue);
		else
			throw new ConcurrentModificationException();
		this.core = core; // Atomic volatile write.
	}

	private void writeObject(ObjectOutputStream out) throws IOException {
		out.defaultWriteObject();
		writeCore(out);
	}

	void writeCore(ObjectOutputStream out) throws IOException {
		core.writeObjectImpl(out); // Atomic volatile read.
	}

	private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
		in.defaultReadObject();
		core = Core.readObjectImpl(indexer, in); // Atomic volatile write.
	}

	// ========== Internal Implementation - Core ==========

	/**
	 * Core class to hold all data of {@link IndexedSet}.
	 */
	private static final class Core<K, V> {
		static final int QUALITY_BASE = 6;
		static final Object REMOVED = new Object(); // Marker object for removed values.

		@SuppressWarnings("unchecked")
		static final Core<?, ?> EMPTY_CORE = new Core(value -> null, 0, GOLDEN_RATIO); // Empty core for empty sets.

		static {
			EMPTY_CORE.overallSize = EMPTY_CORE.matrix.length; // Special value to trigger rehash before first 'put' operation.
		}

		// 'private' fields and methods of Core class shall be accessed only from within Core class itself.

		private final int magic;
		private final int shift;
		private final IndexerFunction<K, ? super V> indexer;
		private final V[] matrix;

		/**
		 * Quality is a complex value that tracks quality of this core's payload,
		 * <code>quality = (total_distance_to_all_payload_values &lt;&lt; QUALITY_BASE) + distance_shift</code>,
		 * where {@code distance_shift} is a current tolerance to the bad payload distance.
		 * Note, that lower value of quality is BETTER.
		 * @see #exceed(long) 
		 */
		private long quality;

		private int payloadSize;
		private int overallSize; // payloadSize + number_of_REMOVED_values

		private long modCount; // counts structural changes when elements are added or removed (replace is Ok)
		private long amortizedCost; // total cost of amortization of all removed and rehashed values, see unamortizedCost()

		@SuppressWarnings("unchecked")
		Core(IndexerFunction<K, ? super V> indexer, int capacity, int magic) {
			if (indexer == null)
				throw new NullPointerException("Indexer is null.");
			this.magic = magic;
			shift = getShift(capacity);
			this.indexer = indexer;
			matrix = (V[])new Object[(-1 >>> shift) + 1];
			quality = QUALITY_BASE + 1; // will rehash when avg_dist > 2, see exceed(...)
		}

		/**
		 * Clones specified core. Implemented as constructor to keep {@link #matrix} field final.
		 */
		Core(Core<K, V> source) {
			magic = source.magic;
			shift = source.shift;
			indexer = source.indexer;
			matrix = source.matrix.clone();
			quality = source.quality;
			payloadSize = source.payloadSize;
			overallSize = source.overallSize;
			modCount = source.modCount;
			amortizedCost = source.amortizedCost;
		}

		/**
		 * Returns increment to this core's {@link #quality} that should be added when new payload
		 * entry is added at position {@code index} with its initial position at {@code initialIndex}.
		 */
		private long qualityInc(int index, int initialIndex) {
			return ((initialIndex - index) & (-1 >>> shift)) << QUALITY_BASE;
		}

		/**
		 * Returns true if average distance to payload values exceeds
		 * <code>1 &lt;&lt; (distanceShift - QUALITY_BASE)</code>,
		 * where only last {@link #QUALITY_BASE} bits of distanceShift parameter are used.
		 * Thus, the method can be directly applied using {@link #quality} itself as an
		 * {@code distanceShift} argument.
		 */
		private boolean exceed(long distanceShift) {
			// mask total distance bits first
			return (quality & (-1L << QUALITY_BASE)) > ((long) payloadSize << distanceShift);
		}

		// compute quality tolerance for new instance, so that it rehashes only when it becomes much worse than now
		private void computeTolerance() {
			while (exceed(quality))
				quality++;
			// increment to next tolerance if it is close to exceeding current tolerance
			if ((quality & (-1L << QUALITY_BASE)) * 3 > ((long) payloadSize << quality) * 2)
				quality++;
		}

		/**
		 * Returns the cost of all put operations to place payload into this core.
		 * @see #amortizedCost
		 */
		private long unamortizedCost() {
			return (quality >>> QUALITY_BASE) + payloadSize;
		}

		private void putValuesIntoEmptyCore(V[] values) {
			for (int i = values.length; --i >= 0;) {
				V value = values[i]; // Atomic read.
				if (value == null || value == REMOVED)
					continue;
				int index = getInitialIndexByValue(value);
				int initialIndex = index;
				while (matrix[index] != null)
					index = (index - 1) & (-1 >>> shift);
				matrix[index] = value;
				quality += qualityInc(index, initialIndex);
				payloadSize++;
				overallSize++;
				// Check if there are too many elements in the source.
				// Error may happen either if source state is broken
				// or if elements are added to source concurrently.
				// Ignoring such error here will lead to a dead loop above.
				if (overallSize > (THRESHOLD_UP >>> shift))
					throw new ConcurrentModificationException("Concurrent modification during rehash");
			}
		}

		private Core<K, V> rehashInternal(IndexerFunction<K, ? super V> indexer, int capacity) {
			/* GENERAL DESCRIPTION OF REHASH ALGORITHM:
			   1. Try to rehash at most 4 times. Twice at regular capacity, once at 2x capacity, and once at 4x capacity.
			   2. First attempt can keep old magic if previous distance is good enough, other attempts use random magic.
			   3. If the first attempt immediately satisfies perfect limits then return.
			   4. If we have to make additional attempts, then the best result is picked.
			      In this case result is considered acceptable if it satisfies consequently worse limits.
			   5. After four attempts the best result is returned even if it is unacceptable by the above rules.
			 */
			capacity = Math.min(Math.max(capacity, payloadSize), MAX_CAPACITY);
			long totalCost = amortizedCost + unamortizedCost();
			Core<K, V> result = new Core<>(indexer, capacity, exceed(QUALITY_BASE + 1) ? nextMagic(magic, capacity) : magic);
			result.putValuesIntoEmptyCore(matrix);
			totalCost += result.unamortizedCost();
			if (result.exceed(QUALITY_BASE + 1)) // only if quality is not very good
				for (int k = 0; k < 3; k++) {
					Core<K, V> other = new Core<>(indexer, capacity, nextMagic(magic, capacity));
					other.putValuesIntoEmptyCore(matrix);
					totalCost += other.unamortizedCost();
					if (other.quality < result.quality) // lower quality is better
						result = other;
					if (!result.exceed(QUALITY_BASE + 2 + k))
						break; // break when we have acceptable quality
					capacity = Math.min(capacity * 2, MAX_CAPACITY);
				}
			result.computeTolerance();
			// update result stats
			result.modCount = modCount;
			result.amortizedCost = totalCost - result.unamortizedCost();
			return result;
		}

		Core<K, V> rehash(IndexerFunction<K, ? super V> indexer, int capacity) {
			long modCount = this.modCount; // Atomic read.
			Core<K, V> result = rehashInternal(indexer, capacity);
			if (modCount != this.modCount) // Atomic read.
				throw new ConcurrentModificationException("Concurrent modification during rehash");
			return result;
		}

		boolean needRehash() {
			return overallSize > (THRESHOLD_UP >>> shift) || exceed(quality);
		}

		Core<K, V> rehashIfNeeded(IndexerFunction<K, ? super V> indexer, int capacity) {
			return needRehash() ?
				rehash(indexer, capacity) : this;
		}

		Core<K, V> ensureCapacity(IndexerFunction<K, ? super V> indexer, int capacity) {
			return capacity > (THRESHOLD_UP >>> shift) && shift > MIN_SHIFT ?
				rehash(indexer, capacity) : this;
		}

		Core<K, V> trimToSize(IndexerFunction<K, ? super V> indexer) {
			return payloadSize < (THRESHOLD_DOWN >>> shift) && shift < MAX_SHIFT ?
				rehash(indexer, 0) : this;
		}

		Core<K, V> clear() {
			if (this == EMPTY_CORE)
				return this;
			for (int i = matrix.length; --i >= 0;)
				matrix[i] = null;
			modCount += payloadSize;
			amortizedCost += unamortizedCost();
			quality = QUALITY_BASE + 1;
			payloadSize = 0;
			overallSize = 0;
			return this;
		}

		int size() {
			return payloadSize; // Atomic read.
		}

		long getModCount() {
			return modCount; // Atomic read.
		}

		int getInitialIndexByValue(V value) {
			return (indexer.hashCodeByValue(value) * magic) >>> shift;
		}

		V getByValue(V value) {
			int index = getInitialIndexByValue(value);
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByValue(value, testValue))
					return testValue;
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		V getByKey(K key) {
			int index = (indexer.hashCodeByKey(key) * magic) >>> shift;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByKey(key, testValue))
					return testValue;
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		V getByKey(long key) {
			int index = (indexer.hashCodeByKey(key) * magic) >>> shift;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByKey(key, testValue))
					return testValue;
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		@SuppressWarnings("unchecked")
		V put(V value, boolean replaceOnly) {
			// These are sanity checks - they can be removed once testing completed.
			assert this != EMPTY_CORE : "Putting into EMPTY core.";
			assert value != REMOVED : "Value is an internal special marker object.";

			if (value == null)
				throw new NullPointerException("Value is null.");
			int index = getInitialIndexByValue(value);
			int initialIndex = index;
			int removedIndex = -1;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByValue(value, testValue)) {
					matrix[index] = value;
					return testValue;
				}
				if (testValue == REMOVED && removedIndex < 0)
					removedIndex = index;
				index = (index - 1) & (-1 >>> shift);
			}
			if (replaceOnly)
				return null;
			if (removedIndex < 0) {
				matrix[index] = value;
				overallSize++;
			} else
				matrix[index = removedIndex] = value;
			quality += qualityInc(index, initialIndex);
			payloadSize++;
			modCount++;
			return null;
		}

		V removeValue(V value) {
			int index = getInitialIndexByValue(value);
			int initialIndex = index;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByValue(value, testValue)) {
					removeAt(index, initialIndex);
					return testValue;
				}
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		V removeKey(K key) {
			int index = (indexer.hashCodeByKey(key) * magic) >>> shift;
			int initialIndex = index;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByKey(key, testValue)) {
					removeAt(index, initialIndex);
					return testValue;
				}
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		V removeKey(long key) {
			int index = (indexer.hashCodeByKey(key) * magic) >>> shift;
			int initialIndex = index;
			V testValue;
			while ((testValue = matrix[index]) != null) { // Atomic read.
				if (testValue != REMOVED && indexer.matchesByKey(key, testValue)) {
					removeAt(index, initialIndex);
					return testValue;
				}
				index = (index - 1) & (-1 >>> shift);
			}
			return null;
		}

		int getMaxIndex() {
			return matrix.length - 1;
		}

		V getAt(int index) {
			return matrix[index];
		}

		@SuppressWarnings("unchecked")
		void removeAt(int index, int initialIndex) {
			matrix[index] = (V)REMOVED;
			quality -= qualityInc(index, initialIndex);
			payloadSize--;
			if (matrix[(index - 1) & (-1 >>> shift)] == null)
				while (matrix[index] == REMOVED) {
					matrix[index] = null;
					overallSize--;
					index = (index + 1) & (-1 >>> shift);
				}
			modCount++;
			// we paid twice -- first adding this element, then removing it
			amortizedCost += 2 * ((qualityInc(index, initialIndex) >>> QUALITY_BASE) + 1);
		}

		@SuppressWarnings("unchecked")
		<T> T[] toArray(T[] a) {
			// If (a == null) then returned array shall be of exact length, otherwise it can be larger.
			int size = payloadSize; // Atomic read.
			Object[] result = a == null ? new Object[size] : a.length >= size ? a :
				(Object[])Array.newInstance(a.getClass().getComponentType(), size);
			int n = 0;
			for (int i = matrix.length; --i >= 0;) {
				Object value = matrix[i]; // Atomic read.
				if (value == null || value == REMOVED)
					continue;
				if (n >= result.length) {
					// More elements were added concurrently. Enlarge result array.
					// Do not grow more than twice.
					// Do not grow more than possible remaining elements (i + 1).
					// Do not fail when (n == 0).
					Object[] tmp = (Object[])Array.newInstance(result.getClass().getComponentType(), n + Math.min(n, i) + 1);
					System.arraycopy(result, 0, tmp, 0, n);
					result = tmp;
				}
				result[n++] = value;
			}
			if (n < result.length && a == null) {
				// Shrink allocated array to exact size.
				Object[] tmp = new Object[n];
				System.arraycopy(result, 0, tmp, 0, n);
				result = tmp;
			}
			if (n < result.length)
				result[n] = null;
			return (T[])result;
		}

		void writeObjectImpl(ObjectOutputStream out) throws IOException {
			int n = payloadSize; // Atomic read.
			// if (n == 0) then empty set, no elements written
			// if (n > 0) then fixed set with exactly n elements written
			// if (n < 0) then dynamic set with approximately (-n) elements plus marker null element written
			out.writeInt(n);
			for (int i = matrix.length; --i >= 0;) {
				Object value = matrix[i]; // Atomic read.
				if (value != null && value != REMOVED && n-- > 0) // Do not write more than n values anyway.
					out.writeObject(value);
			}
			if (n != 0)
				throw new IOException("Concurrent modification detected.");
		}

		@SuppressWarnings("unchecked")
		static <K, V> Core<K, V> readObjectImpl(IndexerFunction<K, ? super V> indexer, ObjectInputStream in) throws IOException, ClassNotFoundException {
			int n = in.readInt();
			// if (n == 0) then empty set, no elements written
			// if (n > 0) then fixed set with exactly n elements written
			// if (n < 0) then dynamic set with approximately (-n) elements plus marker null element written
			if (n == 0)
				return (Core<K, V>)EMPTY_CORE;
			Core<K, V> core = new Core<>(indexer, Math.abs(n), GOLDEN_RATIO);
			if (n > 0)
				for (int i = 0; i < n; i++) {
					core = core.rehashIfNeeded(indexer, n); // to protect from bad magic
					core.put((V)in.readObject(), false);
				}
			else
				for (V value; (value = (V)in.readObject()) != null;) {
					core = core.rehashIfNeeded(indexer, -n); // to protect from bad magic
					core.put(value, false);
				}
			return core;
		}

		IndexedSetStats getStats() {
			return new IndexedSetStats(payloadSize, matrix.length, quality >>> QUALITY_BASE, amortizedCost + unamortizedCost(), modCount);
		}
	}

	// ========== Internal Implementation - Iterator ==========

	/**
	 * Asynchronous iterator over {@link IndexedSet}.
	 */
	private static final class IndexedIterator<K, V> implements Iterator<Object> {
		static final int VALUE_CONCURRENT = 0;
		static final int VALUE_FAILFAST = 1;
		static final int KEY_FAILFAST = 2;
		static final int ENTRY_FAILFAST = 3;

		@SuppressWarnings("unchecked")
		static final Iterator<?> EMPTY_ITERATOR = new IndexedIterator(null, Core.EMPTY_CORE, VALUE_CONCURRENT);

		// 'private' fields and methods of IndexedIterator class shall be accessed only from within IndexedIterator class itself.

		private final IndexedSet<K, V> set;
		private final Core<K, V> core;
		private final int type;

		private long modCount;

		private V nextValue;
		private int nextIndex;
		private V lastValue;
		private int lastIndex;

		IndexedIterator(IndexedSet<K, V> set, Core<K, V> core, int type) {
			this.set = set;
			this.core = core;
			this.type = type;
			modCount = core.getModCount();
			nextIndex = core.getMaxIndex() + 1;
			fillNext();
		}

		private void fillNext() {
			if (type != VALUE_CONCURRENT)
				set.checkModification(core, modCount);
			while (--nextIndex >= 0) {
				nextValue = core.getAt(nextIndex); // Atomic read.
				if (nextValue != null && nextValue != Core.REMOVED)
					return;
			}
			nextValue = null; // No more elements - clear leftover state.
		}

		@Override
		public boolean hasNext() {
			return nextValue != null;
		}

		@Override
		public Object next() {
			if (nextValue == null)
				throw new NoSuchElementException();
			lastValue = nextValue;
			lastIndex = nextIndex;
			fillNext();
			if (type == KEY_FAILFAST)
				return set.getIndexerFunction().getObjectKey(lastValue);
			if (type == ENTRY_FAILFAST)
				return new IndexedEntry<>(set, lastValue);
			return lastValue;
		}

		@Override
		public void remove() {
			if (lastValue == null)
				throw new IllegalStateException();
			set.removeIterated(core, modCount, type == VALUE_CONCURRENT, lastValue, lastIndex);
			modCount = core.getModCount();
			lastValue = null;
		}
	}

	// ========== Internal Implementation - Entry ==========

	/**
	 * IndexedEntry class is a wrapper to convert indexed API to collections API.
	 */
	private static final class IndexedEntry<K, V> implements Map.Entry<K, V> {
		// 'private' fields and methods of IndexedEntry class shall be accessed only from within IndexedEntry class itself.

		private final IndexedSet<K, V> set;
		private V value;

		IndexedEntry(IndexedSet<K, V> set, V value) {
			this.set = set;
			this.value = value;
		}

		@Override
		public K getKey() {
			return set.getIndexerFunction().getObjectKey(value);
		}

		@Override
		public V getValue() {
			return value;
		}

		@Override
		public V setValue(V value) {
			if (value == null)
				throw new NullPointerException("Value is null.");
			V oldValue = this.value;
			if (!set.getIndexerFunction().matchesByValue(value, oldValue))
				throw new IllegalArgumentException("New value does not match old value.");
			set.put(this.value = value);
			return oldValue;
		}

		public boolean equals(Object obj) {
			if (!(obj instanceof Map.Entry))
				return false;
			Map.Entry<?, ?> e = (Map.Entry<?, ?>)obj;
			Object key = getKey();
			Object ekey = e.getKey();
			return (key == null ? ekey == null : key.equals(ekey)) && value.equals(e.getValue());
		}

		public int hashCode() {
			K key = getKey();
			return (key == null ? 0 : key.hashCode()) ^ value.hashCode();
		}

		public String toString() {
			return getKey() + "=" + value;
		}
	}

	// ========== Internal Implementation - Helper Static Constants and Methods ==========

	/*
	 * This section contains constants and methods to support matrix-based data structures.
	 * Such data structures and related algorithms are also known as "direct linear probe hashing".
	 * The code below is a copy-paste from com.devexperts.qd.impl.matrix.Hashing class.
	 */

	static final int THRESHOLD_UP = (int)((1L << 32) * 5 / 9);
	static final int THRESHOLD_DOWN = (int)((1L << 32) * 5 / 27);

	static final int THRESHOLD_ALLOC_UP = (int)((1L << 32) * 4 / 9);
	static final int MAX_SHIFT = 29;
	static final int MIN_SHIFT = 2;
	static final int MAX_CAPACITY = THRESHOLD_ALLOC_UP >>> MIN_SHIFT;

	/**
	 * Calculates appropriate 'shift' for specified capacity.
	 */
	static int getShift(int capacity) {
		int shift = MAX_SHIFT;
		while ((THRESHOLD_ALLOC_UP >>> shift) < capacity && shift >= MIN_SHIFT)
			shift--;
		if (shift < MIN_SHIFT)
			throw new IllegalArgumentException("Capacity is too large: " + capacity);
		return shift;
	}

	private static final int GOLDEN_RATIO = 0x9E3779B9;
	private static final int MAGIC = 0xC96B5A35;
	private static int magicSeed = (int)(System.currentTimeMillis() * Runtime.getRuntime().freeMemory());

	/**
	 * Generates next MAGIC number with proper distribution and difference of bits.
	 */
	static int nextMagic(int prevMagic) {
		// Generate next pseudo-random number with lowest bit set to '1'.
		int magic = (magicSeed = magicSeed * MAGIC + 1) | 1;
		// Enforce that any 4 bits are neither '0000' nor '1111'.
		// Start earlier to enforce that highest 2 bits are neither '00' nor '11'.
		for (int i = 31; --i >= 0;) {
			int bits = (magic >> i) & 0x0F;
			if (bits == 0 || bits == 0x0F) {
				magic ^= 1 << i;
				i -= 2;
			}
		}
		// Recover cleared lowest bit.
		if ((magic & 1) == 0)
			magic ^= 3; // Convert '10' (the only possible case) into '01'.
		// Enforce that any 8 bits have at least 1 difference from previous number.
		for (int i = 25; --i >= 0;) {
			if ((((magic ^ prevMagic) >> i) & 0xFF) == 0) {
				// Reverse bit i+1 and enforce that bit i+2 differs from it.
				// This may lead to 4-bit (but not longer) sequences of '0' or '1'.
				magic ^= ((magic ^ (magic << 1)) & (4 << i)) ^ (2 << i);
				i -= 6;
			}
		}
		return magic;
	}

	/**
	 * Generates next MAGIC number by selecting best one from several candidates.
	 * Number of checked candidates depends on specified capacity.
	 */
	static int nextMagic(int prevMagic, int capacity) {
		int magic = nextMagic(prevMagic);
		if (capacity < 32)
			return magic;
		double eval = evaluateContinuedFraction(magic);
		int attempts = 30 - Integer.numberOfLeadingZeros(capacity);
		for (int i = 0; i < attempts; i++) {
			int m = nextMagic(prevMagic);
			double e = evaluateContinuedFraction(m);
			if (e > eval) {
				magic = m;
				eval = e;
			}
		}
		return magic;
	}

	/**
	 * Evaluates quality of specified MAGIC number as a minimal normalized distance
	 * to rational approximations generated by continued fraction.
	 * The larger distance - the better magic.
	 */
	static double evaluateContinuedFraction(int magic) {
		// for explanation of continued fraction look at https://en.wikipedia.org/wiki/Continued_fraction
		// 'x' is a fractional representation of magic scaled into a range (0, 1); both bounds exclusive
		// 'rem' is a current remainder to compute next continued fraction coefficient; it's in a range [0, 1)
		// 'a' is a current continued fraction coefficient - aka a[i]; it's always a natural number
		// 'p/q' is a current rational approximation of 'x' with an error less than 1/q^2
		// (p1,q1) and (p2,q2) are previous (p,q) approximations - aka (p[i-1],q[i-1]) and (p[i-2],q[i-2])
		// 'grade' is a minimal (worst) distance from 'x' to it's approximations normalized by q^2
		double x = (double)(magic & 0xFFFFFFFFL) / (1L << 32);
		double rem = x;
		long p2 = 1;
		long q2 = 0;
		long p1 = 0;
		long q1 = 1;
		double grade = x;
		for (int i = 1; i <= 20; i++) {
			rem = 1 / rem;
			long a = (long)rem;
			rem -= a;
			long p = a * p1 + p2;
			long q = a * q1 + q2;
			p2 = p1;
			q2 = q1;
			p1 = p;
			q1 = q;
			grade = Math.min(grade, Math.abs(x * q - p) * q);
			if (grade < 1e-6 || rem < 1e-6 || q > (1 << 20))
				break;
		}
		return grade;
	}
}
