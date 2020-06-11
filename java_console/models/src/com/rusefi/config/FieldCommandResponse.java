package com.rusefi.config;

import com.rusefi.config.generated.Fields;
import com.rusefi.core.Pair;

import static com.rusefi.config.FieldType.*;

public class FieldCommandResponse {
    private static final String BIT_VALUE_PREFIX = "bit" + Fields.CONSOLE_DATA_PROTOCOL_TAG;
    private static final String INT_VALUE_PREFIX = INT_TYPE_STRING + Fields.CONSOLE_DATA_PROTOCOL_TAG;
    private static final String FLOAT_VALUE_PREFIX = FLOAT_TYPE_STRING + Fields.CONSOLE_DATA_PROTOCOL_TAG;

    public static Pair<Integer, ?> parseResponse(String message) {
        try {
            int atPosition = message.indexOf(Fields.CONSOLE_DATA_PROTOCOL_TAG);
            if (atPosition == -1)
                return null;
            String firstToken = message.substring(0, atPosition);
            if (firstToken.equals(INT_TYPE_STRING) || firstToken.equals(BYTE_TYPE_STRING) || firstToken.equals(SHORT_TYPE_STRING)) {
                message = message.substring(atPosition + Fields.CONSOLE_DATA_PROTOCOL_TAG.length());
                String[] a = message.split(" is ");
                if (a.length != 2)
                    return null;
                int index = Integer.parseInt(a[0]);
                int value = Integer.parseInt(a[1]);
                return new Pair<>(index, value);
            }
            if (isFloatValueMessage(message)) {
                message = message.substring(FLOAT_VALUE_PREFIX.length());
                String[] a = message.split(" is ");
                if (a.length != 2)
                    return null;
                int index = Integer.parseInt(a[0]);
                double value = Double.parseDouble(a[1]);
                return new Pair<>(index, value);
            }
            return null;
        } catch (NumberFormatException e) {
            return null;
        }

    }

    public static boolean isIntValueMessage(String message) {
        return message.startsWith(INT_VALUE_PREFIX) || message.startsWith(BYTE_TYPE_STRING) || message.startsWith(SHORT_TYPE_STRING);
    }

    public static boolean isBitValueMessage(String message) {
        return message.startsWith(BIT_VALUE_PREFIX);
    }

    public static boolean isFloatValueMessage(String message) {
        return message.startsWith(FLOAT_VALUE_PREFIX);
    }
}
