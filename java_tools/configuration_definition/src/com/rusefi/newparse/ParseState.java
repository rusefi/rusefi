package com.rusefi.newparse;

import com.rusefi.generated.RusefiConfigGrammarBaseListener;
import com.rusefi.generated.RusefiConfigGrammarParser;
import com.rusefi.newparse.parsing.*;
import jdk.nashorn.internal.runtime.regexp.joni.constants.StringType;

import java.util.*;
import java.util.stream.Collectors;

public class ParseState extends RusefiConfigGrammarBaseListener {
    Map<String, Definition> definitions = new HashMap<>();
    Map<String, Struct> structs = new HashMap<>();
    List<Struct> structList = new ArrayList<>();
    Map<String, Typedef> typedefs = new HashMap<>();

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

    public void addDefinition(String name, String value, Definition.OverwritePolicy overwritePolicy) {
        Definition existingDefinition = definitions.getOrDefault(name, null);

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

    public void setDefinitionPolicy(Definition.OverwritePolicy policy) {
        this.definitionOverwritePolicy = policy;
    }

    @Override
    public void exitDefinition(RusefiConfigGrammarParser.DefinitionContext ctx) {
        String name = ctx.identifier().getText();

        String value;

        if (!this.evalResults.isEmpty()) {
            value = this.evalResults.remove().toString();
        } else {
            // glue the list of definitions back together
            value = ctx.restOfLine().getText();
        }

        addDefinition(name, value, this.definitionOverwritePolicy);
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
        int startBit = Integer.parseInt(ctx.integer(1).getText());
        int endBit = Integer.parseInt(ctx.integer(2).getText());
        Type datatype = Type.findByTsType(ctx.Datatype().getText());

        String values = ctx.enumRhs().getText();

        // TODO: many enum defs are missing so this doesn't work yet
        /*
        if (values.startsWith("@@")) {
            Definition def = this.definitions.get(values.replaceAll("@", ""));

            if (def == null) {
                throw new RuntimeException("couldn't find definition for " + values);
            }

            values = def.value;
        }*/

        this.typedefs.put(this.typedefName, new EnumTypedef(this.typedefName, datatype, startBit, endBit, values));
    }

    @Override
    public void exitArrayTypedefSuffix(RusefiConfigGrammarParser.ArrayTypedefSuffixContext ctx) {
        int arrayLength = this.arrayDim;

        Type datatype = Type.findByTsType(ctx.Datatype().getText());

        FieldOptions options = new FieldOptions();
        handleFieldOptionsList(options, ctx.fieldOptionsList());

        this.typedefs.put(this.typedefName, new ArrayTypedef(this.typedefName, arrayLength, datatype, options));
    }

    @Override
    public void exitStringTypedefSuffix(RusefiConfigGrammarParser.StringTypedefSuffixContext ctx) {
        Float stringLength = this.evalResults.remove();

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
                options.comment = ctx.SemicolonedString().getText();
            } else if (ctx.SemicolonedSuffix() != null) {
                options.comment = ctx.SemicolonedSuffix().getText();
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

            if (key.equals("unit")) {
                options.units = sValue;
            } else if (key.equals("comment")) {
                options.comment = sValue;
            } else if (key.equals("digits")) {
                options.digits = Integer.parseInt(sValue);
            } else {
                Float value = evalResults.remove();

                switch (key) {
                    case "min": options.min = value; break;
                    case "max": options.max = value; break;
                    case "scale": options.scale = value; break;
                    case "offset": options.offset = value; break;
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
                scope.structFields.add(new ArrayField<ScalarField>(prototype, arTypedef.length, false));
                return;
            } else if (typedef instanceof EnumTypedef) {
                EnumTypedef bTypedef = (EnumTypedef) typedef;

                options = new FieldOptions();

                // Merge the read-in options list with the default from the typedef (if exists)
                handleFieldOptionsList(options, ctx.fieldOptionsList());

                scope.structFields.add(new EnumField(bTypedef.type, type, name, bTypedef.values, options));
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

        String comment = ctx.SemicolonedSuffix() == null ? null : ctx.SemicolonedSuffix().getText();

        group.addBitField(new BitField(name, comment));
    }

    @Override
    public void exitArrayField(RusefiConfigGrammarParser.ArrayFieldContext ctx) {
        String type = ctx.identifier(0).getText();
        String name = ctx.identifier(1).getText();
        int length = this.arrayDim;
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

                EnumField prototype = new EnumField(bTypedef.type, type, name, bTypedef.values, options);

                scope.structFields.add(new ArrayField<EnumField>(prototype, length, iterate));
                return;
            } else if (typedef instanceof StringTypedef) {
                StringTypedef sTypedef = (StringTypedef) typedef;

                // iterate required for strings
                assert(iterate);

                StringField prototype = new StringField(name, sTypedef.size);
                scope.structFields.add(new ArrayField<StringField>(prototype, length, iterate));
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

        scope.structFields.add(new ArrayField(prototype, length, iterate));
    }

    private int arrayDim = 0;

    @Override
    public void exitArrayLengthSpec(RusefiConfigGrammarParser.ArrayLengthSpecContext ctx) {
        arrayDim = evalResults.remove().intValue();

        if (ctx.ArrayDimensionSeparator() != null) {
            arrayDim *= evalResults.remove().intValue();
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

    private Stack<Float> evalStack = new Stack<>();


    @Override
    public void exitEvalNumber(RusefiConfigGrammarParser.EvalNumberContext ctx) {
        evalStack.push(Float.parseFloat(ctx.floatNum().getText()));
    }

    @Override
    public void exitEvalReplacement(RusefiConfigGrammarParser.EvalReplacementContext ctx) {
        // Strip any @@ symbols
        String defName = ctx.getText().replaceAll("@", "");

        if (!this.definitions.containsKey(defName)) {
            throw new RuntimeException("Definition not found for " + ctx.getText());
        }

        // Find the matching definition, parse it, and push on to the eval stack
        evalStack.push(Float.parseFloat(this.definitions.get(defName).value));
    }

    @Override
    public void exitEvalMul(RusefiConfigGrammarParser.EvalMulContext ctx) {
        Float right = evalStack.pop();
        Float left = evalStack.pop();

        evalStack.push(left * right);
    }

    @Override
    public void exitEvalDiv(RusefiConfigGrammarParser.EvalDivContext ctx) {
        Float right = evalStack.pop();
        Float left = evalStack.pop();

        evalStack.push(left / right);
    }

    @Override
    public void exitEvalAdd(RusefiConfigGrammarParser.EvalAddContext ctx) {
        Float right = evalStack.pop();
        Float left = evalStack.pop();

        evalStack.push(left + right);
    }

    @Override
    public void exitEvalSub(RusefiConfigGrammarParser.EvalSubContext ctx) {
        Float right = evalStack.pop();
        Float left = evalStack.pop();

        evalStack.push(left - right);
    }

    private Queue<Float> evalResults = new LinkedList<>();

    @Override
    public void exitNumexpr(RusefiConfigGrammarParser.NumexprContext ctx) {
        assert(evalStack.size() == 1);
        evalResults.add(evalStack.pop());
    }
}
