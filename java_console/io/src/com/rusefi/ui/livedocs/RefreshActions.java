package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;

public abstract class RefreshActions {
    public abstract void refresh(BinaryProtocol bp, byte[] response);
}
