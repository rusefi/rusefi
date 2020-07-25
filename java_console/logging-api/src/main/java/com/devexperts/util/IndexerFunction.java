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

/**
 * A strategy that distinguishes and identifies elements in an {@link IndexedSet} and {@link IndexedMap}.
 * The <b>IndexerFunction</b> defines 3 equivalent ways to identify elements:
 * <ul>
 * <li> <b>by value</b> - mandatory and primary method to identify element by itself
 * <li> <b>by object key</b> - optional method that identifies elements using object key
 * <li> <b>by number key</b> - optional method that identifies elements using number key
 * </ul>
 *
 * <p>The <b>IndexerFunction</b> is not restricted to use <b>explicit key</b> concept for identification.
 * Identity of an element may be defined by a number of attributes, specified in a value itself,
 * in a template object, in a formal key object, or encoded in a number key. The <b>IndexerFunction</b>
 * may use all these ways interchangeable to distinguish and identify elements.
 *
 * <p>Being a strategy, the <b>IndexerFunction</b> is required to be stateless, concurrent and thread-safe.
 *
 * <p>The <b>IndexerFunction</b> is a functional interface with a sole abstract method that shall be implemented
 * in order to use identification using explicit object key - no other methods are required to be
 * overridden in such simple cases.
 * There are two other functional interfaces {@link IndexerFunction.IntKey} and {@link IndexerFunction.LongKey}
 * which are similarly designed with sole abstract methods to simplify identification using explicit number keys.
 * There is also a functional interface {@link IndexerFunction.IdentityKey} which is similarly designed
 * with sole abstract method for cases when explicit object keys must be compared by reference rather than
 * using their {@link Object#equals(Object) equals} method.
 *
 * <p>The <b>IndexerFunction</b> is {@link Serializable}, so that all concrete subclasses
 * shall be <b>serializable</b> in order to support serialization of indexed set and map.
 */
@FunctionalInterface
public interface IndexerFunction<K, V> extends Serializable {

	/**
	 * Default strategy that treats values as their own keys <b>(key&nbsp;==&nbsp;value)</b> and delegates to
	 * {@link Object#hashCode() Object.hashCode()} and {@link Object#equals(Object) Object.equals(Object)}
	 * methods as appropriate. This strategy does not support primitive keys.
	 *
	 * <p>This strategy basically turns {@link IndexedSet} into plain hash set of objects and {@link IndexedMap}
	 * into a self-reference mapping.
	 */
	@SuppressWarnings("rawtypes")
	public static final IndexerFunction DEFAULT = new DefaultIndexerFunction();

	// ========== Object Key Operations ==========

	/**
	 * Returns object key for specified value to be used for hashing and identification;
	 * called when explicit object key is needed or when other methods delegate operations as specified.
	 */
	public K getObjectKey(V value);

	/**
	 * Returns hash code for specified object key; called when performing operations using object keys.
	 *
	 * <p>This implementation delegates to
	 * <code>(key&nbsp;==&nbsp;null&nbsp;?&nbsp;0&nbsp;:&nbsp;key.{@link Object#hashCode() hashCode}())</code> expression.
	 */
	public default int hashCodeByKey(K key) {
		return key == null ? 0 : key.hashCode();
	}

	/**
	 * Determines if specified object key matches specified value; called when performing operations using object keys.
	 *
	 * <p>This implementation delegates to
	 * <code>(key&nbsp;==&nbsp;null&nbsp;?&nbsp;{@link #getObjectKey(Object) getObjectKey}(value)&nbsp;==&nbsp;null&nbsp;:&nbsp;key.{@link Object#equals(Object) equals}({@link #getObjectKey(Object) getObjectKey}(value)))</code> expression.
	 */
	public default boolean matchesByKey(K key, V value) {
		return key == null ? getObjectKey(value) == null : key.equals(getObjectKey(value));
	}

	// ========== Value Operations ==========

	/**
	 * Returns hash code for specified value; called when performing value-based operations, including <b>rehash</b>.
	 *
	 * <p>This implementation delegates to
	 * <code>{@link #hashCodeByKey(Object) hashCodeByKey}({@link #getObjectKey(Object) getObjectKey}(value))</code> expression.
	 */
	public default int hashCodeByValue(V value) {
		return hashCodeByKey(getObjectKey(value));
	}

