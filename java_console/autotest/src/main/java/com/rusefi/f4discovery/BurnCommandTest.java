package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.io.LinkManager;
import com.rusefi.io.commands.BurnCommand;
import org.junit.Test;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static org.junit.Assert.assertTrue;

public class BurnCommandTest extends RusefiTestBase {
    @Test
    public void executeBurnCommand() throws InterruptedException {
        LinkManager linkManager = ecu.getLinkManager();
        assertBurn(linkManager);
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
        assertTrue("burn command", result.get());
    }
}
