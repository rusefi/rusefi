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

import java.lang.reflect.Array;
import java.util.AbstractSet;
import java.util.Collection;
import java.util.Iterator;
import java.util.Set;

/**
 * Provides a skeletal implementation of the {@link Set Set} interface to minimize the effort
 * required to implement this interface. Unlike {@link AbstractSet AbstractSet} skeletal implementation,
 * this one is more forgiving to concurrent modifications of this set during implemented bulk operations.
 */
public abstract class AbstractConcurrentSet<E> implements Set<E> {

	// ========== Construction and Clearing ==========

	/**
	 * Sole constructor; for invocation by subclass constructors, typically implicit.
	 */
	protected AbstractConcurrentSet() {}

	/**
	 * Removes all of the elements from this set.
	 * <p>
	 * This implementation iterates all elements of this set and removes them using {@link Iterator#remove()} method.
	 */
	public void clear() {
		for (Iterator<E> it = iterator(); it.hasNext();) {
			it.next();
			it.remove();
		}
	}

	// ========== Query Operations ==========

	/**
	 * Tests if this set has no elements.
	 * <p>
	 * This implementation checks emptiness using {@link #size()} method.
	 */
	public boolean isEmpty() {
		return size() == 0;
	}

	/**
	 * Returns an array containing all of the elements in this set.
	 * Obeys the general contract of the {@link Collection#toArray()} method.
	 * <p>
	 * This implementation iterates all elements of this set and adds them into the array.
	 */
	public Object[] toArray() {
		return toArrayImpl(null);
	}

	/**
	 * Returns an array containing all of the elements in this set whose runtime type
	 * is that of the specified array.
	 * Obeys the general contract of the {@link Collection#toArray(Object[])} method.
	 * <p>
	 * This implementation iterates all elements of this set and adds them into the array.
	 */
	public <T> T[] toArray(T[] a) {
		return toArrayImpl(a);
	}

	@SuppressWarnings("unchecked")
	private <T> T[] toArrayImpl(T[] a) {
		// If (a == null) then returned array shall be of exact length, otherwise it can be larger.
		int size = size(); // Atomic read.
		Object[] result = a == null ? new Object[size] : a.length >= size ? a :
			(Object[])Array.newInstance(a.getClass().getComponentType(), size);
		int n = 0;
		for (E o : this) {
			if (n >= result.length) {
				// More elements were added concurrently. Enlarge result array.
				// Grow twice in size, but do not fail when (n == 0).
				Object[] tmp = (Object[])Array.newInstance(result.getClass().getComponentType(), n + n + 1);
				System.arraycopy(result, 0, tmp, 0, n);
				result = tmp;
			}
			result[n++] = o;
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

	// ========== Bulk Operations ==========

	/**
	 * Tests if this set contains all of the elements in the specified collection.
	 * <p>
	 * This implementation iterates all elements of specified collection and tests
	 * them one-by-one using {@link #contains(Object) contains(element)} method.
	 */
	public boolean containsAll(Collection<?> c) {
		for (Object o : c)
			if (!contains(o))
				return false;
		return true;
	}

	/**
	 * Adds all of the elements in the specified collection into this set and
	 * returns <b>true</b> if this operation has increased the size of this set.
	 * <p>
	 * This implementation iterates all elements of specified collection and adds
	 * them one-by-one using {@link #add(Object) add(element)} method.
	 */
	public boolean addAll(Collection<? extends E> c) {
		boolean modified = false;
		for (E o : c)
			if (add(o))
				modified = true;
		return modified;
	}

	/**
	 * Removes all of the elements in the specified collection from this set and
	 * returns <b>true</b> if this operation has decreased the size of this set.
	 * <p>
	 * This implementation compares size of specified collection with the size of this set,
	 * then iterates smaller collection and removes elements that need to be removed.
	 */
	public boolean removeAll(Collection<?> c) {
		boolean modified = false;
		if (size() > c.size()) {
			for (Object o : c)
				if (remove(o))
					modified = true;
		} else {
			for (Iterator<E> it = iterator(); it.hasNext();)
				if (c.contains(it.next())) {
					it.remove();
					modified = true;
				}
		}
		return modified;
	}

	/**
	 * Retains only the elements in this set that are contained in the specified collection.
	 * <p>
	 * This implementation iterates all elements of this set, checks if they are contained in
	 * the specified collection, and removes them if needed using {@link Iterator#remove()} method.
	 */
	public boolean retainAll(Collection<?> c) {
		boolean modified = false;
		for (Iterator<E> it = iterator(); it.hasNext();)
			if (!c.contains(it.next())) {
				it.remove();
				modified = true;
			}
		return modified;
	}

	// ========== Comparison and Hashing ==========

	/**
	 * Compares the specified object with this set for equality.
	 * Obeys the general contract of the {@link Set#equals(Object)} method.
	 * <p>
	 * This implementation compares size of specified set with the size of this set and then
	 * checks element containment using {@link #containsAll(Collection) containsAll((Set)o)} method.
	 */
	public boolean equals(Object o) {
		return o == this || o instanceof Set && size() == ((Set)o).size() && containsAll((Set)o);
	}

	/**
	 * Returns the hash code value for this set.
	 * Obeys the general contract of the {@link Set#hashCode()} method.
	 * <p>
	 * This implementation iterates all elements of this set and adds their hash codes.
	 */
	public int hashCode() {
		int hash = 0;
		for (E o : this)
			if (o != null)
				hash += o.hashCode();
		return hash;
	}

	// ========== String Conversion ==========

	/**
	 * Returns a string representation of this set.
	 * <p>
	 * This implementation iterates all elements of this set and concatenates their string representations.
	 */
	public String toString() {
		StringBuilder sb = new StringBuilder(size() * 3 + 10);
		sb.append("[");
		String separator = "";
		for (E o : this) {
			sb.append(separator);
			sb.append(o);
			separator = ", ";
		}
		sb.append("]");
		return sb.toString();
	}
}
