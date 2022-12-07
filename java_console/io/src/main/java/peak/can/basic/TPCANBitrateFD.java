/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANBitrateFD.java 7391 2020-08-10 08:32:30Z Fabrice $
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
 * FD bit rates for the CAN FD controller. You can
 * define your own bit rate with the FD string, as shown in this exemple:
 * "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, 
 * data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1"
 */
public class TPCANBitrateFD {
    private String value;

    /**
     * Creates a CAN FD bitrate
     * @param value A CAN FD bitrate string (for instance:
     * "f_clock_mhz=20, nom_brp=5, nom_tseg1=2, nom_tseg2=1, nom_sjw=1, 
     *  data_brp=2, data_tseg1=3, data_tseg2=1, data_sjw=1")
     */
    public TPCANBitrateFD(String value) {
        this.value = value;
    }

    /**
     * Returns the string configuration of the bitrate code.
     * @return The bitrate string configuration
     */
    public String getValue() {
        return this.value;
    }
    /**
     * Sets string configuration of the bitrate code.
     * @param value The new bitrate string configuration
     */
    public void setValue(String value) {
        this.value = value;
    }
};
