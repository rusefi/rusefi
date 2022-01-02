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
//        printWithCounter("enterEveryRule");
    }

    @Override
    public void exitEveryRule(ParserRuleContext parserRuleContext) {
//        printWithCounter("exitEveryRule");
    }

    @Override
    public void exitTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        printWithCounter("exitTranslationUnit");

    }

    @Override
    public void enterTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        printWithCounter("enterTranslationUnit");

    }

    @Override
    public void enterPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        printWithCounter("enterPrimaryExpression " + ctx.getText());

    }

    @Override
    public void exitPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        printWithCounter("exitPrimaryExpression");

    }

    @Override
    public void enterIdExpression(CPP14Parser.IdExpressionContext ctx) {
        printWithCounter("enterIdExpression");

    }

    @Override
    public void exitIdExpression(CPP14Parser.IdExpressionContext ctx) {
        printWithCounter("exitIdExpression");

    }

    @Override
    public void enterUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        printWithCounter("enterUnqualifiedId");

    }

    @Override
    public void exitUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        printWithCounter("exitUnqualifiedId");

    }

    @Override
    public void enterQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        printWithCounter("enterQualifiedId");

    }

    @Override
    public void exitQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        printWithCounter("exitQualifiedId");

    }

    @Override
    public void enterNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        printWithCounter("enterNestedNameSpecifier");

    }

    @Override
    public void exitNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        printWithCounter("exitNestedNameSpecifier");

    }

    @Override
    public void enterLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        printWithCounter("enterLambdaExpression");

    }

    @Override
    public void exitLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        printWithCounter("exitLambdaExpression");

    }

    @Override
    public void enterLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        printWithCounter("enterLambdaIntroducer");

    }

    @Override
    public void exitLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        printWithCounter("exitLambdaIntroducer");

    }

    @Override
    public void enterLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        printWithCounter("enterLambdaCapture");

    }

    @Override
    public void exitLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        printWithCounter("exitLambdaCapture");

    }

    @Override
    public void enterCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        printWithCounter("enterCaptureDefault");

    }

    @Override
    public void exitCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        printWithCounter("exitCaptureDefault");

    }

    @Override
    public void enterCaptureList(CPP14Parser.CaptureListContext ctx) {
        printWithCounter("enterCaptureList");

    }

    @Override
    public void exitCaptureList(CPP14Parser.CaptureListContext ctx) {
        printWithCounter("exitCaptureList");

    }

    @Override
    public void enterCapture(CPP14Parser.CaptureContext ctx) {
        printWithCounter("enterCapture");

    }

    @Override
    public void exitCapture(CPP14Parser.CaptureContext ctx) {
        printWithCounter("exitCapture");

    }

    @Override
    public void enterSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        printWithCounter("enterSimpleCapture");

    }

    @Override
    public void exitSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        printWithCounter("exitSimpleCapture");

    }

    @Override
    public void enterInitcapture(CPP14Parser.InitcaptureContext ctx) {
        printWithCounter("enterInitcapture");

    }

    @Override
    public void exitInitcapture(CPP14Parser.InitcaptureContext ctx) {
        printWithCounter("exitInitcapture");

    }

    @Override
    public void enterLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        printWithCounter("enterLambdaDeclarator");

    }

    @Override
    public void exitLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        printWithCounter("exitLambdaDeclarator");

    }

    @Override
    public void enterPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        printWithCounter("enterPostfixExpression");

    }

    @Override
    public void exitPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        printWithCounter("exitPostfixExpression");

    }

    @Override
    public void enterTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        printWithCounter("enterTypeIdOfTheTypeId");

    }

    @Override
    public void exitTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        printWithCounter("exitTypeIdOfTheTypeId");

    }

    @Override
    public void enterExpressionList(CPP14Parser.ExpressionListContext ctx) {
        printWithCounter("enterExpressionList");

    }

    @Override
    public void exitExpressionList(CPP14Parser.ExpressionListContext ctx) {
        printWithCounter("exitExpressionList");

    }

    @Override
    public void enterPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        printWithCounter("enterPseudoDestructorName");

    }

    @Override
    public void exitPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        printWithCounter("exitPseudoDestructorName");

    }

    @Override
    public void enterUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        printWithCounter("enterUnaryExpression");

    }

    @Override
    public void exitUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        printWithCounter("exitUnaryExpression");

    }

    @Override
    public void enterUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        printWithCounter("enterUnaryOperator");

    }

    @Override
    public void exitUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        printWithCounter("exitUnaryOperator");

    }

    @Override
    public void enterNewExpression(CPP14Parser.NewExpressionContext ctx) {
        printWithCounter("enterNewExpression");

    }

    @Override
    public void exitNewExpression(CPP14Parser.NewExpressionContext ctx) {
        printWithCounter("exitNewExpression");

    }

    @Override
    public void enterNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        printWithCounter("enterNewPlacement");

    }

    @Override
    public void exitNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        printWithCounter("exitNewPlacement");

    }

    @Override
    public void enterNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        printWithCounter("enterNewTypeId");

    }

    @Override
    public void exitNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        printWithCounter("exitNewTypeId");

    }

    @Override
    public void enterNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        printWithCounter("enterNewDeclarator");

    }

    @Override
    public void exitNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        printWithCounter("exitNewDeclarator");

    }

    @Override
    public void enterNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        printWithCounter("enterNoPointerNewDeclarator");

    }

    @Override
    public void exitNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        printWithCounter("exitNoPointerNewDeclarator");

    }

    @Override
    public void enterNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        printWithCounter("enterNewInitializer");

    }

    @Override
    public void exitNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        printWithCounter("exitNewInitializer");

    }

    @Override
    public void enterDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        printWithCounter("enterDeleteExpression");

    }

    @Override
    public void exitDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        printWithCounter("exitDeleteExpression");

    }

    @Override
    public void enterNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        printWithCounter("enterNoExceptExpression");

    }

    @Override
    public void exitNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        printWithCounter("exitNoExceptExpression");

    }

    @Override
    public void enterCastExpression(CPP14Parser.CastExpressionContext ctx) {
        printWithCounter("enterCastExpression");

    }

    @Override
    public void exitCastExpression(CPP14Parser.CastExpressionContext ctx) {
        printWithCounter("exitCastExpression");

    }

    @Override
    public void enterPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        printWithCounter("enterPointerMemberExpression");

    }

    @Override
    public void exitPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        printWithCounter("exitPointerMemberExpression");

    }

    @Override
    public void enterMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        printWithCounter("enterMultiplicativeExpression");

    }

    @Override
    public void exitMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        printWithCounter("exitMultiplicativeExpression");

    }

    @Override
    public void enterAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        printWithCounter("enterAdditiveExpression");

    }

    @Override
    public void exitAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        printWithCounter("exitAdditiveExpression");

    }

    @Override
    public void enterShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        printWithCounter("enterShiftExpression");

    }

    @Override
    public void exitShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        printWithCounter("exitShiftExpression");

    }

    @Override
    public void enterShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        printWithCounter("enterShiftOperator");

    }

    @Override
    public void exitShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        printWithCounter("exitShiftOperator");

    }

    @Override
    public void enterRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        printWithCounter("enterRelationalExpression");

    }

    @Override
    public void exitRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        printWithCounter("exitRelationalExpression");

    }

    @Override
    public void enterEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        printWithCounter("enterEqualityExpression");

    }

    @Override
    public void exitEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        printWithCounter("exitEqualityExpression");

    }

    @Override
    public void enterAndExpression(CPP14Parser.AndExpressionContext ctx) {
        printWithCounter("enterAndExpression");

    }

    @Override
    public void exitAndExpression(CPP14Parser.AndExpressionContext ctx) {
        printWithCounter("exitAndExpression");

    }

    @Override
    public void enterExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        printWithCounter("enterExclusiveOrExpression");

    }

    @Override
    public void exitExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        printWithCounter("exitExclusiveOrExpression");

    }

    @Override
    public void enterInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        printWithCounter("enterInclusiveOrExpression");

    }

    @Override
    public void exitInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        printWithCounter("exitInclusiveOrExpression");

    }

    @Override
    public void enterLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        printWithCounter("enterLogicalAndExpression");

    }

    @Override
    public void exitLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        printWithCounter("exitLogicalAndExpression");

    }

    @Override
    public void enterLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        printWithCounter("enterLogicalOrExpression");

    }

    @Override
    public void exitLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        printWithCounter("exitLogicalOrExpression");

    }

    @Override
    public void enterConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        printWithCounter("enterConditionalExpression " + ctx.getText());

    }

    @Override
    public void exitConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        printWithCounter("exitConditionalExpression");

    }

    @Override
    public void enterAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        printWithCounter("enterAssignmentExpression");

    }

    @Override
    public void exitAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        printWithCounter("exitAssignmentExpression");

    }

    @Override
    public void enterAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        printWithCounter("enterAssignmentOperator");

    }

    @Override
    public void exitAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        printWithCounter("exitAssignmentOperator");

    }

    @Override
    public void enterExpression(CPP14Parser.ExpressionContext ctx) {
        printWithCounter("enterExpression");

    }

    @Override
    public void exitExpression(CPP14Parser.ExpressionContext ctx) {
        printWithCounter("exitExpression");

    }

    @Override
    public void enterConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        printWithCounter("enterConstantExpression");

    }

    @Override
    public void exitConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        printWithCounter("exitConstantExpression");

    }

    @Override
    public void enterStatement(CPP14Parser.StatementContext ctx) {
        printWithCounter("enterStatement " + ctx.getText());

    }

    @Override
    public void exitStatement(CPP14Parser.StatementContext ctx) {
        printWithCounter("exitStatement");

    }

    @Override
    public void enterLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        printWithCounter("enterLabeledStatement " + ctx.getText());

    }

    @Override
    public void exitLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        printWithCounter("exitLabeledStatement");

    }

    @Override
    public void enterExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        printWithCounter("enterExpressionStatement " + ctx.getText());
    }

    @Override
    public void exitExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        printWithCounter("exitExpressionStatement");

    }

    @Override
    public void enterCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        printWithCounter("enterCompoundStatement " + ctx.statementSeq().getText());

    }

    @Override
    public void exitCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        printWithCounter("exitCompoundStatement");

    }

    @Override
    public void enterStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        printWithCounter("enterStatementSeq " + ctx.getText());

    }

    @Override
    public void exitStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        printWithCounter("exitStatementSeq");

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
        printWithCounter("CONDITIONAL exitCondition - end of condition variable");

    }

    @Override
    public void enterIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        printWithCounter("enterIterationStatement " + ctx.getText());
    }

    @Override
    public void exitIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        printWithCounter("exitIterationStatement");
    }

    @Override
    public void enterForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        printWithCounter("enterForInitStatement");
    }

    @Override
    public void exitForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        printWithCounter("exitForInitStatement");
    }

    @Override
    public void enterForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        printWithCounter("enterForRangeDeclaration");

    }

    @Override
    public void exitForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        printWithCounter("exitForRangeDeclaration");

    }

    @Override
    public void enterForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        printWithCounter("enterForRangeInitializer");

    }

    @Override
    public void exitForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        printWithCounter("exitForRangeInitializer");

    }

    @Override
    public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        printWithCounter("enterJumpStatement " + ctx.getText());

    }

    @Override
    public void exitJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        printWithCounter("exitJumpStatement");

    }

    @Override
    public void enterDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        printWithCounter("enterDeclarationStatement " + ctx.getText());

    }

    @Override
    public void exitDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        printWithCounter("exitDeclarationStatement");

    }

    @Override
    public void enterDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        printWithCounter("enterDeclarationseq " + ctx);

    }

    @Override
    public void exitDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        printWithCounter("exitDeclarationseq");

    }

    @Override
    public void enterDeclaration(CPP14Parser.DeclarationContext ctx) {
        printWithCounter("enterDeclaration " + ctx);

    }

    @Override
    public void exitDeclaration(CPP14Parser.DeclarationContext ctx) {
        printWithCounter("exitDeclaration");

    }

    @Override
    public void enterBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        printWithCounter("enterBlockDeclaration");

    }

    @Override
    public void exitBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        printWithCounter("exitBlockDeclaration");

    }

    @Override
    public void enterAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        printWithCounter("enterAliasDeclaration");

    }

    @Override
    public void exitAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        printWithCounter("exitAliasDeclaration");

    }

    @Override
    public void enterSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        printWithCounter("enterSimpleDeclaration");

    }

    @Override
    public void exitSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        printWithCounter("exitSimpleDeclaration");

    }

    @Override
    public void enterStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        printWithCounter("enterStaticAssertDeclaration");

    }

    @Override
    public void exitStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        printWithCounter("exitStaticAssertDeclaration");

    }

    @Override
    public void enterEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        printWithCounter("enterEmptyDeclaration");

    }

    @Override
    public void exitEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        printWithCounter("exitEmptyDeclaration");

    }

    @Override
    public void enterAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        printWithCounter("enterAttributeDeclaration");

    }

    @Override
    public void exitAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        printWithCounter("exitAttributeDeclaration");

    }

    @Override
    public void enterDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        printWithCounter("enterDeclSpecifier");

    }

    @Override
    public void exitDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        printWithCounter("exitDeclSpecifier");

    }

    @Override
    public void enterDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        printWithCounter("enterDeclSpecifierSeq");

    }

    @Override
    public void exitDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        printWithCounter("exitDeclSpecifierSeq");

    }

    @Override
    public void enterStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        printWithCounter("enterStorageClassSpecifier");

    }

    @Override
    public void exitStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        printWithCounter("exitStorageClassSpecifier");

    }

    @Override
    public void enterFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        printWithCounter("enterFunctionSpecifier");

    }

    @Override
    public void exitFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        printWithCounter("exitFunctionSpecifier");

    }

    @Override
    public void enterTypedefName(CPP14Parser.TypedefNameContext ctx) {
        printWithCounter("enterTypedefName");

    }

    @Override
    public void exitTypedefName(CPP14Parser.TypedefNameContext ctx) {
        printWithCounter("exitTypedefName");

    }

    @Override
    public void enterTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        printWithCounter("enterTypeSpecifier");

    }

    @Override
    public void exitTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        printWithCounter("exitTypeSpecifier");

    }

    @Override
    public void enterTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        printWithCounter("enterTrailingTypeSpecifier");

    }

    @Override
    public void exitTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        printWithCounter("exitTrailingTypeSpecifier");

    }

    @Override
    public void enterTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        printWithCounter("enterTypeSpecifierSeq");

    }

    @Override
    public void exitTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        printWithCounter("exitTypeSpecifierSeq");

    }

    @Override
    public void enterTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        printWithCounter("enterTrailingTypeSpecifierSeq");

    }

    @Override
    public void exitTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        printWithCounter("exitTrailingTypeSpecifierSeq");

    }

    @Override
    public void enterSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        printWithCounter("enterSimpleTypeLengthModifier");

    }

    @Override
    public void exitSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        printWithCounter("exitSimpleTypeLengthModifier");

    }

    @Override
    public void enterSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        printWithCounter("enterSimpleTypeSignednessModifier");

    }

    @Override
    public void exitSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        printWithCounter("exitSimpleTypeSignednessModifier");

    }

    @Override
    public void enterSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        printWithCounter("enterSimpleTypeSpecifier");

    }

    @Override
    public void exitSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        printWithCounter("exitSimpleTypeSpecifier");

    }

    @Override
    public void enterTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        printWithCounter("enterTheTypeName");

    }

    @Override
    public void exitTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        printWithCounter("exitTheTypeName");

    }

    @Override
    public void enterDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        printWithCounter("enterDecltypeSpecifier");

    }

    @Override
    public void exitDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        printWithCounter("exitDecltypeSpecifier");

    }

    @Override
    public void enterElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        printWithCounter("enterElaboratedTypeSpecifier");

    }

    @Override
    public void exitElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        printWithCounter("exitElaboratedTypeSpecifier");

    }

    @Override
    public void enterEnumName(CPP14Parser.EnumNameContext ctx) {
        printWithCounter("enterEnumName");

    }

    @Override
    public void exitEnumName(CPP14Parser.EnumNameContext ctx) {
        printWithCounter("exitEnumName");

    }

    @Override
    public void enterEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        printWithCounter("enterEnumSpecifier");

    }

    @Override
    public void exitEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        printWithCounter("exitEnumSpecifier");

    }

    @Override
    public void enterEnumHead(CPP14Parser.EnumHeadContext ctx) {
        printWithCounter("enterEnumHead");

    }

    @Override
    public void exitEnumHead(CPP14Parser.EnumHeadContext ctx) {
        printWithCounter("exitEnumHead");

    }

    @Override
    public void enterOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        printWithCounter("enterOpaqueEnumDeclaration");

    }

    @Override
    public void exitOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        printWithCounter("exitOpaqueEnumDeclaration");

    }

    @Override
    public void enterEnumkey(CPP14Parser.EnumkeyContext ctx) {
        printWithCounter("enterEnumkey");

    }

    @Override
    public void exitEnumkey(CPP14Parser.EnumkeyContext ctx) {
        printWithCounter("exitEnumkey");

    }

    @Override
    public void enterEnumbase(CPP14Parser.EnumbaseContext ctx) {
        printWithCounter("enterEnumbase");

    }

    @Override
    public void exitEnumbase(CPP14Parser.EnumbaseContext ctx) {
        printWithCounter("exitEnumbase");

    }

    @Override
    public void enterEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        printWithCounter("enterEnumeratorList");

    }

    @Override
    public void exitEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        printWithCounter("exitEnumeratorList");

    }

    @Override
    public void enterEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        printWithCounter("enterEnumeratorDefinition");

    }

    @Override
    public void exitEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        printWithCounter("exitEnumeratorDefinition");

    }

    @Override
    public void enterEnumerator(CPP14Parser.EnumeratorContext ctx) {
        printWithCounter("enterEnumerator");

    }

    @Override
    public void exitEnumerator(CPP14Parser.EnumeratorContext ctx) {
        printWithCounter("exitEnumerator");

    }

    @Override
    public void enterNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        printWithCounter("enterNamespaceName");

    }

    @Override
    public void exitNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        printWithCounter("exitNamespaceName");

    }

    @Override
    public void enterOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        printWithCounter("enterOriginalNamespaceName");

    }

    @Override
    public void exitOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        printWithCounter("exitOriginalNamespaceName");

    }

    @Override
    public void enterNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        printWithCounter("enterNamespaceDefinition");

    }

    @Override
    public void exitNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        printWithCounter("exitNamespaceDefinition");

    }

    @Override
    public void enterNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        printWithCounter("enterNamespaceAlias");

    }

    @Override
    public void exitNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        printWithCounter("exitNamespaceAlias");

    }

    @Override
    public void enterNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        printWithCounter("enterNamespaceAliasDefinition");

    }

    @Override
    public void exitNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        printWithCounter("exitNamespaceAliasDefinition");

    }

    @Override
    public void enterQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        printWithCounter("enterQualifiednamespacespecifier");

    }

    @Override
    public void exitQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        printWithCounter("exitQualifiednamespacespecifier");

    }

    @Override
    public void enterUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        printWithCounter("enterUsingDeclaration");

    }

    @Override
    public void exitUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        printWithCounter("exitUsingDeclaration");

    }

    @Override
    public void enterUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        printWithCounter("enterUsingDirective");

    }

    @Override
    public void exitUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        printWithCounter("exitUsingDirective");

    }

    @Override
    public void enterAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        printWithCounter("enterAsmDefinition");

    }

    @Override
    public void exitAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        printWithCounter("exitAsmDefinition");

    }

    @Override
    public void enterLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        printWithCounter("enterLinkageSpecification");

    }

    @Override
    public void exitLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        printWithCounter("exitLinkageSpecification");

    }

    @Override
    public void enterAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        printWithCounter("enterAttributeSpecifierSeq");

    }

    @Override
    public void exitAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        printWithCounter("exitAttributeSpecifierSeq");

    }

    @Override
    public void enterAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        printWithCounter("enterAttributeSpecifier");

    }

    @Override
    public void exitAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        printWithCounter("exitAttributeSpecifier");

    }

    @Override
    public void enterAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        printWithCounter("enterAlignmentspecifier");

    }

    @Override
    public void exitAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        printWithCounter("exitAlignmentspecifier");

    }

    @Override
    public void enterAttributeList(CPP14Parser.AttributeListContext ctx) {
        printWithCounter("enterAttributeList");

    }

    @Override
    public void exitAttributeList(CPP14Parser.AttributeListContext ctx) {
        printWithCounter("exitAttributeList");

    }

    @Override
    public void enterAttribute(CPP14Parser.AttributeContext ctx) {
        printWithCounter("enterAttribute");

    }

    @Override
    public void exitAttribute(CPP14Parser.AttributeContext ctx) {
        printWithCounter("exitAttribute");

    }

    @Override
    public void enterAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        printWithCounter("enterAttributeNamespace");

    }

    @Override
    public void exitAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        printWithCounter("exitAttributeNamespace");

    }

    @Override
    public void enterAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        printWithCounter("enterAttributeArgumentClause");

    }

    @Override
    public void exitAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        printWithCounter("exitAttributeArgumentClause");

    }

    @Override
    public void enterBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        printWithCounter("enterBalancedTokenSeq");

    }

    @Override
    public void exitBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        printWithCounter("exitBalancedTokenSeq");

    }

    @Override
    public void enterBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        printWithCounter("enterBalancedtoken");

    }

    @Override
    public void exitBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        printWithCounter("exitBalancedtoken");

    }

    @Override
    public void enterInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        printWithCounter("enterInitDeclaratorList");

    }

    @Override
    public void exitInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        printWithCounter("exitInitDeclaratorList");

    }

    @Override
    public void enterInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        printWithCounter("enterInitDeclarator");

    }

    @Override
    public void exitInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        printWithCounter("exitInitDeclarator");

    }

    @Override
    public void enterDeclarator(CPP14Parser.DeclaratorContext ctx) {
        printWithCounter("enterDeclarator " + ctx.pointerDeclarator().getText());

    }

    @Override
    public void exitDeclarator(CPP14Parser.DeclaratorContext ctx) {
        printWithCounter("exitDeclarator");

    }

    @Override
    public void enterPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        printWithCounter("enterPointerDeclarator");

    }

    @Override
    public void exitPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        printWithCounter("exitPointerDeclarator");

    }

    @Override
    public void enterNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        printWithCounter("enterNoPointerDeclarator");

    }

    @Override
    public void exitNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        printWithCounter("exitNoPointerDeclarator");

    }

    @Override
    public void enterParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        printWithCounter("enterParametersAndQualifiers");

    }

    @Override
    public void exitParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        printWithCounter("exitParametersAndQualifiers");

    }

    @Override
    public void enterTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        printWithCounter("enterTrailingReturnType");

    }

    @Override
    public void exitTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        printWithCounter("exitTrailingReturnType");

    }

    @Override
    public void enterPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        printWithCounter("enterPointerOperator");

    }

    @Override
    public void exitPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        printWithCounter("exitPointerOperator");

    }

    @Override
    public void enterCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        printWithCounter("enterCvqualifierseq");

    }

    @Override
    public void exitCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        printWithCounter("exitCvqualifierseq");

    }

    @Override
    public void enterCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        printWithCounter("enterCvQualifier");

    }

    @Override
    public void exitCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        printWithCounter("exitCvQualifier");

    }

    @Override
    public void enterRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        printWithCounter("enterRefqualifier");

    }

    @Override
    public void exitRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        printWithCounter("exitRefqualifier");

    }

    @Override
    public void enterDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        printWithCounter("enterDeclaratorid");

    }

    @Override
    public void exitDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        printWithCounter("exitDeclaratorid");

    }

    @Override
    public void enterTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        printWithCounter("enterTheTypeId");

    }

    @Override
    public void exitTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        printWithCounter("exitTheTypeId");

    }

    @Override
    public void enterAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        printWithCounter("enterAbstractDeclarator");

    }

    @Override
    public void exitAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        printWithCounter("exitAbstractDeclarator");

    }

    @Override
    public void enterPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        printWithCounter("enterPointerAbstractDeclarator");

    }

    @Override
    public void exitPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        printWithCounter("exitPointerAbstractDeclarator");

    }

    @Override
    public void enterNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        printWithCounter("enterNoPointerAbstractDeclarator");

    }

    @Override
    public void exitNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        printWithCounter("exitNoPointerAbstractDeclarator");

    }

    @Override
    public void enterAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        printWithCounter("enterAbstractPackDeclarator");

    }

    @Override
    public void exitAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        printWithCounter("exitAbstractPackDeclarator");

    }

    @Override
    public void enterNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        printWithCounter("enterNoPointerAbstractPackDeclarator");

    }

    @Override
    public void exitNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        printWithCounter("exitNoPointerAbstractPackDeclarator");

    }

    @Override
    public void enterParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        printWithCounter("enterParameterDeclarationClause");

    }

    @Override
    public void exitParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        printWithCounter("exitParameterDeclarationClause");

    }

    @Override
    public void enterParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        printWithCounter("enterParameterDeclarationList");

    }

    @Override
    public void exitParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        printWithCounter("exitParameterDeclarationList");

    }

    @Override
    public void enterParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        printWithCounter("enterParameterDeclaration");

    }

    @Override
    public void exitParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        printWithCounter("exitParameterDeclaration");

    }

    @Override
    public void enterFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        printWithCounter("enterFunctionDefinition " + ctx + " " + ctx.declarator().getText());

    }

    @Override
    public void exitFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        printWithCounter("exitFunctionDefinition");

    }

    @Override
    public void enterFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        printWithCounter("enterFunctionBody");

    }

    @Override
    public void exitFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        printWithCounter("exitFunctionBody");

    }

    @Override
    public void enterInitializer(CPP14Parser.InitializerContext ctx) {
        printWithCounter("enterInitializer");

    }

    @Override
    public void exitInitializer(CPP14Parser.InitializerContext ctx) {
        printWithCounter("exitInitializer");

    }

    @Override
    public void enterBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        printWithCounter("enterBraceOrEqualInitializer");

    }

    @Override
    public void exitBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        printWithCounter("exitBraceOrEqualInitializer");

    }

    @Override
    public void enterInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        printWithCounter("enterInitializerClause");

    }

    @Override
    public void exitInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        printWithCounter("exitInitializerClause");

    }

    @Override
    public void enterInitializerList(CPP14Parser.InitializerListContext ctx) {
        printWithCounter("enterInitializerList");

    }

    @Override
    public void exitInitializerList(CPP14Parser.InitializerListContext ctx) {
        printWithCounter("exitInitializerList");

    }

    @Override
    public void enterBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        printWithCounter("enterBracedInitList");

    }

    @Override
    public void exitBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        printWithCounter("exitBracedInitList");

    }

    @Override
    public void enterClassName(CPP14Parser.ClassNameContext ctx) {
        printWithCounter("enterClassName");

    }

    @Override
    public void exitClassName(CPP14Parser.ClassNameContext ctx) {
        printWithCounter("exitClassName");

    }

    @Override
    public void enterClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        printWithCounter("enterClassSpecifier");

    }

    @Override
    public void exitClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        printWithCounter("exitClassSpecifier");

    }

    @Override
    public void enterClassHead(CPP14Parser.ClassHeadContext ctx) {
        printWithCounter("enterClassHead");

    }

    @Override
    public void exitClassHead(CPP14Parser.ClassHeadContext ctx) {
        printWithCounter("exitClassHead");

    }

    @Override
    public void enterClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        printWithCounter("enterClassHeadName");

    }

    @Override
    public void exitClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        printWithCounter("exitClassHeadName");

    }

    @Override
    public void enterClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        printWithCounter("enterClassVirtSpecifier");

    }

    @Override
    public void exitClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        printWithCounter("exitClassVirtSpecifier");

    }

    @Override
    public void enterClassKey(CPP14Parser.ClassKeyContext ctx) {
        printWithCounter("enterClassKey");

    }

    @Override
    public void exitClassKey(CPP14Parser.ClassKeyContext ctx) {
        printWithCounter("exitClassKey");

    }

    @Override
    public void enterMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        printWithCounter("enterMemberSpecification");

    }

    @Override
    public void exitMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        printWithCounter("exitMemberSpecification");

    }

    @Override
    public void enterMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        printWithCounter("enterMemberdeclaration");

    }

    @Override
    public void exitMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        printWithCounter("exitMemberdeclaration");

    }

    @Override
    public void enterMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        printWithCounter("enterMemberDeclaratorList");

    }

    @Override
    public void exitMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        printWithCounter("exitMemberDeclaratorList");

    }

    @Override
    public void enterMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        printWithCounter("enterMemberDeclarator");

    }

    @Override
    public void exitMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        printWithCounter("exitMemberDeclarator");

    }

    @Override
    public void enterVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        printWithCounter("enterVirtualSpecifierSeq");

    }

    @Override
    public void exitVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        printWithCounter("exitVirtualSpecifierSeq");

    }

    @Override
    public void enterVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        printWithCounter("enterVirtualSpecifier");

    }

    @Override
    public void exitVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        printWithCounter("exitVirtualSpecifier");

    }

    @Override
    public void enterPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        printWithCounter("enterPureSpecifier");

    }

    @Override
    public void exitPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        printWithCounter("exitPureSpecifier");

    }

    @Override
    public void enterBaseClause(CPP14Parser.BaseClauseContext ctx) {
        printWithCounter("enterBaseClause");

    }

    @Override
    public void exitBaseClause(CPP14Parser.BaseClauseContext ctx) {
        printWithCounter("exitBaseClause");

    }

    @Override
    public void enterBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        printWithCounter("enterBaseSpecifierList");

    }

    @Override
    public void exitBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        printWithCounter("exitBaseSpecifierList");

    }

    @Override
    public void enterBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        printWithCounter("enterBaseSpecifier");

    }

    @Override
    public void exitBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        printWithCounter("exitBaseSpecifier");

    }

    @Override
    public void enterClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        printWithCounter("enterClassOrDeclType");

    }

    @Override
    public void exitClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        printWithCounter("exitClassOrDeclType");

    }

    @Override
    public void enterBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        printWithCounter("enterBaseTypeSpecifier");

    }

    @Override
    public void exitBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        printWithCounter("exitBaseTypeSpecifier");

    }

    @Override
    public void enterAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        printWithCounter("enterAccessSpecifier");

    }

    @Override
    public void exitAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        printWithCounter("exitAccessSpecifier");

    }

    @Override
    public void enterConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        printWithCounter("enterConversionFunctionId");

    }

    @Override
    public void exitConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        printWithCounter("exitConversionFunctionId");

    }

    @Override
    public void enterConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        printWithCounter("enterConversionTypeId");

    }

    @Override
    public void exitConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        printWithCounter("exitConversionTypeId");

    }

    @Override
    public void enterConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        printWithCounter("enterConversionDeclarator");

    }

    @Override
    public void exitConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        printWithCounter("exitConversionDeclarator");

    }

    @Override
    public void enterConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        printWithCounter("enterConstructorInitializer");

    }

    @Override
    public void exitConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        printWithCounter("exitConstructorInitializer");

    }

    @Override
    public void enterMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        printWithCounter("enterMemInitializerList");

    }

    @Override
    public void exitMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        printWithCounter("exitMemInitializerList");

    }

    @Override
    public void enterMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        printWithCounter("enterMemInitializer");

    }

    @Override
    public void exitMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        printWithCounter("exitMemInitializer");

    }

    @Override
    public void enterMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        printWithCounter("enterMeminitializerid");

    }

    @Override
    public void exitMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        printWithCounter("exitMeminitializerid");

    }

    @Override
    public void enterOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        printWithCounter("enterOperatorFunctionId");

    }

    @Override
    public void exitOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        printWithCounter("exitOperatorFunctionId");

    }

    @Override
    public void enterLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        printWithCounter("enterLiteralOperatorId");

    }

    @Override
    public void exitLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        printWithCounter("exitLiteralOperatorId");

    }

    @Override
    public void enterTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        printWithCounter("enterTemplateDeclaration");

    }

    @Override
    public void exitTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        printWithCounter("exitTemplateDeclaration");

    }

    @Override
    public void enterTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        printWithCounter("enterTemplateparameterList");

    }

    @Override
    public void exitTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        printWithCounter("exitTemplateparameterList");

    }

    @Override
    public void enterTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        printWithCounter("enterTemplateParameter");

    }

    @Override
    public void exitTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        printWithCounter("exitTemplateParameter");

    }

    @Override
    public void enterTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        printWithCounter("enterTypeParameter");

    }

    @Override
    public void exitTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        printWithCounter("exitTypeParameter");

    }

    @Override
    public void enterSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        printWithCounter("enterSimpleTemplateId");

    }

    @Override
    public void exitSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        printWithCounter("exitSimpleTemplateId");

    }

    @Override
    public void enterTemplateId(CPP14Parser.TemplateIdContext ctx) {
        printWithCounter("enterTemplateId");

    }

    @Override
    public void exitTemplateId(CPP14Parser.TemplateIdContext ctx) {
        printWithCounter("exitTemplateId");

    }

    @Override
    public void enterTemplateName(CPP14Parser.TemplateNameContext ctx) {
        printWithCounter("enterTemplateName");

    }

    @Override
    public void exitTemplateName(CPP14Parser.TemplateNameContext ctx) {
        printWithCounter("exitTemplateName");

    }

    @Override
    public void enterTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        printWithCounter("enterTemplateArgumentList");

    }

    @Override
    public void exitTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        printWithCounter("exitTemplateArgumentList");

    }

    @Override
    public void enterTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        printWithCounter("enterTemplateArgument");

    }

    @Override
    public void exitTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        printWithCounter("exitTemplateArgument");

    }

    @Override
    public void enterTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        printWithCounter("enterTypeNameSpecifier");

    }

    @Override
    public void exitTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        printWithCounter("exitTypeNameSpecifier");

    }

    @Override
    public void enterExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        printWithCounter("enterExplicitInstantiation");

    }

    @Override
    public void exitExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        printWithCounter("exitExplicitInstantiation");

    }

    @Override
    public void enterExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        printWithCounter("enterExplicitSpecialization");

    }

    @Override
    public void exitExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        printWithCounter("exitExplicitSpecialization");

    }

    @Override
    public void enterTryBlock(CPP14Parser.TryBlockContext ctx) {
        printWithCounter("enterTryBlock");

    }

    @Override
    public void exitTryBlock(CPP14Parser.TryBlockContext ctx) {
        printWithCounter("exitTryBlock");

    }

    @Override
    public void enterFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        printWithCounter("enterFunctionTryBlock");

    }

    @Override
    public void exitFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        printWithCounter("exitFunctionTryBlock");

    }

    @Override
    public void enterHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        printWithCounter("enterHandlerSeq");

    }

    @Override
    public void exitHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        printWithCounter("exitHandlerSeq");

    }

    @Override
    public void enterHandler(CPP14Parser.HandlerContext ctx) {
        printWithCounter("enterHandler");

    }

    @Override
    public void exitHandler(CPP14Parser.HandlerContext ctx) {
        printWithCounter("exitHandler");

    }

    @Override
    public void enterExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        printWithCounter("enterExceptionDeclaration");

    }

    @Override
    public void exitExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        printWithCounter("exitExceptionDeclaration");

    }

    @Override
    public void enterThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        printWithCounter("enterThrowExpression");

    }

    @Override
    public void exitThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        printWithCounter("exitThrowExpression");

    }

    @Override
    public void enterExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        printWithCounter("enterExceptionSpecification");

    }

    @Override
    public void exitExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        printWithCounter("exitExceptionSpecification");

    }

    @Override
    public void enterDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        printWithCounter("enterDynamicExceptionSpecification");

    }

    @Override
    public void exitDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        printWithCounter("exitDynamicExceptionSpecification");

    }

    @Override
    public void enterTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        printWithCounter("enterTypeIdList");

    }

    @Override
    public void exitTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        printWithCounter("exitTypeIdList");

    }

    @Override
    public void enterNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        printWithCounter("enterNoeExceptSpecification");

    }

    @Override
    public void exitNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        printWithCounter("exitNoeExceptSpecification");

    }

    @Override
    public void enterTheOperator(CPP14Parser.TheOperatorContext ctx) {
        printWithCounter("enterTheOperator");

    }

    @Override
    public void exitTheOperator(CPP14Parser.TheOperatorContext ctx) {
        printWithCounter("exitTheOperator");

    }

    @Override
    public void enterLiteral(CPP14Parser.LiteralContext ctx) {
        printWithCounter("enterLiteral");

    }

    @Override
    public void exitLiteral(CPP14Parser.LiteralContext ctx) {
        printWithCounter("exitLiteral");

    }

    @Override
    public void visitTerminal(TerminalNode terminalNode) {
        printWithCounter("visitTerminal " + terminalNode.getSymbol().getText());

    }

    @Override
    public void visitErrorNode(ErrorNode errorNode) {
        printWithCounter("visitErrorNode");

    }
}
