package com.rusefi.newparse;

import com.rusefi.EnumsReader;
import com.rusefi.VariableRegistry;
import com.rusefi.enum_reader.Value;
import com.rusefi.generated.RusefiConfigGrammarBaseListener;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.newparse.parsing.*;
import org.antlr.v4.runtime.tree.ParseTreeListener;

import java.util.*;
import java.util.regex.Pattern;

import static com.rusefi.VariableRegistry.AUTO_ENUM_SUFFIX;

public class ParseState implements DefinitionsState {
    private final Map<String, Definition> definitions = new HashMap<>();
    private final Map<String, Struct> structs = new HashMap<>();
    private final List<Struct> structList = new ArrayList<>();
    private final Map<String, Typedef> typedefs = new HashMap<>();
    private static final Pattern CHAR_LITERAL = Pattern.compile("'.'");

    private final EnumsReader enumsReader;
    private Definition.OverwritePolicy definitionOverwritePolicy = Definition.OverwritePolicy.NotAllowed;

    private String typedefName = null;
    private final Queue<Double> evalResults = new LinkedList<>();
    private Scope scope = null;
    private final Stack<Scope> scopes = new Stack<>();

    private Struct lastStruct = null;

    public ParseState() {
        this.enumsReader = null;
    }

    public ParseState(EnumsReader enumsReader) {
        this.enumsReader = enumsReader;

        for (Map.Entry<String, EnumsReader.EnumState> enumType : this.enumsReader.getEnums().entrySet()) {
            String name = enumType.getKey();

            for (Value enumValue : enumType.getValue().values()) {
                try {
                    int value = enumValue.getIntValue();

                    this.handleIntDefinition(name + "_" + enumValue.getName(), value);
                } catch (Exception exc) {
                    // ignore parse failures
                }
            }
        }
    }

    private void handleIntDefinition(String name, int value) {
        addDefinition(name, value);

        // Also add ints as 16b hex
        addDefinition(name + "_16_hex", String.format("\\x%02x\\x%02x", (value >> 8) & 0xFF, value & 0xFF));
    }

