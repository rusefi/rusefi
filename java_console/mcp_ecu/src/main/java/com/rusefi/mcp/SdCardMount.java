package com.rusefi.mcp;

import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Integration;
import com.rusefi.config.generated.VariableRegistryValues;
import com.rusefi.core.ISensorHolder;
import com.rusefi.core.OutputChannelSnapshot;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.LinkManager;
import com.rusefi.enums.SD_MODE;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

/** Gateway SD mount protocol, confirmed by fresh output polls rather than logger activity. */
final class SdCardMount {
    // Wire values of SD_MODE in firmware/hw_layer/mmc_card.h.
    private static final String[] MODE_NAMES = {"idle", "ecu", "pc", "unmounted", "formatting"};
    private static final String MODE_CHANNEL = VariableRegistryValues.OUTPUT_CHANNEL_SD_CARD_MODE;
    private static final String PRESENT_CHANNEL = VariableRegistryValues.OUTPUT_CHANNEL_SD_PRESENT;

    private SdCardMount() {
    }

    @SuppressWarnings("unchecked")
    static JSONObject mount(LinkManager link, SensorCentral sensors, boolean toEcu, long timeoutMs) throws Exception {
        BinaryProtocol protocol = link.getBinaryProtocol();
        IniFileModel ini = protocol == null ? null : protocol.getIniFileNullable();
        if (ini == null || !ini.getAllOutputChannels().containsKey(MODE_CHANNEL)
                || !ini.getAllOutputChannels().containsKey(PRESENT_CHANNEL)) {
            throw new IOException("SD mount confirmation requires firmware and a matching .ini with sdCardMode and sd_present");
        }

        String target = toEcu ? "ecu" : "pc";
        int targetMode = (toEcu ? SD_MODE.SD_MODE_ECU : SD_MODE.SD_MODE_PC).ordinal();
        long deadline = System.nanoTime() + TimeUnit.MILLISECONDS.toNanos(timeoutMs);
        JSONObject result = new JSONObject();
        result.put("success", false);
        result.put("requestedMode", target);
        result.put("mounted", false);

        byte[] payload = ByteBuffer.allocate(4)
                .putShort((short) VariableRegistryValues.ts_command_e_TS_X14)
                .putShort((short) (toEcu ? VariableRegistryValues.ts_14_command_TS_SD_MOUNT_ECU
                        : VariableRegistryValues.ts_14_command_TS_SD_MOUNT_PC)).array();
        Future<?> command = link.submit(() -> {
            byte[] response = protocol.executeCommand(Integration.TS_IO_TEST_COMMAND, payload, "mount SD to " + target);
            if (response == null || response.length == 0 || response[0] != (byte) Integration.TS_RESPONSE_OK) {
                throw new IllegalStateException("SD mount request was not acknowledged; completion is unconfirmed");
            }
        });
        try {
            command.get(Math.max(1, deadline - System.nanoTime()), TimeUnit.NANOSECONDS);
        } finally {
            // Prevent a queued request from switching ownership after the caller has timed out.
            command.cancel(false);
        }

        // Register after the acknowledgement. Cached values (including from an earlier connection)
        // must never satisfy the request. Polling runs on the same communication executor as the command.
        ArrayBlockingQueue<OutputChannelSnapshot> polls = new ArrayBlockingQueue<>(1);
        SensorCentral.SnapshotListenerToken listener = sensors.addSnapshotListener(snapshot -> {
            polls.poll();
            polls.offer(snapshot);
        });
        try {
            while (System.nanoTime() < deadline) {
                if (!link.isActive()) {
                    result.put("error", "ECU disconnected during SD mount; completion is unconfirmed. " +
                            "Reconnect and read sdCardMode before retrying.");
                    return result;
                }
                long remaining = deadline - System.nanoTime();
                OutputChannelSnapshot snapshot = polls.poll(Math.max(1,
                        Math.min(remaining, TimeUnit.MILLISECONDS.toNanos(100))), TimeUnit.NANOSECONDS);
                if (snapshot == null) {
                    continue;
                }
                byte[] response = snapshot.getResponse();
                Double mode = ISensorHolder.tryReadOutputChannel(snapshot, response, MODE_CHANNEL, ini, MODE_CHANNEL);
                Double present = ISensorHolder.tryReadOutputChannel(snapshot, response, PRESENT_CHANNEL, ini, PRESENT_CHANNEL);
                if (mode == null || present == null) {
                    continue;
                }
                result.put("sdCardMode", mode);
                result.put("sd_present", present);
                result.put("mode", mode >= 0 && mode < MODE_NAMES.length && mode == Math.rint(mode)
                        ? MODE_NAMES[mode.intValue()] : "unknown");
                if (present > 0.5 && mode == targetMode) {
                    result.put("success", true);
                    result.put("mounted", true);
                    return result;
                }
            }
            result.put("error", "Timed out waiting for SD card to mount to " + target +
                    "; completion is unconfirmed. Check card presence and sdCardMode. " +
                    "Safely eject any PC drive before switching to ECU.");
            return result;
        } finally {
            listener.remove();
        }
    }
}
