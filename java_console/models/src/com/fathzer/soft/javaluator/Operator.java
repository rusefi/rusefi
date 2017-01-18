package com.fathzer.soft.javaluator;

/** An <a href="http://en.wikipedia.org/wiki/Operator_(mathematics)">operator</a>.
 * @author Jean-Marc Astesana
 * @see <a href="../../../license.html">License information</a>
 */
public class Operator {
	public final String rpnSymbol;

	/** An Operator's <a href="http://en.wikipedia.org/wiki/Operator_associativity">associativity</a>.
	 */
	public enum Associativity {
		/** Left associativity.*/
		LEFT,
		/** Right associativity. */
		RIGHT,
		/** No associativity.*/
		NONE
	}
	private String symbol;
	private int precedence;
	private int operandCount;
	private Associativity associativity;

	/** Constructor.
	 * @param symbol The operator name (Currently, the name's length must be one character). 
	 * @param operandCount The number of operands of the operator (must be 1 or 2).
	 * @param associativity true if operator is left associative
	 * @param precedence The <a href="http://en.wikipedia.org/wiki/Order_of_operations">precedence</a> of the operator.
	 * <br>The precedence is the priority of the operator. An operator with an higher precedence will be executed before an operator with a lower precedence.
	 * Example : In "<i>1+3*4</i>" * has a higher precedence than +, so the expression is interpreted as 1+(3*4).
	 * @throws IllegalArgumentException if operandCount if not 1 or 2 or if associativity is none
	 * @throws NullPointerException if symbol or associativity are null
	 */
	public Operator(String symbol, int operandCount, Associativity associativity, int precedence) {
		this(symbol, operandCount, associativity, precedence, null);
	}

	public Operator(String symbol, int operandCount, Associativity associativity, int precedence, String rpnSymbol) {
		this.rpnSymbol = rpnSymbol;
		if (symbol==null || associativity==null) {
			throw new NullPointerException();
		}
		if (symbol.length()==0) {
			throw new IllegalArgumentException("Operator symbol can't be null");
		}
		if ((operandCount<1) || (operandCount>2)) {
			throw new IllegalArgumentException("Only unary and binary operators are supported");
		}
		if (Associativity.NONE.equals(associativity)) {
			throw new IllegalArgumentException("None associativity operators are not supported");
		}
		this.symbol = symbol;
		this.operandCount = operandCount;
		this.associativity = associativity;
		this.precedence = precedence;
	}

	/** Gets the operator's symbol.
	 * @return a String
	 */
	public String getSymbol() {
		return this.symbol;
	}


	public String getRpnSymbol() {
		if (rpnSymbol != null)
			return rpnSymbol;
		return symbol;
	}


	/** Gets the operator's operand count. 
	 * @return an integer
	 */
	public int getOperandCount() {
		return this.operandCount;
	}
	
	/** Gets this operator's associativity.
	 * @return true if the operator is left associative.
	 * @see <a href="http://en.wikipedia.org/wiki/Operator_associativity">Operator's associativity in Wikipedia</a>
	 */
	public Associativity getAssociativity() {
		return this.associativity;
	}
	
	/** Gets the operator's precedence.
	 * @return an integer
	 * @see <a href="http://en.wikipedia.org/wiki/Order_of_operations">Operator's associativity in Wikipedia</a>
	 */
	public int getPrecedence() {
		return this.precedence;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#hashCode()
	 */
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + operandCount;
		result = prime * result + ((associativity == null) ? 0 : associativity.hashCode());
		result = prime * result + ((symbol == null) ? 0 : symbol.hashCode());
		result = prime * result + precedence;
		return result;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if ((obj == null) || (obj instanceof Operator)) {
			return false;
		}
		Operator other = (Operator) obj;
		if ((operandCount != other.operandCount) || (associativity != other.associativity)) {
			return false;
		}
		if (symbol == null) {
			if (other.symbol != null) {
				return false;
			}
		} else if (!symbol.equals(other.symbol)) {
			return false;
		}
		if (precedence != other.precedence) {
			return false;
		}
		return true;
	}
}