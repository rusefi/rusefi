package com.fathzer.soft.javaluator;

/** A token.
 * <br>When evaluating an expression, it is first split into tokens.
 * These tokens can be operators, constants, etc ...
 * @author Jean-Marc Astesana
 * @see <a href="../../../license.html">License information</a>
 */
public class Token {
	private enum Kind {
		OPEN_BRACKET,
		CLOSE_BRACKET,
		FUNCTION_SEPARATOR,
		FUNCTION,
		OPERATOR,
		LITERAL
	}
	static final Token FUNCTION_ARG_SEPARATOR = new Token(Kind.FUNCTION_SEPARATOR, null);
	
	private Kind kind;
	private Object content;
	
	static Token buildLiteral(String literal) {
		return new Token(Kind.LITERAL, literal);
	}

	static Token buildOperator(Operator ope) {
		return new Token(Kind.OPERATOR, ope);
	}

	static Token buildFunction(Function function) {
		return new Token(Kind.FUNCTION, function);
	}
	
	static Token buildOpenToken(BracketPair pair) {
		return new Token(Kind.OPEN_BRACKET, pair);
	}

	static Token buildCloseToken(BracketPair pair) {
		return new Token(Kind.CLOSE_BRACKET, pair);
	}

	private Token(Kind kind, Object content) {
		super();
		if ((kind.equals(Kind.OPERATOR) && !(content instanceof Operator)) ||
				(kind.equals(Kind.FUNCTION) && !(content instanceof Function)) ||
				(kind.equals(Kind.LITERAL) && !(content instanceof String))) {
			throw new IllegalArgumentException();
		}
		this.kind = kind;
		this.content = content;
	}
	
	BracketPair getBrackets() {
		return (BracketPair) this.content;
	}

	Operator getOperator() {
		return (Operator) this.content;
	}

	Function getFunction() {
		return (Function) this.content;
	}

	Kind getKind() {
		return kind;
	}

	/** Tests whether the token is an operator.
	 * @return true if the token is an operator
	 */
	public boolean isOperator() {
		return kind.equals(Kind.OPERATOR);
	}

	/** Tests whether the token is a function.
	 * @return true if the token is a function
	 */
	public boolean isFunction() {
		return kind.equals(Kind.FUNCTION);
	}

	/** Tests whether the token is an open bracket.
	 * @return true if the token is an open bracket
	 */
	public boolean isOpenBracket() {
		return kind.equals(Kind.OPEN_BRACKET);
	}

	/** Tests whether the token is a close bracket.
	 * @return true if the token is a close bracket
	 */
	public boolean isCloseBracket() {
		return kind.equals(Kind.CLOSE_BRACKET);
	}

	/** Tests whether the token is a function argument separator.
	 * @return true if the token is a function argument separator
	 */
	public boolean isFunctionArgumentSeparator() {
		return kind.equals(Kind.FUNCTION_SEPARATOR);
	}
	
	/** Tests whether the token is a literal or a constant or a variable name.
	 * @return true if the token is a literal, a constant or a variable name
	 */
	public boolean isLiteral() {
		return kind.equals(Kind.LITERAL);
	}

	Operator.Associativity getAssociativity() {
		return getOperator().getAssociativity();
	}

	int getPrecedence() {
		return getOperator().getPrecedence();
	}

	String getLiteral() {
		if (!this.kind.equals(Kind.LITERAL)) {
			throw new IllegalArgumentException();
		}
		return (String)this.content;
	}

	String getBooleanHackedLiteral() {
		if ("true".equalsIgnoreCase((String) content))
			return "1";
		if ("false".equalsIgnoreCase((String) content))
			return "0";
		return getLiteral();
	}

	@Override
	public String toString() {
		return "Token{" +
				"kind=" + kind +
				", content=" + content +
				'}';
	}
}
