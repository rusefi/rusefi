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

import org.apache.logging.log4j.core.Layout;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.config.Configuration;
import org.apache.logging.log4j.core.config.DefaultConfiguration;
import org.apache.logging.log4j.core.config.Node;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginConfiguration;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;
import org.apache.logging.log4j.core.layout.AbstractStringLayout;
import org.apache.logging.log4j.core.layout.ByteBufferDestination;
import org.apache.logging.log4j.core.layout.Encoder;
import org.apache.logging.log4j.core.pattern.MessagePatternConverter;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.nio.charset.Charset;
import java.util.Collections;
import java.util.Map;
import java.util.function.BiConsumer;

/**
 * Custom pattern layout for log4j2. Message formatting is delegated to {@link LogFormatter}.
 */
@SuppressWarnings("unused") //used by Log4j2
@Plugin(name = "dxFeedPatternLayout", category = Node.CATEGORY, elementType = Layout.ELEMENT_TYPE, printObject = true)
public class DxFeedPatternLayout extends AbstractStringLayout {
	private static final String LINE_SEP = DefaultLogging.getProperty("line.separator", "\n");

	private final BiConsumer<Object, StringBuilder> msgConsumer;
	private final LogFormatter logFormatter;

	private DxFeedPatternLayout(Configuration configuration) {
		super(configuration, Charset.defaultCharset(), null, null);
		MessagePatternConverter messagePatternConverter = MessagePatternConverter.newInstance(configuration, null);
		msgConsumer = (o, sb) -> {
			// Format message
			messagePatternConverter.format(o, sb);
			if (o instanceof LogEvent) {
				// Format exception
				Throwable throwable = ((LogEvent)o).getThrown();
				if (throwable != null) {
					sb.append(LINE_SEP);
					StringWriter w = new StringWriter();
					throwable.printStackTrace(new PrintWriter(w));
					sb.append(w.getBuffer());
					// Remove extra line separator
					sb.setLength(sb.length() - LINE_SEP.length());
				}
			}
		};
		logFormatter = new LogFormatter();
	}

	@Override
	public String toSerializable(LogEvent event) {
		StringBuilder text = getStringBuilder();
		String s = format(event, text).toString();
		trimToMaxSize(text);
		return s;
	}

	@Override
	public void encode(LogEvent event, ByteBufferDestination destination) {
		StringBuilder text = getStringBuilder();
		format(event, text);
		Encoder<StringBuilder> encoder = getStringBuilderEncoder();
		encoder.encode(text, destination);
		trimToMaxSize(text);
	}

	private StringBuilder format(LogEvent event, StringBuilder text) {
		char level = event.getLevel().name().charAt(0);
		logFormatter.format(level, event.getTimeMillis(), event.getThreadName(), event.getLoggerName(), msgConsumer,
			event, text);
		return text;
	}

	@Override
	public Map<String, String> getContentFormat() {
		return Collections.emptyMap();
	}

	@Override
	public String toString() {
		return getContentFormat().toString();
	}

	public static DxFeedPatternLayout createDefaultLayout() {
		return createDefaultLayout(null);
	}

	/**
	 * Creates a DxFeedPatternLayout using the default options and the given configuration. Options include using UTF-8.
	 */
	@PluginFactory
	public static DxFeedPatternLayout createDefaultLayout(@PluginConfiguration Configuration configuration) {
		if (configuration == null)
			configuration = new DefaultConfiguration();
		return new DxFeedPatternLayout(configuration);
	}
}