	/**
	 * Determines if specified new value matches specified old value; called when performing value-based operations.
	 *
	 * <p>This implementation delegates to
	 * <code>{@link #matchesByKey(Object, Object) matchesByKey}({@link #getObjectKey(Object) getObjectKey}(newValue),&nbsp;oldValue)</code> expression.
	 */
	public default boolean matchesByValue(V newValue, V oldValue) {
		return matchesByKey(getObjectKey(newValue), oldValue);
	}

	// ========== Number Key Operations (Optional) ==========

	/**
	 * Returns number key for specified value to be used for hashing and identification;
	 * called when explicit number key is needed or when other methods delegate operations as specified.
	 *
	 * <p>This implementation delegates to
	 * <code>{@link #hashCodeByKey(long) hashCodeByKey}(((Number){@link #getObjectKey(Object) getObjectKey}(value)).{@link Number#longValue() longValue}())</code> expression.
	 */
	public default long getNumberKey(V value) {
		return hashCodeByKey(((Number)getObjectKey(value)).longValue());
	}

	/**
	 * Returns hash code for specified number key; called when performing operations using long keys.
	 *
	 * <p>This implementation delegates to
	 * <code>Long.{@link Long#hashCode(long) hashCode}(key)</code> expression.
	 */
	public default int hashCodeByKey(long key) {
		return Long.hashCode(key);
	}

	/**
	 * Determines if specified number key matches specified value; called when performing operations using number keys.
	 *
	 * <p>This implementation delegates to
	 * <code>(key&nbsp;==&nbsp;{@link #getNumberKey(Object) getNumberKey}(value))</code> expression.
	 */
	public default boolean matchesByKey(long key, V value) {
		return key == getNumberKey(value);
	}

	/**
	 * An {@link IndexerFunction} that distinguishes and identifies elements using <b>int</b> keys.
	 *
	 * <p>It assumes that elements are fully identifiable by plain numeric identifier and treats object keys as a mere wrappers.
	 * The hash function is computed by taking <b>int</b> key value.
	 */
	@FunctionalInterface
	public interface IntKey<V> extends IndexerFunction<Integer, V> {
		/**
		 * Returns number key for specified value to be used for hashing and identification.
		 */
		public int getIntKey(V value);

		/**
		 * Returns number key for specified value to be used for hashing and identification;
		 * called when explicit number key is needed or when other methods delegate operations as specified.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #getIntKey(Object) getIntKey}(value)</code> expression.
		 */
		@Override
		public default long getNumberKey(V value) {
			return getIntKey(value);
		}

		/**
		 * Returns hash code for specified value; called when performing value-based operations, including <b>rehash</b>.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #hashCodeByKey(long) hashCodeByKey}({@link #getIntKey(Object) getIntKey}(value))</code> expression.
		 */
		@Override
		public default int hashCodeByValue(V value) {
			return hashCodeByKey(getIntKey(value));
		}

		/**
		 * Determines if specified new value matches specified old value; called when performing value-based operations.
		 *
		 * <p>This implementation delegates to
		 * <code>({@link #getIntKey(Object) getIntKey}(newValue)&nbsp;==&nbsp;{@link #getIntKey(Object) getIntKey}(oldValue))</code> expression.
		 */
		@Override
		public default boolean matchesByValue(V newValue, V oldValue) {
			return getIntKey(newValue) == getIntKey(oldValue);
		}

		/**
		 * Returns object key for specified value to be used for hashing and identification;
		 * called when explicit object key is needed or when other methods delegate operations as specified.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #getIntKey(Object) getIntKey}(value)</code> expression.
		 */
		@Override
		public default Integer getObjectKey(V value) {
			return getIntKey(value);
		}

		/**
		 * Returns hash code for specified object key; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #hashCodeByKey(long) hashCodeByKey}(key.{@link Integer#intValue() intValue}())</code> expression.
		 */
		@Override
		public default int hashCodeByKey(Integer key) {
			return hashCodeByKey(key.intValue());
		}

		/**
		 * Determines if specified object key matches specified value; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>(key&nbsp;==&nbsp;{@link #getIntKey(Object) getIntKey}(value))</code> expression.
		 */
		@Override
		public default boolean matchesByKey(Integer key, V value) {
			return key == getIntKey(value);
		}
	}

