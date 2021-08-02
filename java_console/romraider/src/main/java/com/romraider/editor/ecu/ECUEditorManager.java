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

package com.romraider.editor.ecu;

import com.rusefi.ui.util.UiUtils;


public class ECUEditorManager {
    private static ECUEditor editor = null;

    private ECUEditorManager() {
        throw new UnsupportedOperationException();
    }

    public static ECUEditor getECUEditor() {
        if (editor == null) {
            try {
                UiUtils.invokeAndWait(new Runnable() {
                    public void run() {
                        editor = new ECUEditor();
                    }
                });
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
        return editor;
    }

}
