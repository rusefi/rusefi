package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImageTestHelper;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileModelMocks;
import com.opensr5.io.DataListener;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.binaryprotocol.IncomingDataBuffer;
import com.rusefi.config.generated.Integration;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.io.serial.AbstractIoStream;
import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.when;

class TuningToolbarPersistenceDefectTest {
    @Test
    void burnBeforeDebouncedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                assertTrue(current.toolbar.isUploadTimerRunningForUnitTest());

                button(current.toolbar, "Burn to ECU").doClick();
                current.toolbar.flushUploadTimerForUnitTest();
            });
            // Burn completion posts the toolbar baseline update to the EDT.
            runOnEdt(() -> { });
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            // Burn must upload the captured edit before persisting it, even ahead of the debounce.
            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b,
                fixture[0].protocol.getControllerConfiguration().getContent());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void burnUsesAnImmutableSnapshotAndDoesNotCompleteOverNewerEdits() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        AtomicBoolean succeeded = new AtomicBoolean();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));

                assertFalse(button(current.toolbar, "Burn to ECU").isEnabled());
                assertFalse(button(current.toolbar, "Discard changes").isEnabled());
                current.session.set(ConfigurationImageTestHelper.newLittleEndian(current.c));
                current.calibration.update((String) null, current.ini, current.session.get());
                current.toolbar.onEdit(ConfigurationImageTestHelper.newLittleEndian(current.b));
                submitted.remove(0).run();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            assertArrayEquals(fixture[0].c, fixture[0].session.get().getContent());
            assertFalse(succeeded.get(), "newer edits must suppress the completion callback");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void undoDuringBurnSuppressesExitCallbackForReplacedTune() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        AtomicBoolean succeeded = new AtomicBoolean();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushBeforeNavigate();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));
                assertFalse(button(current.toolbar, "Discard changes").isEnabled());

                button(current.toolbar, "Undo").doClick();
                assertArrayEquals(current.a, current.session.get().getContent());
                submitted.remove(0).run();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].b, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b, fixture[0].toolbar.getBaselineImage().getContent());
            assertFalse(succeeded.get(), "Undo replaced the image while Burn was in flight");
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void failedWriteDoesNotAdvanceTheToolbarBaselineOrReportSuccess() throws Exception {
        assertBurnFailure(false);
    }

    @Test
    void failedBurnDoesNotAdvanceTheToolbarBaselineOrReportSuccess() throws Exception {
        assertBurnFailure(true);
    }

    @Test
    void acknowledgedPageZeroWritesUpdateOnlyTheirDestinationRangeAndKeepLoadedBaseline() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                byte[] source = {90, 91, 92, 93, 94};
                current.protocol.writeInBlocks(source, 1, 3, 2, 0);

                byte[] expected = current.a.clone();
                expected[3] = 91;
                expected[4] = 92;
                assertArrayEquals(expected, current.protocol.getControllerConfiguration().getContent());
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());

                current.protocol.writeInBlocks(new byte[]{77, 78}, 0, 1, 2, 1);
                assertArrayEquals(expected, current.protocol.getControllerConfiguration().getContent(),
                    "extra-page writes must not mutate the main-page image");
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void acknowledgedRamChangesCanBeRevertedByTheNextDiff() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(current.b));
                current.protocol.uploadChangesWithoutBurn(ConfigurationImageTestHelper.newLittleEndian(current.a));

                assertArrayEquals(current.a, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.getControllerConfiguration().getContent());
                assertEquals(2, current.protocol.writeCount);
                assertArrayEquals(current.a, current.protocol.getCachedImage().getContent());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void pageZeroWriteWithoutLoadedMainImageIsIgnoredByComparisonCache() throws Exception {
        UIContext context = new UIContext();
        context.getLinkManager().setNotifyGlobalStatusOnClose(false);
        FakeProtocol protocol = new FakeProtocol(context.getLinkManager(), null);
        IniFileModel ini = IniFileModelMocks.withGauges();
        when(ini.getBlockingFactor()).thenReturn(64);
        protocol.setIniFileForUnitTest(ini);
        try {
            protocol.writeInBlocks(new byte[]{9, 8}, 0, 0, 2, 0);
            assertNull(protocol.getControllerConfiguration());
        } finally {
            context.getLinkManager().close();
            protocol.stream.close();
        }
    }

    @Test
    void failedDebouncedUploadReportsErrorOnEdt() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                fixture[0].protocol.failWrite = true;
                fixture[0].toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });
                fixture[0].editToB();
                fixture[0].toolbar.flushUploadTimerForUnitTest();
            });
            runOnEdt(() -> { });
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("Failed to upload tune"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardStopsPendingTimerAndDoesNotUploadObsoleteEdit() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                assertTrue(current.toolbar.isUploadTimerRunningForUnitTest());

                button(current.toolbar, "Discard changes").doClick();
                assertFalse(current.toolbar.isUploadTimerRunningForUnitTest());
                assertEquals(1, submitted.size(), "only the restoration upload should be queued");
                submitted.remove(0).run();
                assertArrayEquals(current.a, current.protocol.ram);
                assertEquals(0, current.protocol.burnCount, "Discard must not issue a burn command");
                assertEquals(0, current.protocol.writeCount, "obsolete B must never be written");
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardRestorationRunsAfterAnAlreadyQueuedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                button(current.toolbar, "Discard changes").doClick();
                assertEquals(2, submitted.size());

                submitted.remove(0).run();
                assertArrayEquals(current.b, current.protocol.ram);
                submitted.remove(0).run();
                assertArrayEquals(current.a, current.protocol.ram);
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void offlineDiscardDoesNotSubmitEcuRestore() throws Exception {
        Fixture[] fixture = new Fixture[1];
        List<Runnable> submitted = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(submitted::add);
                Fixture current = fixture[0];
                current.editToB();
                current.context.getLinkManager().setBinaryProtocolForTests(null);

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertTrue(submitted.isEmpty());
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void offlineDiscardWithConnectedProtocolDoesNotUploadLocalBaseline() throws Exception {
        runOnEdt(() -> {
            List<Runnable> submitted = new ArrayList<>();
            try (Fixture current = new Fixture(submitted::add)) {
                current.editToB();
                current.context.setOfflineMode(true);

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertTrue(submitted.isEmpty(), "An unreconciled offline tune must not reach the connected ECU");
                assertEquals(0, current.protocol.writeCount);
                assertEquals(0, current.protocol.burnCount);
            }
        });
    }

    @Test
    void queuedDiscardSkipsRestorationAfterSwitchingOffline() throws Exception {
        runOnEdt(() -> {
            List<Runnable> submitted = new ArrayList<>();
            try (Fixture current = new Fixture(submitted::add)) {
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                submitted.remove(0).run();
                button(current.toolbar, "Discard changes").doClick();
                assertEquals(1, submitted.size());
                current.context.setOfflineMode(true);

                submitted.remove(0).run();

                assertArrayEquals(current.a, current.session.get().getContent());
                assertArrayEquals(current.b, current.protocol.ram, "Queued restore must respect the offline transition");
                assertEquals(1, current.protocol.writeCount);
                assertEquals(0, current.protocol.burnCount);
            }
        });
    }

    @Test
    void failedDiscardRestoreReportsOnEdtWithoutBurning() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                current.protocol.failWrite = true;
                current.toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });

                button(current.toolbar, "Discard changes").doClick();
            });
            runOnEdt(() -> { });

            assertArrayEquals(fixture[0].a, fixture[0].session.get().getContent());
            assertArrayEquals(fixture[0].b, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].a, fixture[0].protocol.flash);
            assertEquals(0, fixture[0].protocol.burnCount);
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("Failed to restore tune to ECU"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardSubmissionFailureReportsOnEdtAfterLocalRestore() throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean errorOnEdt = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture(operation -> {
                    throw new IllegalStateException("queue unavailable");
                });
                Fixture current = fixture[0];
                current.toolbar.setErrorHandler(message -> {
                    errors.add(message);
                    errorOnEdt.set(SwingUtilities.isEventDispatchThread());
                });
                current.editToB();

                button(current.toolbar, "Discard changes").doClick();

                assertArrayEquals(current.a, current.session.get().getContent());
            });
            runOnEdt(() -> { });
            assertEquals(1, errors.size());
            assertTrue(errors.get(0).contains("queue unavailable"));
            assertTrue(errorOnEdt.get());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static void assertBurnFailure(boolean failBurn) throws Exception {
        Fixture[] fixture = new Fixture[1];
        AtomicBoolean succeeded = new AtomicBoolean();
        List<String> errors = new ArrayList<>();
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.protocol.failWrite = !failBurn;
                current.protocol.failBurn = failBurn;
                current.toolbar.setErrorHandler(errors::add);
                current.editToB();
                current.toolbar.burnToEcuAndThen(current.calibration, () -> succeeded.set(true));
            });
            runOnEdt(() -> { });
            assertArrayEquals(fixture[0].a, fixture[0].toolbar.getBaselineImage().getContent());
            assertFalse(succeeded.get());
            assertEquals(1, errors.size());
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    @Test
    void discardAfterDebouncedUpload() throws Exception {
        Fixture[] fixture = new Fixture[1];
        try {
            runOnEdt(() -> {
                fixture[0] = new Fixture();
                Fixture current = fixture[0];
                current.editToB();
                current.toolbar.flushUploadTimerForUnitTest();
                assertArrayEquals(current.b, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);

                button(current.toolbar, "Discard changes").doClick();

                // Discard restores the UI and ECU RAM, but does not burn the restoration.
                assertArrayEquals(current.a, current.session.get().getContent());
                assertArrayEquals(current.a, current.calibration.getWorkingImage().getContent());
                assertArrayEquals(current.a, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);
                assertEquals(0, current.protocol.burnCount);
            });
        } finally {
            if (fixture[0] != null) {
                runOnEdt(fixture[0]::close);
            }
        }
    }

    private static void runOnEdt(Runnable action) throws Exception {
        SwingUtilities.invokeAndWait(action);
    }

    private static JButton button(TuningToolbarWidget toolbar, String text) {
        return findButton(toolbar.getPanel(), text);
    }

    private static JButton findButton(Container container, String text) {
        for (Component component : container.getComponents()) {
            if (component instanceof JButton && text.equals(((JButton) component).getText())) {
                return (JButton) component;
            }
            if (component instanceof Container) {
                JButton found = findButton((Container) component, text);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    private static final class Fixture implements AutoCloseable {
        private final byte[] a = {1, 2, 3, 4, 5, 6, 7, 8};
        private final byte[] b = {1, 42, 3, 4, 5, 6, 7, 8};
        private final byte[] c = {1, 42, 3, 4, 55, 6, 7, 8};
        private final UIContext context = new UIContext();
        private final IniFileModel ini = IniFileModelMocks.withGauges();
        private final FakeProtocol protocol;
        private final AtomicReference<ConfigurationImage> session =
            new AtomicReference<>(ConfigurationImageTestHelper.newLittleEndian(a));
        private final CalibrationDialogWidget calibration = new CalibrationDialogWidget(context);
        private final TuningToolbarWidget toolbar;

        private Fixture() {
            this(Runnable::run);
        }

        private Fixture(java.util.function.Consumer<Runnable> submit) {
            when(ini.getBlockingFactor()).thenReturn(64);
            context.iniFileState.setIniFileModelForTest(ini);
            context.getLinkManager().setNotifyGlobalStatusOnClose(false);
            protocol = new FakeProtocol(context.getLinkManager(), a);
            protocol.setIniFileForUnitTest(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            calibration.update((String) null, ini, session.get());
            toolbar = new TuningToolbarWidget(context, calibration, new AtomicReference<>(),
                session, ConfigurationImageTestHelper.newLittleEndian(a), UpdateOperationCallbacks.DUMMY,
                () -> { }, failed -> { }, submit);
        }

        private void editToB() {
            session.set(ConfigurationImageTestHelper.newLittleEndian(b));
            calibration.update((String) null, ini, session.get());
            toolbar.onEdit(ConfigurationImageTestHelper.newLittleEndian(a));
        }

        @Override
        public void close() {
            toolbar.onDisconnect();
            context.getLinkManager().close();
            protocol.stream.close();
        }
    }

    /** Real BinaryProtocol diff/write/burn/cache logic over a deterministic fake ECU. */
    private static final class FakeProtocol extends BinaryProtocol {
        private final FakeStream stream;
        private final byte[] ram;
        private byte[] flash;
        private boolean failWrite;
        private boolean failBurn;
        private int writeCount;
        private int burnCount;

        private FakeProtocol(LinkManager linkManager, byte[] initial) {
            this(linkManager, new FakeStream(), initial);
        }

        private FakeProtocol(LinkManager linkManager, FakeStream stream, byte[] initial) {
            super(linkManager, stream);
            this.stream = stream;
            ram = initial == null ? new byte[8] : initial.clone();
            flash = initial == null ? new byte[8] : initial.clone();
            if (initial != null) {
                setConfigurationImage(ConfigurationImageTestHelper.newLittleEndian(initial));
                cacheCurrentImage();
            }
        }

        @Override
        public byte[] executeCommand(char opcode, byte[] packet, String description, int timeoutMs) {
            if (opcode == (char) Integration.TS_CHUNK_WRITE_COMMAND) {
                if (failWrite) {
                    stream.close();
                    return null;
                }
                ByteBuffer header = ByteBuffer.wrap(packet).order(getWireByteOrder());
                int page = Short.toUnsignedInt(header.getShort());
                int offset = Short.toUnsignedInt(header.getShort());
                int count = Short.toUnsignedInt(header.getShort());
                if (page == 0) {
                    System.arraycopy(packet, 6, ram, offset, count);
                }
                writeCount++;
            } else if (opcode == (char) Integration.TS_BURN_COMMAND) {
                burnCount++;
                if (failBurn) {
                    stream.close();
                    return null;
                }
                flash = ram.clone();
                return new byte[]{(byte) Integration.TS_RESPONSE_BURN_OK};
            }
            return new byte[]{(byte) Integration.TS_RESPONSE_OK};
        }

        @Override
        public boolean isSinglePageController() {
            return false;
        }
    }

    private static final class FakeStream extends AbstractIoStream {
        private final IncomingDataBuffer buffer = new IncomingDataBuffer("tune test", getStreamStats());

        @Override
        public void setInputListener(DataListener listener) {
        }

        @Override
        public IncomingDataBuffer getDataBuffer() {
            return buffer;
        }
    }
}
