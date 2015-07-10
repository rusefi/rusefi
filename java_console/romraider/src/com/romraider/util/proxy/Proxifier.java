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

package com.romraider.util.proxy;

import static java.lang.reflect.Proxy.newProxyInstance;
import java.lang.reflect.Constructor;

public final class Proxifier {
    public static <T> T proxy(T t, Class<? extends Wrapper> cls) {
        Wrapper wrapper = instantiate(cls, t);
        return proxy(t, wrapper);
    }

    private static <T> T proxy(T t, Wrapper wrapper) {
        Class<?> cls = t.getClass();
        ClassLoader loader = cls.getClassLoader();
        Class<?>[] interfaces = cls.getInterfaces();
        return (T) newProxyInstance(loader, interfaces, wrapper);
    }

    private static <T> Wrapper instantiate(Class<? extends Wrapper> wrapper, T t) {
        try {
            Constructor<?> constructor = wrapper.getConstructor(Object.class);
            return (Wrapper) constructor.newInstance(t);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
