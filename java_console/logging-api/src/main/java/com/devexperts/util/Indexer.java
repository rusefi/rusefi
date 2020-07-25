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
 *
 * <p>The <b>Indexer</b> is {@link Serializable}, so that all concrete subclasses
 * shall be <b>serializable</b> in order to support serialization of indexed set and map..
 *
 * @deprecated Use a functional interface {@link IndexerFunction} instead.
 */
@Deprecated
public abstract class Indexer<K, V> implements IndexerFunction<K, V> {
	private static final long serialVersionUID = 0L;

	/**
	 * Default strategy that treats values as their own keys <b>(key&nbsp;==&nbsp;value)</b> and delegates to
	 * {@link Object#hashCode() Object.hashCode()} and {@link Object#equals(Object) Object.equals(Object)}
	 * methods as appropriate. This strategy does not support primitive keys.
	 *
	 * <p>This strategy basically turns {@link IndexedSet} into plain hash set of objects and {@link IndexedMap}
	 * into a self-reference mapping.
	 */
	@SuppressWarnings("rawtypes")
	public static final Indexer DEFAULT = new DefaultIndexer();

	// ========== Standard Subclasses ==========

	/**
	 * Default strategy that treats values as their own keys <b>(key&nbsp;==&nbsp;value)</b>.
	 */
	@SuppressWarnings("rawtypes")
	static final class DefaultIndexer extends Indexer {
		private static final long serialVersionUID = 0;

		DefaultIndexer() {}

		@Override
		public Object getObjectKey(Object value) {
			return value;
		}

		@SuppressWarnings("ReadResolveAndWriteReplaceProtected")
		public Object readResolve() {
			return Indexer.DEFAULT;
		}
	}

	static class DelegateIndexer<K, V> extends Indexer<K, V> {
		private static final long serialVersionUID = 0L;

		private final IndexerFunction<K, V> indexer;

		DelegateIndexer(IndexerFunction<K, V> indexer) {
			this.indexer = indexer;
		}

		@Override
		public K getObjectKey(V value) {
			return indexer.getObjectKey(value);
		}

		@Override
		public int hashCodeByKey(K key) {
			return indexer.hashCodeByKey(key);
		}

		@Override
		public boolean matchesByKey(K key, V value) {
			return indexer.matchesByKey(key, value);
		}

		@Override
		public int hashCodeByValue(V value) {
			return indexer.hashCodeByValue(value);
		}

		@Override
		public boolean matchesByValue(V newValue, V oldValue) {
			return indexer.matchesByValue(newValue, oldValue);
		}

		@Override
		public long getNumberKey(V value) {
			return indexer.getNumberKey(value);
		}

		@Override
		public int hashCodeByKey(long key) {
			return indexer.hashCodeByKey(key);
		}

		@Override
		public boolean matchesByKey(long key, V value) {
			return indexer.matchesByKey(key, value);
		}
	}
}
