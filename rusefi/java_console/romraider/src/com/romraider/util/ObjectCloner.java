/*
 * RomRaider Open-Source Tuning, Logging and Reflashing
 * Copyright (C) 2006-2012 RomRaider.com
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

package com.romraider.util;

import java.io.*;

public final class ObjectCloner {

    private ObjectCloner() {
    }

    // returns a deep copy of an object
    public static Object deepCopy(Object obj) {
        /*ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try {
            ObjectOutputStream oos = new ObjectOutputStream(bos);
            try {
                ByteArrayInputStream bin = new ByteArrayInputStream(bos.toByteArray());
                try {
                    ObjectInputStream ois = new ObjectInputStream(bin);
                    try {
                        // serialize and pass the object
                        oos.writeObject(obj);
                        oos.flush();

                        // return the new object
                        return ois.readObject();

                    } finally {
                        ois.close();
                    }
                } finally {
                    bin.close();
                }
            } finally {
                oos.close();
            }
        } finally {
            bos.close();
        }*/

        /*ByteArrayOutputStream baos = new ByteArrayOutputStream();
       ObjectOutputStream oos = new ObjectOutputStream(baos);
       oos.writeObject(obj);
       ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
       ObjectInputStream ois = new ObjectInputStream(bais);
       Object deepCopy = ois.readObject();
       return deepCopy;*/

        //obj2DeepCopy must be serializable
        ObjectOutputStream outStream = null;
        ObjectInputStream inStream = null;

        try {
            ByteArrayOutputStream byteOut = new ByteArrayOutputStream();
            outStream = new ObjectOutputStream(byteOut);
            outStream.writeObject(obj);
            outStream.flush();
            ByteArrayInputStream byteIn = new ByteArrayInputStream(byteOut.toByteArray());
            inStream = new ObjectInputStream(byteIn);

            // read the serialized, and deep copied, object and return it
            return inStream.readObject();

        } catch (Throwable e) {
            throw new IllegalStateException(e);

        } finally {
            //always close your streams in finally clauses
            close(outStream);
            close(inStream);
        }


    }

    public static void close(Closeable closeable) {
        if(closeable!=null) {
            try {
                closeable.close();
            } catch (IOException e) {
                // ignoring exception during close()
            }
        }
    }
}