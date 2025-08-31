
#pragma once

#include <string>
#include <memory>
#include "../Settings.h"
#include "../AST.h"
#include "../Symbols.h"
#include "../TypeTable.h"
#include "../Tacky.h"
#include "../Assembly.h"
#include "ASTPrettyPrint.h"
#include "SymbolTablePrint.h"
#include "TypeTablePrint.h"
#include "TackyPrettyPrint.h"
#include "CodeGenPrettyPrint.h"
#include "AssemblySymbolTablePrint.h"

class DebugPrinter
{
public:
    DebugPrinter(const Settings &settings);

    void printAST(const AST::Program &ast, const std::string &stage);
    void printSymbols(const Symbols::SymbolTable &symbolTable, const std::string &stage);
    void printTypes(const TypeTableNS::TypeTable &typeTable, const std::string &stage);
    void printTacky(const TACKY::Program &tacky, const std::string &stage);
    void printAssembly(const Assembly::Program &prog, const std::string &stage);
    void printAsmSymbols(const AssemblySymbols::AsmSymbolTable &asmSymbolTable, const std::string &stage);

private:
    bool _isDebug;
    ASTPrettyPrint _astPrinter;
    SymbolTablePrint _symbolTablePrinter;
    TypeTablePrint _typeTablePrinter;
    TackyPrettyPrint _tackyPrinter;
    CodeGenPrettyPrint _codeGenPrinter;
    AssemblySymbolTablePrint _asmSymbolTablePrinter;
};
