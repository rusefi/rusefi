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

import net.jcip.annotations.NotThreadSafe;

import java.io.EOFException;
import java.nio.BufferOverflowException;

/**
 * Description of CircularByteBuffer.
 */
@NotThreadSafe
public class CircularByteBuffer extends ByteBuffer {
    private final int size;

    private final byte[] buf;

    private int length;

    private int nextGet;

    private int nextPut;

    /**
     * Constructs the CircularByteBuffer.
     *
     * @param size
     */
    public CircularByteBuffer(int size) {
        this.size = size;
        buf = new byte[size];
    }

    @Override
    public void clear() {
        length = 0;
        nextGet = 0;
        nextPut = 0;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public int length() {
        return length;
    }

    @Override
    public byte get() throws EOFException {
        if (isEmpty())
            throw new EOFException();

        length--;
        byte b = buf[nextGet++];
        if (nextGet >= size)
            nextGet = 0;
        return b;
    }

    @Override
    public void put(byte b) throws BufferOverflowException {
        if (isFull())
            throw new BufferOverflowException();

        length++;
        buf[nextPut++] = b;
        if (nextPut >= size)
            nextPut = 0;
    }
}