/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANMessageType.java 7391 2020-08-10 08:32:30Z Fabrice $
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

import java.util.EnumSet;

/**
 * Represents the type of a PCAN message
 */
public enum TPCANMessageType {

	/**
	 * The PCAN message is a CAN Standard Frame (11-bit identifier)
	 */
	PCAN_MESSAGE_STANDARD((byte) 0x00),
	/**
	 * The PCAN message is a CAN Remote-Transfer-Request Frame
	 */
	PCAN_MESSAGE_RTR((byte) 0x01),
	/**
	 * The PCAN message is a CAN Extended Frame (29-bit identifier)
	 */
	PCAN_MESSAGE_EXTENDED((byte) 0x02),
	/**
	 * The PCAN message represents a FD frame in terms of CiA Specs
	 */
	PCAN_MESSAGE_FD((byte) 0x04),
	/**
	 * The PCAN message represents a FD bit rate switch (CAN data at a higher
	 * bitrate)
	 */
	PCAN_MESSAGE_BRS((byte) 0x08),
	/**
	 * The PCAN message represents a FD error state indicator(CAN FD transmitter was
	 * error active)
	 */
	PCAN_MESSAGE_ESI((byte) 0x10),
	/**
	 * The PCAN message represents an error frame
	 */
	PCAN_MESSAGE_ERRFRAME((byte) 0x40),
	/**
	 * The PCAN message represents a PCAN status message
	 */
	PCAN_MESSAGE_STATUS((byte) 0x80);

	private TPCANMessageType(byte value) {
		this.value = value;
	}

	/**
	 * The value of the message type
	 * 
	 * @return Value of the message type
	 */
	public byte getValue() {
		return this.value;
	}

	private final byte value;

	/**
	 * Gets the value of an EnumSet
	 * 
	 * @param type collection of TPCANMessageType
	 * @return value of the EnumSet
	 */
	public static byte getValue(EnumSet<TPCANMessageType> type) {
		byte result = 0;
		for (TPCANMessageType t : type)
			result |= t.value;
		return result;
	}
}
