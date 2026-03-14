package com.rusefi.trigger;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;
import org.yaml.snakeyaml.Yaml;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;


import static com.rusefi.config.generated.TriggerVariableRegistryValues.*;

public class TriggerWheelInfo {
    private static final Logging log = Logging.getLogging(TriggerWheelInfo.class);
    static final String DEFAULT_FILE = ".." + File.separator + "firmware/config/triggers.yaml";

    private final int id;
    private final boolean isSecondWheelCam;
    private final double tdcPosition;
    private final String triggerName;
    private final String triggerHumanName;
    private final List<ComplexTriggerSignal> signals;
    private final boolean knownOperationMode;
    private final boolean hidden;
    private final boolean useOnlyPrimaryForSync;
    private final boolean isSynchronizationNeeded;
    private final boolean shapeWithoutTdc;
    private final TriggerGaps gaps;
    private final String syncEdge;
    private final String operationMode;

    public TriggerWheelInfo(int id, double tdcPosition, String triggerName, String triggerHumanName, List<ComplexTriggerSignal> signals,
                            boolean isSecondWheelCam,
                            boolean knownOperationMode,
                            boolean hidden,
                            boolean useOnlyPrimaryForSync,
                            boolean isSynchronizationNeeded,
                            boolean shapeWithoutTdc,
                            TriggerGaps gaps, String syncEdge, String operationMode) {
        this.id = id;
        this.isSecondWheelCam = isSecondWheelCam;
        this.tdcPosition = tdcPosition;
        this.triggerName = triggerName;
        this.triggerHumanName = triggerHumanName;
        this.signals = signals;
        this.knownOperationMode = knownOperationMode;
        this.hidden = hidden;
        this.useOnlyPrimaryForSync = useOnlyPrimaryForSync;
        this.isSynchronizationNeeded = isSynchronizationNeeded;
        this.shapeWithoutTdc = shapeWithoutTdc;
        this.gaps = gaps;
        this.syncEdge = syncEdge;
        this.operationMode = operationMode;
    }

    // Get a booleaan flag from the object that has been parsed from the YAML.
    // If it's not found, use the default value from `getDefaultValue()`
    private static boolean getFlag(Map<String, Object> triggerMap, String flag) {
        return Boolean.parseBoolean(Objects.toString(triggerMap.get(flag), getDefaultValue(flag)));
    }

