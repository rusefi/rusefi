/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANTimestamp.java 7391 2020-08-10 08:32:30Z Fabrice $
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
 * Defines the point of time at which a CAN message was received.
 */
public class TPCANTimestamp
{
    private long millis;
    private short millis_overflow;
    private short micros;

    /**
     * Default constructor
     */
    public TPCANTimestamp()
    {
    }

    /**
     * Gets microseconds
     * @return microseconds (0-999)
     */
    public short getMicros()
    {
        return micros;
    }

    /**
     * Sets microseconds
     * @param micros microseconds (0-999)
     */
    public void setMicros(short micros)
    {
        this.micros = micros;
    }

    /**
     * Gets milliseconds
     * @return milliseconds
     */
    public long getMillis()
    {
        return millis;
    }

    /**
     * Sets milliseconds
     * @param millis milliseconds
     */
    public void setMillis(long millis)
    {
        this.millis = millis;
    }

     /**
     * Gets milliseconds overflow
     * @return milliseconds overflow
     */
    public short getMillis_overflow()
    {
        return millis_overflow;
    }

    /**
     * Sets milliseconds overflow
     * @param millis_overflow milliseconds overflow
     */
    public void setMillis_overflow(short millis_overflow)
    {
        this.millis_overflow = millis_overflow;
    }
}
