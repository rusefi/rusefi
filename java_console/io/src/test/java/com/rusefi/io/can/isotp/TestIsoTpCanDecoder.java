package com.rusefi.io.can.isotp;

public class TestIsoTpCanDecoder extends IsoTpCanDecoder {
    public TestIsoTpCanDecoder() {
    }

    public TestIsoTpCanDecoder(int isoHeaderByteIndex, byte[] flowControl) {
        super(isoHeaderByteIndex, flowControl);
    }

    @Override
    protected void onTpFirstFrame() {

    }
}
