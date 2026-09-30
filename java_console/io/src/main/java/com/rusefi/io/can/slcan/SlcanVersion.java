package com.rusefi.io.can.slcan;

import java.util.regex.Pattern;

/**
 * Recognized replies to the SLCAN 'V' (version) command. One rule is shared by port discovery,
 * the console SLCAN connector and the M74.9 flash tools so every adapter family is accepted or
 * rejected consistently.
 * <ul>
 * <li>Lawicel/CANable 1 firmware: {@code Vhhhh} (hardware and software version nibbles);</li>
 * <li>CANable 2 firmware: a git revision and repository, e.g.
 * {@code 16e7497-dirty github.com/normaldotcom/canable2.git};</li>
 * <li>WeAct Studio USB2CANFDV1 firmware, derived from CANable 2:
 * {@code WeAct Studio V1.0.0.3_bb264e71}.</li>
 * </ul>
 * CANable 2 and its WeAct derivative do not acknowledge setup commands; their ordered V reply is
 * used as a barrier instead, see {@link #isCanableFamily(String)}.
 */
public final class SlcanVersion {
    private static final Pattern LAWICEL = Pattern.compile("V[0-9A-Fa-f]{4}");
    private static final Pattern CANABLE2 = Pattern.compile(
        "[0-9a-fA-F]{7,40}(-dirty)? github\\.com/normaldotcom/canable2(-fw)?(\\.git)?");
    private static final Pattern WEACT = Pattern.compile(
        "WeAct Studio V[0-9]+(\\.[0-9]+)*(_[0-9a-fA-F]{7,40})?(-dirty)?");

    private SlcanVersion() {
    }

    /** True for a complete line that is a recognized reply to the SLCAN 'V' command. */
    public static boolean isVersionReply(String line) {
        return line != null && (LAWICEL.matcher(line).matches() || isCanableFamily(line));
    }

    /**
     * True for CANable 2 family firmware (CANable 2 and WeAct USB2CANFDV1), which reports a build
     * revision instead of Vhhhh and sends no acknowledgements for C/S/O commands.
     */
    public static boolean isCanableFamily(String line) {
        return line != null && (CANABLE2.matcher(line).matches() || WEACT.matcher(line).matches());
    }
}
