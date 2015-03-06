/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.romraider.logger.ecu.definition;

import static com.romraider.logger.ecu.definition.EcuDataType.PARAMETER;
import static com.romraider.util.ParamChecker.checkNotNull;
import static com.romraider.util.ParamChecker.checkNotNullOrEmpty;
import java.util.HashSet;
import java.util.Set;

public final class EcuParameterImpl implements EcuParameter {
    private final String id;
    private final String name;
    private final String description;
    private final EcuAddress address;
    private final EcuDataConvertor[] convertors;
    private final Set<ConvertorUpdateListener> listeners = new HashSet<ConvertorUpdateListener>();
    private int selectedConvertorIndex;
    private boolean selected;

    public EcuParameterImpl(String id, String name, String description, EcuAddress address, EcuDataConvertor[] convertors) {
        checkNotNullOrEmpty(id, "id");
        checkNotNullOrEmpty(name, "name");
        checkNotNull(description, "description");
        checkNotNull(address, "address");
        checkNotNullOrEmpty(convertors, "convertors");
        this.id = id;
        this.name = name;
        this.description = description;
        this.address = address;
        this.convertors = convertors;
    }

    public String getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getDescription() {
        return description;
    }

    public EcuAddress getAddress() {
        return address;
    }

    public EcuDataConvertor getSelectedConvertor() {
        return convertors[selectedConvertorIndex];
    }

    public EcuDataConvertor[] getConvertors() {
        return convertors;
    }

    public void selectConvertor(EcuDataConvertor convertor) {
        if (convertor != getSelectedConvertor()) {
            for (int i = 0; i < convertors.length; i++) {
                EcuDataConvertor dataConvertor = convertors[i];
                if (convertor == dataConvertor) {
                    selectedConvertorIndex = i;
                }
            }
            notifyUpdateListeners();
        }
    }

    public EcuDataType getDataType() {
        return PARAMETER;
    }

    public boolean isSelected() {
        return selected;
    }

    public void setSelected(boolean selected) {
        this.selected = selected;
    }

    public void addConvertorUpdateListener(ConvertorUpdateListener listener) {
        checkNotNull(listener, "listener");
        listeners.add(listener);
    }

    private void notifyUpdateListeners() {
        for (ConvertorUpdateListener listener : listeners) {
            listener.notifyConvertorUpdate(this);
        }
    }
}
