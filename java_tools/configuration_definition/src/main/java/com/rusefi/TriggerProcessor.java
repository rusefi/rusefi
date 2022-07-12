package com.rusefi;

import com.rusefi.models.trigger.WheelMetaReader;

import java.io.FileNotFoundException;

public class TriggerProcessor {
    /**
     * this method is used for manual testing only
     */
    public static void main(String[] args) throws FileNotFoundException {
        WheelMetaReader.readTriggers("../../" + WheelMetaReader.CONTROLLERS_TRIGGER_DECODERS_TRIGGERS_META_YAML);
    }
}
