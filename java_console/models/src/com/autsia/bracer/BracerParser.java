/**
 * TODO: for rusEfi purposes, we can remove Complex and thus not depend on
 * apache-commons-math
 *
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

package com.autsia.bracer;

import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Collection;
import java.util.Collections;
import java.util.Locale;
import java.util.Stack;
import java.util.StringTokenizer;

/**
 * Class for parsing and evaluating math expressions
 *
 * @author Dmytro Titov
 * @version 7.0
 * @since 1.0
 */
public class BracerParser {
    /* list of available functions */
    private final String[] FUNCTIONS = {"abs", "acos", "arg", "asin", "atan",
            "conj", "cos", "cosh", "exp", "imag", "log", "neg", "pow", "real",
            "sin", "time_since_boot", "sqrt", "tan", "rpm", "not"};
    /* list of available operators */
    private final String OPERATORS = "<>=+-*/&|!";
    /* separator of arguments */
    private final String SEPARATOR = ",";
    /* variable token */
    private final String VARIABLE = "var";
    /* temporary stack that holds operators, functions and brackets */
    private Stack<String> stackOperations = new Stack<>();
    /* stack for holding expression converted to reversed polish notation */
    private Stack<String> stackRPN = new Stack<>();
    /* stack for holding the calculations result */
    private Stack<String> stackAnswer = new Stack<>();

    /**
     * Class ctor for setting up the complex format of the parser
     *
     * @since 2.0
     */
    public BracerParser() {
    }

    /**
     * Parses the math expression (complicated formula) and stores the result
     *
     * @param expression <code>String</code> input expression (math formula)
     * @throws <code>ParseException</code> if the input expression is not
     *                                     correct
     * @since 3.0
     */
    public void parse(String expression) throws ParseException {
        /* cleaning stacks */
        stackOperations.clear();
        stackRPN.clear();

		/*
         * make some preparations: remove spaces; handle unary + and -, handle
		 * degree character
		 */
        expression = expression.replace(" ", "")
                .replace("(-", "(0-").replace(",-", ",0-").replace("(+", "(0+")
                .replace(",+", ",0+").replace("true", "1").replace("false", "0")
                .replace("or", "|").replace("and", "&");
        if (expression.charAt(0) == '-' || expression.charAt(0) == '+') {
            expression = "0" + expression;
        }
        /* splitting input string into tokens */
        StringTokenizer stringTokenizer = new StringTokenizer(expression,
                OPERATORS + SEPARATOR + "()", true);

		/* loop for handling each token - shunting-yard algorithm */
        while (stringTokenizer.hasMoreTokens()) {
            String token = stringTokenizer.nextToken();
            if (isSeparator(token)) {
                while (!stackOperations.empty()
                        && !isOpenBracket(stackOperations.lastElement())) {
                    stackRPN.push(stackOperations.pop());
                }
            } else if (isOpenBracket(token)) {
                stackOperations.push(token);
            } else if (isCloseBracket(token)) {
                while (!stackOperations.empty()
                        && !isOpenBracket(stackOperations.lastElement())) {
                    stackRPN.push(stackOperations.pop());
                }
                stackOperations.pop();
                if (!stackOperations.empty()
                        && isFunction(stackOperations.lastElement())) {
                    stackRPN.push(stackOperations.pop());
                }
            } else if (isNumber(token)) {
                stackRPN.push(token);
            } else if (isOperator(token)) {
                while (!stackOperations.empty()
                        && isOperator(stackOperations.lastElement())
                        && getPrecedence(token) <= getPrecedence(stackOperations
                        .lastElement())) {
                    stackRPN.push(stackOperations.pop());
                }
                stackOperations.push(token);
            } else if (isFunction(token)) {
                stackOperations.push(token);
            } else {
                throw new ParseException("Unrecognized token: " + token, 0);
            }
        }
        while (!stackOperations.empty()) {
            stackRPN.push(stackOperations.pop());
        }

		/* reverse stack */
        Collections.reverse(stackRPN);
    }

    /**
     * Evaluates once parsed math expression with no variable included
     *
     * @return <code>String</code> representation of the result
     * @throws <code>ParseException</code> if the input expression is not
     *                                     correct
     * @since 1.0
     */
    public String evaluate() throws ParseException {
        if (!stackRPN.contains("var")) {
            return evaluate(0);
        }
        throw new ParseException("Unrecognized token: var", 0);
    }

