/* $Id$
 *
 * Copyright 2007-2008 Cisco Systems Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

package etch.util;

import java.io.EOFException;
import java.nio.BufferOverflowException;

/**
 * Byte buffer with operation to support getting and putting
 * bytes.
 */
abstract public class ByteBuffer {
    /**
     * @return true if the buffer is empty.
     */
    public boolean isEmpty() {
        return length() == 0;
    }

    /**
     * @return true if the buffer is full.
     */
    public boolean isFull() {
        return length() == size();
    }

    /**
     * @return the amount of data the buffer can hold.
     */
    abstract public int size();

    /**
     * @return the amount of data in the buffer.
     */
    abstract public int length();

    /**
     * @return the next byte from the buffer.
     * @throws EOFException if the buffer is empty.
     */
    abstract public byte get() throws EOFException;

    /**
     * Gets bytes from the buffer to fill buf.
     *
     * @param buf
     * @return the length of data put into buf.
     */
    public int get(byte[] buf) {
        return get(buf, 0, buf.length);
    }

    /**
     * Gets bytes from the buffer to fill buf, starting at the specified
     * position for the specified length.
     *
     * @param buf
     * @param off
     * @param len
     * @return the length of data put into buf.
     */
    public int get(byte[] buf, int off, int len) {
        if (off < 0)
            throw new IllegalArgumentException("off < 0");

        if (len < 0)
            throw new IllegalArgumentException("len < 0");

        if (off + len > buf.length)
            throw new IllegalArgumentException("off+len > buf.length");

        if (len == 0)
            return 0;

        int count = 0;
        try {
            while (len > 0) {
                buf[off] = get();
                count++;
                off++;
                len--;
            }
        } catch (EOFException e) {
            // nothing to do.
        }
        return count;
    }

    /**
     * Puts a byte into the buffer.
     *
     * @param b
     * @throws BufferOverflowException if the buffer is full
     */
    abstract public void put(byte b) throws BufferOverflowException;

    /**
     * Puts the bytes from buf into the buffer.
     *
     * @param buf
     * @return the amount of data copied into the buffer.
     */
    public int put(byte[] buf) {
        return put(buf, 0, buf.length);
    }

    /**
     * Puts the bytes from buf into the buffer, starting at the specified
     * position for the specified length;
     *
     * @param buf
     * @param off
     * @param len
     * @return the amount of data copied into the buffer.
     */
    public int put(byte[] buf, int off, int len) {
        if (off < 0)
            throw new IllegalArgumentException("off < 0");

        if (len < 0)
            throw new IllegalArgumentException("len < 0");

        if (off + len > buf.length)
            throw new IllegalArgumentException("off+len > buf.length");

        if (len == 0)
            return 0;

        int count = 0;
        try {
            while (len > 0) {
                put(buf[off]);
                count++;
                off++;
                len--;
            }
        } catch (BufferOverflowException e) {
            // nothing to do.
        }
        return count;
    }

    /**
     * Clears the buffer.
     */
    abstract public void clear();

    /**
     * @return a little-endian 32-bit integer from the buffer.
     * @throws EOFException
     */
    public int getInt() throws EOFException {
        int b0 = get() & 255;
        int b1 = get() & 255;
        int b2 = get() & 255;
        int b3 = get() & 255;
        return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
    }

    public int getShort() throws EOFException {
        int b0 = get() & 255;
        int b1 = get() & 255;
        return (b0 | (b1 << 8));
    }

    public int getByte() throws EOFException {
        int b0 = get() & 255;
        return b0;
    }
}