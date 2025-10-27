package com.rusefi.io.can.isotp;

public class TestIsoTpCanDecoder extends IsoTpCanDecoder {
    public TestIsoTpCanDecoder() {
    }

    public TestIsoTpCanDecoder(int isoHeaderByteIndex) {
        super(isoHeaderByteIndex);
    }

    @Override
    protected void onTpFirstFrame() {

    }
}
