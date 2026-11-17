package com.rusefi.ui.console;

import com.rusefi.ui.UIContext;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

/** The title bar names the .msq being worked on; it follows every load, save and Save As. */
class TuneFileTitleTest {
    @Test
    void titleShowsOnlyTheFileName() {
        String path = new File(new File("tunes", "car"), "base.msq").getPath();
        assertEquals("base.msq", MainFrame.tuneFileLabel(path));
    }

    @Test
    void noLabelWithoutATuneFile() {
        assertNull(MainFrame.tuneFileLabel(null));
        assertNull(MainFrame.tuneFileLabel(""));
    }

    @Test
    void changingTheTuneFileNotifiesListeners() {
        UIContext ctx = new UIContext();
        AtomicInteger changes = new AtomicInteger();
        ctx.addTuneFileListener(changes::incrementAndGet);

        ctx.setTuneFilePath("a.msq");
        assertEquals("a.msq", ctx.getTuneFilePath());
        ctx.setTuneFilePath(null);
        assertNull(ctx.getTuneFilePath());
        assertEquals(2, changes.get());
    }
}
