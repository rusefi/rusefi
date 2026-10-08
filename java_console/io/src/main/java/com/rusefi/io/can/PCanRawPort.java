package com.rusefi.io.can;

import peak.can.MutableInteger;
import peak.can.basic.PCANBasic;
import peak.can.basic.TPCANBaudrate;
import peak.can.basic.TPCANHandle;
import peak.can.basic.TPCANMessageType;
import peak.can.basic.TPCANMsg;
import peak.can.basic.TPCANParameter;
import peak.can.basic.TPCANParameterValue;
import peak.can.basic.TPCANStatus;
import peak.can.basic.TPCANType;

import java.io.IOException;
import java.util.Arrays;
import java.util.EnumSet;
import java.util.Optional;
import java.util.Objects;
import java.util.Set;
import java.util.concurrent.TimeUnit;

/** Blocking Classic CAN port backed by the PCAN-Basic API. */
public class PCanRawPort implements RawCanPort {
    private static final long RECEIVE_POLL_MS = 1;

    // PCAN-Basic channel state is process-wide even when callers create separate PCANBasic objects.
    // In particular, initializing or uninitializing a scanner instance can invalidate an in-progress
    // persistent connection on the same channel. Serialize every native operation and give each physical
    // channel to exactly one PCanRawPort until close completes.
    private static final Object NATIVE_LOCK = new Object();
    private static final Set<TPCANHandle> OWNED_CHANNELS = EnumSet.noneOf(TPCANHandle.class);

    private static final PCanClock SYSTEM_CLOCK = new PCanClock() {
        @Override
        public long nanoTime() {
            return System.nanoTime();
        }

        @Override
        public void sleep(long millis) throws InterruptedException {
            Thread.sleep(millis);
        }
    };

    private final Object lifecycleLock = new Object();
    private final TPCANHandle channel;
    private final PCanApiFactory apiFactory;
    private final PCanClock clock;

    private PCanApi api;
    private CanAddress receiveAddress;
    private boolean ownsChannel;

    public PCanRawPort() {
        this(PCanHelper.CHANNEL, PCanBasicApi::new, SYSTEM_CLOCK);
    }

    PCanRawPort(TPCANHandle channel, PCanApiFactory apiFactory, PCanClock clock) {
        this.channel = channel;
        this.apiFactory = apiFactory;
        this.clock = clock;
    }

    @Override
    public void open(CanAddress receiveAddress) throws IOException {
        Objects.requireNonNull(receiveAddress, "receiveAddress");
        synchronized (lifecycleLock) {
            if (api != null) {
                throw new IOException("PCAN port is already open");
            }

            final PCanApi newApi;
            try {
                newApi = apiFactory.create();
            } catch (RuntimeException | LinkageError e) {
                throw new IOException("Unable to load PCAN API", e);
            }

            boolean channelInitialized = false;
            synchronized (NATIVE_LOCK) {
                claimChannel();
                try {
                    if (!newApi.initializeApi()) {
                        throw new IOException("Unable to initialize PCAN API");
                    }
                    requireOk("initialize channel", newApi.initialize(channel, TPCANBaudrate.PCAN_BAUD_500K));
                    channelInitialized = true;
                    // Native ID filtering dropped ECU replies on Windows; filter in receive() instead.
                    requireOk("configure receive filter", newApi.setValue(
                        channel,
                        TPCANParameter.PCAN_MESSAGE_FILTER,
                        new MutableInteger(TPCANParameterValue.PCAN_FILTER_OPEN.getValue()),
                        Integer.BYTES));
                    api = newApi;
                    this.receiveAddress = receiveAddress;
                } catch (IOException e) {
                    if (channelInitialized) {
                        uninitializeAfterFailedOpen(newApi, e);
                    }
                    releaseChannel();
                    throw e;
                } catch (RuntimeException | LinkageError e) {
                    IOException failure = new IOException("Unable to open PCAN port", e);
                    if (channelInitialized) {
                        uninitializeAfterFailedOpen(newApi, failure);
                    }
                    releaseChannel();
                    throw failure;
                }
            }
        }
    }

    @Override
    public void send(ClassicCanFrame frame) throws IOException {
        CanAddress address = frame.getAddress();
        byte[] payload = frame.getPayload();
        TPCANMsg message = new TPCANMsg(
            address.getId(),
            address.isExtended()
                ? TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue()
                : TPCANMessageType.PCAN_MESSAGE_STANDARD.getValue(),
            (byte) payload.length,
            payload);

        synchronized (lifecycleLock) {
            synchronized (NATIVE_LOCK) {
                final TPCANStatus status;
                try {
                    status = currentApi().write(channel, message);
                } catch (RuntimeException | LinkageError e) {
                    throw new IOException("PCAN write failed", e);
                }
                requireOk("write", status);
            }
        }
    }

