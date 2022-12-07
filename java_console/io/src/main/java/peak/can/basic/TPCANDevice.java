/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANDevice.java 7391 2020-08-10 08:32:30Z Fabrice $
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
 * Represents a PCAN device
 */
public enum TPCANDevice {

    /**
     * Undefined, unknown or not selected PCAN device value
     */
    PCAN_NONE((byte) 0x00),
    /**
     * PCAN Non-Plug'n'Play devices. NOT USED WITHIN PCAN-Basic API
     */
    PCAN_PEAKCAN((byte) 0x01),
    /**
     * PCAN-ISA, PCAN-PC/104, and PCAN-PC/104-Plus
     */
    PCAN_ISA((byte) 0x02),
    /**
     * PCAN-Dongle
     */
    PCAN_DNG((byte) 0x03),
    /**
     * PCAN-PCI, PCAN-cPCI, PCAN-miniPCI, and PCAN-PCI Express
     */
    PCAN_PCI((byte) 0x04),
    /**
     * PCAN-USB and PCAN-USB Pro
     */
    PCAN_USB((byte) 0x05),
    /**
     * PCAN-PC Card
     */
    PCAN_PCC((byte) 0x06),
    /**
     * PCAN Virtual hardware. NOT USED WITHIN PCAN-Basic API
     */
    PCAN_VIRTUAL((byte) 0x07),
    /**
     * PCAN Gateway devices
     */
    PCAN_LAN((byte) 0x08);

    private TPCANDevice(byte value) {
        this.value = value;
    }

    /**
     * The value of the CAN device
     * @return Byte value of the CAN device
     */
    public byte getValue() {
        return this.value;
    }
    private final byte value;
};
