package com.rusefi.ui;

import com.rusefi.ui.widgets.CommandIcon;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertSame;

class RecentCommandsIconTest {
    @Test
    void everyCommandGetsAnIconByItsFirstWord() {
        assertSame(CommandIcon.INFO, RecentCommands.iconFor("sensorinfo"));
        assertSame(CommandIcon.ENABLE, RecentCommands.iconFor("enable self_stimulation"));
        assertSame(CommandIcon.DISABLE, RecentCommands.iconFor("disable injection"));
        assertSame(CommandIcon.SPARK, RecentCommands.iconFor("sparkbench 5 400 2"));
        assertSame(CommandIcon.FUEL, RecentCommands.iconFor("fuelbench 5 400 2"));
        assertSame(CommandIcon.IDLE, RecentCommands.iconFor("set_idle_position 50"));
        assertSame(CommandIcon.IDLE, RecentCommands.iconFor("blipidle 80 2000"));
        assertSame(CommandIcon.RESET, RecentCommands.iconFor("luareset"));
        assertSame(CommandIcon.SAVE, RecentCommands.iconFor("rewriteconfig"));
        assertSame(CommandIcon.LIST, RecentCommands.iconFor("showconfig"));
        assertSame(CommandIcon.COMMAND, RecentCommands.iconFor("set_rpm_hard_limit 7000"), "Typed commands get the prompt icon");
    }
}
