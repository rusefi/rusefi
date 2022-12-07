/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: MutableTPCANHandle.java 7377 2020-08-07 14:40:53Z Fabrice $
 * @LastChange $Date: 2020-08-07 16:40:53 +0200 (ven., 07 août 2020) $
 * 
 * Demo Application for PCANBasic JAVA JNI Interface.
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
 * The MutableTPCANHandle class wraps a value of the immutable enumeration TPCANHandle in an object (to be used with the JNI library).
 * 
 * @version 1.0
 * @LastChange $Date: $
 * @author $Author: $
 *
 * @Copyright (C) 1999-2021  PEAK-System Technik GmbH, Darmstadt
 * more Info at http://www.peak-system.com
 */
public class MutableTPCANHandle
{

    /**
     * Constructor
     * @param value TPCANHandle value
     */
    public MutableTPCANHandle()
    {
        this(TPCANHandle.PCAN_NONEBUS);
    }
    /**
     * Constructor
     * @param value TPCANHandle value
     */
    public MutableTPCANHandle(TPCANHandle value)
    {
        this.value = value;
    }

    /**
     * Constructor based on internal TPCANHandle enum value 
     * @param value TPCANHandle as string
     */
    public MutableTPCANHandle(short value)
    {
    	this.value = TPCANHandle.PCAN_NONEBUS;
    	for(TPCANHandle handle: TPCANHandle.values()) {
    		if (value == handle.getValue())
    			this.value = handle;
    	}
    }

    /**
     * Gets TPCANHandle value
     * @return TPCANHandle value
     */
    public TPCANHandle getValue()
    {
        return value;
    }

    /**
     * Sets TPCANHandle value
     * @param value TPCANHandle value
     */
    public void setValue(TPCANHandle value)
    {
        this.value = value;
    }
    public TPCANHandle value;

    /**
     * Overrides toString() to display int value
     * @return MutableInteger's value as a string
     */
    @Override
    public String toString()
    {
        return value.toString();
    }      
}
