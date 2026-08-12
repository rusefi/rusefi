package com.rusefi.functional_tests;

import com.devexperts.logging.Logging;
import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.IoUtil;
import com.rusefi.Timeouts;
import com.rusefi.config.generated.Integration;
import com.rusefi.core.ISensorCentral;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.WellKnownGauges;
import com.rusefi.enums.engine_type_e;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;
import com.rusefi.waves.EngineReport;
import org.jetbrains.annotations.NotNull;

import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.IoUtil.*;
import static com.rusefi.waves.EngineReport.isCloseEnough;

public class EcuTestHelper {
    public static final Function<String, Object> FAIL = errorCode -> {
        if (errorCode != null)
            throw new IllegalStateException("Failed " + errorCode);
        return null;
    };
    private static final Logging log = getLogging(EcuTestHelper.class);

    public static int currentEngineType;
    public final CommandQueue commandQueue;
    @NotNull
    private final LinkManager linkManager;

    public EcuTestHelper(LinkManager linkManager) {
        this.commandQueue = linkManager.getCommandQueue();
        this.linkManager = linkManager;
    }

    public static void assertRpmDoesNotJump(int rpm, int settleTime, int testDuration, Function<String, Object> callback, CommandQueue commandQueue) {
        IoUtil.changeRpm(commandQueue, rpm);
        sleepSeconds(settleTime);
        AtomicReference<String> result = new AtomicReference<>();
        long start = System.currentTimeMillis();

        ISensorCentral.ListenerToken listener = SensorCentral.getInstance().addListener(WellKnownGauges.RPMGauge.getOutputChannelName(), actualRpm -> {
            if (!isCloseEnough(rpm, actualRpm)) {
                long seconds = (System.currentTimeMillis() - start) / 1000;
                result.set("Got " + actualRpm + " while trying to stay at " + rpm + " after " + seconds + " seconds");
            }
        });

        sleepSeconds(testDuration);
        callback.apply(result.get());

        listener.remove();
    }

    @NotNull
    public LinkManager getLinkManager() {
        return linkManager;
    }

    public static void assertSomewhatClose(double expected, double actual) {
        EcuTestHelper.assertSomewhatClose("", expected, actual);
    }

    public static void assertSomewhatClose(String msg, double expected, double actual) {
        EcuTestHelper.assertSomewhatClose(msg, expected, actual, EngineReport.RATIO);
    }

    /**
     * todo: it's time to leverage junit a bit more properly
     */
    public static void assertSomewhatClose(String msg, double expected, double actual, double ratio) {
        if (!isCloseEnough(expected, actual, ratio))
            throw new IllegalStateException(msg + " Expected " + expected + " but got " + actual);
    }

    @NotNull
    public static EcuTestHelper createInstance(boolean allowHardwareTriggerInput) {
        EcuTestHelper ecu = new EcuTestHelper(ControllerConnectorState.getLinkManager());
        if (allowHardwareTriggerInput) {
            ecu.sendCommand(getEnableCommand(Integration.CMD_TRIGGER_HW_INPUT));
        } else {
            ecu.sendCommand(getDisableCommand(Integration.CMD_TRIGGER_HW_INPUT));
        }
        ecu.enableFunctionalMode();
        return ecu;
    }

    public void sendCommand(String command) {
        sendCommand(command, Timeouts.CMD_TIMEOUT);
    }

    public void sendCommand(String command, int timeoutMs) {
        TestHelper.INSTANCE.assertNotFatal();
        IoUtil.sendBlockingCommand(command, timeoutMs, commandQueue);
    }

    /**
     * this seem to adjust engine sniffer behaviour
     */
    public void enableFunctionalMode() {
        sendCommand(getEnableCommand(Integration.CMD_FUNCTIONAL_TEST_MODE));
    }

    public void changeRpm(final int rpm) {
        IoUtil.changeRpm(commandQueue, rpm);
    }

    public void setEngineType(engine_type_e engine_type_e) {
        int type = engine_type_e.ordinal();
        log.info("AUTOTEST setEngineType " + type);
        currentEngineType = type;
//        sendCommand(CMD_PINS);
        /*
         * we need to stop all activity - that means:
         * - stopping input event
         * - waiting for scheduled actuator actions to run out
         * - disabling PWM
         */
        sendCommand(getDisableCommand(Integration.CMD_SELF_STIMULATION));
        sendCommand(getDisableCommand(Integration.CMD_INJECTION));
        sendCommand(getDisableCommand(Integration.CMD_IGNITION));
        sendCommand(getDisableCommand(Integration.CMD_PWM));
        // changing engine type while engine is running does not work well - we rightfully
        // get invalid configuration critical errors
        sleepSeconds(2);
        sendCommand("set " + Integration.CMD_ENGINE_TYPE + " " + type, Timeouts.SET_ENGINE_TIMEOUT);
        // TODO: document the reason for this sleep?!
        sleepSeconds(1);
        sendCommand(getEnableCommand(Integration.CMD_PWM));
        sendCommand(getEnableCommand(Integration.CMD_SELF_STIMULATION));
//        // we need to skip one chart since it might have been produced with previous engine type
//        TestingUtils.nextChart(commandQueue);
    }
}
