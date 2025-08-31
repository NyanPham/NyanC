
#include "SemanticAnalyzer.h"
#include "IdentifierResolution.h"
#include "ValidateLabels.h"
#include "LoopLabeling.h"
#include "CollectSwitchCases.h"
#include "TypeChecker.h"

SemanticAnalyzer::SemanticAnalyzer(Symbols::SymbolTable &symbolTable, TypeTableNS::TypeTable &typeTable)
    : _symbolTable(symbolTable), _typeTable(typeTable) {}

std::shared_ptr<AST::Program> SemanticAnalyzer::analyze(std::shared_ptr<AST::Program> ast)
{
    auto idResolver = IdentifierResolution();
    auto transformedAst = idResolver.resolve(ast);

    auto validateLabels = ValidateLabels();
    auto validatedAst = validateLabels.validateLabels(transformedAst);

    auto loopLabeler = LoopLabeling();
    auto labeledAst = loopLabeler.labelLoops(validatedAst);

    auto typeChecker = TypeChecker(_symbolTable, _typeTable);
    auto typeCheckedAst = typeChecker.typeCheck(labeledAst);

    auto switchCasesCollector = CollectSwitchCases();
    auto finalAst = switchCasesCollector.analyzeSwitches(typeCheckedAst);

    return finalAst;
}

Symbols::SymbolTable &SemanticAnalyzer::getSymbolTable()
{
    return _symbolTable;
}

TypeTableNS::TypeTable &SemanticAnalyzer::getTypeTable()
{
    return _typeTable;
}
