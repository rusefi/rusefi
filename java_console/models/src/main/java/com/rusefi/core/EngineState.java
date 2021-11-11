package com.rusefi.core;

import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.rusefi.config.generated.Fields;
import com.rusefi.io.LinkDecoder;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.util.*;

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
    public static final Class<EngineState> ENGINE_STATE_CLASS = EngineState.class;
    private final Object lock = new Object();

    public void replaceStringValueAction(String key, ValueCallback<String> callback) {
        removeAction(key);
        registerStringValueAction(key, callback);
    }

    /**
     * text protocol key and callback associated with this key
     */
    public static class StringActionPair extends Pair<String, ValueCallback<String>> {
        public final String prefix;

        public StringActionPair(String key, ValueCallback<String> second) {
            super(key, second);
            prefix = key.toLowerCase() + Fields.LOG_DELIMETER;
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
        buffer = new ResponseBuffer(new ResponseBuffer.ResponseListener() {
            public void onResponse(String response) {
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
            }
        }
        );

        registerStringValueAction(Fields.PROTOCOL_MSG, value -> MessagesCentral.getInstance().postMessage(ENGINE_STATE_CLASS, value));
    }

    /**
     * @see #unpackString(String)
     */
    public static String packString(String a) {
        return "line" + PACKING_DELIMITER + a.length() + PACKING_DELIMITER + a;
    }

    /**
     * This method extract the content of a 'line with known length' packet
     * <p/>
     * serial protocol is not error-prone, so our simple approach is to validate the length of incoming strings
     *
     * @return null in case of error, line message if valid packed ine
     * @see #packString(String)
     */
    public static String unpackString(String message, Logger logger) {
        String prefix = "line" + PACKING_DELIMITER;
        /**
         * If we get this tag we have probably connected to the wrong port
         * todo: as of 2019 this logic maybe makes no sense any more since pure text protocol was reduce/removed?
         */
        if (message.contains(Fields.PROTOCOL_TEST_RESPONSE_TAG)) {
            JOptionPane.showMessageDialog(null, "Are you sure you are not connected to TS port?");
            return null;
        }
        if (!message.startsWith(prefix)) {
            logger.info("EngineState: unexpected header: " + message + " while looking for " + prefix);
            return null;
        }
        message = message.substring(prefix.length());
        int delimiterIndex = message.indexOf(PACKING_DELIMITER);
        if (delimiterIndex == -1) {
            logger.info("Delimiter not found in: " + message);
            return null;
        }
        String lengthToken = message.substring(0, delimiterIndex);
        int expectedLen;
        try {
            expectedLen = Integer.parseInt(lengthToken);
        } catch (NumberFormatException e) {
            logger.info("invalid len: " + lengthToken);
            return null;
        }

        String response = message.substring(delimiterIndex + 1);
        if (response.length() != expectedLen) {
            logger.info("message len does not match header: " + message);
            response = null;
        }
        return response;
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
            int keyEnd = response.indexOf(Fields.LOG_DELIMETER);
            if (keyEnd == -1) {
                // discarding invalid line
                return "";
            }
            String unknownKey = response.substring(0, keyEnd);
            int valueEnd = response.indexOf(Fields.LOG_DELIMETER, keyEnd + 1);
            if (valueEnd == -1) {
                // discarding invalid line
                return "";
            }
            String value = response.substring(keyEnd, valueEnd);
            log.info("Invalid key [" + unknownKey + "] value [" + value + "]");
            // trying to process the rest of the line
            response = response.substring(valueEnd + Fields.LOG_DELIMETER.length());
        }
        return response;
    }

    public static String skipToken(String string) {
        int keyEnd = string.indexOf(Fields.LOG_DELIMETER);
        if (keyEnd == -1) {
            // discarding invalid line
            return "";
        }
        return string.substring(keyEnd + Fields.LOG_DELIMETER.length());
    }

    public static String handleStringActionPair(String response, StringActionPair pair, EngineStateListener listener) {
        if (startWithIgnoreCase(response, pair.prefix)) {
            String key = pair.first;
            int beginIndex = key.length() + 1;
            int endIndex = response.indexOf(Fields.LOG_DELIMETER, beginIndex);
            if (endIndex == -1)
                endIndex = response.length();

            String strValue = response.substring(beginIndex, endIndex);
            pair.second.onUpdate(strValue);
            if (listener != null)
                listener.onKeyValue(key, strValue);

            response = response.substring(endIndex);
            if (!response.isEmpty())
                response = response.substring(1); // skipping the Fields.LOG_DELIMETER
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

    public void registerStringValueAction(String key, ValueCallback<String> callback) {
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

    public interface ValueCallback<V> {
        static final ValueCallback<?> VOID = new ValueCallback() {
            @Override
            public void onUpdate(Object value) {

            }
        };

        void onUpdate(V value);
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