    private static boolean isNumeric(String str) {
        try {
            Integer.parseInt(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    public Struct getLastStruct() {
        return lastStruct;
    }

    private String[] resolveEnumValues(String enumName) {
        if (this.enumsReader == null) {
            return new String[0];
        }

        TreeMap<Integer, String> valueNameById = new TreeMap<>();

        EnumsReader.EnumState stringValueMap = this.enumsReader.getEnums().get(enumName);
        if (stringValueMap == null)
            return null;
        for (Value value : stringValueMap.values()) {
            if (isNumeric(value.getValue())) {
                valueNameById.put(value.getIntValue(), value.getName());
            } else {
                Definition def = this.definitions.get(value.getValue());
                if (def == null)
                    throw new IllegalStateException("No value for " + value);
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

    public Definition findDefinition(String name) {
        return definitions.getOrDefault(name, null);
    }

    /**
     * we are in a lengthy period of transition between two implementations
     */
    @Override
    public void addDefinition(VariableRegistry variableRegistry, String name, String value, Definition.OverwritePolicy overwritePolicy) {
        // old implementation
        variableRegistry.register(name, value);
        // new implementation
        addDefinition(name, value, overwritePolicy);
    }

    public void addDefinition(String name, Object value, Definition.OverwritePolicy overwritePolicy) {
        Definition existingDefinition = definitions.getOrDefault(name, null);

        if (existingDefinition != null) {
            switch (existingDefinition.overwritePolicy) {
                case NotAllowed:
                    throw new IllegalStateException("Tried to add definition for " + name + ", but one already existed.");
                case Replace:
                    definitions.remove(name);
                    break;
                case IgnoreNew:
                    // ignore the new definition, do nothing
                    return;
            }
        }

        definitions.put(name, new Definition(name, value, overwritePolicy));
    }

    private void addDefinition(String name, Object value) {
        addDefinition(name, value, definitionOverwritePolicy);
    }

    public ParseTreeListener getListener() {
        return new RusefiConfigGrammarBaseListener() {

    @Override
    public void exitContent(RusefiConfigGrammarParser.ContentContext ctx) {
        assert(scopes.empty());
        assert(scope == null);
        assert(typedefName == null);
        assert(evalResults.isEmpty());
        assert(evalStack.empty());
    }

    @Override
    public void exitDefinition(RusefiConfigGrammarParser.DefinitionContext ctx) {
        String name = ctx.identifier().getText();

        if (ctx.integer() != null) {
            handleIntDefinition(name, Integer.parseInt(ctx.integer().getText()));
        } else if (ctx.floatNum() != null) {
            addDefinition(name, Double.parseDouble(ctx.floatNum().getText()));
        } else if (ctx.numexpr() != null) {
            double evalResult = evalResults.remove();
            int floored = (int)Math.floor(evalResult);

            if (Math.abs(floored - evalResult) < 0.001) {
                // value is an int, process as such
                handleIntDefinition(name, floored);
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

    @Override
    public void enterTypedef(RusefiConfigGrammarParser.TypedefContext ctx) {
        typedefName = ctx.identifier().getText();
    }

    @Override
    public void exitTypedef(RusefiConfigGrammarParser.TypedefContext ctx) {
        typedefName = null;
    }

    @Override
    public void exitScalarTypedefSuffix(RusefiConfigGrammarParser.ScalarTypedefSuffixContext ctx) {
    }

    @Override
    public void enterEnumTypedefSuffix(RusefiConfigGrammarParser.EnumTypedefSuffixContext ctx) {
    }

    @Override
    public void exitStringTypedefSuffix(RusefiConfigGrammarParser.StringTypedefSuffixContext ctx) {
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
                options.scale = evalResults.remove();
                options.offset = evalResults.remove();
                options.min = evalResults.remove();
                options.max = evalResults.remove();
                options.digits = Integer.parseInt(ctx.integer().getText());

                // we should have consumed everything on the results list
                assert(evalResults.size() == 0);
            }

            return;
        }

        for (RusefiConfigGrammarParser.FieldOptionContext fo : ctx.fieldOption()) {
            String key = fo.getChild(0).getText();

            String sValue = fo.getChild(2).getText();

            switch (key) {
                case "unit":
                    options.units = sValue;
                    break;
                case "comment":
                    options.comment = sValue;
                    break;
                case "digits":
                    options.digits = Integer.parseInt(sValue);
                    break;
                default:
                    Double value = evalResults.remove();

                    switch (key) {
                        case "min":
                            options.min = value;
                            break;
                        case "max":
                            options.max = value;
                            break;
                        case "scale":
                            options.scale = value;
                            break;
                        case "offset":
                            options.offset = value;
                            break;
                    }
                    break;
            }
        }

        // we should have consumed everything on the results list
        assert(evalResults.size() == 0);
    }

    @Override
    public void exitScalarField(RusefiConfigGrammarParser.ScalarFieldContext ctx) {
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

                // If this group is full, create a new one instead of continuing on here.
                if (group.bitFields.size() == 32) {
                    group = null;
                }
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
    }

    @Override
    public void exitArrayLengthSpec(RusefiConfigGrammarParser.ArrayLengthSpecContext ctx) {
    }

    @Override
    public void enterUnusedField(RusefiConfigGrammarParser.UnusedFieldContext ctx) {
        scope.structFields.add(new UnusedField(Integer.parseInt(ctx.integer().getText())));
    }

    @Override
    public void exitStruct(RusefiConfigGrammarParser.StructContext ctx) {
        String structName = ctx.identifier().getText();

        assert(scope != null);

        String comment = ctx.restOfLine() == null ? null : ctx.restOfLine().getText();

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

        // unions must have at least 1 member
        assert(!scope.structFields.isEmpty());

        Union u = new Union(scope.structFields);

        // Restore the containing scope
        scope = scopes.pop();

        // Lastly, add the union to the scope
        scope.structFields.add(u);
    }

    private final Stack<Double> evalStack = new Stack<>();

    @Override
    public void exitEvalNumber(RusefiConfigGrammarParser.EvalNumberContext ctx) {
        evalStack.push(Double.parseDouble(ctx.floatNum().getText()));
    }

    @Override
    public void exitEvalReplacement(RusefiConfigGrammarParser.EvalReplacementContext ctx) {
        // Strip any @@ symbols
        String defName = ctx.getText().replaceAll("@", "");

        if (!definitions.containsKey(defName)) {
            throw new RuntimeException("Definition not found for " + ctx.getText());
        }

        // Find the matching definition and push on to the eval stack
        Definition def = definitions.get(defName);

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

    @Override
    public void exitNumexpr(RusefiConfigGrammarParser.NumexprContext ctx) {
        assert(evalStack.size() == 1);
        evalResults.add(evalStack.pop());
    }

        };
    }

    static class Scope {
        public final List<Field> structFields = new ArrayList<>();
    }
}
