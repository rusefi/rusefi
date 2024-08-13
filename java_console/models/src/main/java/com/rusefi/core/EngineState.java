package com.rusefi.core;

import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkDecoder;
import org.jetbrains.annotations.NotNull;

import java.util.*;
import java.util.function.Consumer;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Date: 12/25/12
 * Andrey Belomutskiy, (c) 2013-2020
 *
 * @see #registerStringValueAction
 */
public class EngineState {
    private static final Logging log = getLogging(EngineState.class);

    public static final String PACKING_DELIMITER = ":";
    private final Object lock = new Object();

    public void replaceStringValueAction(String key, Consumer<String> callback) {
        removeAction(key);
        registerStringValueAction(key, callback);
    }

    /**
     * text protocol key and callback associated with this key
     */
    public static class StringActionPair extends Pair<String, Consumer<String>> {
        public final String prefix;

        public StringActionPair(String key, Consumer<String> second) {
            super(key, second);
            prefix = key.toLowerCase() + Fields.LOG_DELIMITER;
        }

        @Override
        public String toString() {
            return "Pair(" + first + "," + second + "}";
        }
    }

    private final ResponseBuffer buffer;
    private final List<StringActionPair> actions = new ArrayList<>();
    private final Set<String> keys = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    public EngineState(@NotNull final EngineStateListener listener) {
        buffer = new ResponseBuffer(response -> {
            if (response != null) {
                // let's remove timestamp if we get content from a log file not controller
                int i = response.indexOf(Logger.END_OF_TIMESTAND_TAG);
                if (i != -1)
                    response = response.substring(i + Logger.END_OF_TIMESTAND_TAG.length());
                String copy = response;
                listener.beforeLine(response);
                while (!response.isEmpty())
                    response = handleResponse(response, listener);
                listener.afterLine(copy);
            }
        });

        registerStringValueAction(Fields.PROTOCOL_MSG, value -> MessagesCentral.getInstance().postMessage(EngineState.class, value));
    }

    /**
     * @param response input string
     * @param listener obviously
     * @return unused part of the response
     */
    private String handleResponse(String response, EngineStateListener listener) {
        String originalResponse = response;
        synchronized (lock) {
            for (StringActionPair pair : actions)
                response = handleStringActionPair(response, pair, listener);
        }
        if (originalResponse.length() == response.length()) {
            log.info("EngineState.unknown: " + response);
            int keyEnd = response.indexOf(Fields.LOG_DELIMITER);
            if (keyEnd == -1) {
                // discarding invalid line
                return "";
            }
            String unknownKey = response.substring(0, keyEnd);
            int valueEnd = response.indexOf(Fields.LOG_DELIMITER, keyEnd + 1);
            if (valueEnd == -1) {
                // discarding invalid line
                return "";
            }
            String value = response.substring(keyEnd, valueEnd);
            log.info("Invalid key [" + unknownKey + "] value [" + value + "]");
            // trying to process the rest of the line
            response = response.substring(valueEnd + Fields.LOG_DELIMITER.length());
        }
        return response;
    }

    public static String skipToken(String string) {
        int keyEnd = string.indexOf(Fields.LOG_DELIMITER);
        if (keyEnd == -1) {
            // discarding invalid line
            return "";
        }
        return string.substring(keyEnd + Fields.LOG_DELIMITER.length());
    }

    public static String handleStringActionPair(String response, StringActionPair pair, EngineStateListener listener) {
        if (startWithIgnoreCase(response, pair.prefix)) {
            String key = pair.first;
            int beginIndex = key.length() + 1;
            int endIndex = response.indexOf(Fields.LOG_DELIMITER, beginIndex);
            if (endIndex == -1)
                endIndex = response.length();

            String strValue = response.substring(beginIndex, endIndex);
            pair.second.accept(strValue);
            if (listener != null)
                listener.onKeyValue(key, strValue);

            response = response.substring(endIndex);
            if (!response.isEmpty())
                response = response.substring(1); // skipping the Fields.LOG_DELIMITER
            return response;
        }
        return response;
    }

    public static boolean startWithIgnoreCase(String line, String prefix) {
        /**
         * this implementation is faster than 'line.toLowerCase().startsWith(prefix)', that's important if processing
         * huge log files
         */
        int pLen = prefix.length();
        if (line.length() < pLen)
            return false;
        for (int i = 0; i < pLen; i++) {
            char lineChar = Character.toLowerCase(line.charAt(i));
            char prefixChar = prefix.charAt(i);
            if (Character.isLetter(prefixChar) && !Character.isLowerCase(prefixChar))
                throw new IllegalStateException("Not lower: " + prefix);
            if (lineChar != prefixChar)
                return false;
        }
        return true;

    }

//    private char hexChar(int value) {
//        value = value & 0xF;
//        if (value < 10)
//            return (char) ('0' + value);
//        return (char) ('A' - 10 + value);
//    }
//
//    private int parseChar(char c) {
//        if (c <= '9')
//            return c - '0';
//        return Character.toLowerCase(c) - 'a' + 10;
//    }

    public void registerStringValueAction(String key, Consumer<String> callback) {
        synchronized (lock) {
            if (keys.contains(key))
                throw new IllegalStateException("Already registered: " + key);
            keys.add(key);
            actions.add(new StringActionPair(key, callback));
        }
    }

    public void removeAction(String key) {
        synchronized (lock) {
            keys.remove(key);
            for (Iterator<StringActionPair> it = actions.iterator(); it.hasNext(); ) {
                if (it.next().first.equals(key)) {
                    it.remove();
                    break;
                }
            }
        }
    }

    public void processNewData(String append, LinkDecoder decoder) {
        buffer.append(append, decoder);
    }

    public interface EngineStateListener {
        void beforeLine(String fullLine);

        void onKeyValue(String key, String value);

        void afterLine(String fullLine);
    }

    public static class EngineStateListenerImpl implements EngineStateListener {
        public void beforeLine(String fullLine) {
        }

        @Override
        public void onKeyValue(String key, String value) {
        }

        public void afterLine(String fullLine) {
        }
    }
}
