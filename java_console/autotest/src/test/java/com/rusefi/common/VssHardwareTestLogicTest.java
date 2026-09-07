package com.rusefi.common;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.autotest.ControllerConnectorState;
import com.rusefi.config.FieldType;
import com.rusefi.core.SensorCentral;
import com.rusefi.core.SensorNames;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Collections;
import java.util.Locale;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.*;

class VssHardwareTestLogicTest {
    private final SensorCentral sensors = SensorCentral.getInstance();
    private final ScheduledExecutorService polling = Executors.newSingleThreadScheduledExecutor();
    private String previousFirmwareVersion;

    @BeforeEach
    void setUp() {
        sensors.reset();
        previousFirmwareVersion = ControllerConnectorState.firmwareVersion;
        ControllerConnectorState.firmwareVersion = "test";
    }

    @AfterEach
    void tearDown() throws InterruptedException {
        polling.shutdownNow();
        assertTrue(polling.awaitTermination(5, TimeUnit.SECONDS));
        sensors.reset();
        ControllerConnectorState.firmwareVersion = previousFirmwareVersion;
    }

    @Test
    void acceptsFreshSpeedAfterCalibrationChange() {
        assertDoesNotThrow(() -> runWithFakeVss(0));
    }

    @Test
    void waitsForDelayedMeasurement() {
        assertDoesNotThrow(() -> runWithFakeVss(2500));
    }

    @Test
    void cachedMatchingValueDoesNotReplaceFreshSample() {
        sensors.setValue(0, SensorNames.VEHICLESPEEDKPH);
        IllegalStateException failure = assertThrows(IllegalStateException.class,
                () -> EcuTestHelper.assertSensorEventually("VSS", SensorNames.VEHICLESPEEDKPH, 0, 20));
        assertTrue(failure.getMessage().contains("last fresh value was NaN"), failure.getMessage());
        assertVssDemandReleased();
    }

    @Test
    void unchangedFreshZeroIsAcceptedAndSubscriptionReleased() throws Exception {
        IniFileModel ini = createIni();
        sensors.setValue(0, SensorNames.VEHICLESPEEDKPH);
        AtomicBoolean sawDemand = new AtomicBoolean();
        polling.scheduleAtFixedRate(() -> {
            if (sensors.getOutputChannelDemand().getChannels().contains(SensorNames.VEHICLESPEEDKPH.toLowerCase(Locale.US))) {
                sawDemand.set(true);
            }
            publish(ini, 0);
        }, 0, 10, TimeUnit.MILLISECONDS);

        EcuTestHelper.assertSensorEventually("VSS", SensorNames.VEHICLESPEEDKPH, 0, 1000);
        assertTrue(sawDemand.get());
        assertVssDemandReleased();
    }

    @Test
    void wrongFreshValueStillFails() throws Exception {
        IniFileModel ini = createIni();
        polling.scheduleAtFixedRate(() -> publish(ini, 0), 0, 10, TimeUnit.MILLISECONDS);
        IllegalStateException failure = assertThrows(IllegalStateException.class,
                () -> EcuTestHelper.assertSensorEventually("VSS", SensorNames.VEHICLESPEEDKPH, 160.46, 1000));
        assertTrue(failure.getMessage().contains("last fresh value was 0.0"), failure.getMessage());
        assertVssDemandReleased();
    }

    @Test
    void interruptedWaitReleasesSubscription() {
        Thread.currentThread().interrupt();
        try {
            IllegalStateException failure = assertThrows(IllegalStateException.class,
                    () -> EcuTestHelper.assertSensorEventually("VSS", SensorNames.VEHICLESPEEDKPH, 0, 1000));
            assertTrue(failure.getCause() instanceof InterruptedException);
            assertTrue(Thread.currentThread().isInterrupted());
            assertVssDemandReleased();
        } finally {
            Thread.interrupted();
        }
    }

    private void assertVssDemandReleased() {
        assertFalse(sensors.getOutputChannelDemand().getChannels().contains(SensorNames.VEHICLESPEEDKPH.toLowerCase(Locale.US)));
    }

    private static IniFileModel createIni() throws Exception {
        IniFileModel ini = mock(IniFileModel.class);
        IniField field = new ScalarIniField(SensorNames.VEHICLESPEEDKPH, 0, "kph", FieldType.FLOAT, 1, "0", 0);
        when(ini.getAllOutputChannels()).thenReturn(Collections.singletonMap(SensorNames.VEHICLESPEEDKPH, field));
        when(ini.getOutputChannel(SensorNames.VEHICLESPEEDKPH)).thenReturn(field);
        return ini;
    }

    private void publish(IniFileModel ini, double speed) {
        byte[] response = ByteBuffer.allocate(5).order(ByteOrder.LITTLE_ENDIAN)
                .put((byte) 0).putFloat((float) speed).array();
        sensors.grabSensorValues(response, ini, null);
    }

    private void runWithFakeVss(long inputDelayMs) throws Exception {
        IniFileModel ini = createIni();
        AtomicInteger wheelRevPerKm = new AtomicInteger(551);
        AtomicLong inputReadyAt = new AtomicLong(Long.MAX_VALUE);
        Runnable publish = () -> {
            double speed = System.nanoTime() >= inputReadyAt.get()
                    ? 100 * 3600 / (wheelRevPerKm.get() * (4.3 * 6 / 23) * 4) : 0;
            publish(ini, speed);
        };

        EcuTestHelper ecu = mock(EcuTestHelper.class);
        doAnswer(invocation -> {
            String command = invocation.getArgument(0);
            if (command.equals("set driveWheelRevPerKm 500")) {
                wheelRevPerKm.set(500);
                // Command acknowledgement can arrive after the next output sample.
                polling.submit(publish).get(5, TimeUnit.SECONDS);
            } else if (command.equals("set vss_pin PA6")) {
                inputReadyAt.set(System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(inputDelayMs));
            }
            return null;
        }).when(ecu).sendCommand(anyString());

        sensors.setValue(0, SensorNames.VEHICLESPEEDKPH);
        polling.scheduleAtFixedRate(publish, 0, 10, TimeUnit.MILLISECONDS);
        VssHardwareTestLogic.runIdleVssTest(ecu, "PD2", "PA6");
    }
}
