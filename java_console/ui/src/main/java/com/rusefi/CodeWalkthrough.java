package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.livedata.ParseResult;
import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserBaseListener;
import com.rusefi.ui.livedata.Range;
import com.rusefi.ui.livedata.SourceCodePainter;
import com.rusefi.ui.livedata.VariableValueSource;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import org.antlr.v4.runtime.tree.TerminalNode;
import org.jetbrains.annotations.NotNull;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import static com.devexperts.logging.Logging.getLogging;

/**
 * see PrintCPP14ParserListener
 */
public class CodeWalkthrough {
    private static final Logging log = getLogging(CodeWalkthrough.class);
    // active statements - light green
    public static final Color ACTIVE_STATEMENT = new Color(102, 255, 102);
    // cost past active return statement
    public static final Color PASSIVE_CODE = Color.lightGray;
    public static final Color BROKEN_CODE = Color.orange;
    public static final Color TRUE_CONDITION = Color.GREEN;
    public static final Color FALSE_CONDITION = Color.RED;
    public static final Color CONFIG = Color.BLUE;

    static {
        log.configureDebugEnabled(false);
    }

    private static final String CONFIG_MAGIC_PREFIX = "engineConfiguration";

    public static ParseResult applyVariables(VariableValueSource valueSource, String sourceCode, SourceCodePainter painter, ParseTree tree) {
        Stack<BranchingState> currentState = new Stack<>();
        java.util.List<String> brokenConditions = new ArrayList<>();

        java.util.List<TerminalNode> allTerminals = new java.util.ArrayList<>();

        List<CPP14Parser.UnqualifiedIdContext> functions = new ArrayList<>();

        new ParseTreeWalker().walk(new CPP14ParserBaseListener() {
            @Override
            public void enterFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
                // new method is starting new all over
                resetState(currentState);
                CPP14Parser.DeclaratoridContext declaratorid = ctx.declarator().pointerDeclarator().noPointerDeclarator().noPointerDeclarator().declaratorid();
                CPP14Parser.IdExpressionContext idExpressionContext = declaratorid.idExpression();
                CPP14Parser.QualifiedIdContext qualifiedIdContext = idExpressionContext.qualifiedId();
                CPP14Parser.UnqualifiedIdContext unqualifiedIdContext;
                if (qualifiedIdContext != null) {
                    unqualifiedIdContext = qualifiedIdContext.unqualifiedId();
                } else {
                    unqualifiedIdContext = idExpressionContext.unqualifiedId();
                }
                functions.add(unqualifiedIdContext);
            }

            @Override
            public void enterDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
                super.enterDeclarationStatement(ctx);
                colorStatement(ctx, painter);
            }

            @Override
            public void enterExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
                super.enterExpressionStatement(ctx);
                colorStatement(ctx, painter);
            }

            @Override
            public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {
                super.enterJumpStatement(ctx);
                colorStatement(ctx, painter);
                if ("return".equalsIgnoreCase(ctx.getStart().getText()) &&
                        !currentState.isEmpty() &&
                        getOverallState(currentState) == BranchingState.TRUE) {
                    // we have experienced 'return' in 'green' active flow looks like end of execution for this method?
                    currentState.clear();
                }
            }

            @Override
            public void visitTerminal(TerminalNode node) {
                allTerminals.add(node);

                String text = node.getSymbol().getText();
                //valueSource.getValue(text);

                if ("else".equalsIgnoreCase(text)) {
                    if (log.debugEnabled())
                        log.debug("CONDITIONAL ELSE terminal, flipping condition");

                    if (currentState.isEmpty())
                        return;

                    BranchingState onTop = currentState.pop();
                    currentState.add(onTop.flip());
                }

            }

