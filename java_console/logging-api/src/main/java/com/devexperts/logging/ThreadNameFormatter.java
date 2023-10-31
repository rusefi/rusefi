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
package com.devexperts.logging;

import com.devexperts.util.IndexedSet;
import com.devexperts.util.QuickSort;
import com.devexperts.util.SynchronizedIndexedSet;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

class ThreadNameFormatter implements Comparable<ThreadNameFormatter> {

	/**
	 * Configuration as a set of pairs (pattern, replacement).
	 */
	private static final Map<Pattern, String> PATTERNS = new LinkedHashMap<>();

	private static final int MAX_NAME_CONVERSIONS_CACHE_SIZE = 1000;

	/**
	 * Thread name replacement cache: (thread name, replacement string).
	 */
	private static final IndexedSet<String, ThreadNameFormatter> NAME_CONVERSIONS = SynchronizedIndexedSet.create(ThreadNameFormatter::getThreadName);

	static {
		loadPatterns();
	}

	private static void loadPatterns() {
		InputStream config_input_stream = null;
		try {
			String config_path = DefaultLogging.getProperty(LogFormatter.CONFIG_FILE_PROPERTY, null);
			if (config_path != null)
				throw new UnsupportedOperationException("loadPatterns");
/*
				try {
					config_input_stream = new URLInputStream(config_path);
				} catch (IOException e) {
					System.err.println("Cannot find log formatter configuration file: '" + config_path + "'");
					System.err.println("No thread name conversion will be performed.");
					return;
				}
*/			else
				config_input_stream = LogFormatter.class.getResourceAsStream(LogFormatter.DEFAULT_CONFIG_FILE);

			if (config_input_stream == null)
				return;

			BufferedReader reader = new BufferedReader(
				new InputStreamReader(config_input_stream, StandardCharsets.ISO_8859_1));

			Pattern config_line_pattern = Pattern.compile("((?:[^=]|(?:\\\\=))*[^\\\\=])=(.*)");
			Pattern whitespace_line_pattern = Pattern.compile("\\s*");
			Pattern comment_line_pattern = Pattern.compile("#.*|!.*");
			String line;
			Set<String> patterns_set = new HashSet<>();
			while ((line = reader.readLine()) != null) {
				Matcher config_line_matcher = config_line_pattern.matcher(line);
				// If it is whitespace or comment line
				if (whitespace_line_pattern.matcher(line).matches() || comment_line_pattern.matcher(line).matches())
					continue;
				if (!config_line_matcher.matches()) {
					System.err.println("The following line cannot be parsed in log formatter configuration file: '" + line + "'");
					continue;
				}
				String config_pattern = config_line_matcher.group(1);
				String config_replacement = config_line_matcher.group(2);
				if (!patterns_set.add(config_pattern)) {
					System.err.println("Duplicate pattern found in log formatter configuration file: '" + config_pattern + "'");
					continue;
				}
				try {
					PATTERNS.put(Pattern.compile(config_pattern), config_replacement);
				} catch (PatternSyntaxException e) {
					System.err.println("Cannot parse config pattern in log formatter configuration file: '" + config_pattern + "'");
				}
			}
		} catch (IOException e) {
			// Do not wish to log using logger until initialization has completed.
			System.err.println("Cannot read log formatter configuration file");
			e.printStackTrace(System.err);
		} finally {
			try {
				if (config_input_stream != null) {
					config_input_stream.close();
				}
			} catch (IOException e) {
				// Do not wish to log using logger until initialization has completed.
				System.err.println("Cannot close log formatter configuration file");
				e.printStackTrace(System.err);
			}
		}
	}

	/**
	 * Formats thread name according to thread name conversion rules.
	 *
	 * @return Formatted thread name
	 */
	static String formatThreadName(long time, String thread_name) {
		ThreadNameFormatter entry = NAME_CONVERSIONS.getByKey(thread_name);
		if (entry == null) {
			cleanupNameConversionsIfNeeded();
			entry = new ThreadNameFormatter(thread_name, calculateThreadNameReplacement(thread_name));
			NAME_CONVERSIONS.put(entry);
		}
		entry.last_time = time;
		return entry.replacement_name;
	}

	private static void cleanupNameConversionsIfNeeded() {
		if (NAME_CONVERSIONS.size() <= MAX_NAME_CONVERSIONS_CACHE_SIZE)
			return; // everything is Ok

		synchronized (NAME_CONVERSIONS) {
			if (NAME_CONVERSIONS.size() <= MAX_NAME_CONVERSIONS_CACHE_SIZE)
				return; // everything is Ok
			ThreadNameFormatter[] entries = NAME_CONVERSIONS.toArray(new ThreadNameFormatter[NAME_CONVERSIONS.size()]);
			QuickSort.sort(entries);
			for (int i = 0; i < entries.length - MAX_NAME_CONVERSIONS_CACHE_SIZE / 2; i++)
				NAME_CONVERSIONS.removeKey(entries[i].thread_name);
		}
	}

	private static String calculateThreadNameReplacement(String thread_name) {
		for (Map.Entry<Pattern, String> entry : PATTERNS.entrySet()) {
			Matcher matcher = entry.getKey().matcher(thread_name);
			if (matcher.matches()) {
				String config_replacement = entry.getValue();
				try {
					return matcher.replaceAll(config_replacement);
				} catch (IndexOutOfBoundsException e) {
					// The replacement string refers to a capturing group that does not exist in the pattern.
					// To prevent cycling log it as is.
					// Incorrect replacement. Just use thread name.
					System.err.println("Cannot parse replacement string in log formatter configuration file: '" + config_replacement + "'");
				}
			}
		}
		return thread_name;
	}

	final String thread_name;
	final String replacement_name;
	long last_time; // Atomicity, visibility and consistency of this field are unimportant.

	ThreadNameFormatter(String thread_name, String replacement_name) {
		this.thread_name = thread_name;
		this.replacement_name = replacement_name;
	}

	private String getThreadName() {
		return thread_name;
	}

	public int compareTo(ThreadNameFormatter o) {
		return last_time < o.last_time ? -1 : last_time > o.last_time ? 1 : 0;
	}
}
