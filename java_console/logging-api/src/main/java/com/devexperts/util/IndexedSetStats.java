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
import java.util.Locale;

/**
 * Provides access to statistics of the {@link IndexedSet} static structure.
 * Statistics are tracked only during modification operations.
 * It has self-explanatory {@link #toString()} method that can be used to periodically dump
 * information about important caches that are based on the {@code IndexedSet}.
 */
public class IndexedSetStats implements Serializable {
	private static final long serialVersionUID = 0;

	private final int payload_size;
	private final int allocated_size;
	private final long payload_distance;
	private final long amortized_cost;
	private final long mod_count;

	IndexedSetStats(int payload_size, int allocated_size, long payload_distance, long amortized_cost, long mod_count) {
		this.payload_size = payload_size;
		this.allocated_size = allocated_size;
		this.payload_distance = payload_distance;
		this.amortized_cost = amortized_cost;
		this.mod_count = mod_count;
	}

	public int getSize() {
		return payload_size;
	}

	public int getAllocatedSize() {
		return allocated_size;
	}

	public double getFillFactor() {
		return (double)payload_size / allocated_size;
	}

	public double getAverageDistance() {
		return payload_distance == 0 ? 0 : (double)payload_distance / payload_size;
	}

	public double getAmortizedCost() {
		return amortized_cost == 0 ? 0 : (double)amortized_cost / payload_size;
	}

	public long getModCount() {
		return mod_count;
	}

	@Override
	public String toString() {
		return String.format(Locale.US, "size %d, filled %.1f%%, avgdist %.3f, amortized %.3f, mods %d",
			getSize(), getFillFactor() * 100, getAverageDistance(), getAmortizedCost(), getModCount());
	}
}
