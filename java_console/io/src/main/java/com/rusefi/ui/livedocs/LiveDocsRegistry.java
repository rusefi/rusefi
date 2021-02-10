package com.rusefi.ui.livedocs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.config.Field;
import com.rusefi.config.generated.Fields;
import com.rusefi.ldmp.StateDictionary;

import java.util.ArrayList;
import java.util.List;

import static com.rusefi.binaryprotocol.IoHelper.putShort;
import static com.rusefi.binaryprotocol.IoHelper.swap16;

/**
 * Singleton map of all live documentation entities. Using this registry we know all the entities to update periodically.
 * Andrey Belomutskiy, (c) 2013-2020
 */
public enum LiveDocsRegistry {
    INSTANCE;

    private final List<LiveDocHolder> liveDocs = new ArrayList<>();

    public void register(LiveDocHolder holder) {
        liveDocs.add(holder);
    }

    public void refresh(BinaryProtocol binaryProtocol) {
        for (LiveDocHolder holder : liveDocs) {
            boolean visible = holder.isVisible();
            if (visible) {
                for (LiveDataContext context : holder.getActions().getActions().keySet()) {
                    refresh(binaryProtocol, holder, context);
                }
            }
        }
    }

    private void refresh(BinaryProtocol binaryProtocol, LiveDocHolder holder, LiveDataContext context) {
        int liveDocRequestId = context.getId();
        Field[] values = StateDictionary.INSTANCE.getFields("refresh", context);
        int size = Field.getStructureSize(values);

        byte[] packet = new byte[5];
        packet[0] = Fields.TS_GET_STRUCT;
        putShort(packet, 1, swap16(liveDocRequestId)); // offset
        putShort(packet, 3, swap16(size));

        byte[] responseWithCode = binaryProtocol.executeCommand(packet, "get LiveDoc");
        if (responseWithCode == null || responseWithCode.length != (size + 1) || responseWithCode[0] != Fields.TS_RESPONSE_OK)
            return;

        byte[] response = new byte[size];

        System.arraycopy(responseWithCode, 1, response, 0, size);

        holder.update(binaryProtocol, context, response);
    }
}
