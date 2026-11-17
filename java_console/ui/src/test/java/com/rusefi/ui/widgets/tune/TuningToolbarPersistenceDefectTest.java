package com.rusefi.ui.widgets.tune;

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
            // Characterization: Burn ran before the RAM write, then advanced the diff image to B.
            assertArrayEquals(fixture[0].a, fixture[0].protocol.ram);
            assertArrayEquals(fixture[0].a, fixture[0].protocol.flash);
            assertArrayEquals(fixture[0].b,
                fixture[0].protocol.getControllerConfiguration().getContent());
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

                // Characterization: Discard changes the UI image only; ECU RAM still contains B.
                assertArrayEquals(current.a, current.session.get().getContent());
                assertArrayEquals(current.a, current.calibration.getWorkingImage().getContent());
                assertArrayEquals(current.b, current.protocol.ram);
                assertArrayEquals(current.a, current.protocol.flash);
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
        private final UIContext context = new UIContext();
        private final IniFileModel ini = IniFileModelMocks.withGauges();
        private final FakeProtocol protocol;
        private final AtomicReference<ConfigurationImage> session =
            new AtomicReference<>(new ConfigurationImage(a));
        private final CalibrationDialogWidget calibration = new CalibrationDialogWidget(context);
        private final TuningToolbarWidget toolbar;

        private Fixture() {
            when(ini.getBlockingFactor()).thenReturn(64);
            context.iniFileState.setIniFileModelForTest(ini);
            context.getLinkManager().setNotifyGlobalStatusOnClose(false);
            protocol = new FakeProtocol(context.getLinkManager(), a);
            protocol.setIniFileForUnitTest(ini);
            context.getLinkManager().setBinaryProtocolForTests(protocol);
            calibration.update((String) null, ini, session.get());
            toolbar = new TuningToolbarWidget(context, calibration, new AtomicReference<>(),
                session, new ConfigurationImage(a), UpdateOperationCallbacks.DUMMY,
                () -> { }, failed -> { }, Runnable::run);
        }

        private void editToB() {
            session.set(new ConfigurationImage(b));
            calibration.update((String) null, ini, session.get());
            toolbar.onEdit(new ConfigurationImage(a));
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

        private FakeProtocol(LinkManager linkManager, byte[] initial) {
            this(linkManager, new FakeStream(), initial);
        }

        private FakeProtocol(LinkManager linkManager, FakeStream stream, byte[] initial) {
            super(linkManager, stream);
            this.stream = stream;
            ram = initial.clone();
            flash = initial.clone();
            setConfigurationImage(new ConfigurationImage(initial));
            cacheCurrentImage();
        }

        @Override
        public byte[] executeCommand(char opcode, byte[] packet, String description, int timeoutMs) {
            if (opcode == (char) Integration.TS_CHUNK_WRITE_COMMAND) {
                ByteBuffer header = ByteBuffer.wrap(packet).order(getWireByteOrder());
                int page = Short.toUnsignedInt(header.getShort());
                int offset = Short.toUnsignedInt(header.getShort());
                int count = Short.toUnsignedInt(header.getShort());
                if (page != 0) {
                    throw new IllegalArgumentException("Unexpected tune page " + page);
                }
                System.arraycopy(packet, 6, ram, offset, count);
            } else if (opcode == (char) Integration.TS_BURN_COMMAND) {
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
