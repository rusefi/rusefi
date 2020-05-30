package com.rusefi.binaryprotocol;

import com.rusefi.composite.CompositeEvent;

import java.util.List;

public interface StreamFile {
    void append(List<CompositeEvent> events);

    void close();
}