	/**
	 * An {@link IndexerFunction} that distinguishes and identifies elements using <b>long</b> keys.
	 *
	 * <p>It assumes that elements are fully identifiable by plain numeric identifier and treats object keys as a mere wrappers.
	 * The hash function is computed using <code>{@link Long#hashCode(long) Long.hashCode}(key)</code> expression.
	 */
	@FunctionalInterface
	public interface LongKey<V> extends IndexerFunction<Long, V> {
		/**
		 * Returns number key for specified value to be used for hashing and identification;
		 * called when explicit number key is needed or when other methods delegate operations as specified.
		 */
		@Override
		public long getNumberKey(V value);

		/**
		 * Returns hash code for specified value; called when performing value-based operations, including <b>rehash</b>.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #hashCodeByKey(long) hashCodeByKey}({@link #getNumberKey(Object) getNumberKey}(value))</code> expression.
		 */
		@Override
		public default int hashCodeByValue(V value) {
			return hashCodeByKey(getNumberKey(value));
		}

		/**
		 * Determines if specified new value matches specified old value; called when performing value-based operations.
		 *
		 * <p>This implementation delegates to
		 * <code>({@link #getNumberKey(Object) getNumberKey}(newValue)&nbsp;==&nbsp;{@link #getNumberKey(Object) getNumberKey}(oldValue))</code> expression.
		 */
		@Override
		public default boolean matchesByValue(V newValue, V oldValue) {
			return getNumberKey(newValue) == getNumberKey(oldValue);
		}

		/**
		 * Returns object key for specified value to be used for hashing and identification;
		 * called when explicit object key is needed or when other methods delegate operations as specified.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #getNumberKey(Object) getNumberKey}(value)</code> expression.
		 */
		@Override
		public default Long getObjectKey(V value) {
			return getNumberKey(value);
		}

		/**
		 * Returns hash code for specified object key; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>{@link #hashCodeByKey(long) hashCodeByKey}(key.{@link Long#longValue() longValue}())</code> expression.
		 */
		@Override
		public default int hashCodeByKey(Long key) {
			return hashCodeByKey(key.longValue());
		}

		/**
		 * Determines if specified object key matches specified value; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>(key&nbsp;==&nbsp;{@link #getNumberKey(Object) getNumberKey}(value))</code> expression.
		 */
		@Override
		public default boolean matchesByKey(Long key, V value) {
			return key == getNumberKey(value);
		}
	}

	/**
	 * A specialized {@link IndexerFunction} that distinguishes and identifies elements using identity comparison of object keys.
	 *
	 * <p>It uses {@link System#identityHashCode(Object) System.identityHashCode(Object)} method instead of
	 * {@link Object#hashCode() Object.hashCode()} method to compute hashcode and reference comparison
	 * instead of {@link Object#equals(Object) Object.equals(Object)} method to determine identity.
	 *
	 * <p>In order to use this functional interface, cast a lambda expression to it when creating {@link IndexedSet}. For example,<br>
	 * <code>IndexedSet&lt;Key, Value&gt; set = IndexedSet.{@link IndexedSet#create(IndexerFunction) create}((IndexerFunction.IdentityKey&lt;Key, Value&gt;)Value::getKey);</code>
	 */
	@FunctionalInterface
	public interface IdentityKey<K, V> extends IndexerFunction<K, V> {
		/**
		 * Returns hash code for specified object key; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>System.{@link System#identityHashCode(Object) identityHashCode}(key)</code> expression.
		 */
		@Override
		public default int hashCodeByKey(K key) {
			return System.identityHashCode(key);
		}

		/**
		 * Determines if specified object key matches specified value; called when performing operations using object keys.
		 *
		 * <p>This implementation delegates to
		 * <code>(key&nbsp;==&nbsp;{@link #getObjectKey(Object) getObjectKey}(value))</code> expression.
		 */
		@Override
		public default boolean matchesByKey(K key, V value) {
			return key == getObjectKey(value);
		}
	}

	/**
	 * Default strategy that treats values as their own keys <b>(key&nbsp;==&nbsp;value)</b>.
	 */
	@SuppressWarnings("rawtypes")
	static final class DefaultIndexerFunction implements IndexerFunction {
		private static final long serialVersionUID = 0;

		DefaultIndexerFunction() {}

		@Override
		public Object getObjectKey(Object value) {
			return value;
		}

		// Substitute DefaultIndexer implementation for backward compatibility
		private Object writeReplace() {
			return Indexer.DEFAULT;
		}

		private Object readResolve() {
			return IndexerFunction.DEFAULT;
		}
	}

}
