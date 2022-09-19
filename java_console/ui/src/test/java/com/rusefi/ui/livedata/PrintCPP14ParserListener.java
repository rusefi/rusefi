package com.rusefi.ui.livedata;

import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserListener;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ErrorNode;
import org.antlr.v4.runtime.tree.TerminalNode;

public class PrintCPP14ParserListener implements CPP14ParserListener {
    private int lineCounter = 0;

    private void printWithCounter(String line) {
        System.out.println(lineCounter++ + " " + line);
    }

    @Override
    public void enterEveryRule(ParserRuleContext parserRuleContext) {
//        printWithCounter("enterEveryRule " + ctx.getText());
    }

    @Override
    public void exitEveryRule(ParserRuleContext parserRuleContext) {
//        printWithCounter("exitEveryRule " + ctx.getText());
    }

    @Override
    public void exitTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        printWithCounter("exitTranslationUnit " + ctx.getText());

    }

    @Override
    public void enterTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        printWithCounter("enterTranslationUnit " + ctx.getText());

    }

    @Override
    public void enterPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        printWithCounter("enterPrimaryExpression " + ctx.getText());

    }

    @Override
    public void exitPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        printWithCounter("exitPrimaryExpression " + ctx.getText());

    }

    @Override
    public void enterIdExpression(CPP14Parser.IdExpressionContext ctx) {
        printWithCounter("enterIdExpression " + ctx.getText());

    }

    @Override
    public void exitIdExpression(CPP14Parser.IdExpressionContext ctx) {
        printWithCounter("exitIdExpression " + ctx.getText());

    }

    @Override
    public void enterUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        printWithCounter("enterUnqualifiedId " + ctx.getText());

    }

    @Override
    public void exitUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        printWithCounter("exitUnqualifiedId " + ctx.getText());

    }

    @Override
    public void enterQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        printWithCounter("enterQualifiedId " + ctx.getText());

    }

    @Override
    public void exitQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        printWithCounter("exitQualifiedId " + ctx.getText());

    }

    @Override
    public void enterNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        printWithCounter("enterNestedNameSpecifier " + ctx.getText());

    }

    @Override
    public void exitNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        printWithCounter("exitNestedNameSpecifier " + ctx.getText());

    }

    @Override
    public void enterLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        printWithCounter("enterLambdaExpression " + ctx.getText());

    }

    @Override
    public void exitLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        printWithCounter("exitLambdaExpression " + ctx.getText());

    }

    @Override
    public void enterLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        printWithCounter("enterLambdaIntroducer " + ctx.getText());

    }

    @Override
    public void exitLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        printWithCounter("exitLambdaIntroducer " + ctx.getText());

    }

    @Override
    public void enterLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        printWithCounter("enterLambdaCapture " + ctx.getText());

    }

    @Override
    public void exitLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        printWithCounter("exitLambdaCapture " + ctx.getText());

    }

    @Override
    public void enterCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        printWithCounter("enterCaptureDefault " + ctx.getText());

    }

    @Override
    public void exitCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        printWithCounter("exitCaptureDefault " + ctx.getText());

    }

    @Override
    public void enterCaptureList(CPP14Parser.CaptureListContext ctx) {
        printWithCounter("enterCaptureList " + ctx.getText());

    }

    @Override
    public void exitCaptureList(CPP14Parser.CaptureListContext ctx) {
        printWithCounter("exitCaptureList " + ctx.getText());

    }

    @Override
    public void enterCapture(CPP14Parser.CaptureContext ctx) {
        printWithCounter("enterCapture " + ctx.getText());

    }

    @Override
    public void exitCapture(CPP14Parser.CaptureContext ctx) {
        printWithCounter("exitCapture " + ctx.getText());

    }

    @Override
    public void enterSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        printWithCounter("enterSimpleCapture " + ctx.getText());

    }

    @Override
    public void exitSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        printWithCounter("exitSimpleCapture " + ctx.getText());

    }

    @Override
    public void enterInitcapture(CPP14Parser.InitcaptureContext ctx) {
        printWithCounter("enterInitcapture " + ctx.getText());

    }

    @Override
    public void exitInitcapture(CPP14Parser.InitcaptureContext ctx) {
        printWithCounter("exitInitcapture " + ctx.getText());

    }

    @Override
    public void enterLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        printWithCounter("enterLambdaDeclarator " + ctx.getText());

    }

    @Override
    public void exitLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        printWithCounter("exitLambdaDeclarator " + ctx.getText());

    }

    @Override
    public void enterPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        printWithCounter("enterPostfixExpression " + ctx.getText());

    }

    @Override
    public void exitPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        printWithCounter("exitPostfixExpression " + ctx.getText());

    }

    @Override
    public void enterTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        printWithCounter("enterTypeIdOfTheTypeId " + ctx.getText());

    }

    @Override
    public void exitTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        printWithCounter("exitTypeIdOfTheTypeId " + ctx.getText());

    }

    @Override
    public void enterExpressionList(CPP14Parser.ExpressionListContext ctx) {
        printWithCounter("enterExpressionList " + ctx.getText());

    }

    @Override
    public void exitExpressionList(CPP14Parser.ExpressionListContext ctx) {
        printWithCounter("exitExpressionList " + ctx.getText());

    }

    @Override
    public void enterPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        printWithCounter("enterPseudoDestructorName " + ctx.getText());

    }

    @Override
    public void exitPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        printWithCounter("exitPseudoDestructorName " + ctx.getText());

    }

    @Override
    public void enterUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        printWithCounter("enterUnaryExpression " + ctx.getText());

    }

    @Override
    public void exitUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        printWithCounter("exitUnaryExpression " + ctx.getText());

    }

    @Override
    public void enterUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        printWithCounter("enterUnaryOperator " + ctx.getText());

    }

    @Override
    public void exitUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        printWithCounter("exitUnaryOperator " + ctx.getText());

    }

    @Override
    public void enterNewExpression(CPP14Parser.NewExpressionContext ctx) {
        printWithCounter("enterNewExpression " + ctx.getText());

    }

    @Override
    public void exitNewExpression(CPP14Parser.NewExpressionContext ctx) {
        printWithCounter("exitNewExpression " + ctx.getText());

    }

    @Override
    public void enterNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        printWithCounter("enterNewPlacement " + ctx.getText());

    }

    @Override
    public void exitNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        printWithCounter("exitNewPlacement " + ctx.getText());

    }

    @Override
    public void enterNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        printWithCounter("enterNewTypeId " + ctx.getText());

    }

    @Override
    public void exitNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        printWithCounter("exitNewTypeId " + ctx.getText());

    }

    @Override
    public void enterNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        printWithCounter("enterNewDeclarator " + ctx.getText());

    }

    @Override
    public void exitNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        printWithCounter("exitNewDeclarator " + ctx.getText());

    }

    @Override
    public void enterNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        printWithCounter("enterNoPointerNewDeclarator " + ctx.getText());

    }

    @Override
    public void exitNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        printWithCounter("exitNoPointerNewDeclarator " + ctx.getText());

    }

    @Override
    public void enterNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        printWithCounter("enterNewInitializer " + ctx.getText());

    }

    @Override
    public void exitNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        printWithCounter("exitNewInitializer " + ctx.getText());

    }

    @Override
    public void enterDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        printWithCounter("enterDeleteExpression " + ctx.getText());

    }

    @Override
    public void exitDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        printWithCounter("exitDeleteExpression " + ctx.getText());

    }

    @Override
    public void enterNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        printWithCounter("enterNoExceptExpression " + ctx.getText());

    }

    @Override
    public void exitNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        printWithCounter("exitNoExceptExpression " + ctx.getText());

    }

    @Override
    public void enterCastExpression(CPP14Parser.CastExpressionContext ctx) {
        printWithCounter("enterCastExpression " + ctx.getText());

    }

    @Override
    public void exitCastExpression(CPP14Parser.CastExpressionContext ctx) {
        printWithCounter("exitCastExpression " + ctx.getText());

    }

    @Override
    public void enterPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        printWithCounter("enterPointerMemberExpression " + ctx.getText());

    }

    @Override
    public void exitPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        printWithCounter("exitPointerMemberExpression " + ctx.getText());

    }

    @Override
    public void enterMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        printWithCounter("enterMultiplicativeExpression " + ctx.getText());

    }

    @Override
    public void exitMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        printWithCounter("exitMultiplicativeExpression " + ctx.getText());

    }

    @Override
    public void enterAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        printWithCounter("enterAdditiveExpression " + ctx.getText());

    }

    @Override
    public void exitAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        printWithCounter("exitAdditiveExpression " + ctx.getText());

    }

    @Override
    public void enterShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        printWithCounter("enterShiftExpression " + ctx.getText());

    }

    @Override
    public void exitShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        printWithCounter("exitShiftExpression " + ctx.getText());

    }

    @Override
    public void enterShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        printWithCounter("enterShiftOperator " + ctx.getText());

    }

    @Override
    public void exitShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        printWithCounter("exitShiftOperator " + ctx.getText());

    }

    @Override
    public void enterRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        printWithCounter("enterRelationalExpression " + ctx.getText());

    }

    @Override
    public void exitRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        printWithCounter("exitRelationalExpression " + ctx.getText());

    }

    @Override
    public void enterEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        printWithCounter("enterEqualityExpression " + ctx.getText());

    }

    @Override
    public void exitEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        printWithCounter("exitEqualityExpression " + ctx.getText());

    }

    @Override
    public void enterAndExpression(CPP14Parser.AndExpressionContext ctx) {
        printWithCounter("enterAndExpression " + ctx.getText());

    }

    @Override
    public void exitAndExpression(CPP14Parser.AndExpressionContext ctx) {
        printWithCounter("exitAndExpression " + ctx.getText());

    }

    @Override
    public void enterExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        printWithCounter("enterExclusiveOrExpression " + ctx.getText());

    }

    @Override
    public void exitExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        printWithCounter("exitExclusiveOrExpression " + ctx.getText());

    }

    @Override
    public void enterInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        printWithCounter("enterInclusiveOrExpression " + ctx.getText());

    }

    @Override
    public void exitInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        printWithCounter("exitInclusiveOrExpression " + ctx.getText());

    }

    @Override
    public void enterLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        printWithCounter("enterLogicalAndExpression " + ctx.getText());

    }

    @Override
    public void exitLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        printWithCounter("exitLogicalAndExpression " + ctx.getText());

    }

    @Override
    public void enterLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        printWithCounter("enterLogicalOrExpression " + ctx.getText());

    }

    @Override
    public void exitLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        printWithCounter("exitLogicalOrExpression " + ctx.getText());

    }

    @Override
    public void enterConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        printWithCounter("enterConditionalExpression " + ctx.getText());

    }

    @Override
    public void exitConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        printWithCounter("exitConditionalExpression " + ctx.getText());

    }

    @Override
    public void enterAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        printWithCounter("enterAssignmentExpression " + ctx.getText());

    }

    @Override
    public void exitAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        printWithCounter("exitAssignmentExpression " + ctx.getText());

    }

    @Override
    public void enterAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        printWithCounter("enterAssignmentOperator " + ctx.getText());

    }

    @Override
    public void exitAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        printWithCounter("exitAssignmentOperator " + ctx.getText());

    }

    @Override
    public void enterExpression(CPP14Parser.ExpressionContext ctx) {
        printWithCounter("enterExpression " + ctx.getText());

    }

    @Override
    public void exitExpression(CPP14Parser.ExpressionContext ctx) {
        printWithCounter("exitExpression " + ctx.getText());

    }

    @Override
    public void enterConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        printWithCounter("enterConstantExpression " + ctx.getText());

    }

    @Override
    public void exitConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        printWithCounter("exitConstantExpression " + ctx.getText());

    }

    @Override
    public void enterStatement(CPP14Parser.StatementContext ctx) {
        printWithCounter("enterStatement " + ctx.getText());

    }

    @Override
    public void exitStatement(CPP14Parser.StatementContext ctx) {
        printWithCounter("exitStatement " + ctx.getText());

    }

    @Override
    public void enterLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        printWithCounter("enterLabeledStatement " + ctx.getText());

    }

    @Override
    public void exitLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        printWithCounter("exitLabeledStatement " + ctx.getText());

    }

    @Override
    public void enterExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        printWithCounter("enterExpressionStatement " + ctx.getText());
    }

    @Override
    public void exitExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        printWithCounter("exitExpressionStatement " + ctx.getText());

    }

    @Override
    public void enterCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        printWithCounter("enterCompoundStatement " + (ctx.statementSeq() != null ? ctx.statementSeq().getText() : ""));
    }

    @Override
    public void exitCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        printWithCounter("exitCompoundStatement " + ctx.getText());

    }

    @Override
    public void enterStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        printWithCounter("enterStatementSeq " + ctx.getText());

    }

    @Override
    public void exitStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        printWithCounter("exitStatementSeq " + ctx.getText());

    }

    @Override
    public void enterSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
        printWithCounter("CONDITIONAL enterSelectionStatement " + ctx.getText() + " statements" + ctx.statement());
    }

    @Override
    public void exitSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
        printWithCounter("CONDITIONAL exitSelectionStatement" + ctx.getText());
    }

    @Override
    public void enterCondition(CPP14Parser.ConditionContext ctx) {
        printWithCounter("CONDITIONAL ENTER enterCondition " + ctx.expression().getText());

    }

    @Override
    public void exitCondition(CPP14Parser.ConditionContext ctx) {
        printWithCounter("CONDITIONAL exitCondition - end of condition variable " + ctx.getText());

    }

    @Override
    public void enterIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        printWithCounter("enterIterationStatement " + ctx.getText());
    }

    @Override
    public void exitIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        printWithCounter("exitIterationStatement " + ctx.getText());
    }

    @Override
    public void enterForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        printWithCounter("enterForInitStatement " + ctx.getText());
    }

    @Override
    public void exitForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        printWithCounter("exitForInitStatement " + ctx.getText());
    }

    @Override
    public void enterForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        printWithCounter("enterForRangeDeclaration " + ctx.getText());

    }

    @Override
    public void exitForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        printWithCounter("exitForRangeDeclaration " + ctx.getText());

    }

    @Override
    public void enterForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        printWithCounter("enterForRangeInitializer " + ctx.getText());

    }

    @Override
    public void exitForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        printWithCounter("exitForRangeInitializer " + ctx.getText());

    }

    @Override
    public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        printWithCounter("enterJumpStatement " + ctx.getText());

    }

    @Override
    public void exitJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        printWithCounter("exitJumpStatement " + ctx.getText());

    }

    @Override
    public void enterDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        printWithCounter("enterDeclarationStatement " + ctx.getText());

    }

    @Override
    public void exitDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        printWithCounter("exitDeclarationStatement " + ctx.getText());

    }

    @Override
    public void enterDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        printWithCounter("enterDeclarationseq " + ctx);

    }

    @Override
    public void exitDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        printWithCounter("exitDeclarationseq " + ctx.getText());

    }

    @Override
    public void enterDeclaration(CPP14Parser.DeclarationContext ctx) {
        printWithCounter("enterDeclaration " + ctx);

    }

    @Override
    public void exitDeclaration(CPP14Parser.DeclarationContext ctx) {
        printWithCounter("exitDeclaration " + ctx.getText());

    }

    @Override
    public void enterBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        printWithCounter("enterBlockDeclaration " + ctx.getText());

    }

    @Override
    public void exitBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        printWithCounter("exitBlockDeclaration " + ctx.getText());

    }

    @Override
    public void enterAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        printWithCounter("enterAliasDeclaration " + ctx.getText());

    }

    @Override
    public void exitAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        printWithCounter("exitAliasDeclaration " + ctx.getText());

    }

    @Override
    public void enterSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        printWithCounter("enterSimpleDeclaration " + ctx.getText());

    }

    @Override
    public void exitSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        printWithCounter("exitSimpleDeclaration " + ctx.getText());

    }

    @Override
    public void enterStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        printWithCounter("enterStaticAssertDeclaration " + ctx.getText());

    }

    @Override
    public void exitStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        printWithCounter("exitStaticAssertDeclaration " + ctx.getText());

    }

    @Override
    public void enterEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        printWithCounter("enterEmptyDeclaration " + ctx.getText());

    }

    @Override
    public void exitEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        printWithCounter("exitEmptyDeclaration " + ctx.getText());

    }

    @Override
    public void enterAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        printWithCounter("enterAttributeDeclaration " + ctx.getText());

    }

    @Override
    public void exitAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        printWithCounter("exitAttributeDeclaration " + ctx.getText());

    }

    @Override
    public void enterDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        printWithCounter("enterDeclSpecifier " + ctx.getText());

    }

    @Override
    public void exitDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        printWithCounter("exitDeclSpecifier " + ctx.getText());

    }

    @Override
    public void enterDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        printWithCounter("enterDeclSpecifierSeq " + ctx.getText());

    }

    @Override
    public void exitDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        printWithCounter("exitDeclSpecifierSeq " + ctx.getText());

    }

    @Override
    public void enterStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        printWithCounter("enterStorageClassSpecifier " + ctx.getText());

    }

    @Override
    public void exitStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        printWithCounter("exitStorageClassSpecifier " + ctx.getText());

    }

    @Override
    public void enterFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        printWithCounter("enterFunctionSpecifier " + ctx.getText());

    }

    @Override
    public void exitFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        printWithCounter("exitFunctionSpecifier " + ctx.getText());

    }

    @Override
    public void enterTypedefName(CPP14Parser.TypedefNameContext ctx) {
        printWithCounter("enterTypedefName " + ctx.getText());

    }

    @Override
    public void exitTypedefName(CPP14Parser.TypedefNameContext ctx) {
        printWithCounter("exitTypedefName " + ctx.getText());

    }

    @Override
    public void enterTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        printWithCounter("enterTypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        printWithCounter("exitTypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        printWithCounter("enterTrailingTypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        printWithCounter("exitTrailingTypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        printWithCounter("enterTypeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void exitTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        printWithCounter("exitTypeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void enterTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        printWithCounter("enterTrailingTypeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void exitTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        printWithCounter("exitTrailingTypeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void enterSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        printWithCounter("enterSimpleTypeLengthModifier " + ctx.getText());

    }

    @Override
    public void exitSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        printWithCounter("exitSimpleTypeLengthModifier " + ctx.getText());

    }

    @Override
    public void enterSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        printWithCounter("enterSimpleTypeSignednessModifier " + ctx.getText());

    }

    @Override
    public void exitSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        printWithCounter("exitSimpleTypeSignednessModifier " + ctx.getText());

    }

    @Override
    public void enterSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        printWithCounter("enterSimpleTypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        printWithCounter("exitSimpleTypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        printWithCounter("enterTheTypeName " + ctx.getText());

    }

    @Override
    public void exitTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        printWithCounter("exitTheTypeName " + ctx.getText());

    }

    @Override
    public void enterDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        printWithCounter("enterDecltypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        printWithCounter("exitDecltypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        printWithCounter("enterElaboratedTypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        printWithCounter("exitElaboratedTypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterEnumName(CPP14Parser.EnumNameContext ctx) {
        printWithCounter("enterEnumName " + ctx.getText());

    }

    @Override
    public void exitEnumName(CPP14Parser.EnumNameContext ctx) {
        printWithCounter("exitEnumName " + ctx.getText());

    }

    @Override
    public void enterEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        printWithCounter("enterEnumSpecifier " + ctx.getText());

    }

    @Override
    public void exitEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        printWithCounter("exitEnumSpecifier " + ctx.getText());

    }

    @Override
    public void enterEnumHead(CPP14Parser.EnumHeadContext ctx) {
        printWithCounter("enterEnumHead " + ctx.getText());

    }

    @Override
    public void exitEnumHead(CPP14Parser.EnumHeadContext ctx) {
        printWithCounter("exitEnumHead " + ctx.getText());

    }

    @Override
    public void enterOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        printWithCounter("enterOpaqueEnumDeclaration " + ctx.getText());

    }

    @Override
    public void exitOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        printWithCounter("exitOpaqueEnumDeclaration " + ctx.getText());

    }

    @Override
    public void enterEnumkey(CPP14Parser.EnumkeyContext ctx) {
        printWithCounter("enterEnumkey " + ctx.getText());

    }

    @Override
    public void exitEnumkey(CPP14Parser.EnumkeyContext ctx) {
        printWithCounter("exitEnumkey " + ctx.getText());

    }

    @Override
    public void enterEnumbase(CPP14Parser.EnumbaseContext ctx) {
        printWithCounter("enterEnumbase " + ctx.getText());

    }

    @Override
    public void exitEnumbase(CPP14Parser.EnumbaseContext ctx) {
        printWithCounter("exitEnumbase " + ctx.getText());

    }

    @Override
    public void enterEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        printWithCounter("enterEnumeratorList " + ctx.getText());

    }

    @Override
    public void exitEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        printWithCounter("exitEnumeratorList " + ctx.getText());

    }

    @Override
    public void enterEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        printWithCounter("enterEnumeratorDefinition " + ctx.getText());

    }

    @Override
    public void exitEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        printWithCounter("exitEnumeratorDefinition " + ctx.getText());

    }

    @Override
    public void enterEnumerator(CPP14Parser.EnumeratorContext ctx) {
        printWithCounter("enterEnumerator " + ctx.getText());

    }

    @Override
    public void exitEnumerator(CPP14Parser.EnumeratorContext ctx) {
        printWithCounter("exitEnumerator " + ctx.getText());

    }

    @Override
    public void enterNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        printWithCounter("enterNamespaceName " + ctx.getText());

    }

    @Override
    public void exitNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        printWithCounter("exitNamespaceName " + ctx.getText());

    }

    @Override
    public void enterOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        printWithCounter("enterOriginalNamespaceName " + ctx.getText());

    }

    @Override
    public void exitOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        printWithCounter("exitOriginalNamespaceName " + ctx.getText());

    }

    @Override
    public void enterNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        printWithCounter("enterNamespaceDefinition " + ctx.getText());

    }

    @Override
    public void exitNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        printWithCounter("exitNamespaceDefinition " + ctx.getText());

    }

    @Override
    public void enterNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        printWithCounter("enterNamespaceAlias " + ctx.getText());

    }

    @Override
    public void exitNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        printWithCounter("exitNamespaceAlias " + ctx.getText());

    }

    @Override
    public void enterNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        printWithCounter("enterNamespaceAliasDefinition " + ctx.getText());

    }

    @Override
    public void exitNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        printWithCounter("exitNamespaceAliasDefinition " + ctx.getText());

    }

    @Override
    public void enterQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        printWithCounter("enterQualifiednamespacespecifier " + ctx.getText());

    }

    @Override
    public void exitQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        printWithCounter("exitQualifiednamespacespecifier " + ctx.getText());

    }

    @Override
    public void enterUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        printWithCounter("enterUsingDeclaration " + ctx.getText());

    }

    @Override
    public void exitUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        printWithCounter("exitUsingDeclaration " + ctx.getText());

    }

    @Override
    public void enterUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        printWithCounter("enterUsingDirective " + ctx.getText());

    }

    @Override
    public void exitUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        printWithCounter("exitUsingDirective " + ctx.getText());

    }

    @Override
    public void enterAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        printWithCounter("enterAsmDefinition " + ctx.getText());

    }

    @Override
    public void exitAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        printWithCounter("exitAsmDefinition " + ctx.getText());

    }

    @Override
    public void enterLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        printWithCounter("enterLinkageSpecification " + ctx.getText());

    }

    @Override
    public void exitLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        printWithCounter("exitLinkageSpecification " + ctx.getText());

    }

    @Override
    public void enterAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        printWithCounter("enterAttributeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void exitAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        printWithCounter("exitAttributeSpecifierSeq " + ctx.getText());

    }

    @Override
    public void enterAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        printWithCounter("enterAttributeSpecifier " + ctx.getText());

    }

    @Override
    public void exitAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        printWithCounter("exitAttributeSpecifier " + ctx.getText());

    }

    @Override
    public void enterAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        printWithCounter("enterAlignmentspecifier " + ctx.getText());

    }

    @Override
    public void exitAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        printWithCounter("exitAlignmentspecifier " + ctx.getText());

    }

    @Override
    public void enterAttributeList(CPP14Parser.AttributeListContext ctx) {
        printWithCounter("enterAttributeList " + ctx.getText());

    }

    @Override
    public void exitAttributeList(CPP14Parser.AttributeListContext ctx) {
        printWithCounter("exitAttributeList " + ctx.getText());

    }

    @Override
    public void enterAttribute(CPP14Parser.AttributeContext ctx) {
        printWithCounter("enterAttribute " + ctx.getText());

    }

    @Override
    public void exitAttribute(CPP14Parser.AttributeContext ctx) {
        printWithCounter("exitAttribute " + ctx.getText());

    }

    @Override
    public void enterAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        printWithCounter("enterAttributeNamespace " + ctx.getText());

    }

    @Override
    public void exitAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        printWithCounter("exitAttributeNamespace " + ctx.getText());

    }

    @Override
    public void enterAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        printWithCounter("enterAttributeArgumentClause " + ctx.getText());

    }

    @Override
    public void exitAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        printWithCounter("exitAttributeArgumentClause " + ctx.getText());

    }

    @Override
    public void enterBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        printWithCounter("enterBalancedTokenSeq " + ctx.getText());

    }

    @Override
    public void exitBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        printWithCounter("exitBalancedTokenSeq " + ctx.getText());

    }

    @Override
    public void enterBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        printWithCounter("enterBalancedtoken " + ctx.getText());

    }

    @Override
    public void exitBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        printWithCounter("exitBalancedtoken " + ctx.getText());

    }

    @Override
    public void enterInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        printWithCounter("enterInitDeclaratorList " + ctx.getText());

    }

    @Override
    public void exitInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        printWithCounter("exitInitDeclaratorList " + ctx.getText());

    }

    @Override
    public void enterInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        printWithCounter("enterInitDeclarator " + ctx.getText());

    }

    @Override
    public void exitInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        printWithCounter("exitInitDeclarator " + ctx.getText());

    }

    @Override
    public void enterDeclarator(CPP14Parser.DeclaratorContext ctx) {
        printWithCounter("enterDeclarator " + ctx.pointerDeclarator().getText());

    }

    @Override
    public void exitDeclarator(CPP14Parser.DeclaratorContext ctx) {
        printWithCounter("exitDeclarator " + ctx.getText());

    }

    @Override
    public void enterPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        printWithCounter("enterPointerDeclarator " + ctx.getText());

    }

    @Override
    public void exitPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        printWithCounter("exitPointerDeclarator " + ctx.getText());

    }

    @Override
    public void enterNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        printWithCounter("enterNoPointerDeclarator " + ctx.getText());

    }

    @Override
    public void exitNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        printWithCounter("exitNoPointerDeclarator " + ctx.getText());

    }

    @Override
    public void enterParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        printWithCounter("enterParametersAndQualifiers " + ctx.getText());

    }

    @Override
    public void exitParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        printWithCounter("exitParametersAndQualifiers " + ctx.getText());

    }

    @Override
    public void enterTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        printWithCounter("enterTrailingReturnType " + ctx.getText());

    }

    @Override
    public void exitTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        printWithCounter("exitTrailingReturnType " + ctx.getText());

    }

    @Override
    public void enterPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        printWithCounter("enterPointerOperator " + ctx.getText());

    }

    @Override
    public void exitPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        printWithCounter("exitPointerOperator " + ctx.getText());

    }

    @Override
    public void enterCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        printWithCounter("enterCvqualifierseq " + ctx.getText());

    }

    @Override
    public void exitCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        printWithCounter("exitCvqualifierseq " + ctx.getText());

    }

    @Override
    public void enterCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        printWithCounter("enterCvQualifier " + ctx.getText());

    }

    @Override
    public void exitCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        printWithCounter("exitCvQualifier " + ctx.getText());

    }

    @Override
    public void enterRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        printWithCounter("enterRefqualifier " + ctx.getText());

    }

    @Override
    public void exitRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        printWithCounter("exitRefqualifier " + ctx.getText());

    }

    @Override
    public void enterDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        printWithCounter("enterDeclaratorid " + ctx.getText());

    }

    @Override
    public void exitDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        printWithCounter("exitDeclaratorid " + ctx.getText());

    }

    @Override
    public void enterTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        printWithCounter("enterTheTypeId " + ctx.getText());

    }

    @Override
    public void exitTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        printWithCounter("exitTheTypeId " + ctx.getText());

    }

    @Override
    public void enterAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        printWithCounter("enterAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void exitAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        printWithCounter("exitAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void enterPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        printWithCounter("enterPointerAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void exitPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        printWithCounter("exitPointerAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void enterNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        printWithCounter("enterNoPointerAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void exitNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        printWithCounter("exitNoPointerAbstractDeclarator " + ctx.getText());

    }

    @Override
    public void enterAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        printWithCounter("enterAbstractPackDeclarator " + ctx.getText());

    }

    @Override
    public void exitAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        printWithCounter("exitAbstractPackDeclarator " + ctx.getText());

    }

    @Override
    public void enterNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        printWithCounter("enterNoPointerAbstractPackDeclarator " + ctx.getText());

    }

    @Override
    public void exitNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        printWithCounter("exitNoPointerAbstractPackDeclarator " + ctx.getText());

    }

    @Override
    public void enterParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        printWithCounter("enterParameterDeclarationClause " + ctx.getText());

    }

    @Override
    public void exitParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        printWithCounter("exitParameterDeclarationClause " + ctx.getText());

    }

    @Override
    public void enterParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        printWithCounter("enterParameterDeclarationList " + ctx.getText());

    }

    @Override
    public void exitParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        printWithCounter("exitParameterDeclarationList " + ctx.getText());

    }

    @Override
    public void enterParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        printWithCounter("enterParameterDeclaration " + ctx.getText());

    }

    @Override
    public void exitParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        printWithCounter("exitParameterDeclaration " + ctx.getText());

    }

    @Override
    public void enterFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        printWithCounter("enterFunctionDefinition " + ctx + " " + ctx.declarator().getText());

    }

    @Override
    public void exitFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        printWithCounter("exitFunctionDefinition " + ctx.getText());

    }

    @Override
    public void enterFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        printWithCounter("enterFunctionBody " + ctx.getText());

    }

    @Override
    public void exitFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        printWithCounter("exitFunctionBody " + ctx.getText());

    }

    @Override
    public void enterInitializer(CPP14Parser.InitializerContext ctx) {
        printWithCounter("enterInitializer " + ctx.getText());

    }

    @Override
    public void exitInitializer(CPP14Parser.InitializerContext ctx) {
        printWithCounter("exitInitializer " + ctx.getText());

    }

    @Override
    public void enterBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        printWithCounter("enterBraceOrEqualInitializer " + ctx.getText());

    }

    @Override
    public void exitBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        printWithCounter("exitBraceOrEqualInitializer " + ctx.getText());

    }

    @Override
    public void enterInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        printWithCounter("enterInitializerClause " + ctx.getText());

    }

    @Override
    public void exitInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        printWithCounter("exitInitializerClause " + ctx.getText());

    }

    @Override
    public void enterInitializerList(CPP14Parser.InitializerListContext ctx) {
        printWithCounter("enterInitializerList " + ctx.getText());

    }

    @Override
    public void exitInitializerList(CPP14Parser.InitializerListContext ctx) {
        printWithCounter("exitInitializerList " + ctx.getText());

    }

    @Override
    public void enterBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        printWithCounter("enterBracedInitList " + ctx.getText());

    }

    @Override
    public void exitBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        printWithCounter("exitBracedInitList " + ctx.getText());

    }

    @Override
    public void enterClassName(CPP14Parser.ClassNameContext ctx) {
        printWithCounter("enterClassName " + ctx.getText());

    }

    @Override
    public void exitClassName(CPP14Parser.ClassNameContext ctx) {
        printWithCounter("exitClassName " + ctx.getText());

    }

    @Override
    public void enterClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        printWithCounter("enterClassSpecifier " + ctx.getText());

    }

    @Override
    public void exitClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        printWithCounter("exitClassSpecifier " + ctx.getText());

    }

    @Override
    public void enterClassHead(CPP14Parser.ClassHeadContext ctx) {
        printWithCounter("enterClassHead " + ctx.getText());

    }

    @Override
    public void exitClassHead(CPP14Parser.ClassHeadContext ctx) {
        printWithCounter("exitClassHead " + ctx.getText());

    }

    @Override
    public void enterClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        printWithCounter("enterClassHeadName " + ctx.getText());

    }

    @Override
    public void exitClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        printWithCounter("exitClassHeadName " + ctx.getText());

    }

    @Override
    public void enterClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        printWithCounter("enterClassVirtSpecifier " + ctx.getText());

    }

    @Override
    public void exitClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        printWithCounter("exitClassVirtSpecifier " + ctx.getText());

    }

    @Override
    public void enterClassKey(CPP14Parser.ClassKeyContext ctx) {
        printWithCounter("enterClassKey " + ctx.getText());

    }

    @Override
    public void exitClassKey(CPP14Parser.ClassKeyContext ctx) {
        printWithCounter("exitClassKey " + ctx.getText());

    }

    @Override
    public void enterMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        printWithCounter("enterMemberSpecification " + ctx.getText());

    }

    @Override
    public void exitMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        printWithCounter("exitMemberSpecification " + ctx.getText());

    }

    @Override
    public void enterMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        printWithCounter("enterMemberdeclaration " + ctx.getText());

    }

    @Override
    public void exitMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        printWithCounter("exitMemberdeclaration " + ctx.getText());

    }

    @Override
    public void enterMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        printWithCounter("enterMemberDeclaratorList " + ctx.getText());

    }

    @Override
    public void exitMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        printWithCounter("exitMemberDeclaratorList " + ctx.getText());

    }

    @Override
    public void enterMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        printWithCounter("enterMemberDeclarator " + ctx.getText());

    }

    @Override
    public void exitMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        printWithCounter("exitMemberDeclarator " + ctx.getText());

    }

    @Override
    public void enterVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        printWithCounter("enterVirtualSpecifierSeq " + ctx.getText());

    }

    @Override
    public void exitVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        printWithCounter("exitVirtualSpecifierSeq " + ctx.getText());

    }

    @Override
    public void enterVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        printWithCounter("enterVirtualSpecifier " + ctx.getText());

    }

    @Override
    public void exitVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        printWithCounter("exitVirtualSpecifier " + ctx.getText());

    }

    @Override
    public void enterPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        printWithCounter("enterPureSpecifier " + ctx.getText());

    }

    @Override
    public void exitPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        printWithCounter("exitPureSpecifier " + ctx.getText());

    }

    @Override
    public void enterBaseClause(CPP14Parser.BaseClauseContext ctx) {
        printWithCounter("enterBaseClause " + ctx.getText());

    }

    @Override
    public void exitBaseClause(CPP14Parser.BaseClauseContext ctx) {
        printWithCounter("exitBaseClause " + ctx.getText());

    }

    @Override
    public void enterBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        printWithCounter("enterBaseSpecifierList " + ctx.getText());

    }

    @Override
    public void exitBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        printWithCounter("exitBaseSpecifierList " + ctx.getText());

    }

    @Override
    public void enterBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        printWithCounter("enterBaseSpecifier " + ctx.getText());

    }

    @Override
    public void exitBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        printWithCounter("exitBaseSpecifier " + ctx.getText());

    }

    @Override
    public void enterClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        printWithCounter("enterClassOrDeclType " + ctx.getText());

    }

    @Override
    public void exitClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        printWithCounter("exitClassOrDeclType " + ctx.getText());

    }

    @Override
    public void enterBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        printWithCounter("enterBaseTypeSpecifier " + ctx.getText());

    }

    @Override
    public void exitBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        printWithCounter("exitBaseTypeSpecifier " + ctx.getText());

    }

    @Override
    public void enterAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        printWithCounter("enterAccessSpecifier " + ctx.getText());

    }

    @Override
    public void exitAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        printWithCounter("exitAccessSpecifier " + ctx.getText());

    }

    @Override
    public void enterConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        printWithCounter("enterConversionFunctionId " + ctx.getText());

    }

    @Override
    public void exitConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        printWithCounter("exitConversionFunctionId " + ctx.getText());

    }

    @Override
    public void enterConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        printWithCounter("enterConversionTypeId " + ctx.getText());

    }

    @Override
    public void exitConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        printWithCounter("exitConversionTypeId " + ctx.getText());

    }

    @Override
    public void enterConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        printWithCounter("enterConversionDeclarator " + ctx.getText());

    }

    @Override
    public void exitConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        printWithCounter("exitConversionDeclarator " + ctx.getText());

    }

    @Override
    public void enterConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        printWithCounter("enterConstructorInitializer " + ctx.getText());

    }

    @Override
    public void exitConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        printWithCounter("exitConstructorInitializer " + ctx.getText());

    }

    @Override
    public void enterMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        printWithCounter("enterMemInitializerList " + ctx.getText());

    }

    @Override
    public void exitMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        printWithCounter("exitMemInitializerList " + ctx.getText());

    }

    @Override
    public void enterMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        printWithCounter("enterMemInitializer " + ctx.getText());

    }

    @Override
    public void exitMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        printWithCounter("exitMemInitializer " + ctx.getText());

    }

    @Override
    public void enterMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        printWithCounter("enterMeminitializerid " + ctx.getText());

    }

    @Override
    public void exitMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        printWithCounter("exitMeminitializerid " + ctx.getText());

    }

    @Override
    public void enterOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        printWithCounter("enterOperatorFunctionId " + ctx.getText());

    }

    @Override
    public void exitOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        printWithCounter("exitOperatorFunctionId " + ctx.getText());

    }

    @Override
    public void enterLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        printWithCounter("enterLiteralOperatorId " + ctx.getText());

    }

    @Override
    public void exitLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        printWithCounter("exitLiteralOperatorId " + ctx.getText());

    }

    @Override
    public void enterTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        printWithCounter("enterTemplateDeclaration " + ctx.getText());

    }

    @Override
    public void exitTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        printWithCounter("exitTemplateDeclaration " + ctx.getText());

    }

    @Override
    public void enterTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        printWithCounter("enterTemplateparameterList " + ctx.getText());

    }

    @Override
    public void exitTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        printWithCounter("exitTemplateparameterList " + ctx.getText());

    }

    @Override
    public void enterTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        printWithCounter("enterTemplateParameter " + ctx.getText());

    }

    @Override
    public void exitTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        printWithCounter("exitTemplateParameter " + ctx.getText());

    }

    @Override
    public void enterTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        printWithCounter("enterTypeParameter " + ctx.getText());

    }

    @Override
    public void exitTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        printWithCounter("exitTypeParameter " + ctx.getText());

    }

    @Override
    public void enterSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        printWithCounter("enterSimpleTemplateId " + ctx.getText());

    }

    @Override
    public void exitSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        printWithCounter("exitSimpleTemplateId " + ctx.getText());

    }

    @Override
    public void enterTemplateId(CPP14Parser.TemplateIdContext ctx) {
        printWithCounter("enterTemplateId " + ctx.getText());

    }

    @Override
    public void exitTemplateId(CPP14Parser.TemplateIdContext ctx) {
        printWithCounter("exitTemplateId " + ctx.getText());

    }

    @Override
    public void enterTemplateName(CPP14Parser.TemplateNameContext ctx) {
        printWithCounter("enterTemplateName " + ctx.getText());

    }

    @Override
    public void exitTemplateName(CPP14Parser.TemplateNameContext ctx) {
        printWithCounter("exitTemplateName " + ctx.getText());

    }

    @Override
    public void enterTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        printWithCounter("enterTemplateArgumentList " + ctx.getText());

    }

    @Override
    public void exitTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        printWithCounter("exitTemplateArgumentList " + ctx.getText());

    }

    @Override
    public void enterTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        printWithCounter("enterTemplateArgument " + ctx.getText());

    }

    @Override
    public void exitTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        printWithCounter("exitTemplateArgument " + ctx.getText());

    }

    @Override
    public void enterTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        printWithCounter("enterTypeNameSpecifier " + ctx.getText());

    }

    @Override
    public void exitTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        printWithCounter("exitTypeNameSpecifier " + ctx.getText());

    }

    @Override
    public void enterExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        printWithCounter("enterExplicitInstantiation " + ctx.getText());

    }

    @Override
    public void exitExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        printWithCounter("exitExplicitInstantiation " + ctx.getText());

    }

    @Override
    public void enterExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        printWithCounter("enterExplicitSpecialization " + ctx.getText());

    }

    @Override
    public void exitExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        printWithCounter("exitExplicitSpecialization " + ctx.getText());

    }

    @Override
    public void enterTryBlock(CPP14Parser.TryBlockContext ctx) {
        printWithCounter("enterTryBlock " + ctx.getText());

    }

    @Override
    public void exitTryBlock(CPP14Parser.TryBlockContext ctx) {
        printWithCounter("exitTryBlock " + ctx.getText());

    }

    @Override
    public void enterFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        printWithCounter("enterFunctionTryBlock " + ctx.getText());

    }

    @Override
    public void exitFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        printWithCounter("exitFunctionTryBlock " + ctx.getText());

    }

    @Override
    public void enterHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        printWithCounter("enterHandlerSeq " + ctx.getText());

    }

    @Override
    public void exitHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        printWithCounter("exitHandlerSeq " + ctx.getText());

    }

    @Override
    public void enterHandler(CPP14Parser.HandlerContext ctx) {
        printWithCounter("enterHandler " + ctx.getText());

    }

    @Override
    public void exitHandler(CPP14Parser.HandlerContext ctx) {
        printWithCounter("exitHandler " + ctx.getText());

    }

    @Override
    public void enterExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        printWithCounter("enterExceptionDeclaration " + ctx.getText());

    }

    @Override
    public void exitExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        printWithCounter("exitExceptionDeclaration " + ctx.getText());

    }

    @Override
    public void enterThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        printWithCounter("enterThrowExpression " + ctx.getText());

    }

    @Override
    public void exitThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        printWithCounter("exitThrowExpression " + ctx.getText());

    }

    @Override
    public void enterExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        printWithCounter("enterExceptionSpecification " + ctx.getText());

    }

    @Override
    public void exitExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        printWithCounter("exitExceptionSpecification " + ctx.getText());

    }

    @Override
    public void enterDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        printWithCounter("enterDynamicExceptionSpecification " + ctx.getText());

    }

    @Override
    public void exitDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        printWithCounter("exitDynamicExceptionSpecification " + ctx.getText());

    }

    @Override
    public void enterTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        printWithCounter("enterTypeIdList " + ctx.getText());

    }

    @Override
    public void exitTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        printWithCounter("exitTypeIdList " + ctx.getText());

    }

    @Override
    public void enterNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        printWithCounter("enterNoeExceptSpecification " + ctx.getText());

    }

    @Override
    public void exitNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        printWithCounter("exitNoeExceptSpecification " + ctx.getText());

    }

    @Override
    public void enterTheOperator(CPP14Parser.TheOperatorContext ctx) {
        printWithCounter("enterTheOperator " + ctx.getText());

    }

    @Override
    public void exitTheOperator(CPP14Parser.TheOperatorContext ctx) {
        printWithCounter("exitTheOperator " + ctx.getText());

    }

    @Override
    public void enterLiteral(CPP14Parser.LiteralContext ctx) {
        printWithCounter("enterLiteral " + ctx.getText());

    }

    @Override
    public void exitLiteral(CPP14Parser.LiteralContext ctx) {
        printWithCounter("exitLiteral " + ctx.getText());

    }

    @Override
    public void visitTerminal(TerminalNode terminalNode) {
        printWithCounter("visitTerminal " + terminalNode.getSymbol().getText());

    }

    @Override
    public void visitErrorNode(ErrorNode errorNode) {
        printWithCounter("visitErrorNode " + errorNode);

    }
}
