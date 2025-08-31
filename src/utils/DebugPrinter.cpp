#include "DebugPrinter.h"
#include <iostream>

DebugPrinter::DebugPrinter(const Settings &settings)
    : _isDebug(settings.getIsDebug()) {}

void DebugPrinter::printAST(const AST::Program &ast, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- AST: " << stage << " ---" << std::endl;
        _astPrinter.print(ast);
    }
}

void DebugPrinter::printSymbols(const Symbols::SymbolTable &symbolTable, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- Symbol Table: " << stage << " ---" << std::endl;
        _symbolTablePrinter.print(symbolTable);
    }
}

void DebugPrinter::printTypes(const TypeTableNS::TypeTable &typeTable, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- Type Table: " << stage << " ---" << std::endl;
        _typeTablePrinter.print(typeTable);
    }
}

void DebugPrinter::printTacky(const TACKY::Program &tacky, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- TACKY: " << stage << " ---" << std::endl;
        _tackyPrinter.print(tacky);
    }
}

void DebugPrinter::printAssembly(const Assembly::Program &prog, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- Assembly: " << stage << " ---" << std::endl;
        _codeGenPrinter.print(prog);
    }
}

void DebugPrinter::printAsmSymbols(const AssemblySymbols::AsmSymbolTable &asmSymbolTable, const std::string &stage)
{
    if (_isDebug)
    {
        std::cout << "\n--- Assembly Symbol Table: " << stage << " ---" << std::endl;
        _asmSymbolTablePrinter.print(asmSymbolTable);
    }
}
