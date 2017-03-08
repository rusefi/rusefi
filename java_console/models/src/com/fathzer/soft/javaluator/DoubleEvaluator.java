package com.fathzer.soft.javaluator;

import org.jetbrains.annotations.NotNull;

import java.text.NumberFormat;
import java.text.ParsePosition;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Locale;

/** An evaluator that is able to evaluate arithmetic expressions on real numbers.
 * <br>Built-in operators:<ul>
 * <li>+: Addition</li>
 * <li>-: Subtraction</li>
 * <li>-: Unary minus</li>
 * <li>*: Multiplication</li>
 * <li>/: Division</li>
 * <li>^: Exponentiation.<br>Warning: Exponentiation is implemented using java.lang.Math.pow which has some limitations (please read oracle documentation about this method to known details).<br>For example (-1)^(1/3) returns NaN.</li>
 * <li>%: Modulo</li>
 * </ul>
 * Built-in functions:<ul>
 * <li>abs: absolute value</li>
 * <li>acos: arc cosine</li>
 * <li>asin: arc sine</li>
 * <li>atan: arc tangent</li>
 * <li>average: average of arguments</li>
 * <li>ceil: nearest upper integer</li>
 * <li>cos: cosine</li>
 * <li>cosh: hyperbolic cosine</li>
 * <li>floor: nearest lower integer</li>
 * <li>ln: natural logarithm (base e)</li>
 * <li>log: base 10 logarithm</li>
 * <li>max: maximum of arguments</li>
 * <li>min: minimum of arguments</li>
 * <li>round: nearest integer</li>
 * <li>sin: sine</li>
 * <li>sinh: hyperbolic sine</li>
 * <li>sum: sum of arguments</li>
 * <li>tan: tangent</li>
 * <li>tanh: hyperbolic tangent</li>
 * <li>random: pseudo-random number (between 0 and 1)</li>
 * </ul>
 * Built-in constants:<ul>
 * <li>e: Base of natural algorithms</li>
 * <li>pi: Ratio of the circumference of a circle to its diameter</li>
 * </ul>
 * @author Jean-Marc Astesana
 * @see <a href="../../../license.html">License information</a>
 */
public class DoubleEvaluator extends AbstractEvaluator<Double> {
	@NotNull
	public static DoubleEvaluator process(String expression) {
        DoubleEvaluator evaluator = new DoubleEvaluator();
        evaluator.evaluate(expression.toLowerCase());
        return evaluator;
    }

	/** The order or operations (operator precedence) is not clearly defined, especially between the unary minus operator and exponentiation
	 * operator (see <a href="http://en.wikipedia.org/wiki/Order_of_operations#Exceptions_to_the_standard">http://en.wikipedia.org/wiki/Order_of_operations</a>).
	 * These constants define the operator precedence styles.
	 */
	public static enum Style {
		/** The most commonly operator precedence, where the unary minus as a lower precedence than the exponentiation.
		 * <br>With this style, used by Google, Wolfram alpha, and many others, -2^2=-4.
		 */
		STANDARD, 
	}

	/** The true constant. */
	public final static Constant TRUE = new Constant("true");
	/** The false constant. */
	public final static Constant FALSE = new Constant("false");

	/** Returns the smallest integer >= argument */
	public static final Function CEIL = new Function("ceil", 1);
	/** Returns the largest integer <= argument */
	public static final Function FLOOR = new Function("floor", 1);
	/** Returns the closest integer of a number */
	public static final Function ROUND = new Function("round", 1);
	/** Returns the absolute value of a number */
	public static final Function ABS = new Function("abs", 1);

	/** Returns the trigonometric sine of an angle. The angle is expressed in radian.*/
	public static final Function SINE = new Function("sin", 1);
	/** Returns the trigonometric cosine of an angle. The angle is expressed in radian.*/
	public static final Function COSINE = new Function("cos", 1);
	/** Returns the trigonometric tangent of an angle. The angle is expressed in radian.*/
	public static final Function TANGENT = new Function("tan", 1);
	/** Returns the trigonometric arc-cosine of an angle. The angle is expressed in radian.*/
	public static final Function ACOSINE = new Function("acos", 1);
	/** Returns the trigonometric arc-sine of an angle. The angle is expressed in radian.*/
	public static final Function ASINE = new Function("asin", 1);
	/** Returns the trigonometric arc-tangent of an angle. The angle is expressed in radian.*/
	public static final Function ATAN = new Function("atan", 1);