    /**
     * Evaluates once parsed math expression with "var" variable included
     *
     * @param variableValue User-specified <code>Double</code> value
     * @return <code>String</code> representation of the result
     * @throws <code>ParseException</code> if the input expression is not
     *                                     correct
     * @since 3.0
     */
    public String evaluate(double variableValue) throws ParseException {
        /* check if is there something to evaluate */
        if (stackRPN.empty()) {
            return "";
        }

		/* clean answer stack */
        stackAnswer.clear();

		/* get the clone of the RPN stack for further evaluating */
        @SuppressWarnings("unchecked")
        Stack<String> stackRPN = (Stack<String>) this.stackRPN.clone();

		/* enroll the variable value into expression */
        Collections.replaceAll(stackRPN, VARIABLE,
                Double.toString(variableValue));

		/* evaluating the RPN expression */
        while (!stackRPN.empty()) {
            String token = stackRPN.pop();
            if (isNumber(token)) {
                stackAnswer.push(token);
            } else if (isOperator(token)) {
                Double a = Double.valueOf(stackAnswer.pop());
                Double b = Double.valueOf(stackAnswer.pop());
                boolean aBoolean = a.doubleValue() == 1.0;
                boolean bBoolean = b.doubleValue() == 1.0;
                switch (token) {
                    case "+":
                        stackAnswer.push(Double.toString(b + a));
                        break;
                    case "-":
                        stackAnswer.push(Double.toString(b - (a)));
                        break;
                    case "*":
                        stackAnswer.push(Double.toString(b * (a)));
                        break;
                    case "/":
                        stackAnswer.push(Double.toString(b / (a)));
                        break;
                    case "|":
                        stackAnswer.push(String.valueOf(aBoolean || bBoolean ? "1" : "0"));
                        break;
                    case "&":
                        stackAnswer.push(String.valueOf(aBoolean && bBoolean ? "1" : "0"));
                        break;
                }
            } else if (isFunction(token)) {
                Double a = Double.valueOf(stackAnswer.pop());
                boolean aBoolean = a.doubleValue() == 1.0;
                switch (token) {
//                    case "abs":
//                        stackAnswer.push(Double.toString(a.abs()));
//                        break;
//                    case "acos":
//                        stackAnswer.push(Double.toString(a.acos()));
//                        break;
//                    case "arg":
//                        stackAnswer.push(complexFormat.format(a.getArgument()));
//                        break;
//                    case "asin":
//                        stackAnswer.push(complexFormat.format(a.asin()));
//                        break;
//                    case "atan":
//                        stackAnswer.push(complexFormat.format(a.atan()));
//                        break;
//                    case "conj":
//                        stackAnswer.push(complexFormat.format(a.conjugate()));
//                        break;
                    case "cos":
                        stackAnswer.push(Double.toString(Math.cos(a)));
                        break;
//                    case "cosh":
//                        stackAnswer.push(complexFormat.format(a.cosh()));
//                        break;
                    case "exp":
                        stackAnswer.push(Double.toString(Math.exp(a)));
                        break;
//                    case "imag":
//                        stackAnswer.push(complexFormat.format(a.getImaginary()));
//                        break;
//                    case "log":
//                        stackAnswer.push(complexFormat.format(a.log()));
//                        break;
//                    case "neg":
//                        stackAnswer.push(complexFormat.format(a.negate()));
//                        break;
                    case "sin":
                        stackAnswer.push(Double.toString(Math.sin(a)));
                        break;
//                    case "sqrt":
//                        stackAnswer.push(Double.toString(a.sqrt()));
//                        break;
//                    case "tan":
//                        stackAnswer.push(Double.toString(a.tan()));
//                        break;
//                    case "tanh":
//                        stackAnswer.push(Double.toString(a.tanh()));
//                        break;
                    case "pow":
                        Double b = Double.valueOf(stackAnswer.pop());
                        stackAnswer.push(Double.toString(Math.pow(a, b)));
                        break;
                    case "not":
                        stackAnswer.push(String.valueOf(!aBoolean ? "1" : "0"));
                        break;
                    default:
                        throw new IllegalStateException("Unexpected: " + token);
                }
            }
        }

        if (stackAnswer.size() > 1) {
            throw new ParseException("Some operator is missing", 0);
        }

        return stackAnswer.pop();
    }

    /**
     * Get back an <b>unmodifiable copy</b> of the stack
     */
    public Collection<String> getStackRPN() {
        return Collections.unmodifiableCollection(stackRPN);
    }

    /**
     * Check if the token is number (0-9, <code>IMAGINARY</code> or
     * <code>VARIABLE</code>)
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isNumber(String token) {
        try {
            Double.parseDouble(token);
        } catch (Exception e) {
            return token.equals(VARIABLE);
        }
        return true;
    }

    /**
     * Check if the token is function (e.g. "sin")
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isFunction(String token) {
        for (String item : FUNCTIONS) {
            if (item.equals(token)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Check if the token is <code>SEPARATOR</code>
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isSeparator(String token) {
        return token.equals(SEPARATOR);
    }

    /**
     * Check if the token is opening bracket
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isOpenBracket(String token) {
        return token.equals("(");
    }

    /**
     * Check if the token is closing bracket
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isCloseBracket(String token) {
        return token.equals(")");
    }

    /**
     * Check if the token is operator (e.g. "+")
     *
     * @param token Input <code>String</code> token
     * @return <code>boolean</code> output
     * @since 1.0
     */
    private boolean isOperator(String token) {
        return OPERATORS.contains(token);
    }

    /**
     * Gets the precedence of the operator
     *
     * @param token Input <code>String</code> token
     * @return <code>byte</code> precedence
     * @since 1.0
     */
    private byte getPrecedence(String token) {
        if (token.equals("+") || token.equals("-")) {
            return 1;
        }
        return 2;
    }
}
