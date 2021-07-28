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

package com.romraider.util;

import java.io.File;

/**
 * Shorten a long text file path for use in title bar or limited width display
 * component.
 */
public final class FormatFilename {
    private final static String separator = System.getProperty("file.separator");

    private FormatFilename() {
    }

    /**
     * Shorten a text file path name if > 45 characters.
     * @param file - the File to shorten the path name for.
     * @return the shortened name
     */
    public final static String getShortName(File file) {
        String filePath = file.getAbsolutePath();
        if (filePath.length() > 45) {
            filePath = getShortName(filePath);
        }
        return filePath;
    }

    public final static String getShortName(String filePath) {
        String regex = separator;
        if (separator.equals("\\")) {
            regex = "\\" + separator;
        }
        final String[] filePathParts = filePath.split(regex);
        String logFileName = filePath;
        if (filePathParts.length > 3) {
            logFileName = String.format(
                            "%s%s%s...%s%s%s%s",
                            filePathParts[0],
                            separator,
                            filePathParts[1].substring(0,
                                    Math.min(4, filePathParts[1].length())),
                            separator,
                            filePathParts[filePathParts.length - 2],
                            separator,
                            filePathParts[filePathParts.length - 1]);
        }
        return logFileName;
    }
}
