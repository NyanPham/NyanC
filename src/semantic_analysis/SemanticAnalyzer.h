
#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <memory>
#include "../AST.h"
#include "../Symbols.h"
#include "../TypeTable.h"

class SemanticAnalyzer
{
public:
    SemanticAnalyzer(Symbols::SymbolTable &symbolTable, TypeTableNS::TypeTable &typeTable);

    std::shared_ptr<AST::Program> analyze(std::shared_ptr<AST::Program> ast);

    Symbols::SymbolTable &getSymbolTable();
    TypeTableNS::TypeTable &getTypeTable();

private:
    Symbols::SymbolTable &_symbolTable;
    TypeTableNS::TypeTable &_typeTable;
};

#endif