            @Override
            public void enterCondition(CPP14Parser.ConditionContext ctx) {
                String conditionVariable = ctx.getText();
                if (log.debugEnabled())
                    log.debug("CONDITIONAL: REQUESTING VALUE " + conditionVariable);

                if (currentState.isEmpty()) {
                    painter.paintBackground(PASSIVE_CODE, new Range(ctx));
                } else {
                    VariableValueSource.VariableState state = valueSource.getValue(conditionVariable);
                    Boolean value;
                    if (state == null) {
                        value = null;
                    } else {
                        value = state.getValue() != 0;
                    }
                    BranchingState branchingState = BranchingState.valueOf(value);
                    if (log.debugEnabled())
                        log.debug("CURRENT STATE ADD " + state);
                    currentState.add(branchingState);
                    if (branchingState == BranchingState.BROKEN) {
                        brokenConditions.add(conditionVariable);
                        painter.paintBackground(BROKEN_CODE, new Range(ctx));
                    } else if (branchingState == BranchingState.TRUE) {
                        painter.paintBackground(TRUE_CONDITION, new Range(ctx));
                    } else {
                        painter.paintBackground(FALSE_CONDITION, new Range(ctx));
                    }
                }
            }

            @Override
            public void exitSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
                super.exitSelectionStatement(ctx);
                if (currentState.isEmpty())
                    return; // we are here if some conditional variables were not resolved
                BranchingState onTop = currentState.pop();
                if (onTop == BranchingState.BROKEN)
                    currentState.push(BranchingState.BROKEN);
                if (log.debugEnabled())
                    log.debug("CONDITIONAL: EXIT");
            }


            private void colorStatement(ParserRuleContext ctx, SourceCodePainter painter) {
                Color color;
                if (currentState.isEmpty()) {
                    color = PASSIVE_CODE; // we are past return or past error
                } else {
                    BranchingState isAlive = getOverallState(currentState);
                    if (isAlive == BranchingState.BROKEN) {
                        color = BROKEN_CODE;
                    } else {
                        color = isAlive == BranchingState.TRUE ? ACTIVE_STATEMENT : PASSIVE_CODE;
                    }
                }
                Range range = Range.create(ctx.start, ctx.stop);
                if (range == null) {
                    // parsing error sorry we have to bail out
                    return;
                }
                if (log.debugEnabled())
                    log.info(color + " for " + sourceCode.substring(range.getStart(), range.getStop()));
                painter.paintBackground(color, range);
            }
        }, tree);

        java.util.List<Token> configTokens = new java.util.ArrayList<>();

        for (int i = 0; i < allTerminals.size() - 3; i++) {

            if (allTerminals.get(i).getText().equals(CONFIG_MAGIC_PREFIX) &&
                    allTerminals.get(i + 1).getText().equals("->")
            ) {
                Token token = allTerminals.get(i + 2).getSymbol();
                painter.paintForeground(CONFIG, Range.create(token, token));
                configTokens.add(token);
            }
        }
        return new ParseResult(configTokens, brokenConditions, functions);
    }

    private static void resetState(Stack<BranchingState> currentState) {
        currentState.clear();
        currentState.add(BranchingState.TRUE);
    }

    private static BranchingState getOverallState(Stack<BranchingState> currentState) {
        for (BranchingState value : currentState) {
            if (value == BranchingState.BROKEN)
                return BranchingState.BROKEN;
        }
        for (BranchingState value : currentState) {
            if (value == BranchingState.FALSE)
                return BranchingState.FALSE;
        }
        return BranchingState.TRUE;
    }

    @NotNull
    private static String getOrigin(ParserRuleContext ctx, String s) {
        Range range = new Range(ctx);
        return s.substring(range.getStart(), range.getStop());
    }

    private enum BranchingState {
        TRUE,
        FALSE,
        BROKEN;

        public static BranchingState valueOf(Boolean state) {
            if (state == null)
                return BROKEN;
            return state ? TRUE : FALSE;
        }

        public BranchingState flip() {
            switch (this) {
                case TRUE:
                    return FALSE;
                case FALSE:
                    return TRUE;
                case BROKEN:
                default:
                    return BROKEN;
            }
        }
    }
}
