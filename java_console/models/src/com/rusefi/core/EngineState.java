package com.rusefi.core;

import com.rusefi.FileLog;
import com.rusefi.SensorConversion;
import com.rusefi.io.LinkDecoder;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.util.*;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Date: 12/25/12
 * (c) Andrey Belomutskiy
 *
 * @see #registerStringValueAction
 */
public class EngineState {
    public static final String SEPARATOR = ",";
    public static final ValueCallback<String> NOTHING = new ValueCallback<String>() {
        @Override
        public void onUpdate(String value) {
        }
    };
    public static final String PACKING_DELIMITER = ":";
    public static final String RUS_EFI_VERSION_TAG = "rusEfiVersion";
    public static final String OUTPIN_TAG = "outpin";
    /**
     * If we get this tag we have probably connected to the wrong port
     */
    private static final CharSequence TS_PROTOCOL_TAG = "ts_p_al";
    private final Object lock = new Object();

    public void replaceStringValueAction(String key, ValueCallback<String> callback) {
        removeAction(key);
        registerStringValueAction(key, callback);
    }

    /**
     * text protocol key and callback associated with this key
     */
    private static class StringActionPair extends Pair<String, ValueCallback<String>> {
        public final String prefix;

        StringActionPair(String key, ValueCallback<String> second) {
            super(key, second);
            prefix = key.toLowerCase() + SEPARATOR;
        }

        @Override
        public String toString() {
            return "Pair(" + first + "," + second + "}";
        }
    }

    public final List<EngineTimeListener> timeListeners = new CopyOnWriteArrayList<>();

    private final ResponseBuffer buffer;
    private final List<StringActionPair> actions = new ArrayList<>();
    private final Set<String> keys = new TreeSet<>(String.CASE_INSENSITIVE_ORDER);

