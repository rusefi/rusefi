package com.rusefi.f4discovery;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.RusefiTestBase;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.BurnCommand;
import org.junit.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.devexperts.logging.Logging.getLogging;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class BurnCommandTest extends RusefiTestBase {
    private static final Logging log = getLogging(BurnCommandTest.class);

    @Test
    public void executeBurnCommand() throws InterruptedException {
        LinkManager linkManager = ecu.getLinkManager();
        assertWrite(linkManager);
        // again, just for fun
        assertWrite(linkManager);
        assertBurn(linkManager);
    }

    private static void assertWrite(LinkManager linkManager) throws InterruptedException {
        CountDownLatch latch = new CountDownLatch(1);

        Runnable action = new Runnable() {
            @Override
            public void run() {
                log.info("assertWrite run");
                BinaryProtocol bp = linkManager.getBinaryProtocol();
                ScalarIniField displacementField = (ScalarIniField) bp.getIniFile().getIniField("displacement");
                ByteBuffer bb = wrap(new byte[4]);
                ConfigurationImage.setScalarValue(bb, displacementField.getType(), "3", Field.NO_BIT_OFFSET, 1, 0);
                log.info("assertWrite writeData " + Arrays.toString(bb.array()) + " " + displacementField);
                // here we assert that write command do not break protocol
                bp.writeInBlocks(bb.array(), 0, displacementField.getOffset(), displacementField.getSize());
                log.info("assertWrite countDown");

                latch.countDown();
            }
        };

        linkManager.submit(action);

        boolean result = latch.await(30, TimeUnit.SECONDS);
        log.info("assertWrite result " + result);
        assertTrue(result);
    }

    private static ByteBuffer wrap(byte[] bytes) {
        ByteBuffer wrapped = ByteBuffer.wrap(bytes, 0, bytes.length);
        wrapped.order(ByteOrder.LITTLE_ENDIAN);
        return wrapped;
    }

    private void assertBurn(LinkManager linkManager) throws InterruptedException {
        AtomicReference<Boolean> result = new AtomicReference<>();

        CountDownLatch latch = new CountDownLatch(1);
        Runnable action = () -> {
            result.set(BurnCommand.execute(ecu.getLinkManager().getBinaryProtocol()));
            latch.countDown();
        };
        linkManager.submit(action);

        latch.await(30, TimeUnit.SECONDS);
        assertNotNull(result.get());
        assertTrue("burn command", result.get());
    }
}
