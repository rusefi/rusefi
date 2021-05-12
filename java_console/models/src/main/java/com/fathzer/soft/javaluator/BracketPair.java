package com.fathzer.soft.javaluator;

/** A <a href="http://en.wikipedia.org/wiki/Bracket_(mathematics)">bracket pair</a>.
 * @author Jean-Marc Astesana
 * @see <a href="../../../license.html">License information</a>
 */
public class BracketPair {
	/** The parentheses pair: ().*/
	public static final BracketPair PARENTHESES = new BracketPair('(', ')');
	/** The square brackets pair: [].*/
	public static final BracketPair BRACKETS = new BracketPair('[', ']');
	/** The braces pair: {}.*/
	public static final BracketPair BRACES = new BracketPair('{', '}');
	/** The angle brackets pair: <>.*/
	public static final BracketPair ANGLES = new BracketPair('<', '>');

	private String open;
	private String close;
	
	/** Constructor.
	 * @param open The character used to open the brackets.
	 * @param close The character used to close the brackets.
	 */
	public BracketPair(char open, char close) {
		super();
		this.open = new String(new char[]{open});
		this.close = new String(new char[]{close});
	}

	/** Gets the open bracket character.
	 * @return a char
	 */
	public String getOpen() {
		return open;
	}

	/** Gets the close bracket character.
	 * @return a char
	 */
	public String getClose() {
		return close;
	}

	@Override
	public String toString() {
		return open + close;
	}
}
