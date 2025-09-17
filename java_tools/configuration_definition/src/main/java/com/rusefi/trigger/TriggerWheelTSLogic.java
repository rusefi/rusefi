package com.rusefi.trigger;

import com.rusefi.VariableRegistry;

import static com.rusefi.trigger.TriggerWheelInfo.DEFAULT_WORK_FOLDER;

public class TriggerWheelTSLogic {

    private static final String TRIGGER_TYPE_WITHOUT_KNOWN_LOCATION = "TRIGGER_TYPE_WITHOUT_KNOWN_LOCATION";
    private static final String TRIGGER_TYPE_WITH_SECOND_WHEEL = "TRIGGER_TYPE_WITH_SECOND_WHEEL";
    private static final String TRIGGER_CRANK_BASED = "TRIGGER_CRANK_BASED";

    public static void main(String[] args) {
        // sandbox code
        VariableRegistry variableRegistry = new VariableRegistry();
        new TriggerWheelTSLogic().execute(DEFAULT_WORK_FOLDER, variableRegistry);
    }

    public void execute(String folder, VariableRegistry variableRegistry) {
        if (folder == null) {
            System.out.println(getClass() + ": Folder not specified");
            return;
        }
        // initial value has special provision for TT_CUSTOM_WHEEL
        StringBuilder triggerTypesWithoutKnownLocation = new StringBuilder("trigger_type == 0");
        StringBuilder triggerTypesWithSecondWheel = new StringBuilder();
        StringBuilder triggerTypesCrankBased = new StringBuilder();


        TriggerWheelInfo.readWheels(folder, wheelInfo -> {
//            System.out.println("onWheel " + wheelInfo.getTriggerName());

            if (!wheelInfo.isKnownOperationMode()) {
                appendOrIfNotEmpty(triggerTypesWithoutKnownLocation);
                triggerTypesWithoutKnownLocation.append("trigger_type == ").append(wheelInfo.getId());
            }

            if (wheelInfo.isHasSecondChannel()) {
                appendOrIfNotEmpty(triggerTypesWithSecondWheel);
                triggerTypesWithSecondWheel.append("trigger_type == ").append(wheelInfo.getId());
            }

            if (wheelInfo.isCrankBased() && wheelInfo.isKnownOperationMode()) {
                appendOrIfNotEmpty(triggerTypesCrankBased);
                triggerTypesCrankBased.append("trigger_type == ").append(wheelInfo.getId());
            }

        });

        /*
         * these are templated into tunerstudio.template.ini file
         * note that TT_TOOTHED_WHEEL is not mentioned in the meta file, we handle it manually right in tunerstudio.template.ini file
         */
        variableRegistry.register(TRIGGER_TYPE_WITHOUT_KNOWN_LOCATION, triggerTypesWithoutKnownLocation.toString());
        variableRegistry.register(TRIGGER_TYPE_WITH_SECOND_WHEEL, triggerTypesWithSecondWheel.toString());
        variableRegistry.register(TRIGGER_CRANK_BASED, triggerTypesCrankBased.toString());
    }

    private void appendOrIfNotEmpty(StringBuilder triggerTypesWithSecondWheel) {
        if (triggerTypesWithSecondWheel.length() > 0)
            triggerTypesWithSecondWheel.append(" || ");
    }
}
