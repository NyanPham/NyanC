
#pragma once

#include <memory>
#include "../AST.h"
#include "../Symbols.h"
#include "../TypeTable.h"
#include "../Tacky.h"
#include "../Assembly.h"
#include "../Settings.h"
#include "AssemblySymbols.h"

class Backend
{
public:
    Backend(Settings &settings, Symbols::SymbolTable &symbolTable, TypeTableNS::TypeTable &typeTable);

    std::pair<std::vector<std::shared_ptr<Assembly::Program>>, AssemblySymbols::AsmSymbolTable> generate(std::shared_ptr<TACKY::Program> tacky, const std::string &file);

private:
    Settings &_settings;
    Symbols::SymbolTable &_symbolTable;
    TypeTableNS::TypeTable &_typeTable;
};
