
#include "Backend.h"
#include "../TackyGen.h"
#include "../optimizations/Optimize.h"
#include "CodeGen.h"
#include "../optimizations/AddressTaken.h"
#include "RegAlloc.h"
#include "ReplacePseudos.h"
#include "InstructionFixup.h"

Backend::Backend(Settings &settings, Symbols::SymbolTable &symbolTable, TypeTableNS::TypeTable &typeTable)
    : _settings(settings), _symbolTable(symbolTable), _typeTable(typeTable) {}

std::pair<std::vector<std::shared_ptr<Assembly::Program>>, AssemblySymbols::AsmSymbolTable> Backend::generate(std::shared_ptr<TACKY::Program> tacky, const std::string &file)
{
    auto codeGen = CodeGen(_symbolTable, _typeTable);
    auto asmProg = codeGen.gen(tacky);

    auto aliasedPseudos = analyzeProgram(tacky->getTopLevels());
    auto regAllocedAsmProg = allocateRegisters(aliasedPseudos, asmProg, codeGen.getAsmSymbolTable(), _settings);

    auto replacePseudos = ReplacePseudos(codeGen.getAsmSymbolTable());
    auto replacedAsm = replacePseudos.replacePseudos(regAllocedAsmProg);

    auto instructionFixup = InstructionFixup(codeGen.getAsmSymbolTable());
    auto fixedupAsm = instructionFixup.fixupProgram(replacedAsm);

    return {
        {asmProg, regAllocedAsmProg, replacedAsm, fixedupAsm},
        codeGen.getAsmSymbolTable()};
}