	/** Returns the hyperbolic sine of a number.*/
	public static final Function SINEH = new Function("sinh", 1);
	/** Returns the hyperbolic cosine of a number.*/
	public static final Function COSINEH = new Function("cosh", 1);
	/** Returns the hyperbolic tangent of a number.*/
	public static final Function TANGENTH = new Function("tanh", 1);

	/** Returns the minimum of n numbers (n>=1) */
	public static final Function MIN = new Function("min", 1, Integer.MAX_VALUE);
	/** Returns the maximum of n numbers (n>=1) */
	public static final Function MAX = new Function("max", 1, Integer.MAX_VALUE);
	/** Returns the sum of n numbers (n>=1) */
	public static final Function SUM = new Function("sum", 1, Integer.MAX_VALUE);
	/** Returns the average of n numbers (n>=1) */
	public static final Function AVERAGE = new Function("avg", 1, Integer.MAX_VALUE);

	/** Returns the natural logarithm of a number */
	public static final Function LN = new Function("ln", 1);
	/** Returns the decimal logarithm of a number */
	public static final Function LOG = new Function("log", 1);
	
	/** Returns a pseudo random number */
	public static final Function RANDOM = new Function("random", 0);

	/** The negate unary operator in the standard operator precedence.*/
	public static final Operator NEGATE = new Operator("-", 1, Operator.Associativity.RIGHT, 3, "negate");
	/** The negate unary operator in the Excel like operator precedence.*/
	public static final Operator NEGATE_HIGH = new Operator("-", 1, Operator.Associativity.RIGHT, 5);

	/** The negate unary operator in the standard operator precedence.*/
	public static final Operator NOT = new Operator("!", 1, Operator.Associativity.RIGHT, 3);
	public static final Operator NOT2 = new Operator("not", 1, Operator.Associativity.RIGHT, 3, "!");

	public static final Operator MORE = new Operator(">", 2, Operator.Associativity.LEFT, 6);
	public static final Operator MORE_EQ = new Operator(">=", 2, Operator.Associativity.LEFT, 6);

	public static final Operator LESS = new Operator("<", 2, Operator.Associativity.LEFT, 6);
	public static final Operator LESS_EQ = new Operator("<=", 2, Operator.Associativity.LEFT, 6);

	public static final Operator OR2 = new Operator("or", 2, Operator.Associativity.LEFT, 12, "|");
	public static final Operator OR = new Operator("|", 2, Operator.Associativity.LEFT, 12);
// https://en.wikipedia.org/wiki/Order_of_operations
	public static final Operator AND = new Operator("&", 2, Operator.Associativity.LEFT, 11);
	public static final Operator AND2 = new Operator("and", 2, Operator.Associativity.LEFT, 11, "&");

	/** The substraction operator.*/
	public static final Operator MINUS = new Operator("-", 2, Operator.Associativity.LEFT, 1);
	/** The addition operator.*/
	public static final Operator PLUS = new Operator("+", 2, Operator.Associativity.LEFT, 1);
	/** The multiplication operator.*/
	public static final Operator MULTIPLY = new Operator("*", 2, Operator.Associativity.LEFT, 2);
	/** The division operator.*/
	public static final Operator DIVIDE = new Operator("/", 2, Operator.Associativity.LEFT, 2);
	/** The exponentiation operator.*/
	public static final Operator EXPONENT = new Operator("^", 2, Operator.Associativity.LEFT, 4);
	/** The <a href="http://en.wikipedia.org/wiki/Modulo_operation">modulo operator</a>.*/
	public static final Operator MODULO = new Operator("%", 2, Operator.Associativity.LEFT, 2);

	/** The standard whole set of predefined operators */
	private static final Operator[] OPERATORS = new Operator[]{NEGATE, NOT, NOT2, MORE, MORE_EQ, AND, AND2, OR, OR2, LESS, LESS_EQ, MINUS, PLUS, MULTIPLY, DIVIDE, EXPONENT, MODULO};

