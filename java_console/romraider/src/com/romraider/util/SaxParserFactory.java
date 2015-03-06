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

import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;

public final class SaxParserFactory {

    private SaxParserFactory() {
        throw new UnsupportedOperationException();
    }

    public static SAXParser getSaxParser() throws ParserConfigurationException, SAXException {
        SAXParserFactory parserFactory = SAXParserFactory.newInstance();
        parserFactory.setNamespaceAware(false);
        parserFactory.setValidating(true);
        parserFactory.setXIncludeAware(false);
        return parserFactory.newSAXParser();
    }

    public static void main(String args[]) {
        try {
            SAXParser parser = SaxParserFactory.getSaxParser();
            BufferedInputStream b = new BufferedInputStream(new FileInputStream(new File("/ecu_defs.xml")));
            System.out.println(b.available());
            parser.parse(b, new DefaultHandler());
            System.out.println(parser.isValidating());

        } catch (Exception ex) {
            System.err.println(ex);
        }
    }
}
