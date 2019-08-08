package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.generated.Fields;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.rusefi.binaryprotocol.BinaryProtocolCommands.COMMAND_GET_STRUCT;
import static com.rusefi.binaryprotocol.BinaryProtocolCommands.RESPONSE_OK;
import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;

/**
 * Singleton map of all live documentation entities. Using this registry we know all the entities to update periodically.
 */
public enum LiveDocsRegistry {
    INSTANCE;

    private final List<LiveDocHolder> liveDocs = new ArrayList<>();

    public void register(LiveDocHolder holder) {
        liveDocs.add(holder);
    }

    public void refresh(BinaryProtocol binaryProtocol) {
        for (LiveDocHolder h : liveDocs) {
            boolean visible = h.isVisible();
            System.out.println(h + ": is_visible=" + visible);
            if (visible) {
                int liveDocRequestId = h.getId();
                int size = h.getStructSize();

                byte packet[] = new byte[5];
                packet[0] = COMMAND_GET_STRUCT;
                putShort(packet, 1, swap16(liveDocRequestId)); // offset
                putShort(packet, 3, swap16(size));

                byte[] responseWithCode = binaryProtocol.executeCommand(packet, "get LiveDoc", false);
                if (responseWithCode == null || responseWithCode.length != (size + 1) || responseWithCode[0] != RESPONSE_OK)
                    continue;

                byte [] response = new byte[size];

                System.arraycopy(responseWithCode, 1, response, 0, size);

                h.update(binaryProtocol, response);
            }
        }
    }
}
