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
import org.junit.Test;

import com.autsia.bracer.BracerParser;

/**
 * Test class.
 * User: Dmytro
 * Date: 23.01.13
 * Time: 12:52
 */
public class BracerParserTest {
    private final String INPUT_NOVAR = "-sin(3+4+cos(6)/exp(10/pow(22,-1)))";
    private BracerParser bracerParser;

    @Before
    public void setUp() throws Exception {
        bracerParser = new BracerParser();
    }

    @Test
    public void testEvaluateNoVar() throws Exception {
        bracerParser.parse(INPUT_NOVAR);
        Assert.assertEquals("-0.6570194619480038", bracerParser.evaluate());
    }

    @Test
    public void testSimpleBoolean() throws Exception {
        bracerParser.parse("true or false");
        Assert.assertEquals("1", bracerParser.evaluate());
    }

    @Test
    public void testBoolean() throws Exception {
        bracerParser.parse("( ( true and ( false or ( true and ( ( true ) or ( false ) ) ) ) ) and ( ( false ) ) )");
        Assert.assertEquals("0", bracerParser.evaluate());

        Collection<String> stackRPN = bracerParser.getStackRPN();

        Assert.assertEquals("[&, 0, &, |, &, |, 0, 1, 1, 0, 1]", stackRPN.toString());
    }

    @Test
    public void testBooleanNot1() throws Exception {
        bracerParser.parse("not( ( true and ( false or ( true and ( not( true ) or ( false ) ) ) ) ) and ( ( false ) ) )");
        Assert.assertEquals("1", bracerParser.evaluate());
    }

    @Test
    public void testRusEfi() throws ParseException {
        bracerParser.parse("(time_since_boot < 4) | (rpm > 0)");

        Assert.assertEquals("[|, rpm, >, 0, time_since_boot, <, 4]", bracerParser.getStackRPN().toString());


        bracerParser.parse("(time_since_boot <= 4) | (rpm > 0)");
        Assert.assertEquals("[|, rpm, >, 0, time_since_boot, <=, 4]", bracerParser.getStackRPN().toString());

        bracerParser.parse("(time_since_boot <= 4) | (rpm > 0)");
        Assert.assertEquals("[|, rpm, >, 0, time_since_boot, <=, 4]", bracerParser.getStackRPN().toString());

        bracerParser.parse("(time_since_boot <= 4) OR (rpm > 0)");
        Assert.assertEquals("[OR, rpm, >, 0, time_since_boot, <=, 4]", bracerParser.getStackRPN().toString());
    }

    @Test
    public void testBooleanNot2() throws Exception {
        bracerParser.parse("(((true | false) & not(false)) | (true | false))");
        Assert.assertEquals("1", bracerParser.evaluate());

        Collection<String> stackRPN = bracerParser.getStackRPN();

        Assert.assertEquals("[|, |, 0, 1, &, not, 0, |, 0, 1]", stackRPN.toString());
    }
}
