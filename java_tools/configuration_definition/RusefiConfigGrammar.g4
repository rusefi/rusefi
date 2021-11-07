grammar RusefiConfigGrammar;

@header {
	package com.rusefi.generated;
}

// ...be generous in line endings...
ENDL: ('\n' | '\r\n' | '\r');

LINE_COMMENT: '!' ~[\r\n]* -> skip;
WS: [ \t]+ -> skip ;

// Special tokens need highest priority
Struct: 'struct';
StructNoPrefix: 'struct_no_prefix';
EndStruct: 'end_struct';
Definition: '#define';
Unused: 'unused';
Custom: 'custom';
Datatype: (('S'|'U')('08'|'16'|'32')) | 'F32';
Iterate: 'iterate';
Bits: 'bits';
Bit: 'bit';
Array: 'array';
Scalar: 'scalar';
FsioVisible: 'fsio_visible';
Autoscale: 'autoscale';

ArrayDimensionSeparator: 'x';

MUL: '*';
DIV: '/';
ADD: '+';
SUB: '-';

IntegerChars: [-]?[0-9]+;
FloatChars: IntegerChars [.] ([0-9]+)?;

IdentifierChars : [a-zA-Z_]([a-zA-Z0-9_]*);

// TODO: do we need replacementIdent AND identifier to be here?
replacementIdent: '@@' IdentifierChars '@@' | identifier;

String: [a-zA-Z_0-9.']+;

// match a quote, then anything not a quote, then another quote
QuotedString: '"' ~'"'* '"';

// legacy, remove me!
SemicolonedSuffix: ';' ~([;] | '\n')*;
SemicolonedString: SemicolonedSuffix ';';

integer: IntegerChars;
floatNum: FloatChars | IntegerChars;

expr
    : floatNum			# EvalNumber
    | '{' expr '}'		# EvalParens
    | expr MUL expr		# EvalMul
	| expr DIV expr		# EvalDiv
	| expr ADD expr		# EvalAdd
	| expr SUB expr		# EvalSub
    | replacementIdent	# EvalReplacement
    ;

numexpr: expr;

identifier: IdentifierChars | 'offset' | 'ArrayDimension';

restOfLine
    : ~ENDL*
    | 'true'
    | 'false';

definition
    : Definition identifier integer
    | Definition identifier floatNum
    | Definition identifier numexpr
    | Definition identifier restOfLine;
struct: (Struct | StructNoPrefix) identifier ('@brief' restOfLine)? ENDL+ statements EndStruct;

fieldOption
    : ('min' | 'max' | 'scale' | 'offset' | ) ':' numexpr
    | 'digits' ':' integer
    | ('unit' | 'comment') ':' QuotedString
    ;

fieldOptionsList
    : '(' fieldOption? (',' fieldOption)* ')'
    // TODO: why does the next line have a comma?
    | /* legacy! */ (',' | SemicolonedString) (QuotedString ',' numexpr ',' numexpr ',' numexpr ',' numexpr ',' /*digits =*/integer)?
    | /* legacy! */ SemicolonedSuffix
    ;

arrayLengthSpec: numexpr (ArrayDimensionSeparator numexpr)?;

scalarField: identifier Autoscale? FsioVisible? identifier (fieldOptionsList)?;
arrayField: identifier '[' arrayLengthSpec Iterate? ']' Autoscale? identifier SemicolonedString? (fieldOptionsList)?;
bitField: Bit identifier (',' QuotedString ',' QuotedString)? ('(' 'comment' ':' QuotedString ')')? SemicolonedSuffix?;

unionField: 'union' ENDL+ fields 'end_union';

field
    : scalarField
    | arrayField
    | bitField
    | unionField
    ;

fields
    : (field ENDL+)+
    ;

// Indicates X bytes of free space
unusedField: Unused integer;

enumVal: QuotedString | integer;

enumRhs
    : replacementIdent
    | enumVal (',' enumVal)*
    ;

enumTypedefSuffix: /*ignored*/integer Bits ',' Datatype ',' '@OFFSET@' ',' '[' integer ':' integer ']' ',' enumRhs ;
scalarTypedefSuffix: /*ignored*/integer Scalar ',' Datatype ',' '@OFFSET@' fieldOptionsList ;
stringTypedefSuffix: /*ignored*/replacementIdent 'string' ',' 'ASCII' ',' '@OFFSET@' ',' numexpr;

typedef: Custom identifier (enumTypedefSuffix | scalarTypedefSuffix | stringTypedefSuffix);

// Root statement is allowed to appear in the root of the file
rootStatement
    : definition
    | struct
    | typedef
    // TODO: remove me, and build multi-lambda-table in to the language
    | ('#if' | '#else' | '#endif') restOfLine
    | // empty line counts as a statement
    ;

rootStatements
    : (rootStatement ENDL+)*
    ;

// Statements are allowed to appear inside a struct
statement
    : rootStatement
    | field /* tolerate trailing semicolon */ (';')?
    | unusedField
    ;

statements
    : (statement ENDL+)+
    ;

content: rootStatements EOF;
