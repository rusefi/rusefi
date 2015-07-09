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

/**
 * @author      Steve Wadsworth <lizzardo on RomRaider forum>
 * @since       2012-12-30
 * 
 * This class is intended to contain utility methods needed to determine various 
 * parameters of the JVM in which the application is running.
 * 
 */

public final class JREChecker {
    
    /**
     * Check to see if the JVM is 32-bit
     * 
     * @return true if JVM is 32-bit
     */
    public static boolean is32bit() {
        // determine if we're running in a 32-bit JVM
        // this may need to be extended for other JVM providers and versions
        String bitness = System.getProperty("sun.arch.data.model", "unknown");
        // if we don't know, try harder - additional tests can be added here as
        // necessary
        if (bitness.equals("unknown")) {
            // if sun.arch.data.model isn't found, we may be on a non-Sun
            // (Oracle) VM try some other properties
            if (System.getProperty("java.vm.name").indexOf("64") >= 0 ||
                    System.getProperty("sun.cpu.isalist").indexOf("64") >= 0) {
                bitness = "64";
            }
        }
        // should be either 32, 64, or still unknown.  only known 32 should
        // return true
        if (bitness.equals("32")) {
            return true;
        }
        return false;
    }
}
