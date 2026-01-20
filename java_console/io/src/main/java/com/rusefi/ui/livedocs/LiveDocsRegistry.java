package com.rusefi.ui.livedocs;

import com.rusefi.config.Field;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.live_data_e;
import com.rusefi.ldmp.StateDictionary;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

/**
 * Singleton map of all live documentation entities. Using this registry we know all the entities to update periodically.
 * Andrey Belomutskiy, (c) 2013-2020
 */
public enum LiveDocsRegistry {
    INSTANCE;

    private final List<LiveDocHolder> liveDocs = new ArrayList<>();

    public static int getStructureSize(Field[] values) {
        Field last = values[values.length - 1];
        // todo: at the moment we do not support arrays and
        // todo: a lot of information is missing for example for Bit type, but this implementation is good enough for now
        return last.getOffset() + 4;
    }

    public void register(LiveDocHolder holder) {
        liveDocs.add(holder);
    }

    public void refresh(LiveDataProvider liveDataProvider) {
        for (LiveDocHolder holder : liveDocs) {
            boolean visible = holder.isVisible();
            if (visible) {
                live_data_e context = holder.getId();
                refresh(holder, context, liveDataProvider);
            }
        }
    }

    private void refresh(LiveDocHolder holder, live_data_e context, LiveDataProvider liveDataProvider) {

        byte[] response = liveDataProvider.provide(context);
        if (response == null)
            return;

        holder.update(response);
    }

    @NotNull
    public static LiveDataProvider getLiveDataProvider() {
        return context -> {
            Field[] values = StateDictionary.INSTANCE.getFields(context);
            int size = getStructureSize(values);
            /*
            byte[] packet = new byte[4];
            int offset = context.ordinal();
            ByteRange.packOffsetAndSize(offset, size, packet);
*/
            int structOffset = StateDictionary.INSTANCE.getOffset(context);
            byte[] overallOutputs = SensorCentral.getInstance().getResponse();

            byte[] response = new byte[size];

            System.arraycopy(overallOutputs, structOffset, overallOutputs, 0, size);
            return response;
        };
    }

    public interface LiveDataProvider {
        byte[] provide(live_data_e context);
    }

}
