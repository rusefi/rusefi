/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANMode.java 7391 2020-08-10 08:32:30Z Fabrice $
 * @LastChange $Date: 2020-08-10 10:32:30 +0200 (Mon, 10 Aug 2020) $
 * 
 * PCANBasic JAVA Interface.
 *
 * Copyright (C) 2001-2020  PEAK System-Technik GmbH <www.peak-system.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * PCAN is a registered Trademark of PEAK-System Germany GmbH
 *
 * Author: 		 Jonathan Urban/Uwe Wilhelm/Fabrice Vergnaud
 * Contact:      <linux@peak-system.com>
 * Maintainer:   Fabrice Vergnaud <f.vergnaud@peak-system.com>
 */
package peak.can.basic;

/**
 * Represents a PCAN filter mode.
 */
public enum TPCANMode {

    /**
     * Mode is Standard (11-bit identifier).
     */
    PCAN_MODE_STANDARD(TPCANMessageType.PCAN_MESSAGE_STANDARD.getValue()),
    /**
     * Mode is Extended (29-bit identifier).
     */
    PCAN_MODE_EXTENDED(TPCANMessageType.PCAN_MESSAGE_EXTENDED.getValue());

    private TPCANMode(byte value) {
        this.value = value;
    }

    /**
     * The value of the CAN ID type
     * @return Value of the CAN ID type
     */
    public byte getValue() {
        return this.value;
    }
    private final byte value;
};
