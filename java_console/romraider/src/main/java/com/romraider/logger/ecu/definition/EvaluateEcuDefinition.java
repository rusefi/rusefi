/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2013 RomRaider.com
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

import java.util.Map;

import com.romraider.util.ParamChecker;

/**
 *  From the ECU Definition Map retrieve the definition for the current ECU ID.
 */
public final class EvaluateEcuDefinition {
    public EvaluateEcuDefinition() {
    }

    /**
     * Retrieve the definition for the current ECU ID.
     * @param ecuDefMap - a map of ECU Definitions keyed by ECU ID.
     * @param ecuId - the ECU ID to find the ECU Definition for.
     * @return an ECU Definition or a New ECU Definition with all but the ECU ID set
     * to "unknown".
     */
    public final EcuDefinition getDef(
            Map<String, EcuDefinition> ecuDefMap, String ecuId) {

        ParamChecker.checkNotNull(ecuDefMap, "EcuDefinitionMap");
        EcuDefinition ecuDef = ecuDefMap.get(ecuId);
        if (ecuDef == null) {
            ecuDef = new EcuDefinitionImpl(
                    ecuId, "unknown", "unknown", "unknown", null);
        }
        return ecuDef;
    }
}
