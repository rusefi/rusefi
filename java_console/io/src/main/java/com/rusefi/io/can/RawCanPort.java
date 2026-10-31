package com.rusefi.io.can;

import java.io.IOException;
import java.util.Optional;

/** Blocking Classic CAN port. Implementations must make close safe to call repeatedly. */
public interface RawCanPort extends AutoCloseable {
    void open(CanAddress receiveAddress) throws IOException;

    void send(ClassicCanFrame frame) throws IOException;

    Optional<ClassicCanFrame> receive(int timeoutMs) throws IOException;

    @Override
    void close() throws IOException;
}
