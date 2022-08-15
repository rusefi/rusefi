package com.rusefi.trigger;

import com.rusefi.config.generated.Fields;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static com.rusefi.config.generated.Fields.*;

public class TriggerWheelInfo {
    private static final String TRIGGERTYPE = "TRIGGERTYPE";
    static final String DEFAULT_WORK_FOLDER = ".." + File.separator + "unit_tests";

    private final int id;
    private final boolean isSecondWheelCam;
    private final double tdcPosition;
    private final String triggerName;
    private final List<TriggerSignal> signals;
    private final boolean isCrankBased;
    private final boolean hasSecondChannel;
    private final boolean hardcodedOperationMode;
    private final TriggerGaps gaps;

    public TriggerWheelInfo(int id, double tdcPosition, String triggerName, List<TriggerSignal> signals,
                            boolean isCrankBased,
                            boolean isSecondWheelCam,
                            boolean hasSecondChannel,
                            boolean hardcodedOperationMode, TriggerGaps gaps) {
        this.id = id;
        this.isSecondWheelCam = isSecondWheelCam;
        this.tdcPosition = tdcPosition;
        this.triggerName = triggerName;
        this.signals = signals;
        this.isCrankBased = isCrankBased;
        this.hasSecondChannel = hasSecondChannel;
        this.hardcodedOperationMode = hardcodedOperationMode;
        this.gaps = gaps;
    }

    private static TriggerWheelInfo readTriggerWheelInfo(String line, BufferedReader reader) throws IOException {
        String[] tokens = line.split(" ");
        String idStr = tokens[1];
        int eventCount = Integer.parseInt(tokens[2]);
        String triggerName = tokens[3];
        System.out.println("Processing " + line + " " + idStr);

        int id = Integer.parseInt(idStr);
        double tdcPosition = Double.parseDouble(tokens[4]);

        System.out.println("id=" + id + ", count=" + eventCount + ", name=" + triggerName);


        boolean isCrankBased = false;
        boolean isSecondWheelCam = false;
        boolean hasSecondChannel = false;
        boolean hardcodedOperationMode = false;
        TriggerWheelInfo.TriggerGaps gaps = null;
        while (true) {
            line = reader.readLine();
            if (line == null || line.trim().startsWith("#"))
                break; // here we will read until first comment line which designates end of key-value pairs section
            String[] keyValue = line.split("=");
            if (keyValue.length != 2)
                throw new IllegalStateException("Key/value lines expected: [" + line + "]");
            String key = keyValue[0];
            String value = keyValue[1];
            if (key.startsWith(TRIGGER_GAP_FROM)) {
                int index = getIndex(key);
                gaps.gapFrom[index] = Double.parseDouble(value);
                continue;
            }
            if (key.startsWith(TRIGGER_GAP_TO)) {
                int index = getIndex(key);
                gaps.gapTo[index] = Double.parseDouble(value);
                continue;
            }
            switch (key) {
                case TRIGGER_GAPS_COUNT:
                    int gapTrackingLength = Integer.parseInt(value);
                    gaps = new TriggerGaps(gapTrackingLength);
                    break;
                case TRIGGER_IS_CRANK_KEY:
                    isCrankBased = Boolean.parseBoolean(value);
                    break;
                case TRIGGER_IS_SECOND_WHEEL_CAM:
                    isSecondWheelCam = Boolean.parseBoolean(value);
                    break;
                case TRIGGER_HAS_SECOND_CHANNEL:
                    hasSecondChannel = Boolean.parseBoolean(value);
                    break;
                case TRIGGER_HARDCODED_OPERATION_MODE:
                    hardcodedOperationMode = Boolean.parseBoolean(value);
                    break;
                default:
                    throw new IllegalStateException("Unexpected key/value: " + line);
            }
        }

        List<TriggerSignal> signals = TriggerSignalReader.readSignals(reader, eventCount);

        return new TriggerWheelInfo(id, tdcPosition, triggerName,
                signals,
                isCrankBased,
                isSecondWheelCam,
                hasSecondChannel,
                hardcodedOperationMode,
                gaps
        );
    }

    private static int getIndex(String key) {
        return Integer.parseInt(key.split("\\.")[1]);
    }

    static void readWheels(String workingFolder, TriggerWheelInfoConsumer consumer) {
        String fileName = workingFolder + File.separator + Fields.TRIGGERS_FILE_NAME;
        BufferedReader br;
        try {
            br = new BufferedReader(new FileReader(fileName));

            System.out.println("Reading " + fileName);
            String line;
            while ((line = br.readLine()) != null) {
                if (line.trim().startsWith("#")) {
                    // skipping a comment
                    continue;
                }

                if (line.startsWith(TRIGGERTYPE)) {
                    TriggerWheelInfo triggerWheelInfo = readTriggerWheelInfo(line, br);

                    consumer.onWheel(triggerWheelInfo);
                }
            }
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    @NotNull
    public List<TriggerSignal> getFirstWheeTriggerSignals() {
        List<TriggerSignal> firstWheel = getTriggerSignals(0);
        // todo: support symmetrical crank
        if (isCrankBased) {
            return takeFirstHalf(firstWheel);
        } else {
            return compressAngle(firstWheel);
        }
    }

    public double getTdcPositionIn360() {
        return isCrankBased ? getTdcPosition() : getCompressedAngle(getTdcPosition());
    }

    @NotNull
    private List<TriggerSignal> getTriggerSignals(int index) {
        return getSignals().stream().filter(signal -> signal.getWaveIndex() == index).collect(Collectors.toList());
    }

    @NotNull
    private List<TriggerSignal> takeFirstHalf(List<TriggerSignal> wheel) {
        return wheel.stream().filter(triggerSignal -> triggerSignal.getAngle() < 360).collect(Collectors.toList());
    }

    @NotNull
    private static List<TriggerSignal> compressAngle(List<TriggerSignal> wheel) {
        return wheel.stream().map(triggerSignal -> {
            double compressAngle = getCompressedAngle(triggerSignal.getAngle());
            return new TriggerSignal(triggerSignal.getWaveIndex(), triggerSignal.getState(), compressAngle);
        }).collect(Collectors.toList());
    }

    /**
     * this is about converting 720 cycle of crank wheel shape into normal 360 circle range
     */
    private static double getCompressedAngle(double angle) {
        return angle / 2;
    }

    public List<TriggerSignal> getSecondWheeTriggerSignals() {
        List<TriggerSignal> secondWheel = getTriggerSignals(1);
        if (isSecondWheelCam) {
            return compressAngle(secondWheel);
        } else {
            return takeFirstHalf(secondWheel);
        }
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

    public List<TriggerSignal> getSignals() {
        return signals;
    }

    public boolean isCrankBased() {
        return isCrankBased;
    }

    public boolean isHasSecondChannel() {
        return hasSecondChannel;
    }

    public boolean isHardcodedOperationMode() {
        return hardcodedOperationMode;
    }

    public TriggerGaps getGaps() {
        return gaps;
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
