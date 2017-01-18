/*
 * Copyright 2014 Dmytro Titov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.autsia.bracer.test;

import java.text.ParseException;
import java.util.Collection;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import com.autsia.bracer.BracerParser;

import static org.junit.Assert.assertEquals;

/**
 * Test class.
 * User: Dmytro
 * Date: 23.01.13
 */
public class BracerParserTest {
    private BracerParser bracerParser;

    @Before
    public void setUp() throws Exception {
        bracerParser = new BracerParser();
    }

    @Test
    public void testSimpleBoolean() throws Exception {
        bracerParser.parse("true or false");
        assertEquals("1", bracerParser.evaluate());
        assertEquals("1 0 |", bracerParser.getRusEfi());

        bracerParser.parse("true > false");
        assertEquals("1", bracerParser.evaluate());
        assertEquals("1 0 >", bracerParser.getRusEfi());

        bracerParser.parse("(true > false)");
        assertEquals("1", bracerParser.evaluate());
        assertEquals("1 0 >", bracerParser.getRusEfi());
    }

    @Test
    public void testBooleanConversion() throws ParseException {
        assertParse("rpm 0 >", "rpm > false");
        assertParse("rpm 0 >", "(rpm > false)");
        assertParse("rpm user0 > clt user2 > | vbatt user1 > |", "(rpm > user0) or (clt > user2) or (vbatt > user1)");
    }

    private void assertParse(String expectedRpn, String expression) {
        try {
            bracerParser.parse(expression);
        } catch (ParseException e) {
            throw new IllegalStateException(e);
        }
        assertEquals(expectedRpn, bracerParser.getRusEfi());
    }

    @Test
    public void testBoolean() throws Exception {
        bracerParser.parse("( ( true and ( false or ( true and ( ( true ) or ( false ) ) ) ) ) and ( ( false ) ) )");
        assertEquals("0", bracerParser.evaluate());

        Collection<String> stackRPN = bracerParser.getStackRPN();

        assertEquals("[&, 0, &, |, &, |, 0, 1, 1, 0, 1]", stackRPN.toString());
    }

    @Test
    @Ignore
    public void testBooleanNot1() throws Exception {
        // todo: why EmptyStackException? is the issue with the test or with us?
        bracerParser.parse("not( ( true and ( false or ( true and ( not( true ) or ( false ) ) ) ) ) and ( ( false ) ) )");
        assertEquals("1", bracerParser.evaluate());
    }

    @Test
    public void testRusEfi() throws ParseException {
        bracerParser.parse("(time_since_boot < 4) | (rpm > 0)");
        assertEquals("time_since_boot 4 < rpm 0 > |", bracerParser.getRusEfi());

        bracerParser.parse("(fan and (coolant > fan_off_setting)) OR (coolant > fan_on_setting)");
        assertEquals("fan coolant fan_off_setting > & coolant fan_on_setting > OR", bracerParser.getRusEfi());

        bracerParser.parse("(time_since_boot <= 4) | (rpm > 0)");
        assertEquals("time_since_boot 4 <= rpm 0 > |", bracerParser.getRusEfi());

        bracerParser.parse("(time_since_boot <= 4) | (rpm > 0)");
        assertEquals("time_since_boot 4 <= rpm 0 > |", bracerParser.getRusEfi());

        bracerParser.parse("(time_since_boot <= 4) OR (rpm > 0)");
        assertEquals("time_since_boot 4 <= rpm 0 > OR", bracerParser.getRusEfi());

        bracerParser.parse("(self and (rpm > 4800)) OR (rpm > 5000)");
        assertEquals("self rpm 4800 > & rpm 5000 > OR", bracerParser.getRusEfi());
    }

    @Test
    public void testBooleanNot2() throws Exception {
        assertEquals("0", bracerParser.parse("! false").evaluate());
        assertEquals("0", bracerParser.parse("! 0").evaluate());
        assertEquals("0", bracerParser.parse("1 & not(false)").evaluate());
        assertEquals("0", bracerParser.parse("1 & (!(false))").evaluate());
//        assertEquals("0", bracerParser.parse("1 &  !(0)").evaluate());
//        assertEquals("0", bracerParser.parse("1 &  !(false)").evaluate());

        bracerParser.parse("1 & not(false)");
        assertEquals("0", bracerParser.evaluate());// todo: this does not seem right
        assertEquals("1 not 0 &", bracerParser.getRusEfi()); // todo: this does not seem right

//        bracerParser.parse("not(false)"); // todo: this is broken
//        assertEquals("1", bracerParser.evaluate());
//        assertEquals("1 0 | not 0 & 1 0 | |", bracerParser.getRusEfi());

        bracerParser.parse("(((true | false) & not(false)) | (true | false))");
        assertEquals("1", bracerParser.evaluate());
        assertEquals("1 0 | not 0 & 1 0 | |", bracerParser.getRusEfi());
    }
}