	/** The whole set of predefined functions */
	private static final Function[] FUNCTIONS = new Function[]{SINE, COSINE, TANGENT, ASINE, ACOSINE, ATAN, SINEH, COSINEH, TANGENTH, MIN, MAX, SUM, AVERAGE, LN, LOG, ROUND, CEIL, FLOOR, ABS, RANDOM};
	/** The whole set of predefined constants */
	private static final Constant[] CONSTANTS = new Constant[]{TRUE, FALSE};
	
	private static Parameters DEFAULT_PARAMETERS;
	private static final ThreadLocal<NumberFormat> FORMATTER = new ThreadLocal<NumberFormat>() {
	  @Override
	  protected NumberFormat initialValue() {
	  	return NumberFormat.getNumberInstance(Locale.US);
	  }
	};
	
	/** Gets a copy of DoubleEvaluator standard default parameters.
	 * <br>The returned parameters contains all the predefined operators, functions and constants.
	 * <br>Each call to this method create a new instance of Parameters. 
	 * @return a Paramaters instance
	 * @see Style
	 */
	public static Parameters getDefaultParameters() {
		return getDefaultParameters(Style.STANDARD);
	}
	
	/** Gets a copy of DoubleEvaluator default parameters.
	 * <br>The returned parameters contains all the predefined operators, functions and constants.
	 * <br>Each call to this method create a new instance of Parameters. 
	 * @return a Paramaters instance
	 */
	public static Parameters getDefaultParameters(Style style) {
		Parameters result = new Parameters();
		result.addOperators(Arrays.asList(OPERATORS));
		result.addFunctions(Arrays.asList(FUNCTIONS));
		result.addConstants(Arrays.asList(CONSTANTS));
		result.addFunctionBracket(BracketPair.PARENTHESES);
		result.addExpressionBracket(BracketPair.PARENTHESES);
		return result;
	}

	private static Parameters getParameters() {
		if (DEFAULT_PARAMETERS == null) {
			DEFAULT_PARAMETERS = getDefaultParameters();
		}
		return DEFAULT_PARAMETERS;
	}
	
	/** Constructor.
	 * <br>This default constructor builds an instance with all predefined operators, functions and constants. 
	 */
	public DoubleEvaluator() {
		this(getParameters());
	}

	/** Constructor.
	 * <br>This constructor can be used to reduce the set of supported operators, functions or constants,
	 * or to localize some function or constant's names.
	 * @param parameters The parameters of the evaluator.
	 */
	public DoubleEvaluator(Parameters parameters) {
		super(parameters);
	}

	@Override
	protected Double toValue(String literal, Object evaluationContext) {
		ParsePosition p = new ParsePosition(0);
		Number result = FORMATTER.get().parse(literal, p);
		if (p.getIndex()==0 || p.getIndex()!=literal.length()) {
			return 666.0;
//			throw new IllegalArgumentException(literal+" is not a number");
		}
		return result.doubleValue();
	}
	
	/* (non-Javadoc)
	 * @see net.astesana.javaluator.AbstractEvaluator#evaluate(net.astesana.javaluator.Constant)
	 */
	@Override
	protected Double evaluate(Constant constant, Object evaluationContext) {
		if (TRUE.equals(constant)) {
			return 0.0;
		} else if (FALSE.equals(constant)) {
			return 0.0;
		} else {
			return super.evaluate(constant, evaluationContext);
		}
	}

