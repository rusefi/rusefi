package com.rusefi.newparse;

import com.rusefi.EnumsReader;
import com.rusefi.enum_reader.Value;
import com.rusefi.generated.RusefiConfigGrammarBaseListener;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.newparse.parsing.*;
import jdk.nashorn.internal.runtime.regexp.joni.constants.StringType;
import org.jetbrains.annotations.Nullable;

import java.io.PrintStream;
import java.util.*;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class ParseState extends RusefiConfigGrammarBaseListener {
    Map<String, Definition> definitions = new HashMap<>();
    Map<String, Struct> structs = new HashMap<>();
    List<Struct> structList = new ArrayList<>();
    Map<String, Typedef> typedefs = new HashMap<>();

    private final EnumsReader enumsReader;

    public ParseState(EnumsReader enumsReader) {
        this.enumsReader = enumsReader;
    }

    private static boolean isNumeric(String str) {
        try {
            Integer.parseInt(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    private String[] resolveEnumValues(String enumName) {
        TreeMap<Integer, String> valueNameById = new TreeMap<>();

        Map<String, Value> stringValueMap = this.enumsReader.getEnums().get(enumName);
        if (stringValueMap == null)
            return null;
        for (Value value : stringValueMap.values()) {
            if (value.getValue().contains("ENUM_32_BITS"))
                continue;

            if (isNumeric(value.getValue())) {
                valueNameById.put(value.getIntValue(), value.getName());
            } else {
                Definition def = this.definitions.get(value.getValue());
                if (def == null)
                    throw new IllegalStateException("No value for " + value);;
                valueNameById.put((Integer)def.value, value.getName());
            }
        }

        // Now iterate over all values, assembling as an array in-order
        String[] result = new String[valueNameById.lastKey() + 1];
        for (int i = 0; i < result.length; i++) {
            result[i] = valueNameById.getOrDefault(i, "INVALID");
        }

        return result;
    }

    public List<Struct> getStructs() {
        return structList;
    }

    class Scope {
        public List<Field> structFields = new ArrayList<>();
    }

    Scope scope = null;
    Stack<Scope> scopes = new Stack<>();

    @Override
    public void exitContent(RusefiConfigGrammarParser.ContentContext ctx) {
        assert(this.scopes.empty());
        assert(this.scope == null);

        assert(this.typedefName == null);

        assert(evalResults.isEmpty());
        assert(evalStack.empty());
    }

    private Definition.OverwritePolicy definitionOverwritePolicy = Definition.OverwritePolicy.NotAllowed;

    private void addDefinition(String name, Object value) {
        addDefinition(name, value, this.definitionOverwritePolicy);
    }

    public void addDefinition(String name, Object value, Definition.OverwritePolicy overwritePolicy) {
        Definition existingDefinition = this.definitions.getOrDefault(name, null);

        if (existingDefinition != null) {
            switch (existingDefinition.overwritePolicy) {
                case NotAllowed:
                    throw new IllegalStateException("Tried to add definition for " + name + ", but one already existed.");
                case Replace:
                    definitions.remove(existingDefinition);
                case IgnoreNew:
                    // ignore the new definition, do nothing
                    return;
            }
        }

        definitions.put(name, new Definition(name, value, overwritePolicy));
    }

    public Definition findDefinition(String name) {
        return this.definitions.getOrDefault(name, null);
    }

    public void setDefinitionPolicy(Definition.OverwritePolicy policy) {
        this.definitionOverwritePolicy = policy;
    }

    private static final Pattern CHAR_LITERAL = Pattern.compile("'.'");

    private void handleIntDefinition(String name, int value) {
        addDefinition(name, value);

        // Also add ints as 16b hex
        addDefinition(name + "_16_hex", String.format("\\\\x%02x\\\\x%02x", (value >> 8) & 0xFF, value & 0xFF));
    }

    @Override
    public void exitDefinition(RusefiConfigGrammarParser.DefinitionContext ctx) {
        String name = ctx.identifier().getText();

        if (ctx.integer() != null) {
            handleIntDefinition(name, Integer.parseInt(ctx.integer().getText()));
        } else if (ctx.floatNum() != null) {
            addDefinition(name, Double.parseDouble(ctx.floatNum().getText()));
        } else if (ctx.numexpr() != null) {
            double evalResult = this.evalResults.remove();
            double floored = Math.floor(evalResult);

            if (Math.abs(floored - evalResult) < 0.001) {
                // value is an int, process as such
                handleIntDefinition(name, (int)floored);
            } else {
                // Value is a double, add it
                addDefinition(name, evalResult);
            }
        } else {
            // glue the list of definitions back together
            String defText = ctx.restOfLine().getText();
            addDefinition(name, defText);

            // If the definition matches a char literal, add a special definition for that
            if (CHAR_LITERAL.matcher(defText).find()) {
                addDefinition(name + "_char", defText.charAt(1));
            }
        }
    }

    String typedefName = null;

    @Override
    public void enterTypedef(RusefiConfigGrammarParser.TypedefContext ctx) {
        this.typedefName = ctx.identifier().getText();
    }

    @Override
    public void exitTypedef(RusefiConfigGrammarParser.TypedefContext ctx) {
        this.typedefName = null;
    }

    @Override
    public void exitScalarTypedefSuffix(RusefiConfigGrammarParser.ScalarTypedefSuffixContext ctx) {
        Type datatype = Type.findByTsType(ctx.Datatype().getText());

        FieldOptions options = new FieldOptions();
        handleFieldOptionsList(options, ctx.fieldOptionsList());

        this.typedefs.put(this.typedefName, new ScalarTypedef(this.typedefName, datatype, options));
    }

    @Override
    public void enterEnumTypedefSuffix(RusefiConfigGrammarParser.EnumTypedefSuffixContext ctx) {
        int endBit = Integer.parseInt(ctx.integer(2).getText());
        Type datatype = Type.findByTsType(ctx.Datatype().getText());

        String rhs = ctx.enumRhs().getText();

        String[] values = null;

        if (rhs.startsWith("@@")) {
            String defName = rhs.replaceAll("@", "");

            if (defName.endsWith("_auto_enum")) {
                // clip off the "_auto_enum" part
                defName = defName.substring(0, defName.length() - 10);
                values = resolveEnumValues(defName);
            } else {
                Definition def = this.definitions.get(defName);

                if (def == null) {
                    throw new RuntimeException("couldn't find definition for " + rhs);
                }

                Object value = def.value;

                if (!(value instanceof String)) {
                    throw new RuntimeException("Found definition for " + rhs + " but it wasn't a string as expected");
                }

                rhs = (String)value;
            }
        }

        if (values == null) {
            values = Arrays.stream(rhs.split(","))                    // Split on commas
                        .map(s -> s.trim())                                 // trim whitespace
                        .map(s -> s.replaceAll("\"", ""))   // Remove quotes
                        .toArray(n -> new String[n]);                       // Convert back to array
        }

        this.typedefs.put(this.typedefName, new EnumTypedef(this.typedefName, datatype, endBit, values));
    }

    @Override
    public void exitArrayTypedefSuffix(RusefiConfigGrammarParser.ArrayTypedefSuffixContext ctx) {
        Type datatype = Type.findByTsType(ctx.Datatype().getText());

        FieldOptions options = new FieldOptions();
        handleFieldOptionsList(options, ctx.fieldOptionsList());

        this.typedefs.put(this.typedefName, new ArrayTypedef(this.typedefName, this.arrayDim, datatype, options));
    }

    @Override
    public void exitStringTypedefSuffix(RusefiConfigGrammarParser.StringTypedefSuffixContext ctx) {
        Double stringLength = this.evalResults.remove();

        this.typedefs.put(this.typedefName, new StringTypedef(this.typedefName, stringLength.intValue()));
    }

    @Override
    public void enterStruct(RusefiConfigGrammarParser.StructContext ctx) {
        // If we're already inside a struct, push that context on to the stack
        if (scope != null) {
            scopes.push(scope);
        }

        // Create new scratch space for this scope
        scope = new Scope();
    }

    @Override
    public void enterUnionField(RusefiConfigGrammarParser.UnionFieldContext ctx) {
        // Unions behave like a struct as far as scope is concerned (but is processed differently later
        // to overlap all members, instead of placing them in sequence as in a struct)
        enterStruct(null);
    }

    void handleFieldOptionsList(FieldOptions options, RusefiConfigGrammarParser.FieldOptionsListContext ctx) {
        // Null means no options were configured, use defaults
        if (ctx == null) {
            return;
        }

        if (ctx.fieldOption().size() == 0) {
            if (ctx.SemicolonedString() != null) {
                String text = ctx.SemicolonedString().getText();
                options.comment = text.substring(1, text.length() - 1).trim();
            } else if (ctx.SemicolonedSuffix() != null) {
                options.comment = ctx.SemicolonedSuffix().getText().substring(1).trim();
            } else {
                options.comment = "";
            }

            // this is a legacy field option list, parse it as such
            if (!ctx.numexpr().isEmpty()) {
                options.units = ctx.QuotedString().getText();
                options.scale = evalResults.remove().floatValue();
                options.offset = evalResults.remove().floatValue();
                options.min = evalResults.remove().floatValue();
                options.max = evalResults.remove().floatValue();
                options.digits = Integer.parseInt(ctx.integer().getText());

                // we should have consumed everything on the results list
                assert(evalResults.size() == 0);
            }

            return;
        }

        for (RusefiConfigGrammarParser.FieldOptionContext fo : ctx.fieldOption()) {
            String key = fo.getChild(0).getText();

            String sValue = fo.getChild(2).getText();

            if (key.equals("unit")) {
                options.units = sValue;
            } else if (key.equals("comment")) {
                options.comment = sValue;
            } else if (key.equals("digits")) {
                options.digits = Integer.parseInt(sValue);
            } else {
                Double value = evalResults.remove();

                switch (key) {
                    case "min": options.min = value.floatValue(); break;
                    case "max": options.max = value.floatValue(); break;
                    case "scale": options.scale = value.floatValue(); break;
                    case "offset": options.offset = value.floatValue(); break;
                }
            }
        }

        // we should have consumed everything on the results list
        assert(evalResults.size() == 0);
    }

    @Override
    public void exitScalarField(RusefiConfigGrammarParser.ScalarFieldContext ctx) {
        String type = ctx.identifier(0).getText();
        String name = ctx.identifier(1).getText();

        // First check if this is an instance of a struct
        if (structs.containsKey(type)) {
            scope.structFields.add(new StructField(structs.get(type), name));
            return;
        }

        // Check first if we have a typedef for this type
        Typedef typedef = this.typedefs.get(type);

        FieldOptions options = null;
        if (typedef != null) {
            if (typedef instanceof ScalarTypedef) {
                ScalarTypedef scTypedef = (ScalarTypedef)typedef;
                // Copy the typedef's options list - we don't want to edit it
                options = scTypedef.options.copy();
                // Switch to the "real" type, that is the typedef's type
                type = scTypedef.type.cType;
            } else if (typedef instanceof ArrayTypedef) {
                ArrayTypedef arTypedef = (ArrayTypedef) typedef;
                // Copy the typedef's options list - we don't want to edit it
                options = arTypedef.options.copy();

                // Merge the read-in options list with the default from the typedef (if exists)
                handleFieldOptionsList(options, ctx.fieldOptionsList());

                ScalarField prototype = new ScalarField(arTypedef.type, name, options);
                scope.structFields.add(new ArrayField<>(prototype, arTypedef.length, false));
                return;
            } else if (typedef instanceof EnumTypedef) {
                EnumTypedef bTypedef = (EnumTypedef) typedef;

                options = new FieldOptions();

                // Merge the read-in options list with the default from the typedef (if exists)
                handleFieldOptionsList(options, ctx.fieldOptionsList());

                scope.structFields.add(new EnumField(bTypedef.type, type, name, bTypedef.endBit, bTypedef.values, options));
                return;
            } else if (typedef instanceof StringTypedef) {
                StringTypedef sTypedef = (StringTypedef) typedef;
                scope.structFields.add(new StringField(name, sTypedef.size));
                return;
            } else {
                // TODO: throw
            }
        } else {
            // If no typedef found, it MUST be a native type
            if (!Type.findByCtype(type).isPresent()) {
                throw new RuntimeException("didn't understand type " + type + " for element " + name);
            }

            // no typedef found, create new options list
            options = new FieldOptions();
        }

        // Merge the read-in options list with the default from the typedef (if exists)
        handleFieldOptionsList(options, ctx.fieldOptionsList());

        scope.structFields.add(new ScalarField(Type.findByCtype(type).get(), name, options));
    }

    @Override
    public void enterBitField(RusefiConfigGrammarParser.BitFieldContext ctx) {
        String name = ctx.identifier().getText();

        // Check if there's already a bit group at the end of the current struct
        BitGroup group = null;
        if (!scope.structFields.isEmpty()) {
            Object lastElement = scope.structFields.get(scope.structFields.size() - 1);

            if (lastElement instanceof BitGroup) {
                group = (BitGroup)lastElement;
            }
        }

        // there was no group, create and add it
        if (group == null) {
            group = new BitGroup();
            scope.structFields.add(group);
        }

        String comment = ctx.SemicolonedSuffix() == null ? null : ctx.SemicolonedSuffix().getText().substring(1).trim();

        String trueValue = "\"true\"";
        String falseValue = "\"false\"";

        if (!ctx.QuotedString().isEmpty()) {
            trueValue = ctx.QuotedString(0).getText();
            falseValue = ctx.QuotedString(1).getText();
        }

        group.addBitField(new BitField(name, comment, trueValue, falseValue));
    }

    @Override
    public void exitArrayField(RusefiConfigGrammarParser.ArrayFieldContext ctx) {
        String type = ctx.identifier(0).getText();
        String name = ctx.identifier(1).getText();
        int[] length = this.arrayDim;
        // check if the iterate token is present
        boolean iterate = ctx.Iterate() != null;

        // First check if this is an array of structs
        if (structs.containsKey(type)) {
            // iterate required for structs
            assert(iterate);

            scope.structFields.add(new ArrayField<StructField>(new StructField(structs.get(type), name), length, iterate));
            return;
        }

        // Check first if we have a typedef for this type
        Typedef typedef = this.typedefs.get(type);

        FieldOptions options = null;
        if (typedef != null) {
            if (typedef instanceof ScalarTypedef) {
                ScalarTypedef scTypedef = (ScalarTypedef) typedef;
                // Copy the typedef's options list - we don't want to edit it
                options = scTypedef.options.copy();
                // Switch to the "real" type, that is the typedef's type
                type = scTypedef.type.cType;
            } else if (typedef instanceof EnumTypedef) {
                EnumTypedef bTypedef = (EnumTypedef) typedef;

                options = new FieldOptions();
                handleFieldOptionsList(options, ctx.fieldOptionsList());

                EnumField prototype = new EnumField(bTypedef.type, type, name, bTypedef.endBit, bTypedef.values, options);

                scope.structFields.add(new ArrayField<>(prototype, length, iterate));
                return;
            } else if (typedef instanceof StringTypedef) {
                StringTypedef sTypedef = (StringTypedef) typedef;

                // iterate required for strings
                assert(iterate);

                StringField prototype = new StringField(name, sTypedef.size);
                scope.structFields.add(new ArrayField<>(prototype, length, iterate));
                return;
            } else {
                throw new RuntimeException("didn't understand type " + type + " for element " + name);
            }
        } else {
            // If no typedef found, it MUST be a native type
            if (!Type.findByCtype(type).isPresent()) {
                throw new RuntimeException("didn't understand type " + type + " for element " + name);
            }

            // no typedef found, create new options list
            options = new FieldOptions();
        }

        // Merge the read-in options list with the default from the typedef (if exists)
        handleFieldOptionsList(options, ctx.fieldOptionsList());

        ScalarField prototype = new ScalarField(Type.findByCtype(type).get(), name, options);

        scope.structFields.add(new ArrayField<>(prototype, length, iterate));
    }

    private int[] arrayDim = null;

    @Override
    public void exitArrayLengthSpec(RusefiConfigGrammarParser.ArrayLengthSpecContext ctx) {
        int arrayDim0 = evalResults.remove().intValue();

        if (ctx.ArrayDimensionSeparator() != null) {
            this.arrayDim = new int[] { arrayDim0, evalResults.remove().intValue() };
        } else {
            this.arrayDim = new int[] { arrayDim0 };
        }
    }

    @Override
    public void enterUnusedField(RusefiConfigGrammarParser.UnusedFieldContext ctx) {
        scope.structFields.add(new UnusedField(Integer.parseInt(ctx.integer().getText())));
    }

    private Struct lastStruct = null;

    public Struct getLastStruct() {
        return lastStruct;
    }

    @Override
    public void exitStruct(RusefiConfigGrammarParser.StructContext ctx) {
        String structName = ctx.identifier().getText();

        assert(scope != null);
        assert(scope.structFields != null);

        String comment = ctx.restOfLine() == null ? null : ctx.restOfLine().getText().toString();

        Struct s = new Struct(structName, scope.structFields, ctx.StructNoPrefix() != null, comment);
        structs.put(structName, s);
        structList.add(s);
        lastStruct = s;

        // We're leaving with this struct, re-apply the next struct out so more fields can be added to it
        if (scopes.empty()) {
            scope = null;
        } else {
            scope = scopes.pop();
        }
    }

    @Override
    public void exitUnionField(RusefiConfigGrammarParser.UnionFieldContext ctx) {
        assert(scope != null);
        assert(scope.structFields != null);

        // unions must have at least 1 member
        assert(!scope.structFields.isEmpty());

        Union u = new Union(scope.structFields);

        // Restore the containing scope
        scope = scopes.pop();

        // Lastly, add the union to the scope
        scope.structFields.add(u);
    }

    private Stack<Double> evalStack = new Stack<>();


    @Override
    public void exitEvalNumber(RusefiConfigGrammarParser.EvalNumberContext ctx) {
        evalStack.push(Double.parseDouble(ctx.floatNum().getText()));
    }

    @Override
    public void exitEvalReplacement(RusefiConfigGrammarParser.EvalReplacementContext ctx) {
        // Strip any @@ symbols
        String defName = ctx.getText().replaceAll("@", "");

        if (!this.definitions.containsKey(defName)) {
            throw new RuntimeException("Definition not found for " + ctx.getText());
        }

        // Find the matching definition and push on to the eval stack
        Definition def = this.definitions.get(defName);

        if (!def.isNumeric()) {
            throw new RuntimeException("Tried to use symbol " + defName + " in an expression, but it wasn't a number");
        }

        evalStack.push(def.asDouble());
    }

    @Override
    public void exitEvalMul(RusefiConfigGrammarParser.EvalMulContext ctx) {
        Double right = evalStack.pop();
        Double left = evalStack.pop();

        evalStack.push(left * right);
    }

    @Override
    public void exitEvalDiv(RusefiConfigGrammarParser.EvalDivContext ctx) {
        Double right = evalStack.pop();
        Double left = evalStack.pop();

        evalStack.push(left / right);
    }

    @Override
    public void exitEvalAdd(RusefiConfigGrammarParser.EvalAddContext ctx) {
        Double right = evalStack.pop();
        Double left = evalStack.pop();

        evalStack.push(left + right);
    }

    @Override
    public void exitEvalSub(RusefiConfigGrammarParser.EvalSubContext ctx) {
        Double right = evalStack.pop();
        Double left = evalStack.pop();

        evalStack.push(left - right);
    }

    private Queue<Double> evalResults = new LinkedList<>();

    @Override
    public void exitNumexpr(RusefiConfigGrammarParser.NumexprContext ctx) {
        assert(evalStack.size() == 1);
        evalResults.add(evalStack.pop());
    }
}
