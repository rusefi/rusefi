package com.rusefi.trigger;

import com.rusefi.util.LazyFile;

import java.io.IOException;
import java.util.List;

public class TriggerWheelInit {
    StringBuilder triggerCase = new StringBuilder();

    private final String outputFileName;
    private final LazyFile.LazyFileFactory lazyFileFactory;

    public TriggerWheelInit() {
        this(LazyFile.REAL);
    }

    public TriggerWheelInit(LazyFile.LazyFileFactory lazyFileFactory) {
        this.outputFileName = "./controllers/trigger/decoders/generated_trigger_init.cpp";
        this.lazyFileFactory = lazyFileFactory;
    }

    public void process(TriggerWheelInfo wheelInfo) {
        triggerCase.append("  case trigger_type_e::").append(wheelInfo.getTriggerName()).append(":\n");

        String operationMode = wheelInfo.getOperationMode();
        if (operationMode.equals("")) {
            operationMode = "triggerOperationMode";
        }

        triggerCase.append("    initialize(" + operationMode + ", SyncEdge::" + wheelInfo.getSyncEdge() + ");\n");

        TriggerWheelInfo.TriggerGaps gaps = wheelInfo.getGaps();
				// We build an array of structs and pass it to the setEvents function.
				// This is much more memory-efficient than calling addEvent many times directly from the switch statement.
        String arrname = "events_" + wheelInfo.getTriggerName();
        for (int i = 0; i < gaps.gapFrom.length; i++) {
            triggerCase.append("    setTriggerSynchronizationGap3(" + i + ", " + gaps.gapFrom[i] + ", " + gaps.gapTo[i] + ");\n");
        }

        triggerCase.append("    static const ComplexTriggerSignal " + arrname + "[] = {\n");

        List<ComplexTriggerSignal> signals = wheelInfo.getComplexSignals();
        for (int i = 0; i < signals.size(); i++) {
            ComplexTriggerSignal signal = signals.get(i);
            String angle = String.valueOf(signal.getAngle());
            Float anglef = Float.parseFloat(angle);
            String channel = signal.getWaveIndex() == 0 ? "TriggerWheel::T_PRIMARY" : "TriggerWheel::T_SECONDARY";

            angle = String.format("%.2f", anglef);
            String width = String.format("%.2f", signal.getWidth());
            String count = String.format("%d", signal.getCount());
            String lastAngle = String.format("%.2f", signal.getLastAngle());
            triggerCase.append("      {" + channel + ", " + angle + ", " + width + ", " + count + ", " + lastAngle + "},\n");

        }
        triggerCase.append("    };\n");
        triggerCase.append("    setEvents(" + arrname + ", efi::size(" + arrname + "));\n");
        triggerCase.append("    tdcPosition = " + wheelInfo.getTdcPosition() + ";\n");

				// Set boolean properties if they don't match the default.
        setFlag("useOnlyPrimaryForSync", wheelInfo.isUseOnlyPrimaryForSync());
        setFlag("isSecondWheelCam", wheelInfo.isSecondWheelCam());
        setFlag("isSynchronizationNeeded", wheelInfo.isSynchronizationNeeded());
        setFlag("shapeWithoutTdc", wheelInfo.isShapeWithoutTdc());

        triggerCase.append("    break;\n");
    }

    public void write() throws IOException {
        if (outputFileName != null) {
            LazyFile fw = lazyFileFactory.create(outputFileName);
            fw.write(getHeader() + triggerCase.toString() + getFooter());
            fw.close();
        }
    }

    private void setFlag(String flag, boolean cond) {
        String value = String.valueOf(cond);
        if (! value.equals(TriggerWheelInfo.getDefaultValue(flag))) {
            triggerCase.append("    " + flag + " = " + value + ";\n");
        }

    }

    private String getHeader() {
        return "#include \"pch.h\"\n"
            + "#include \"trigger_structure.h\"\n"
            + "#include \"trigger_universal.h\"\n"
            + "#include \"engine_state.h\"\n"
            + "void TriggerWaveform::initializeTriggerWaveform(operation_mode_e triggerOperationMode, const trigger_config_s &triggerType, bool isCrankWheel) {\n"
            + "  initializePre(triggerType.type);\n"
            + "  switch (triggerType.type) {\n"
            + "  case trigger_type_e::TT_TOOTHED_WHEEL:\n"
            + "    initializeSkippedToothTrigger(this, triggerType.customTotalToothCount,\n"
            + "      triggerType.customSkippedToothCount, triggerOperationMode, SyncEdge::RiseOnly);\n"
            + "    break;\n";
    }

    private String getFooter() {
        return "  default:\n"
            + "    customTrigger(triggerOperationMode, this, triggerType.type);\n"
            + "  }\n"
            + "  initializePost(triggerType.type, isCrankWheel);\n"
            + "}\n";
    }
}
