package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.binaryprotocol.BinaryProtocolLogger;
import com.rusefi.enums.engine_type_e;
import com.rusefi.io.LinkManager;
import org.junit.Test;

public class CompositeLoggerTest extends RusefiTestBase {
    @Test
    public void testVW() {
        ecu.setEngineType(engine_type_e.VW_ABA);
        ecu.changeRpm(1200);
        LinkManager linkManager = ecu.getLinkManager();

        BinaryProtocolLogger binaryProtocolLogger = new BinaryProtocolLogger(linkManager);
        linkManager.submit(new Runnable() {
            @Override
            public void run() {
                binaryProtocolLogger.getComposite(linkManager.getBinaryProtocol());
            }
        });
    }

}
