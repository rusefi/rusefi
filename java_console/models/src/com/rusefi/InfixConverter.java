package com.rusefi;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.fathzer.soft.javaluator.Function;
import com.fathzer.soft.javaluator.Operator;

import java.util.Stack;

public class InfixConverter {
    public static String getHumanInfixFormOrError(String rpn) {
        try {
            return getHumanInfixForm(rpn);
        } catch (Throwable e) {
            return "Failing to parse. Maybe invalid function was used? Valid functions include " + getReadableListOfFunctions();
        }
    }

    public static String getHumanInfixForm(String rpn) {
        String tokens[] = rpn.split(" ");
        Stack<String> stack = new Stack<>();

        for (String token : tokens) {
            if (DoubleEvaluator.getFunction(token) != null) {
                int pCount = DoubleEvaluator.getFunction(token).getMaximumArgumentCount();

                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < pCount; i++) {
                    if (i != 0)
                        sb.insert(0, ", ");
                    if (stack.isEmpty())
                        throw new IllegalStateException("While getting " + pCount + "parameters for " + token);
                    sb.insert(0, stack.pop());
                }

                stack.push(token + "(" + sb + ")");


            } else if (Operator._1_OPERATORS.contains(token)) {
                String a = stack.pop();

                stack.push("(" + token + " " + a + ")");

            } else if (takesTwoParameters(token)) {
                if (stack.size() < 2)
                    throw new IllegalStateException("Not enough " + token);
                String a = stack.pop();
                String b = stack.pop();
                stack.push("(" + b + " " + token + " " + a + ")");
            } else {
                stack.push(token);
            }
        }
        if (stack.size() != 1)
            throw new IllegalStateException("Unexpected stack content: " + stack);
        return stack.pop();
    }

    private static boolean takesTwoParameters(String token) {
        return Operator._2_OPERATORS.contains(token);
    }

    public static String getReadableListOfFunctions() {
        StringBuilder result = new StringBuilder();
        DoubleEvaluator evaluator = new DoubleEvaluator();
        for (Function function : evaluator.getFunctions()) {
            if (result.length() > 0) {
                result.append(", ");
            }
            result.append(function.getName());
        }
        return result.toString();
    }
}
