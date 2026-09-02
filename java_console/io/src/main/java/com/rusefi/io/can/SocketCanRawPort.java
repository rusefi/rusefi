package com.rusefi.io.can;

import tel.schich.javacan.CanChannels;
import tel.schich.javacan.CanFilter;
import tel.schich.javacan.CanFrame;
import tel.schich.javacan.CanId;
import tel.schich.javacan.NetworkDevice;
import tel.schich.javacan.RawCanChannel;
import tel.schich.javacan.platform.linux.LinuxNativeOperationException;

import java.io.IOException;
import java.time.Duration;
import java.util.Optional;

import static tel.schich.javacan.CanFrame.FD_NO_FLAGS;
import static tel.schich.javacan.CanSocketOptions.FILTER;
import static tel.schich.javacan.CanSocketOptions.RECV_OWN_MSGS;
import static tel.schich.javacan.CanSocketOptions.SO_RCVTIMEO;

public class SocketCanRawPort implements RawCanPort {
    private final Object lifecycleLock = new Object();
    private final String deviceName;
    private final NetworkDeviceLookup networkDeviceLookup;
    private final RawCanChannelFactory channelFactory;

    private RawCanChannel channel;

    public SocketCanRawPort() {
        this(System.getProperty("CAN_DEVICE_NAME", "can0"), NetworkDevice::lookup, CanChannels::newRawChannel);
    }

    SocketCanRawPort(String deviceName, NetworkDeviceLookup networkDeviceLookup, RawCanChannelFactory channelFactory) {
        this.deviceName = deviceName;
        this.networkDeviceLookup = networkDeviceLookup;
        this.channelFactory = channelFactory;
    }

    @Override
    public void open(CanAddress address) throws IOException {
        synchronized (lifecycleLock) {
            if (channel != null) {
                throw new IOException("SocketCAN port is already open");
            }

            RawCanChannel newChannel = null;
            try {
                NetworkDevice device = networkDeviceLookup.lookup(deviceName);
                newChannel = channelFactory.create();
                newChannel.bind(device);
                newChannel.configureBlocking(true);
                newChannel.setOption(RECV_OWN_MSGS, false);
                newChannel.setOption(FILTER, new CanFilter[]{new CanFilter(rawId(address))});
                channel = newChannel;
            } catch (IOException e) {
                closeAfterFailedOpen(newChannel, e);
                throw e;
            }
        }
    }

    @Override
    public void send(ClassicCanFrame frame) throws IOException {
        CanAddress address = frame.getAddress();
        CanFrame canFrame = address.isExtended()
            ? CanFrame.createExtended(address.getId(), FD_NO_FLAGS, frame.getPayload())
            : CanFrame.create(address.getId(), FD_NO_FLAGS, frame.getPayload());
        currentChannel().write(canFrame);
    }

    @Override
    public Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException {
        RawCanChannel currentChannel = currentChannel();
        currentChannel.setOption(SO_RCVTIMEO, Duration.ofMillis(timeoutMs));

        final CanFrame frame;
        try {
            frame = currentChannel.read();
        } catch (LinuxNativeOperationException e) {
            if (e.mayTryAgain()) {
                return Optional.empty();
            }
            throw e;
        }

        if (frame.isFDFrame() || frame.isError() || frame.isRemoteTransmissionRequest()) {
            throw new IOException("Received unsupported CAN frame");
        }

        byte[] payload = new byte[frame.getDataLength()];
        frame.getData(payload, 0, payload.length);
        return Optional.of(new ClassicCanFrame(new CanAddress(frame.getId(), frame.isExtended()), payload));
    }

    @Override
    public void close() throws IOException {
        RawCanChannel currentChannel;
        synchronized (lifecycleLock) {
            currentChannel = channel;
            channel = null;
        }

        if (currentChannel != null) {
            currentChannel.close();
        }
    }

    private RawCanChannel currentChannel() throws IOException {
        synchronized (lifecycleLock) {
            if (channel == null) {
                throw new IOException("SocketCAN port is not open");
            }
            return channel;
        }
    }

    private static int rawId(CanAddress address) {
        return address.isExtended() ? address.getId() | CanId.EFF_FLAG : address.getId();
    }

    private static void closeAfterFailedOpen(RawCanChannel channel, IOException originalException) {
        if (channel == null) {
            return;
        }
        try {
            channel.close();
        } catch (IOException closeException) {
            originalException.addSuppressed(closeException);
        }
    }
}

@FunctionalInterface
interface NetworkDeviceLookup {
    NetworkDevice lookup(String name) throws IOException;
}

@FunctionalInterface
interface RawCanChannelFactory {
    RawCanChannel create() throws IOException;
}
