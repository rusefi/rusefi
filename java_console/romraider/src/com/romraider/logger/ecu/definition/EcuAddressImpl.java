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

import static com.romraider.util.HexUtil.asBytes;
import static com.romraider.util.HexUtil.hexToInt;
import static com.romraider.util.HexUtil.intToHexString;
import static com.romraider.util.ParamChecker.checkGreaterThanZero;
import static com.romraider.util.ParamChecker.checkNotNullOrEmpty;
import java.util.LinkedList;
import java.util.List;

public final class EcuAddressImpl implements EcuAddress {
    private final String[] addresses;
    private final byte[] bytes;
    private final int bit;


    public EcuAddressImpl(String address, int length, int bit) {
        checkNotNullOrEmpty(address, "address");
        checkGreaterThanZero(length, "length");
        final String[] addrSting = {address};
        this.addresses = buildAddresses(addrSting, length);
        this.bytes = getAddressBytes(addresses);
        this.bit = bit;
    }

    public EcuAddressImpl(String[] address, int length, int bit) {
        checkNotNullOrEmpty(address, "address");
        checkGreaterThanZero(length, "length");
        this.addresses = buildAddresses(address, length);
        this.bytes = getAddressBytes(addresses);
        this.bit = bit;
    }

    public EcuAddressImpl(String[] addresses) {
        checkNotNullOrEmpty(addresses, "addresses");
        this.addresses = addresses;
        this.bytes = getAddressBytes(addresses);
        this.bit = -1;
    }

    public String[] getAddresses() {
        return addresses;
    }

    public byte[] getBytes() {
        return bytes;
    }

    public int getBit() {
        return bit;
    }

    public int getLength() {
        return addresses.length;
    }

    private String[] buildAddresses(String[] startAddress, int addressLength) {
        final List<String> addresses = new LinkedList<String>();
        if (startAddress.length > addressLength) {
            for (int i = 0; i < startAddress.length; i++) {
                final int address = hexToInt(startAddress[i]);
                addresses.add(
                        padAddress(intToHexString(address), 
                        startAddress[i].length()));
            }
        }
        else {
            int start = hexToInt(startAddress[0]);
            for (int i = 0; i < addressLength; i++) {
                addresses.add(
                        padAddress(intToHexString(start + i),
                        startAddress[0].length()));
            }
        }
        return addresses.toArray(new String[addresses.size()]);
    }

    private String padAddress(String address, int length) {
        if (address.length() == length) {
            return address;
        } else {
            StringBuilder builder = new StringBuilder(length);
            builder.append("0x");
            String s = address.substring(2);
            for (int i = 0; i < length - s.length() - 2; i++) {
                builder.append('0');
            }
            builder.append(s);
            return builder.toString();
        }
    }

    private byte[] getAddressBytes(String[] addresses) {
        byte[] bytes = new byte[0];
        for (String address : addresses) {
            byte[] tmp1 = asBytes(address);
            byte[] tmp2 = new byte[bytes.length + tmp1.length];
            System.arraycopy(bytes, 0, tmp2, 0, bytes.length);
            System.arraycopy(tmp1, 0, tmp2, bytes.length, tmp1.length);
            bytes = tmp2;
        }
        return bytes;
    }
}