    static TriggerWheelInfo readTriggerWheelInfo(int id, Map<String, Object> triggerMap) {
        // If TDC offset isn't specified, use 0
        double tdcPosition = Double.parseDouble(Objects.toString(triggerMap.get("offset"), "0"));
        String triggerName = triggerMap.get("name").toString();
        String triggerHumanName = Objects.toString(triggerMap.get("humanName"), "");
        boolean hidden = Boolean.parseBoolean(String.valueOf(triggerMap.get("hidden")));
        boolean isSecondWheelCam = getFlag(triggerMap, "isSecondWheelCam");
        boolean useOnlyPrimaryForSync = getFlag(triggerMap, "useOnlyPrimaryForSync");
        boolean isSynchronizationNeeded = getFlag(triggerMap, "isSynchronizationNeeded");
        boolean shapeWithoutTdc = getFlag(triggerMap, "shapeWithoutTdc");

        // Operation mode handling is a little special to make things easier for humans.
        // If an operation mode is specified, knownOperationMode will default to true.
        // If not, it will default to false.
        // knownOperationMode will only have to be specified if an operationMode is set,
        // yet the position of the trigger wheel needs to be configurable.
        // This is an unusual scenario.
        // At the time of this writing it only applies to 36-2-2-2,
        // because it is set as a "cam" trigger in 2-stroke mode on rotaries.
        //
        // `knownOperationMode` does two things:
        // 1. If false, allows users to configure whether the trigger is:
        //   a. On the crank on a four-stroke engine
        //   b. On the cam on a four-stroke engine
        //   c. On the crank on a two-stroke engine
        // 2. If true, the mode is displayed in generated trigger images.
        String operationMode = Objects.toString(triggerMap.get("operationMode"), "");
        boolean knownOperationMode = operationMode.equals("") ? false : true;
        String knownOperationModeField = Objects.toString(triggerMap.get("knownOperationMode"), "");
        if (!knownOperationModeField.equals("")) {
            knownOperationMode = Boolean.parseBoolean(knownOperationModeField);
        }

        if (knownOperationMode && operationMode.equals("")) {
            throw new IllegalStateException("If knownOperationMode is set to true, an operationMode must also be set.");
        }

        String syncEdge = triggerMap.get("syncEdge").toString();

        @SuppressWarnings("unchecked")
        List<Map<String, Object>> events = (List<Map<String, Object>>) triggerMap.get("teeth");

        List<ComplexTriggerSignal> complexSignals = TriggerSignalReader.readSignals(events);

        // Convert repeated signals into simple signals (still ComplexTriggerSignals, but with `count = 1`)
        // We do this because it's the easiest common ground for generating TriggerSignals used by TriggerImage,
        // and ComplexTriggerSignals used by TriggerWheelInit, which have stricter requirements than we wish to impose
        // upon the humans configuring triggers.
        List<ComplexTriggerSignal> signals = new ArrayList<>();
        for (int s = 0; s < complexSignals.size(); s++) {
            ComplexTriggerSignal signal = complexSignals.get(s);
            for (int e = 0; e < signal.getCount(); e++) {
                Double mult = (signal.getCount() > 1) ? (e / ((double) signal.getCount() - 1.0)) : 0.0;
                Double angle = signal.getAngle() + ((signal.getLastAngle() - signal.getAngle()) * mult);
                signals.add(new ComplexTriggerSignal(signal.getWaveIndex(), angle, signal.getWidth()));
            }
        }

        @SuppressWarnings("unchecked")
        List<Map<String, Object>> syncGaps = (List<Map<String, Object>>) triggerMap.get("syncGaps");
        TriggerGaps gaps = null;
        int gapsCount = syncGaps.size();
        gaps = new TriggerGaps(gapsCount);
        for (int i = 0; i < gapsCount; i++) {
            gaps.gapFrom[i] = Double.parseDouble(syncGaps.get(i).get("from").toString());
            gaps.gapTo[i] = Double.parseDouble(syncGaps.get(i).get("to").toString());
        }

        return new TriggerWheelInfo(id, tdcPosition, triggerName, triggerHumanName,
                signals,
                isSecondWheelCam,
                knownOperationMode,
                hidden,
                useOnlyPrimaryForSync,
                isSynchronizationNeeded,
                shapeWithoutTdc,
                gaps,
                syncEdge,
                operationMode
        );
    }

    static void readWheels(List<String> files, TriggerWheelInfoConsumer consumer) {
        for (String file : files) {
            readWheels(file, consumer);
        }
    }

    static void readWheels(String fileName, TriggerWheelInfoConsumer consumer) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            Yaml yaml = new Yaml();

            Object obj = yaml.load(br);

            if (!(obj instanceof List)) {
                throw new IllegalStateException("Expected a list of triggers");
            }

            @SuppressWarnings("unchecked")
            List<Map<String, Object>> triggerList = (List<Map<String, Object>>) obj;

