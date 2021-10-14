package com.rusefi.ui.livedata;

import com.rusefi.livedata.generated.CPP14Parser;
import com.rusefi.livedata.generated.CPP14ParserListener;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ErrorNode;
import org.antlr.v4.runtime.tree.TerminalNode;

import java.util.Arrays;

public class PrintCPP14ParserListener implements CPP14ParserListener {
    @Override
    public void enterEveryRule(ParserRuleContext parserRuleContext) {
//        System.out.println("enterEveryRule");
    }

    @Override
    public void exitEveryRule(ParserRuleContext parserRuleContext) {
//        System.out.println("exitEveryRule");
    }

    @Override
    public void exitTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        System.out.println("exitTranslationUnit");

    }

    @Override
    public void enterTranslationUnit(CPP14Parser.TranslationUnitContext ctx) {
        System.out.println("enterTranslationUnit");

    }

    @Override
    public void enterPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        System.out.println("enterPrimaryExpression");

    }

    @Override
    public void exitPrimaryExpression(CPP14Parser.PrimaryExpressionContext ctx) {
        System.out.println("exitPrimaryExpression");

    }

    @Override
    public void enterIdExpression(CPP14Parser.IdExpressionContext ctx) {
        System.out.println("enterIdExpression");

    }

    @Override
    public void exitIdExpression(CPP14Parser.IdExpressionContext ctx) {
        System.out.println("exitIdExpression");

    }

    @Override
    public void enterUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        System.out.println("enterUnqualifiedId");

    }

    @Override
    public void exitUnqualifiedId(CPP14Parser.UnqualifiedIdContext ctx) {
        System.out.println("exitUnqualifiedId");

    }

    @Override
    public void enterQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        System.out.println("enterQualifiedId");

    }

    @Override
    public void exitQualifiedId(CPP14Parser.QualifiedIdContext ctx) {
        System.out.println("exitQualifiedId");

    }

    @Override
    public void enterNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        System.out.println("enterNestedNameSpecifier");

    }

    @Override
    public void exitNestedNameSpecifier(CPP14Parser.NestedNameSpecifierContext ctx) {
        System.out.println("exitNestedNameSpecifier");

    }

    @Override
    public void enterLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        System.out.println("enterLambdaExpression");

    }

    @Override
    public void exitLambdaExpression(CPP14Parser.LambdaExpressionContext ctx) {
        System.out.println("exitLambdaExpression");

    }

    @Override
    public void enterLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        System.out.println("enterLambdaIntroducer");

    }

    @Override
    public void exitLambdaIntroducer(CPP14Parser.LambdaIntroducerContext ctx) {
        System.out.println("exitLambdaIntroducer");

    }

    @Override
    public void enterLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        System.out.println("enterLambdaCapture");

    }

    @Override
    public void exitLambdaCapture(CPP14Parser.LambdaCaptureContext ctx) {
        System.out.println("exitLambdaCapture");

    }

    @Override
    public void enterCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        System.out.println("enterCaptureDefault");

    }

    @Override
    public void exitCaptureDefault(CPP14Parser.CaptureDefaultContext ctx) {
        System.out.println("exitCaptureDefault");

    }

    @Override
    public void enterCaptureList(CPP14Parser.CaptureListContext ctx) {
        System.out.println("enterCaptureList");

    }

    @Override
    public void exitCaptureList(CPP14Parser.CaptureListContext ctx) {
        System.out.println("exitCaptureList");

    }

    @Override
    public void enterCapture(CPP14Parser.CaptureContext ctx) {
        System.out.println("enterCapture");

    }

    @Override
    public void exitCapture(CPP14Parser.CaptureContext ctx) {
        System.out.println("exitCapture");

    }

    @Override
    public void enterSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        System.out.println("enterSimpleCapture");

    }

    @Override
    public void exitSimpleCapture(CPP14Parser.SimpleCaptureContext ctx) {
        System.out.println("exitSimpleCapture");

    }

    @Override
    public void enterInitcapture(CPP14Parser.InitcaptureContext ctx) {
        System.out.println("enterInitcapture");

    }

    @Override
    public void exitInitcapture(CPP14Parser.InitcaptureContext ctx) {
        System.out.println("exitInitcapture");

    }

    @Override
    public void enterLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        System.out.println("enterLambdaDeclarator");

    }

    @Override
    public void exitLambdaDeclarator(CPP14Parser.LambdaDeclaratorContext ctx) {
        System.out.println("exitLambdaDeclarator");

    }

    @Override
    public void enterPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        System.out.println("enterPostfixExpression");

    }

    @Override
    public void exitPostfixExpression(CPP14Parser.PostfixExpressionContext ctx) {
        System.out.println("exitPostfixExpression");

    }

    @Override
    public void enterTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        System.out.println("enterTypeIdOfTheTypeId");

    }

    @Override
    public void exitTypeIdOfTheTypeId(CPP14Parser.TypeIdOfTheTypeIdContext ctx) {
        System.out.println("exitTypeIdOfTheTypeId");

    }

    @Override
    public void enterExpressionList(CPP14Parser.ExpressionListContext ctx) {
        System.out.println("enterExpressionList");

    }

    @Override
    public void exitExpressionList(CPP14Parser.ExpressionListContext ctx) {
        System.out.println("exitExpressionList");

    }

    @Override
    public void enterPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        System.out.println("enterPseudoDestructorName");

    }

    @Override
    public void exitPseudoDestructorName(CPP14Parser.PseudoDestructorNameContext ctx) {
        System.out.println("exitPseudoDestructorName");

    }

    @Override
    public void enterUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        System.out.println("enterUnaryExpression");

    }

    @Override
    public void exitUnaryExpression(CPP14Parser.UnaryExpressionContext ctx) {
        System.out.println("exitUnaryExpression");

    }

    @Override
    public void enterUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        System.out.println("enterUnaryOperator");

    }

    @Override
    public void exitUnaryOperator(CPP14Parser.UnaryOperatorContext ctx) {
        System.out.println("exitUnaryOperator");

    }

    @Override
    public void enterNewExpression(CPP14Parser.NewExpressionContext ctx) {
        System.out.println("enterNewExpression");

    }

    @Override
    public void exitNewExpression(CPP14Parser.NewExpressionContext ctx) {
        System.out.println("exitNewExpression");

    }

    @Override
    public void enterNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        System.out.println("enterNewPlacement");

    }

    @Override
    public void exitNewPlacement(CPP14Parser.NewPlacementContext ctx) {
        System.out.println("exitNewPlacement");

    }

    @Override
    public void enterNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        System.out.println("enterNewTypeId");

    }

    @Override
    public void exitNewTypeId(CPP14Parser.NewTypeIdContext ctx) {
        System.out.println("exitNewTypeId");

    }

    @Override
    public void enterNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        System.out.println("enterNewDeclarator");

    }

    @Override
    public void exitNewDeclarator(CPP14Parser.NewDeclaratorContext ctx) {
        System.out.println("exitNewDeclarator");

    }

    @Override
    public void enterNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        System.out.println("enterNoPointerNewDeclarator");

    }

    @Override
    public void exitNoPointerNewDeclarator(CPP14Parser.NoPointerNewDeclaratorContext ctx) {
        System.out.println("exitNoPointerNewDeclarator");

    }

    @Override
    public void enterNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        System.out.println("enterNewInitializer");

    }

    @Override
    public void exitNewInitializer(CPP14Parser.NewInitializerContext ctx) {
        System.out.println("exitNewInitializer");

    }

    @Override
    public void enterDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        System.out.println("enterDeleteExpression");

    }

    @Override
    public void exitDeleteExpression(CPP14Parser.DeleteExpressionContext ctx) {
        System.out.println("exitDeleteExpression");

    }

    @Override
    public void enterNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        System.out.println("enterNoExceptExpression");

    }

    @Override
    public void exitNoExceptExpression(CPP14Parser.NoExceptExpressionContext ctx) {
        System.out.println("exitNoExceptExpression");

    }

    @Override
    public void enterCastExpression(CPP14Parser.CastExpressionContext ctx) {
        System.out.println("enterCastExpression");

    }

    @Override
    public void exitCastExpression(CPP14Parser.CastExpressionContext ctx) {
        System.out.println("exitCastExpression");

    }

    @Override
    public void enterPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        System.out.println("enterPointerMemberExpression");

    }

    @Override
    public void exitPointerMemberExpression(CPP14Parser.PointerMemberExpressionContext ctx) {
        System.out.println("exitPointerMemberExpression");

    }

    @Override
    public void enterMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        System.out.println("enterMultiplicativeExpression");

    }

    @Override
    public void exitMultiplicativeExpression(CPP14Parser.MultiplicativeExpressionContext ctx) {
        System.out.println("exitMultiplicativeExpression");

    }

    @Override
    public void enterAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        System.out.println("enterAdditiveExpression");

    }

    @Override
    public void exitAdditiveExpression(CPP14Parser.AdditiveExpressionContext ctx) {
        System.out.println("exitAdditiveExpression");

    }

    @Override
    public void enterShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        System.out.println("enterShiftExpression");

    }

    @Override
    public void exitShiftExpression(CPP14Parser.ShiftExpressionContext ctx) {
        System.out.println("exitShiftExpression");

    }

    @Override
    public void enterShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        System.out.println("enterShiftOperator");

    }

    @Override
    public void exitShiftOperator(CPP14Parser.ShiftOperatorContext ctx) {
        System.out.println("exitShiftOperator");

    }

    @Override
    public void enterRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        System.out.println("enterRelationalExpression");

    }

    @Override
    public void exitRelationalExpression(CPP14Parser.RelationalExpressionContext ctx) {
        System.out.println("exitRelationalExpression");

    }

    @Override
    public void enterEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        System.out.println("enterEqualityExpression");

    }

    @Override
    public void exitEqualityExpression(CPP14Parser.EqualityExpressionContext ctx) {
        System.out.println("exitEqualityExpression");

    }

    @Override
    public void enterAndExpression(CPP14Parser.AndExpressionContext ctx) {
        System.out.println("enterAndExpression");

    }

    @Override
    public void exitAndExpression(CPP14Parser.AndExpressionContext ctx) {
        System.out.println("exitAndExpression");

    }

    @Override
    public void enterExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        System.out.println("enterExclusiveOrExpression");

    }

    @Override
    public void exitExclusiveOrExpression(CPP14Parser.ExclusiveOrExpressionContext ctx) {
        System.out.println("exitExclusiveOrExpression");

    }

    @Override
    public void enterInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        System.out.println("enterInclusiveOrExpression");

    }

    @Override
    public void exitInclusiveOrExpression(CPP14Parser.InclusiveOrExpressionContext ctx) {
        System.out.println("exitInclusiveOrExpression");

    }

    @Override
    public void enterLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        System.out.println("enterLogicalAndExpression");

    }

    @Override
    public void exitLogicalAndExpression(CPP14Parser.LogicalAndExpressionContext ctx) {
        System.out.println("exitLogicalAndExpression");

    }

    @Override
    public void enterLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        System.out.println("enterLogicalOrExpression");

    }

    @Override
    public void exitLogicalOrExpression(CPP14Parser.LogicalOrExpressionContext ctx) {
        System.out.println("exitLogicalOrExpression");

    }

    @Override
    public void enterConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        System.out.println("enterConditionalExpression");

    }

    @Override
    public void exitConditionalExpression(CPP14Parser.ConditionalExpressionContext ctx) {
        System.out.println("exitConditionalExpression");

    }

    @Override
    public void enterAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        System.out.println("enterAssignmentExpression");

    }

    @Override
    public void exitAssignmentExpression(CPP14Parser.AssignmentExpressionContext ctx) {
        System.out.println("exitAssignmentExpression");

    }

    @Override
    public void enterAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        System.out.println("enterAssignmentOperator");

    }

    @Override
    public void exitAssignmentOperator(CPP14Parser.AssignmentOperatorContext ctx) {
        System.out.println("exitAssignmentOperator");

    }

    @Override
    public void enterExpression(CPP14Parser.ExpressionContext ctx) {
        System.out.println("enterExpression");

    }

    @Override
    public void exitExpression(CPP14Parser.ExpressionContext ctx) {
        System.out.println("exitExpression");

    }

    @Override
    public void enterConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        System.out.println("enterConstantExpression");

    }

    @Override
    public void exitConstantExpression(CPP14Parser.ConstantExpressionContext ctx) {
        System.out.println("exitConstantExpression");

    }

    @Override
    public void enterStatement(CPP14Parser.StatementContext ctx) {
        System.out.println("enterStatement " + ctx.getText());

    }

    @Override
    public void exitStatement(CPP14Parser.StatementContext ctx) {
        System.out.println("exitStatement");

    }

    @Override
    public void enterLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        System.out.println("enterLabeledStatement");

    }

    @Override
    public void exitLabeledStatement(CPP14Parser.LabeledStatementContext ctx) {
        System.out.println("exitLabeledStatement");

    }

    @Override
    public void enterExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        System.out.println("enterExpressionStatement");

    }

    @Override
    public void exitExpressionStatement(CPP14Parser.ExpressionStatementContext ctx) {
        System.out.println("exitExpressionStatement");

    }

    @Override
    public void enterCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        System.out.println("enterCompoundStatement " + ctx.statementSeq().getText());

    }

    @Override
    public void exitCompoundStatement(CPP14Parser.CompoundStatementContext ctx) {
        System.out.println("exitCompoundStatement");

    }

    @Override
    public void enterStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        System.out.println("enterStatementSeq");

    }

    @Override
    public void exitStatementSeq(CPP14Parser.StatementSeqContext ctx) {
        System.out.println("exitStatementSeq");

    }

    @Override
    public void enterSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
        System.out.println("enterSelectionStatement " + ctx.getText() + " statements" + ctx.statement());

    }

    @Override
    public void exitSelectionStatement(CPP14Parser.SelectionStatementContext ctx) {
        System.out.println("exitSelectionStatement");

    }

    @Override
    public void enterCondition(CPP14Parser.ConditionContext ctx) {
        System.out.println("enterCondition " + ctx.expression().getText());

    }

    @Override
    public void exitCondition(CPP14Parser.ConditionContext ctx) {
        System.out.println("exitCondition");

    }

    @Override
    public void enterIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        System.out.println("enterIterationStatement");

    }

    @Override
    public void exitIterationStatement(CPP14Parser.IterationStatementContext ctx) {
        System.out.println("exitIterationStatement");

    }

    @Override
    public void enterForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        System.out.println("enterForInitStatement");

    }

    @Override
    public void exitForInitStatement(CPP14Parser.ForInitStatementContext ctx) {
        System.out.println("exitForInitStatement");

    }

    @Override
    public void enterForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        System.out.println("enterForRangeDeclaration");

    }

    @Override
    public void exitForRangeDeclaration(CPP14Parser.ForRangeDeclarationContext ctx) {
        System.out.println("exitForRangeDeclaration");

    }

    @Override
    public void enterForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        System.out.println("enterForRangeInitializer");

    }

    @Override
    public void exitForRangeInitializer(CPP14Parser.ForRangeInitializerContext ctx) {
        System.out.println("exitForRangeInitializer");

    }

    @Override
    public void enterJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        System.out.println("enterJumpStatement " + ctx.getText());

    }

    @Override
    public void exitJumpStatement(CPP14Parser.JumpStatementContext ctx) {
        System.out.println("exitJumpStatement");

    }

    @Override
    public void enterDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        System.out.println("enterDeclarationStatement");

    }

    @Override
    public void exitDeclarationStatement(CPP14Parser.DeclarationStatementContext ctx) {
        System.out.println("exitDeclarationStatement");

    }

    @Override
    public void enterDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        System.out.println("enterDeclarationseq " + ctx);

    }

    @Override
    public void exitDeclarationseq(CPP14Parser.DeclarationseqContext ctx) {
        System.out.println("exitDeclarationseq");

    }

    @Override
    public void enterDeclaration(CPP14Parser.DeclarationContext ctx) {
        System.out.println("enterDeclaration");

    }

    @Override
    public void exitDeclaration(CPP14Parser.DeclarationContext ctx) {
        System.out.println("exitDeclaration");

    }

    @Override
    public void enterBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        System.out.println("enterBlockDeclaration");

    }

    @Override
    public void exitBlockDeclaration(CPP14Parser.BlockDeclarationContext ctx) {
        System.out.println("exitBlockDeclaration");

    }

    @Override
    public void enterAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        System.out.println("enterAliasDeclaration");

    }

    @Override
    public void exitAliasDeclaration(CPP14Parser.AliasDeclarationContext ctx) {
        System.out.println("exitAliasDeclaration");

    }

    @Override
    public void enterSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        System.out.println("enterSimpleDeclaration");

    }

    @Override
    public void exitSimpleDeclaration(CPP14Parser.SimpleDeclarationContext ctx) {
        System.out.println("exitSimpleDeclaration");

    }

    @Override
    public void enterStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        System.out.println("enterStaticAssertDeclaration");

    }

    @Override
    public void exitStaticAssertDeclaration(CPP14Parser.StaticAssertDeclarationContext ctx) {
        System.out.println("exitStaticAssertDeclaration");

    }

    @Override
    public void enterEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        System.out.println("enterEmptyDeclaration");

    }

    @Override
    public void exitEmptyDeclaration(CPP14Parser.EmptyDeclarationContext ctx) {
        System.out.println("exitEmptyDeclaration");

    }

    @Override
    public void enterAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        System.out.println("enterAttributeDeclaration");

    }

    @Override
    public void exitAttributeDeclaration(CPP14Parser.AttributeDeclarationContext ctx) {
        System.out.println("exitAttributeDeclaration");

    }

    @Override
    public void enterDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        System.out.println("enterDeclSpecifier");

    }

    @Override
    public void exitDeclSpecifier(CPP14Parser.DeclSpecifierContext ctx) {
        System.out.println("exitDeclSpecifier");

    }

    @Override
    public void enterDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        System.out.println("enterDeclSpecifierSeq");

    }

    @Override
    public void exitDeclSpecifierSeq(CPP14Parser.DeclSpecifierSeqContext ctx) {
        System.out.println("exitDeclSpecifierSeq");

    }

    @Override
    public void enterStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        System.out.println("enterStorageClassSpecifier");

    }

    @Override
    public void exitStorageClassSpecifier(CPP14Parser.StorageClassSpecifierContext ctx) {
        System.out.println("exitStorageClassSpecifier");

    }

    @Override
    public void enterFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        System.out.println("enterFunctionSpecifier");

    }

    @Override
    public void exitFunctionSpecifier(CPP14Parser.FunctionSpecifierContext ctx) {
        System.out.println("exitFunctionSpecifier");

    }

    @Override
    public void enterTypedefName(CPP14Parser.TypedefNameContext ctx) {
        System.out.println("enterTypedefName");

    }

    @Override
    public void exitTypedefName(CPP14Parser.TypedefNameContext ctx) {
        System.out.println("exitTypedefName");

    }

    @Override
    public void enterTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        System.out.println("enterTypeSpecifier");

    }

    @Override
    public void exitTypeSpecifier(CPP14Parser.TypeSpecifierContext ctx) {
        System.out.println("exitTypeSpecifier");

    }

    @Override
    public void enterTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        System.out.println("enterTrailingTypeSpecifier");

    }

    @Override
    public void exitTrailingTypeSpecifier(CPP14Parser.TrailingTypeSpecifierContext ctx) {
        System.out.println("exitTrailingTypeSpecifier");

    }

    @Override
    public void enterTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        System.out.println("enterTypeSpecifierSeq");

    }

    @Override
    public void exitTypeSpecifierSeq(CPP14Parser.TypeSpecifierSeqContext ctx) {
        System.out.println("exitTypeSpecifierSeq");

    }

    @Override
    public void enterTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        System.out.println("enterTrailingTypeSpecifierSeq");

    }

    @Override
    public void exitTrailingTypeSpecifierSeq(CPP14Parser.TrailingTypeSpecifierSeqContext ctx) {
        System.out.println("exitTrailingTypeSpecifierSeq");

    }

    @Override
    public void enterSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        System.out.println("enterSimpleTypeLengthModifier");

    }

    @Override
    public void exitSimpleTypeLengthModifier(CPP14Parser.SimpleTypeLengthModifierContext ctx) {
        System.out.println("exitSimpleTypeLengthModifier");

    }

    @Override
    public void enterSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        System.out.println("enterSimpleTypeSignednessModifier");

    }

    @Override
    public void exitSimpleTypeSignednessModifier(CPP14Parser.SimpleTypeSignednessModifierContext ctx) {
        System.out.println("exitSimpleTypeSignednessModifier");

    }

    @Override
    public void enterSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        System.out.println("enterSimpleTypeSpecifier");

    }

    @Override
    public void exitSimpleTypeSpecifier(CPP14Parser.SimpleTypeSpecifierContext ctx) {
        System.out.println("exitSimpleTypeSpecifier");

    }

    @Override
    public void enterTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        System.out.println("enterTheTypeName");

    }

    @Override
    public void exitTheTypeName(CPP14Parser.TheTypeNameContext ctx) {
        System.out.println("exitTheTypeName");

    }

    @Override
    public void enterDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        System.out.println("enterDecltypeSpecifier");

    }

    @Override
    public void exitDecltypeSpecifier(CPP14Parser.DecltypeSpecifierContext ctx) {
        System.out.println("exitDecltypeSpecifier");

    }

    @Override
    public void enterElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        System.out.println("enterElaboratedTypeSpecifier");

    }

    @Override
    public void exitElaboratedTypeSpecifier(CPP14Parser.ElaboratedTypeSpecifierContext ctx) {
        System.out.println("exitElaboratedTypeSpecifier");

    }

    @Override
    public void enterEnumName(CPP14Parser.EnumNameContext ctx) {
        System.out.println("enterEnumName");

    }

    @Override
    public void exitEnumName(CPP14Parser.EnumNameContext ctx) {
        System.out.println("exitEnumName");

    }

    @Override
    public void enterEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        System.out.println("enterEnumSpecifier");

    }

    @Override
    public void exitEnumSpecifier(CPP14Parser.EnumSpecifierContext ctx) {
        System.out.println("exitEnumSpecifier");

    }

    @Override
    public void enterEnumHead(CPP14Parser.EnumHeadContext ctx) {
        System.out.println("enterEnumHead");

    }

    @Override
    public void exitEnumHead(CPP14Parser.EnumHeadContext ctx) {
        System.out.println("exitEnumHead");

    }

    @Override
    public void enterOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        System.out.println("enterOpaqueEnumDeclaration");

    }

    @Override
    public void exitOpaqueEnumDeclaration(CPP14Parser.OpaqueEnumDeclarationContext ctx) {
        System.out.println("exitOpaqueEnumDeclaration");

    }

    @Override
    public void enterEnumkey(CPP14Parser.EnumkeyContext ctx) {
        System.out.println("enterEnumkey");

    }

    @Override
    public void exitEnumkey(CPP14Parser.EnumkeyContext ctx) {
        System.out.println("exitEnumkey");

    }

    @Override
    public void enterEnumbase(CPP14Parser.EnumbaseContext ctx) {
        System.out.println("enterEnumbase");

    }

    @Override
    public void exitEnumbase(CPP14Parser.EnumbaseContext ctx) {
        System.out.println("exitEnumbase");

    }

    @Override
    public void enterEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        System.out.println("enterEnumeratorList");

    }

    @Override
    public void exitEnumeratorList(CPP14Parser.EnumeratorListContext ctx) {
        System.out.println("exitEnumeratorList");

    }

    @Override
    public void enterEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        System.out.println("enterEnumeratorDefinition");

    }

    @Override
    public void exitEnumeratorDefinition(CPP14Parser.EnumeratorDefinitionContext ctx) {
        System.out.println("exitEnumeratorDefinition");

    }

    @Override
    public void enterEnumerator(CPP14Parser.EnumeratorContext ctx) {
        System.out.println("enterEnumerator");

    }

    @Override
    public void exitEnumerator(CPP14Parser.EnumeratorContext ctx) {
        System.out.println("exitEnumerator");

    }

    @Override
    public void enterNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        System.out.println("enterNamespaceName");

    }

    @Override
    public void exitNamespaceName(CPP14Parser.NamespaceNameContext ctx) {
        System.out.println("exitNamespaceName");

    }

    @Override
    public void enterOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        System.out.println("enterOriginalNamespaceName");

    }

    @Override
    public void exitOriginalNamespaceName(CPP14Parser.OriginalNamespaceNameContext ctx) {
        System.out.println("exitOriginalNamespaceName");

    }

    @Override
    public void enterNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        System.out.println("enterNamespaceDefinition");

    }

    @Override
    public void exitNamespaceDefinition(CPP14Parser.NamespaceDefinitionContext ctx) {
        System.out.println("exitNamespaceDefinition");

    }

    @Override
    public void enterNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        System.out.println("enterNamespaceAlias");

    }

    @Override
    public void exitNamespaceAlias(CPP14Parser.NamespaceAliasContext ctx) {
        System.out.println("exitNamespaceAlias");

    }

    @Override
    public void enterNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        System.out.println("enterNamespaceAliasDefinition");

    }

    @Override
    public void exitNamespaceAliasDefinition(CPP14Parser.NamespaceAliasDefinitionContext ctx) {
        System.out.println("exitNamespaceAliasDefinition");

    }

    @Override
    public void enterQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        System.out.println("enterQualifiednamespacespecifier");

    }

    @Override
    public void exitQualifiednamespacespecifier(CPP14Parser.QualifiednamespacespecifierContext ctx) {
        System.out.println("exitQualifiednamespacespecifier");

    }

    @Override
    public void enterUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        System.out.println("enterUsingDeclaration");

    }

    @Override
    public void exitUsingDeclaration(CPP14Parser.UsingDeclarationContext ctx) {
        System.out.println("exitUsingDeclaration");

    }

    @Override
    public void enterUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        System.out.println("enterUsingDirective");

    }

    @Override
    public void exitUsingDirective(CPP14Parser.UsingDirectiveContext ctx) {
        System.out.println("exitUsingDirective");

    }

    @Override
    public void enterAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        System.out.println("enterAsmDefinition");

    }

    @Override
    public void exitAsmDefinition(CPP14Parser.AsmDefinitionContext ctx) {
        System.out.println("exitAsmDefinition");

    }

    @Override
    public void enterLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        System.out.println("enterLinkageSpecification");

    }

    @Override
    public void exitLinkageSpecification(CPP14Parser.LinkageSpecificationContext ctx) {
        System.out.println("exitLinkageSpecification");

    }

    @Override
    public void enterAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        System.out.println("enterAttributeSpecifierSeq");

    }

    @Override
    public void exitAttributeSpecifierSeq(CPP14Parser.AttributeSpecifierSeqContext ctx) {
        System.out.println("exitAttributeSpecifierSeq");

    }

    @Override
    public void enterAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        System.out.println("enterAttributeSpecifier");

    }

    @Override
    public void exitAttributeSpecifier(CPP14Parser.AttributeSpecifierContext ctx) {
        System.out.println("exitAttributeSpecifier");

    }

    @Override
    public void enterAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        System.out.println("enterAlignmentspecifier");

    }

    @Override
    public void exitAlignmentspecifier(CPP14Parser.AlignmentspecifierContext ctx) {
        System.out.println("exitAlignmentspecifier");

    }

    @Override
    public void enterAttributeList(CPP14Parser.AttributeListContext ctx) {
        System.out.println("enterAttributeList");

    }

    @Override
    public void exitAttributeList(CPP14Parser.AttributeListContext ctx) {
        System.out.println("exitAttributeList");

    }

    @Override
    public void enterAttribute(CPP14Parser.AttributeContext ctx) {
        System.out.println("enterAttribute");

    }

    @Override
    public void exitAttribute(CPP14Parser.AttributeContext ctx) {
        System.out.println("exitAttribute");

    }

    @Override
    public void enterAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        System.out.println("enterAttributeNamespace");

    }

    @Override
    public void exitAttributeNamespace(CPP14Parser.AttributeNamespaceContext ctx) {
        System.out.println("exitAttributeNamespace");

    }

    @Override
    public void enterAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        System.out.println("enterAttributeArgumentClause");

    }

    @Override
    public void exitAttributeArgumentClause(CPP14Parser.AttributeArgumentClauseContext ctx) {
        System.out.println("exitAttributeArgumentClause");

    }

    @Override
    public void enterBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        System.out.println("enterBalancedTokenSeq");

    }

    @Override
    public void exitBalancedTokenSeq(CPP14Parser.BalancedTokenSeqContext ctx) {
        System.out.println("exitBalancedTokenSeq");

    }

    @Override
    public void enterBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        System.out.println("enterBalancedtoken");

    }

    @Override
    public void exitBalancedtoken(CPP14Parser.BalancedtokenContext ctx) {
        System.out.println("exitBalancedtoken");

    }

    @Override
    public void enterInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        System.out.println("enterInitDeclaratorList");

    }

    @Override
    public void exitInitDeclaratorList(CPP14Parser.InitDeclaratorListContext ctx) {
        System.out.println("exitInitDeclaratorList");

    }

    @Override
    public void enterInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        System.out.println("enterInitDeclarator");

    }

    @Override
    public void exitInitDeclarator(CPP14Parser.InitDeclaratorContext ctx) {
        System.out.println("exitInitDeclarator");

    }

    @Override
    public void enterDeclarator(CPP14Parser.DeclaratorContext ctx) {
        System.out.println("enterDeclarator " + ctx.pointerDeclarator().getText());

    }

    @Override
    public void exitDeclarator(CPP14Parser.DeclaratorContext ctx) {
        System.out.println("exitDeclarator");

    }

    @Override
    public void enterPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        System.out.println("enterPointerDeclarator");

    }

    @Override
    public void exitPointerDeclarator(CPP14Parser.PointerDeclaratorContext ctx) {
        System.out.println("exitPointerDeclarator");

    }

    @Override
    public void enterNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        System.out.println("enterNoPointerDeclarator");

    }

    @Override
    public void exitNoPointerDeclarator(CPP14Parser.NoPointerDeclaratorContext ctx) {
        System.out.println("exitNoPointerDeclarator");

    }

    @Override
    public void enterParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        System.out.println("enterParametersAndQualifiers");

    }

    @Override
    public void exitParametersAndQualifiers(CPP14Parser.ParametersAndQualifiersContext ctx) {
        System.out.println("exitParametersAndQualifiers");

    }

    @Override
    public void enterTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        System.out.println("enterTrailingReturnType");

    }

    @Override
    public void exitTrailingReturnType(CPP14Parser.TrailingReturnTypeContext ctx) {
        System.out.println("exitTrailingReturnType");

    }

    @Override
    public void enterPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        System.out.println("enterPointerOperator");

    }

    @Override
    public void exitPointerOperator(CPP14Parser.PointerOperatorContext ctx) {
        System.out.println("exitPointerOperator");

    }

    @Override
    public void enterCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        System.out.println("enterCvqualifierseq");

    }

    @Override
    public void exitCvqualifierseq(CPP14Parser.CvqualifierseqContext ctx) {
        System.out.println("exitCvqualifierseq");

    }

    @Override
    public void enterCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        System.out.println("enterCvQualifier");

    }

    @Override
    public void exitCvQualifier(CPP14Parser.CvQualifierContext ctx) {
        System.out.println("exitCvQualifier");

    }

    @Override
    public void enterRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        System.out.println("enterRefqualifier");

    }

    @Override
    public void exitRefqualifier(CPP14Parser.RefqualifierContext ctx) {
        System.out.println("exitRefqualifier");

    }

    @Override
    public void enterDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        System.out.println("enterDeclaratorid");

    }

    @Override
    public void exitDeclaratorid(CPP14Parser.DeclaratoridContext ctx) {
        System.out.println("exitDeclaratorid");

    }

    @Override
    public void enterTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        System.out.println("enterTheTypeId");

    }

    @Override
    public void exitTheTypeId(CPP14Parser.TheTypeIdContext ctx) {
        System.out.println("exitTheTypeId");

    }

    @Override
    public void enterAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        System.out.println("enterAbstractDeclarator");

    }

    @Override
    public void exitAbstractDeclarator(CPP14Parser.AbstractDeclaratorContext ctx) {
        System.out.println("exitAbstractDeclarator");

    }

    @Override
    public void enterPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        System.out.println("enterPointerAbstractDeclarator");

    }

    @Override
    public void exitPointerAbstractDeclarator(CPP14Parser.PointerAbstractDeclaratorContext ctx) {
        System.out.println("exitPointerAbstractDeclarator");

    }

    @Override
    public void enterNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        System.out.println("enterNoPointerAbstractDeclarator");

    }

    @Override
    public void exitNoPointerAbstractDeclarator(CPP14Parser.NoPointerAbstractDeclaratorContext ctx) {
        System.out.println("exitNoPointerAbstractDeclarator");

    }

    @Override
    public void enterAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        System.out.println("enterAbstractPackDeclarator");

    }

    @Override
    public void exitAbstractPackDeclarator(CPP14Parser.AbstractPackDeclaratorContext ctx) {
        System.out.println("exitAbstractPackDeclarator");

    }

    @Override
    public void enterNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        System.out.println("enterNoPointerAbstractPackDeclarator");

    }

    @Override
    public void exitNoPointerAbstractPackDeclarator(CPP14Parser.NoPointerAbstractPackDeclaratorContext ctx) {
        System.out.println("exitNoPointerAbstractPackDeclarator");

    }

    @Override
    public void enterParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        System.out.println("enterParameterDeclarationClause");

    }

    @Override
    public void exitParameterDeclarationClause(CPP14Parser.ParameterDeclarationClauseContext ctx) {
        System.out.println("exitParameterDeclarationClause");

    }

    @Override
    public void enterParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        System.out.println("enterParameterDeclarationList");

    }

    @Override
    public void exitParameterDeclarationList(CPP14Parser.ParameterDeclarationListContext ctx) {
        System.out.println("exitParameterDeclarationList");

    }

    @Override
    public void enterParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        System.out.println("enterParameterDeclaration");

    }

    @Override
    public void exitParameterDeclaration(CPP14Parser.ParameterDeclarationContext ctx) {
        System.out.println("exitParameterDeclaration");

    }

    @Override
    public void enterFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        System.out.println("enterFunctionDefinition " + ctx + " " + ctx.declarator().getText());

    }

    @Override
    public void exitFunctionDefinition(CPP14Parser.FunctionDefinitionContext ctx) {
        System.out.println("exitFunctionDefinition");

    }

    @Override
    public void enterFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        System.out.println("enterFunctionBody");

    }

    @Override
    public void exitFunctionBody(CPP14Parser.FunctionBodyContext ctx) {
        System.out.println("exitFunctionBody");

    }

    @Override
    public void enterInitializer(CPP14Parser.InitializerContext ctx) {
        System.out.println("enterInitializer");

    }

    @Override
    public void exitInitializer(CPP14Parser.InitializerContext ctx) {
        System.out.println("exitInitializer");

    }

    @Override
    public void enterBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        System.out.println("enterBraceOrEqualInitializer");

    }

    @Override
    public void exitBraceOrEqualInitializer(CPP14Parser.BraceOrEqualInitializerContext ctx) {
        System.out.println("exitBraceOrEqualInitializer");

    }

    @Override
    public void enterInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        System.out.println("enterInitializerClause");

    }

    @Override
    public void exitInitializerClause(CPP14Parser.InitializerClauseContext ctx) {
        System.out.println("exitInitializerClause");

    }

    @Override
    public void enterInitializerList(CPP14Parser.InitializerListContext ctx) {
        System.out.println("enterInitializerList");

    }

    @Override
    public void exitInitializerList(CPP14Parser.InitializerListContext ctx) {
        System.out.println("exitInitializerList");

    }

    @Override
    public void enterBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        System.out.println("enterBracedInitList");

    }

    @Override
    public void exitBracedInitList(CPP14Parser.BracedInitListContext ctx) {
        System.out.println("exitBracedInitList");

    }

    @Override
    public void enterClassName(CPP14Parser.ClassNameContext ctx) {
        System.out.println("enterClassName");

    }

    @Override
    public void exitClassName(CPP14Parser.ClassNameContext ctx) {
        System.out.println("exitClassName");

    }

    @Override
    public void enterClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        System.out.println("enterClassSpecifier");

    }

    @Override
    public void exitClassSpecifier(CPP14Parser.ClassSpecifierContext ctx) {
        System.out.println("exitClassSpecifier");

    }

    @Override
    public void enterClassHead(CPP14Parser.ClassHeadContext ctx) {
        System.out.println("enterClassHead");

    }

    @Override
    public void exitClassHead(CPP14Parser.ClassHeadContext ctx) {
        System.out.println("exitClassHead");

    }

    @Override
    public void enterClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        System.out.println("enterClassHeadName");

    }

    @Override
    public void exitClassHeadName(CPP14Parser.ClassHeadNameContext ctx) {
        System.out.println("exitClassHeadName");

    }

    @Override
    public void enterClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        System.out.println("enterClassVirtSpecifier");

    }

    @Override
    public void exitClassVirtSpecifier(CPP14Parser.ClassVirtSpecifierContext ctx) {
        System.out.println("exitClassVirtSpecifier");

    }

    @Override
    public void enterClassKey(CPP14Parser.ClassKeyContext ctx) {
        System.out.println("enterClassKey");

    }

    @Override
    public void exitClassKey(CPP14Parser.ClassKeyContext ctx) {
        System.out.println("exitClassKey");

    }

    @Override
    public void enterMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        System.out.println("enterMemberSpecification");

    }

    @Override
    public void exitMemberSpecification(CPP14Parser.MemberSpecificationContext ctx) {
        System.out.println("exitMemberSpecification");

    }

    @Override
    public void enterMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        System.out.println("enterMemberdeclaration");

    }

    @Override
    public void exitMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
        System.out.println("exitMemberdeclaration");

    }

    @Override
    public void enterMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        System.out.println("enterMemberDeclaratorList");

    }

    @Override
    public void exitMemberDeclaratorList(CPP14Parser.MemberDeclaratorListContext ctx) {
        System.out.println("exitMemberDeclaratorList");

    }

    @Override
    public void enterMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        System.out.println("enterMemberDeclarator");

    }

    @Override
    public void exitMemberDeclarator(CPP14Parser.MemberDeclaratorContext ctx) {
        System.out.println("exitMemberDeclarator");

    }

    @Override
    public void enterVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        System.out.println("enterVirtualSpecifierSeq");

    }

    @Override
    public void exitVirtualSpecifierSeq(CPP14Parser.VirtualSpecifierSeqContext ctx) {
        System.out.println("exitVirtualSpecifierSeq");

    }

    @Override
    public void enterVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        System.out.println("enterVirtualSpecifier");

    }

    @Override
    public void exitVirtualSpecifier(CPP14Parser.VirtualSpecifierContext ctx) {
        System.out.println("exitVirtualSpecifier");

    }

    @Override
    public void enterPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        System.out.println("enterPureSpecifier");

    }

    @Override
    public void exitPureSpecifier(CPP14Parser.PureSpecifierContext ctx) {
        System.out.println("exitPureSpecifier");

    }

    @Override
    public void enterBaseClause(CPP14Parser.BaseClauseContext ctx) {
        System.out.println("enterBaseClause");

    }

    @Override
    public void exitBaseClause(CPP14Parser.BaseClauseContext ctx) {
        System.out.println("exitBaseClause");

    }

    @Override
    public void enterBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        System.out.println("enterBaseSpecifierList");

    }

    @Override
    public void exitBaseSpecifierList(CPP14Parser.BaseSpecifierListContext ctx) {
        System.out.println("exitBaseSpecifierList");

    }

    @Override
    public void enterBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        System.out.println("enterBaseSpecifier");

    }

    @Override
    public void exitBaseSpecifier(CPP14Parser.BaseSpecifierContext ctx) {
        System.out.println("exitBaseSpecifier");

    }

    @Override
    public void enterClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        System.out.println("enterClassOrDeclType");

    }

    @Override
    public void exitClassOrDeclType(CPP14Parser.ClassOrDeclTypeContext ctx) {
        System.out.println("exitClassOrDeclType");

    }

    @Override
    public void enterBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        System.out.println("enterBaseTypeSpecifier");

    }

    @Override
    public void exitBaseTypeSpecifier(CPP14Parser.BaseTypeSpecifierContext ctx) {
        System.out.println("exitBaseTypeSpecifier");

    }

    @Override
    public void enterAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        System.out.println("enterAccessSpecifier");

    }

    @Override
    public void exitAccessSpecifier(CPP14Parser.AccessSpecifierContext ctx) {
        System.out.println("exitAccessSpecifier");

    }

    @Override
    public void enterConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        System.out.println("enterConversionFunctionId");

    }

    @Override
    public void exitConversionFunctionId(CPP14Parser.ConversionFunctionIdContext ctx) {
        System.out.println("exitConversionFunctionId");

    }

    @Override
    public void enterConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        System.out.println("enterConversionTypeId");

    }

    @Override
    public void exitConversionTypeId(CPP14Parser.ConversionTypeIdContext ctx) {
        System.out.println("exitConversionTypeId");

    }

    @Override
    public void enterConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        System.out.println("enterConversionDeclarator");

    }

    @Override
    public void exitConversionDeclarator(CPP14Parser.ConversionDeclaratorContext ctx) {
        System.out.println("exitConversionDeclarator");

    }

    @Override
    public void enterConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        System.out.println("enterConstructorInitializer");

    }

    @Override
    public void exitConstructorInitializer(CPP14Parser.ConstructorInitializerContext ctx) {
        System.out.println("exitConstructorInitializer");

    }

    @Override
    public void enterMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        System.out.println("enterMemInitializerList");

    }

    @Override
    public void exitMemInitializerList(CPP14Parser.MemInitializerListContext ctx) {
        System.out.println("exitMemInitializerList");

    }

    @Override
    public void enterMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        System.out.println("enterMemInitializer");

    }

    @Override
    public void exitMemInitializer(CPP14Parser.MemInitializerContext ctx) {
        System.out.println("exitMemInitializer");

    }

    @Override
    public void enterMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        System.out.println("enterMeminitializerid");

    }

    @Override
    public void exitMeminitializerid(CPP14Parser.MeminitializeridContext ctx) {
        System.out.println("exitMeminitializerid");

    }

    @Override
    public void enterOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        System.out.println("enterOperatorFunctionId");

    }

    @Override
    public void exitOperatorFunctionId(CPP14Parser.OperatorFunctionIdContext ctx) {
        System.out.println("exitOperatorFunctionId");

    }

    @Override
    public void enterLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        System.out.println("enterLiteralOperatorId");

    }

    @Override
    public void exitLiteralOperatorId(CPP14Parser.LiteralOperatorIdContext ctx) {
        System.out.println("exitLiteralOperatorId");

    }

    @Override
    public void enterTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        System.out.println("enterTemplateDeclaration");

    }

    @Override
    public void exitTemplateDeclaration(CPP14Parser.TemplateDeclarationContext ctx) {
        System.out.println("exitTemplateDeclaration");

    }

    @Override
    public void enterTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        System.out.println("enterTemplateparameterList");

    }

    @Override
    public void exitTemplateparameterList(CPP14Parser.TemplateparameterListContext ctx) {
        System.out.println("exitTemplateparameterList");

    }

    @Override
    public void enterTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        System.out.println("enterTemplateParameter");

    }

    @Override
    public void exitTemplateParameter(CPP14Parser.TemplateParameterContext ctx) {
        System.out.println("exitTemplateParameter");

    }

    @Override
    public void enterTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        System.out.println("enterTypeParameter");

    }

    @Override
    public void exitTypeParameter(CPP14Parser.TypeParameterContext ctx) {
        System.out.println("exitTypeParameter");

    }

    @Override
    public void enterSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        System.out.println("enterSimpleTemplateId");

    }

    @Override
    public void exitSimpleTemplateId(CPP14Parser.SimpleTemplateIdContext ctx) {
        System.out.println("exitSimpleTemplateId");

    }

    @Override
    public void enterTemplateId(CPP14Parser.TemplateIdContext ctx) {
        System.out.println("enterTemplateId");

    }

    @Override
    public void exitTemplateId(CPP14Parser.TemplateIdContext ctx) {
        System.out.println("exitTemplateId");

    }

    @Override
    public void enterTemplateName(CPP14Parser.TemplateNameContext ctx) {
        System.out.println("enterTemplateName");

    }

    @Override
    public void exitTemplateName(CPP14Parser.TemplateNameContext ctx) {
        System.out.println("exitTemplateName");

    }

    @Override
    public void enterTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        System.out.println("enterTemplateArgumentList");

    }

    @Override
    public void exitTemplateArgumentList(CPP14Parser.TemplateArgumentListContext ctx) {
        System.out.println("exitTemplateArgumentList");

    }

    @Override
    public void enterTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        System.out.println("enterTemplateArgument");

    }

    @Override
    public void exitTemplateArgument(CPP14Parser.TemplateArgumentContext ctx) {
        System.out.println("exitTemplateArgument");

    }

    @Override
    public void enterTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        System.out.println("enterTypeNameSpecifier");

    }

    @Override
    public void exitTypeNameSpecifier(CPP14Parser.TypeNameSpecifierContext ctx) {
        System.out.println("exitTypeNameSpecifier");

    }

    @Override
    public void enterExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        System.out.println("enterExplicitInstantiation");

    }

    @Override
    public void exitExplicitInstantiation(CPP14Parser.ExplicitInstantiationContext ctx) {
        System.out.println("exitExplicitInstantiation");

    }

    @Override
    public void enterExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        System.out.println("enterExplicitSpecialization");

    }

    @Override
    public void exitExplicitSpecialization(CPP14Parser.ExplicitSpecializationContext ctx) {
        System.out.println("exitExplicitSpecialization");

    }

    @Override
    public void enterTryBlock(CPP14Parser.TryBlockContext ctx) {
        System.out.println("enterTryBlock");

    }

    @Override
    public void exitTryBlock(CPP14Parser.TryBlockContext ctx) {
        System.out.println("exitTryBlock");

    }

    @Override
    public void enterFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        System.out.println("enterFunctionTryBlock");

    }

    @Override
    public void exitFunctionTryBlock(CPP14Parser.FunctionTryBlockContext ctx) {
        System.out.println("exitFunctionTryBlock");

    }

    @Override
    public void enterHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        System.out.println("enterHandlerSeq");

    }

    @Override
    public void exitHandlerSeq(CPP14Parser.HandlerSeqContext ctx) {
        System.out.println("exitHandlerSeq");

    }

    @Override
    public void enterHandler(CPP14Parser.HandlerContext ctx) {
        System.out.println("enterHandler");

    }

    @Override
    public void exitHandler(CPP14Parser.HandlerContext ctx) {
        System.out.println("exitHandler");

    }

    @Override
    public void enterExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        System.out.println("enterExceptionDeclaration");

    }

    @Override
    public void exitExceptionDeclaration(CPP14Parser.ExceptionDeclarationContext ctx) {
        System.out.println("exitExceptionDeclaration");

    }

    @Override
    public void enterThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        System.out.println("enterThrowExpression");

    }

    @Override
    public void exitThrowExpression(CPP14Parser.ThrowExpressionContext ctx) {
        System.out.println("exitThrowExpression");

    }

    @Override
    public void enterExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        System.out.println("enterExceptionSpecification");

    }

    @Override
    public void exitExceptionSpecification(CPP14Parser.ExceptionSpecificationContext ctx) {
        System.out.println("exitExceptionSpecification");

    }

    @Override
    public void enterDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        System.out.println("enterDynamicExceptionSpecification");

    }

    @Override
    public void exitDynamicExceptionSpecification(CPP14Parser.DynamicExceptionSpecificationContext ctx) {
        System.out.println("exitDynamicExceptionSpecification");

    }

    @Override
    public void enterTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        System.out.println("enterTypeIdList");

    }

    @Override
    public void exitTypeIdList(CPP14Parser.TypeIdListContext ctx) {
        System.out.println("exitTypeIdList");

    }

    @Override
    public void enterNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        System.out.println("enterNoeExceptSpecification");

    }

    @Override
    public void exitNoeExceptSpecification(CPP14Parser.NoeExceptSpecificationContext ctx) {
        System.out.println("exitNoeExceptSpecification");

    }

    @Override
    public void enterTheOperator(CPP14Parser.TheOperatorContext ctx) {
        System.out.println("enterTheOperator");

    }

    @Override
    public void exitTheOperator(CPP14Parser.TheOperatorContext ctx) {
        System.out.println("exitTheOperator");

    }

    @Override
    public void enterLiteral(CPP14Parser.LiteralContext ctx) {
        System.out.println("enterLiteral");

    }

    @Override
    public void exitLiteral(CPP14Parser.LiteralContext ctx) {
        System.out.println("exitLiteral");

    }

    @Override
    public void visitTerminal(TerminalNode terminalNode) {
        System.out.println("visitTerminal " + terminalNode.getSymbol().getText());

    }

    @Override
    public void visitErrorNode(ErrorNode errorNode) {
        System.out.println("visitErrorNode");

    }
}
