/* SPDX-License-Identifier: LGPL-2.1-only */
/*
 * $Id: TPCANChannelInformation.java 7391 2020-08-10 08:32:30Z Fabrice $
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
 * Author: 		 Fabrice Vergnaud
 * Contact:      <linux@peak-system.com>
 * Maintainer:   Fabrice Vergnaud <f.vergnaud@peak-system.com>
 */
package peak.can.basic;

/**
 * Class storing the channel's information retrieved with parameter TPCANParameter.PCAN_ATTACHED_CHANNELS.
 */
public class TPCANChannelInformation {
    private TPCANHandle _channel_handle;	// PCAN channel handle   
    private TPCANDevice _device_type;		// Kind of PCAN device
	private byte _controller_number;		// CAN-Controller number
    private int _device_features;			// Device capabilities flag (see FEATURE_*)
    private String _device_name;			// Device name
    private int _device_id;					// Device number   
    private int _channel_condition;			// Availability status of a PCAN-Channel      

    /**
     * Default constructor
     */
    public TPCANChannelInformation()
    {
    }  

    public TPCANHandle getChannelHandle() {
		return _channel_handle;
	}
	public void setChannelHandle(TPCANHandle handle) {
		this._channel_handle = handle;
	}

	public TPCANDevice getDeviceType() {
		return _device_type;
	}
	public void setDeviceType(TPCANDevice type) {
		this._device_type = type;
	}

	public byte getControllerNumber() {
		return _controller_number;
	}
	public void setControllerNumber(byte value) {
		this._controller_number = value;
	}

	public int getDeviceFeatures() {
		return _device_features;
	}
	public void setDeviceFeatures(int features) {
		this._device_features = features;
	}

	public String getDeviceName() {
		return _device_name;
	}
	public void setDeviceName(String name) {
		this._device_name = name;
	}

	public int getDeviceId() {
		return _device_id;
	}
	public void setDeviceId(int id) {
		this._device_id = id;
	}

	public int getChannelCondition() {
		return _channel_condition;
	}
	public void setChannelCondition(int value) {
		this._channel_condition = value;
	} 
	
	public String toListString(String listIndent) {
		String str = "";
		str += "\n" + listIndent + "channel_handle = " + getChannelHandle();
		str += "\n" + listIndent + "device_type = " + getDeviceType();
		str += "\n" + listIndent + "controller_number = " + getControllerNumber();
		str += "\n" + listIndent + "device_features = " + getDeviceFeatures();
		str += "\n" + listIndent + "device_name = " + getDeviceName();
		str += "\n" + listIndent + "device_id = 0x" + Integer.toHexString(getDeviceId());
		str += "\n" + listIndent + "channel_condition = " + getChannelCondition();
		return str;
	}
}