    @Override
    public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException {
        if (timeoutMs < 0) {
            throw new IllegalArgumentException("timeoutMs must not be negative");
        }

        final long deadline = clock.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
        while (true) {
            // The oversized backing array preserves the existing workaround for malformed traffic
            // described by #4370. Valid Classic CAN frames are still rejected above eight bytes below.
            TPCANMsg message = new TPCANMsg(Byte.MAX_VALUE);
            final TPCANStatus status;
            final CanAddress expectedReceiveAddress;
            synchronized (lifecycleLock) {
                expectedReceiveAddress = receiveAddress;
                synchronized (NATIVE_LOCK) {
                    try {
                        status = currentApi().read(channel, message);
                    } catch (RuntimeException | LinkageError e) {
                        throw new IOException("PCAN read failed", e);
                    }
                }
            }

            if (status == TPCANStatus.PCAN_ERROR_OK) {
                if (matchesAddress(message, expectedReceiveAddress)) {
                    return Optional.of(toClassicCanFrame(message));
                }
                if (clock.nanoTime() >= deadline) {
                    return Optional.empty();
                }
                continue;
            }
            if (status != TPCANStatus.PCAN_ERROR_QRCVEMPTY) {
                throw statusFailure("read", status);
            }
            if (clock.nanoTime() >= deadline) {
                return Optional.empty();
            }

            try {
                clock.sleep(RECEIVE_POLL_MS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new IOException("Interrupted while waiting for a PCAN frame", e);
            }
        }
    }

    @Override
    public void close() throws IOException {
        synchronized (lifecycleLock) {
            final PCanApi currentApi;
            currentApi = api;
            api = null;
            receiveAddress = null;

            if (currentApi != null) {
                synchronized (NATIVE_LOCK) {
                    try {
                        final TPCANStatus status;
                        try {
                            status = currentApi.uninitialize(channel);
                        } catch (RuntimeException | LinkageError e) {
                            throw new IOException("Unable to close PCAN port", e);
                        }
                        requireOk("uninitialize channel", status);
                    } finally {
                        releaseChannel();
                    }
                }
            }
        }
    }

    private void claimChannel() throws IOException {
        if (!OWNED_CHANNELS.add(channel)) {
            throw new IOException("PCAN channel " + channel + " is already in use by this process");
        }
        ownsChannel = true;
    }

    private void releaseChannel() {
        if (ownsChannel) {
            OWNED_CHANNELS.remove(channel);
            ownsChannel = false;
        }
    }

    private PCanApi currentApi() throws IOException {
        synchronized (lifecycleLock) {
            if (api == null) {
                throw new IOException("PCAN port is not open");
            }
            return api;
        }
    }

    private static ClassicCanFrame toClassicCanFrame(TPCANMsg message) throws IOException {
        int type = Byte.toUnsignedInt(message.getType());
        int extendedFlag = Byte.toUnsignedInt(TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue());
        if ((type & ~extendedFlag) != 0) {
            throw new IOException("Received unsupported PCAN frame type 0x" + Integer.toHexString(type));
        }

        int length = Byte.toUnsignedInt(message.getLength());
        byte[] data = message.getData();
        if (length > 8 || data == null || length > data.length) {
            throw new IOException("Received invalid Classic CAN payload length " + length);
        }

        try {
            return new ClassicCanFrame(
                new CanAddress(message.getID(), (type & extendedFlag) != 0),
                Arrays.copyOf(data, length));
        } catch (IllegalArgumentException e) {
            throw new IOException("Received invalid PCAN frame", e);
        }
    }

    private static boolean matchesAddress(TPCANMsg message, CanAddress expectedAddress) {
        int type = Byte.toUnsignedInt(message.getType());
        int extendedFlag = Byte.toUnsignedInt(TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue());
        return message.getID() == expectedAddress.getId()
            && ((type & extendedFlag) != 0) == expectedAddress.isExtended();
    }

    private static void requireOk(String operation, TPCANStatus status) throws IOException {
        if (status != TPCANStatus.PCAN_ERROR_OK) {
            throw statusFailure(operation, status);
        }
    }

    private static IOException statusFailure(String operation, TPCANStatus status) {
        return new IOException("PCAN " + operation + " failed: " + status);
    }

    private void uninitializeAfterFailedOpen(PCanApi api, IOException originalFailure) {
        try {
            TPCANStatus status = api.uninitialize(channel);
            if (status != TPCANStatus.PCAN_ERROR_OK) {
                originalFailure.addSuppressed(statusFailure("uninitialize channel", status));
            }
        } catch (RuntimeException | LinkageError e) {
            originalFailure.addSuppressed(e);
        }
    }

    private static final class PCanBasicApi implements PCanApi {
        private final PCANBasic api = new PCANBasic();

        @Override
        public boolean initializeApi() {
            return api.initializeAPI();
        }

        @Override
        public TPCANStatus initialize(TPCANHandle channel, TPCANBaudrate bitrate) {
            return api.Initialize(channel, bitrate,
                TPCANType.PCAN_TYPE_NONE, 0, (short) 0);
        }

        @Override
        public TPCANStatus setValue(TPCANHandle channel, TPCANParameter parameter, MutableInteger value, int bufferLength) {
            return api.SetValue(channel, parameter, value, bufferLength);
        }

        @Override
        public TPCANStatus write(TPCANHandle channel, TPCANMsg message) {
            return api.Write(channel, message);
        }

        @Override
        public TPCANStatus read(TPCANHandle channel, TPCANMsg message) {
            return api.Read(channel, message, null);
        }

        @Override
        public TPCANStatus uninitialize(TPCANHandle channel) {
            return api.Uninitialize(channel);
        }
    }
}

interface PCanApi {
    boolean initializeApi();

    TPCANStatus initialize(TPCANHandle channel, TPCANBaudrate bitrate);

    TPCANStatus setValue(TPCANHandle channel, TPCANParameter parameter, MutableInteger value, int bufferLength);

    TPCANStatus write(TPCANHandle channel, TPCANMsg message);

    TPCANStatus read(TPCANHandle channel, TPCANMsg message);

    TPCANStatus uninitialize(TPCANHandle channel);
}

@FunctionalInterface
interface PCanApiFactory {
    PCanApi create();
}

interface PCanClock {
    long nanoTime();

    void sleep(long millis) throws InterruptedException;
}