    public EngineState(@NotNull final EngineStateListener listener) {
        buffer = new ResponseBuffer(new ResponseBuffer.ResponseListener() {
            public void onResponse(String response) {
                if (response != null) {
                    int i = response.indexOf(FileLog.END_OF_TIMESTAND_TAG);
                    if (i != -1)
                        response = response.substring(i + FileLog.END_OF_TIMESTAND_TAG.length());
                    String copy = response;
                    listener.beforeLine(response);
                    while (!response.isEmpty())
                        response = handleResponse(response, listener);
                    listener.afterLine(copy);
                }
            }
        }
        );

        SensorCentral.getInstance().initialize(this);

//        SensorStats.startRangeMeasurement(Sensor.CLT, Sensor.COOLANT_WIDTH);
//        SensorStats.startRangeMeasurement(Sensor.IAT, Sensor.INTAKE_AIR_WIDTH);
        SensorStats.startRangeMeasurement(Sensor.VREF, Sensor.VREF_WIDTH);

        SensorStats.startStandardDeviation(Sensor.DWELL0, Sensor.DWELL0_SD);
        SensorStats.startDelta(Sensor.INJECTOR_1_DWELL, Sensor.INJECTOR_2_DWELL, Sensor.INJ_1_2_DELTA);
        SensorStats.startDelta(Sensor.INJECTOR_3_DWELL, Sensor.INJECTOR_4_DWELL, Sensor.INJ_3_4_DELTA);

        SensorStats.startConversion(Sensor.CLT, Sensor.CLT_F, SensorConversion.C_TO_F);
        SensorStats.startConversion(Sensor.IAT, Sensor.IAT_F, SensorConversion.C_TO_F);
        SensorStats.startConversion(Sensor.MAFR, Sensor.MAFR_CFM, new SensorConversion() {
                    @Override
                    public double convertValue(double value) {
                        return value / 2.081989116;
                    }
                });

        registerStringValueAction("IAT", NOTHING);
        registerStringValueAction("int_temp", NOTHING);
        registerStringValueAction("mode", NOTHING);
        registerStringValueAction("TPS", NOTHING);

        registerStringValueAction("adcfast_co", NOTHING);
        registerStringValueAction("adcfast_max", NOTHING);
        registerStringValueAction("adcfast_min", NOTHING);
        registerStringValueAction("key", NOTHING);
        registerStringValueAction("value", NOTHING);

        registerStringValueAction("msg", new ValueCallback<String>() {
            @Override
            public void onUpdate(String value) {
                MessagesCentral.getInstance().postMessage(EngineState.class, value);
            }
        });

        registerStringValueAction("advance", NOTHING);
        registerStringValueAction("dwell", NOTHING);

        registerStringValueAction("offset0", NOTHING);
        registerStringValueAction("offset1", NOTHING);
        registerStringValueAction("crank_period", NOTHING);
        registerStringValueAction("ckp_c", NOTHING);
        registerStringValueAction("ev0", NOTHING);
        registerStringValueAction("ev1", NOTHING);
        registerStringValueAction("adc10", NOTHING);

        registerStringValueAction("isC", NOTHING);
        registerStringValueAction("p_d", NOTHING);
        registerStringValueAction("s_d", NOTHING);
        registerStringValueAction("pEC", NOTHING);
        registerStringValueAction("sEC", NOTHING);
        registerStringValueAction("wEC", NOTHING);
        registerStringValueAction("wWi", NOTHING);
        registerStringValueAction("wWi2", NOTHING);

        registerStringValueAction("cid", NOTHING);
        registerStringValueAction("i_d", NOTHING);
        registerStringValueAction("i_p", NOTHING);
        registerStringValueAction("a_time", NOTHING);

        registerStringValueAction("time", new ValueCallback<String>() {
            public void onUpdate(String value) {
                double time;
                try {
                    time = Double.parseDouble(value);
                } catch (NumberFormatException e) {
                    return;
                }
                listener.onTime(time);
                for (EngineTimeListener l : timeListeners)
                    l.onTime(time);
            }
        });
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
    public static String unpackString(String message) {
        String prefix = "line" + PACKING_DELIMITER;
        if (message.contains(TS_PROTOCOL_TAG)) {
            JOptionPane.showMessageDialog(null, "Are you sure you are not connected to TS port?");
            return null;
        }
        if (!message.startsWith(prefix)) {
            FileLog.MAIN.logLine("EngineState: unexpected header: " + message + " while looking for " + prefix);
            return null;
        }
        message = message.substring(prefix.length());
        int delimiterIndex = message.indexOf(PACKING_DELIMITER);
        if (delimiterIndex == -1) {
            FileLog.MAIN.logLine("Delimiter not found in: " + message);
            return null;
        }
        String lengthToken = message.substring(0, delimiterIndex);
        int expectedLen;
        try {
            expectedLen = Integer.parseInt(lengthToken);
        } catch (NumberFormatException e) {
            FileLog.MAIN.logLine("invalid len: " + lengthToken);
            return null;
        }

        String response = message.substring(delimiterIndex + 1);
        if (response.length() != expectedLen) {
            FileLog.MAIN.logLine("message len does not match header: " + message);
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
            FileLog.MAIN.logLine("EngineState.unknown: " + response);
            int keyEnd = response.indexOf(SEPARATOR);
            if (keyEnd == -1) {
                // discarding invalid line
                return "";
            }
            String unknownKey = response.substring(0, keyEnd);
            int valueEnd = response.indexOf(SEPARATOR, keyEnd + 1);
            if (valueEnd == -1) {
                // discarding invalid line
                return "";
            }
            String value = response.substring(keyEnd, valueEnd);
            FileLog.MAIN.logLine("Invalid key [" + unknownKey + "] value [" + value + "]");
            // trying to process the rest of the line
            response = response.substring(valueEnd + SEPARATOR.length());
        }
        return response;
    }

    private String handleStringActionPair(String response, StringActionPair pair, EngineStateListener listener) {
        if (startWithIgnoreCase(response, pair.prefix)) {
            String key = pair.first;
            int beginIndex = key.length() + 1;
            int endIndex = response.indexOf(SEPARATOR, beginIndex);
            if (endIndex == -1)
                endIndex = response.length();

            String strValue = response.substring(beginIndex, endIndex);
            pair.second.onUpdate(strValue);
            listener.onKeyValue(key, strValue);

            response = response.substring(endIndex);
            if (!response.isEmpty())
                response = response.substring(1); // skipping the separator
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

    public interface EngineStateListener extends EngineTimeListener {
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

        public void onTime(double time) {
        }
    }
}