	/* (non-Javadoc)
	 * @see net.astesana.javaluator.AbstractEvaluator#evaluate(net.astesana.javaluator.Operator, java.util.Iterator)
	 */
	@Override
	protected Double evaluate(Operator operator, Iterator<Double> operands, Object evaluationContext) {
		if (NEGATE.equals(operator) || NEGATE_HIGH.equals(operator)) {
			return -operands.next();
		} else if (NOT.equals(operator) || NOT2.equals(operator)) {
			return boolean2double(operands.next() != 1.0);
		} else if (AND.equals(operator) || AND2.equals(operator)) {
			return boolean2double((operands.next() == 1.0 ) && (operands.next() == 1.0));
		} else if (OR.equals(operator) || OR2.equals(operator)) {
			return boolean2double((operands.next() == 1.0 ) || (operands.next() == 1.0));
		} else if (MORE.equals(operator)) {
			return boolean2double(operands.next() > operands.next());
		} else if (MORE_EQ.equals(operator)) {
			return boolean2double(operands.next() >= operands.next());
		} else if (LESS.equals(operator)) {
			return boolean2double(operands.next() < operands.next());
		} else if (LESS_EQ.equals(operator)) {
			return boolean2double(operands.next() <= operands.next());
		} else if (MINUS.equals(operator)) {
			return operands.next() - operands.next();
		} else if (PLUS.equals(operator)) {
			return operands.next() + operands.next();
		} else if (MULTIPLY.equals(operator)) {
			return operands.next() * operands.next();
		} else if (DIVIDE.equals(operator)) {
			return operands.next() / operands.next();
		} else if (EXPONENT.equals(operator)) {
			return Math.pow(operands.next(),operands.next());
		} else if (MODULO.equals(operator)) {
			return operands.next() % operands.next();
		} else {
			return super.evaluate(operator, operands, evaluationContext);
		}
	}

	private double boolean2double(boolean value) {
		return value ? 1.0 : 0.0;
	}

	/* (non-Javadoc)
	 * @see net.astesana.javaluator.AbstractEvaluator#evaluate(net.astesana.javaluator.Function, java.util.Iterator)
	 */
	@Override
	protected Double evaluate(Function function, Iterator<Double> arguments, Object evaluationContext) {
		Double result;
		if (ABS.equals(function)) {
			result = Math.abs(arguments.next());
		} else if (CEIL.equals(function)) {
			result = Math.ceil(arguments.next());
		} else if (FLOOR.equals(function)) {
			result = Math.floor(arguments.next());
		} else if (ROUND.equals(function)) {
			Double arg = arguments.next();
			if (arg==Double.NEGATIVE_INFINITY || arg==Double.POSITIVE_INFINITY) {
				result = arg;
			} else {
				result = (double) Math.round(arg);
			}
		} else if (SINEH.equals(function)) {
			result = Math.sinh(arguments.next());
		} else if (COSINEH.equals(function)) {
			result = Math.cosh(arguments.next());
		} else if (TANGENTH.equals(function)) {
			result = Math.tanh(arguments.next());
		} else if (SINE.equals(function)) {
			result = Math.sin(arguments.next());
		} else if (COSINE.equals(function)) {
			result = Math.cos(arguments.next());
		} else if (TANGENT.equals(function)) {
			result = Math.tan(arguments.next());
		} else if (ACOSINE.equals(function)) {
			result = Math.acos(arguments.next());
		} else if (ASINE.equals(function)) {
			result = Math.asin(arguments.next());
		} else if (ATAN.equals(function)) {
			result = Math.atan(arguments.next());
		} else if (MIN.equals(function)) {
			result = arguments.next();
			while (arguments.hasNext()) {
				result = Math.min(result, arguments.next());
			}
		} else if (MAX.equals(function)) {
			result = arguments.next();
			while (arguments.hasNext()) {
				result = Math.max(result, arguments.next());
			}
		} else if (SUM.equals(function)) {
			result = 0.;
			while (arguments.hasNext()) {
				result = result + arguments.next();
			}
		} else if (AVERAGE.equals(function)) {
			result = 0.;
			int nb = 0;
			while (arguments.hasNext()) {
				result = result + arguments.next();
				nb++;
			}
			result = result/nb;
		} else if (LN.equals(function)) {
			result = Math.log(arguments.next());
		} else if (LOG.equals(function)) {
			result = Math.log10(arguments.next());
		} else if (RANDOM.equals(function)) {
			result = Math.random();
		} else {
			result = super.evaluate(function, arguments, evaluationContext);
		}
		errIfNaN(result, function);
		return result;
	}
	
	private void errIfNaN(Double result, Function function) {
		if (result.equals(Double.NaN)) {
			throw new IllegalArgumentException("Invalid argument passed to "+function.getName());
		}
	}
}
