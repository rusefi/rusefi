/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: RcvEventDispatcher.java 7391 2020-08-10 08:32:30Z Fabrice $
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
 * This class is a gateway between the PCAN Light JNI and the application to dispatch the CAN Receive-Event.
 *
 * RcvEventDispatcher contains a public static method dispatchRcvEvent which is called from the JNI to notify the Java
 * application when the handle of the Receive-Event detects a state change.
 */
public class RcvEventDispatcher
{
    static private IRcvEventProcessor listener;

    /**
     * Gets the Receive-Event processor
     * @return a IRcvEventProcessor
     */
    public static IRcvEventProcessor getListener()
    {
        return listener;
    }

    /**
     * Sets the Receive-Event processor
     * @param listener a IRcvEventProcessor implementor
     */
    public static void setListener(IRcvEventProcessor listener)
    {
        RcvEventDispatcher.listener = listener;
    }

    /**
     * This static public method will call from JNI to process the Receive-Event
     * by the listener
     * @param channel CAN Channel to dispatch the event to.
     */
    static public void dispatchRcvEvent(TPCANHandle channel)
    {
        if(listener != null)
            listener.processRcvEvent(channel);
    }
}