            int id = 1;
            for (Map<String, Object> triggerMap : triggerList) {
                TriggerWheelInfo triggerWheelInfo = readTriggerWheelInfo(id++, triggerMap);

                consumer.onWheel(triggerWheelInfo);
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    @NotNull
    public List<TriggerSignal> getFirstWheeTriggerSignals() {
        return getTriggerSignals(0);
    }

    public double getTdcPositionIn360() {
        return isCrankBased() ? getTdcPosition() : getCompressedAngle(getTdcPosition());
    }

    @NotNull
    private List<TriggerSignal> getTriggerSignals(int index) {
        return getSignals().stream().filter(signal -> signal.getWaveIndex() == index).collect(Collectors.toList());
    }

    @NotNull
    private List<TriggerSignal> takeFirstHalf(List<TriggerSignal> wheel) {
        return wheel.stream().filter(triggerSignal -> triggerSignal.getAngle() < 360).collect(Collectors.toList());
    }

    /**
     * this is about converting 720 cycle of crank wheel shape into normal 360 circle range
     */
    private static double getCompressedAngle(double angle) {
        return angle / 2;
    }

    // There is a similar function in trigger_central.cpp,
    // but it is different to this one because this one is
    // meant for wheels that are always defined within 0-360.
    private int getCrankDivider() {
        switch (operationMode) {
        case "OM_NONE":
        case "FOUR_STROKE_CAM_SENSOR":
        case "TWO_STROKE":
        case "FOUR_STROKE_CRANK_SENSOR":
            return 1;
        case "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR":
            return 2;
        case "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR":
            return 3;
        case "FOUR_STROKE_SIX_TIMES_CRANK_SENSOR":
            return 6;
        case "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR":
            return 12;
        }
        return 1;
    }

    // There is a similar function in trigger_structure.cpp,
    // but it is different to this one because this one is
    // meant for wheels that are always defined within 0-360.
    private int getCycleDuration() {
        switch (operationMode) {
        case "OM_NONE":
        case "FOUR_STROKE_CAM_SENSOR":
        case "FOUR_STROKE_CRANK_SENSOR":
        case "TWO_STROKE":
            return 360;
        case "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR":
            return 180;
        case "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR":
            return 120;
        case "FOUR_STROKE_SIX_TIMES_CRANK_SENSOR":
            return 60;
        case "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR":
            return 30;
        }
        return 360;
    }

    // Provide default values for unspecified fields
    public static String getDefaultValue(String flag) {
        switch (flag) {
        case "isSynchronizationNeeded":
            return "true";
        case "isSecondWheelCam":
            return "false";
        case "useOnlyPrimaryForSync":
            return "false";
        case "shapeWithoutTdc":
            return "false";
        case "knownOperationMode":
            return "true";
        case "offset":
            return "0";
        }
        return "ERROR";
    }

    public List<TriggerSignal> getSecondWheeTriggerSignals() {
        return takeFirstHalf(getTriggerSignals(1));
    }

    public int getId() {
        return id;
    }

    public double getTdcPosition() {
        return tdcPosition;
    }

    public String getTriggerName() {
        return triggerName;
    }

    public String getTriggerHumanName() {
        return triggerHumanName;
    }

    // Shift the list such that index `index` is at angle `to`.
    public <T extends TriggerEvent> void adjustSignals(List<T> inSignals, double delta) {
        for (T signal : inSignals) {
            signal.adjustAngle(delta, getCycleDuration());
        }
    }

    // Build a list of TriggerSignals used by TriggerImage
    public List<TriggerSignal> getSignals() {
        List<TriggerSignal> multSignals = new ArrayList<>();
        // We need to provide a full rotation of signals,
        // so for symmetric patterns we need to repeat the defined teeth...
        for (int i = 0; i < getCrankDivider(); i++) {
            for (int s = 0; s < signals.size(); s++) {
                ComplexTriggerSignal signal = signals.get(s);
                // ...with an offset.
                Double angle = signal.getAngle() + i * (360 / getCrankDivider());
                Double width = signal.getWidth();
                // Add rise and fall
                multSignals.add(new TriggerSignal(signal.getWaveIndex(), angle, 1));
                multSignals.add(new TriggerSignal(signal.getWaveIndex(), angle + width, 0));
            }
        }

        multSignals.sort((a, b) -> Double.compare(a.getAngle(), b.getAngle()));

        // Add gaps to the TriggerSignals, to be used by TriggerImage
        calculateGaps(multSignals);
        // Find the sync point
        int syncPoint = findSyncPoint(multSignals);
        // Rotate the signals to the sync point so the sync point is straight up in the generated image.
        Collections.rotate(multSignals, -syncPoint);
        adjustSignals(multSignals, -(multSignals.get(0).getAngle()));
        return multSignals;
    }

    // Build a list of ComplexTriggerSignals that has no overlap to simplify the firmware's initialization logic.
    public List<ComplexTriggerSignal> getComplexSignals() {
        List<ComplexTriggerSignal> expandedSignals = new ArrayList<>();
        for (int s = 0; s < signals.size(); s++) {
            ComplexTriggerSignal signal = signals.get(s);
            // Some minimum is required for trigger stimulation. I'm not sure what it is, but this seems to work.
            signal.setWidth(Math.max(signal.getWidth(), 0.5));
            // Special handling for patterns with the primary wheel on the cam and the secondary on the crank
            // The pattern needs to be repeated twice.
            if (!isCrankBased() && !isSecondWheelCam() && signal.getWaveIndex() == 1) {
                // Divide angles by two.
                Double angle = signal.getAngle() / 2;
                Double width = signal.getWidth() / 2;
                // And repeat with an offset.
                expandedSignals.add(new ComplexTriggerSignal(signal.getWaveIndex(), angle, width));
                expandedSignals.add(new ComplexTriggerSignal(signal.getWaveIndex(), angle + 180, width));
            } else {
                expandedSignals.add(signal);
            }
        }

        expandedSignals.sort((a, b) -> Double.compare(a.getAngle(), b.getAngle()));

        // We will use an iterator because we need to change the length of the list as we loop through it.
        ListIterator<ComplexTriggerSignal> csi = expandedSignals.listIterator();
        ComplexTriggerSignal ult = null;
        while (csi.hasNext()) {
            ComplexTriggerSignal current = csi.next();

            if (ult != null) {
                Double every = 0.0;
                if (ult.getCount() > 1) {
                    every = (ult.getLastAngle() - ult.getAngle()) / (ult.getCount() - 1);
                }
                // If the last signal had the same width, is on the same wheel, and the gap matches,
                // increment the last signal's `count` field and drop this signal from the list.
                if (Math.abs(ult.getWidth() - current.getWidth()) < 0.0001f
                    && ult.getWaveIndex() == current.getWaveIndex()
                    && (ult.getCount() == 1 || Math.abs((current.getAngle() - ult.getLastAngle()) - every) < 0.0001f)) {
                    ult.setLastAngle(current.getAngle());
                    ult.setCount(ult.getCount() + 1);
                    csi.remove();
                } else {
                    ult = current;
                }
            } else {
                ult = current;
            }
        }

        // Rotate the list such that the angle of the last element is at the end.
        ComplexTriggerSignal lastSignal = expandedSignals.get(expandedSignals.size() - 1);
        adjustSignals(expandedSignals, getCycleDuration() - (lastSignal.getLastAngle() + lastSignal.getWidth()));
        return expandedSignals;
    }

    // Find the index of the sync point as defined by the specified `syncGap`s.
    // This is dependent on the passed list of  TriggerSignals having gaps set by `calculateGaps()`.
    // The equivalent C++ logic happens in `isSyncPoint()` in trigger_decoder.cpp.
    public int findSyncPoint(List<TriggerSignal> inSignals) {
        // Get the edges from the signals that match `syncEdge` - the edges the trigger decoder will be using to calculate gaps.
        // Pair each with its index in `inSignals`, because that's the value we will need to return.
        List<Map.Entry<Integer, TriggerSignal>> importantEdges =
            IntStream.range(0, inSignals.size())
            .filter(i -> isImportantEdge(inSignals.get(i)))
            .mapToObj(i -> Map.entry(i, inSignals.get(i)))
            .collect(Collectors.toList());
        for (int e = 0; e < importantEdges.size(); e++)  {
            Map.Entry<Integer, TriggerSignal> edge = importantEdges.get(e);
            // If this wheel doesn't need to be synchronized, we'll just use the first important edge
            if (!isSynchronizationNeeded) {
                return edge.getKey();
            }
            boolean matchedSync = true;
            for (int g = 0; g < gaps.gapFrom.length; g++) {
                // Get the important signal preceding this one.
                // If we are on the first signal, this would be the `g`th signal from the end of the list.
                TriggerSignal compareSignal = importantEdges.get(Math.floorMod(e - g, importantEdges.size())).getValue();
                if (compareSignal.getGap() < gaps.gapFrom[g]
                    || compareSignal.getGap() > gaps.gapTo[g]) {
                    matchedSync = false;
                    break;
                }
            }
            if (matchedSync) {
                return edge.getKey();
            }
        }
        return -1;
    }

    // Set the `gap` attribute on each TriggerSignal by computing the ratio of the gap to the signal before it
    // over the gap to the signal before that.
    // | 3 |  6   |
    // 5 / 3 = 2
    public void calculateGaps(List<TriggerSignal> inSignals) {
        List<TriggerSignal> importantEdges = inSignals.stream().filter(s -> isImportantEdge(s)).collect(Collectors.toList());
        // Start by setting the last and before-last signals to the last two list elements.
        TriggerSignal ult = importantEdges.get(Math.floorMod(importantEdges.size() - 1, importantEdges.size()));
        TriggerSignal penult = importantEdges.get(Math.floorMod(importantEdges.size() - 2, importantEdges.size()));
        for (TriggerSignal signal : importantEdges) {
            // Wrap everything to 0-360.
            double ultGap = (signal.getAngle() % 360) - (ult.getAngle() % 360);
            if (ultGap < 0)
                ultGap = ultGap + 360;
            double penultGap = (ult.getAngle() % 360) - (penult.getAngle() % 360);
            if (penultGap < 0)
                penultGap = penultGap + 360;
            // Handle division by zero
            if (ultGap == penultGap) {
                signal.setGap(1.0);
            } else {
                signal.setGap(ultGap / penultGap);
            }
            penult = ult;
            ult = signal;
        }
    }

    // The C++ equivalent of this function is `shouldConsiderEdge()` in trigger_decoder.cpp.
    private boolean isImportantEdge(TriggerSignal s) {
        return (s.getWaveIndex() == 0 || ! useOnlyPrimaryForSync) &&
            ((s.getState() == 0 && ! syncEdge.equals("RiseOnly") && ! syncEdge.equals("Rise")) ||
             (s.getState() == 1 && ! syncEdge.equals("Fall")));
    }

    public boolean isCrankBased() {
        return Set.of("FOUR_STROKE_CRANK_SENSOR",
                      "FOUR_STROKE_THREE_TIMES_CRANK_SENSOR",
                      "FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR",
                      "FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR",
                      "TWO_STROKE"
                      ).contains(operationMode);
    }

    public boolean isSecondWheelCam() {
        return isSecondWheelCam;
    }

    public boolean isHasSecondChannel() {
        return signals.stream().anyMatch(e -> e.getWaveIndex() == 1);
    }

    public boolean isKnownOperationMode() {
        return knownOperationMode;
    }

    public boolean isUseOnlyPrimaryForSync() {
        return useOnlyPrimaryForSync;
    }

    public boolean isSynchronizationNeeded() {
        return isSynchronizationNeeded;
    }

    public boolean isShapeWithoutTdc() {
        return shapeWithoutTdc;
    }

    public boolean isHidden() {
        return hidden;
    }

    public TriggerGaps getGaps() {
        return gaps;
    }

    public String getSyncEdge() {
        return syncEdge;
    }

    public String getOperationMode() {
        return operationMode;
    }

    public interface TriggerWheelInfoConsumer {
        void onWheel(TriggerWheelInfo wheelInfo);
    }

    static class TriggerGaps {
        public double[] gapFrom;
        public double[] gapTo;

        public TriggerGaps(int gapTrackingLength) {
            gapFrom = new double[gapTrackingLength];
            gapTo = new double[gapTrackingLength];
        }

        @Override
        public String toString() {
            return "TriggerGaps{" +
                    "gapFrom=" + Arrays.toString(gapFrom) +
                    ", gapTo=" + Arrays.toString(gapTo) +
                    '}';
        }
    }
}
