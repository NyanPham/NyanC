#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Compiler.h"
#include "Lexer.h"
#include "Parser.h"
#include "UniqueIds.h"
#include "Symbols.h"
#include "semantic_analysis/SemanticAnalyzer.h"
#include "TackyGen.h"
#include "optimizations/Optimize.h"
#include "optimizations/AddressTaken.h"
#include "backend/Backend.h"
#include "Emit.h"
#include "utils/DebugPrinter.h"

std::string Compiler::preprocess(const std::string &src)
{
    _settings.validateExtension(src);
    std::string output = _settings.replaceExtension(src, ".i");
    _settings.runCommand("gcc", {"-E", "-P", src, "-o", output});
    return output;
}

// Helper to extract all instructions from a TACKY::Program
std::vector<std::shared_ptr<TACKY::Instruction>> getAllInstructions(const std::shared_ptr<TACKY::Program> &program)
{
    std::vector<std::shared_ptr<TACKY::Instruction>> result;
    for (const auto &topLevel : program->getTopLevels())
    {
        if (auto func = std::dynamic_pointer_cast<TACKY::Function>(topLevel))
        {
            const auto &instrs = func->getInstructions();
            result.insert(result.end(), instrs.begin(), instrs.end());
        }
    }
    return result;
}

int Compiler::compile(Stage stage, const std::vector<std::string> &srcFiles)
{
    try
    {
        DebugPrinter debugPrinter(_settings);
        std::vector<std::string> preprocessedFiles;

        for (const auto &src : srcFiles)
        {
            std::string preprocessedFile = preprocess(src);
            preprocessedFiles.push_back(preprocessedFile);
        }

        // Process each file based on the stage
        for (const auto &file : preprocessedFiles)
        {
            std::string input = processInputFile(file);

            if (stage == Stage::Lexing)
            {
                auto lexer = Lexer();
                lexer.setInput(input);
                lexer.defineTokenDefs();

                std::vector<Token> tokens = lexer.tokens();

                if (_settings.getIsDebug())
                {
                    for (Token token : tokens)
                    {
                        std::cout << token.toString() << '\n';
                    }
                }

                continue;
            }

            auto parser = Parser();
            auto ast = parser.parse(input);
            debugPrinter.printAST(*ast, "Parsed");

            if (stage == Stage::Parsing)
                continue;

            auto symbolTable = Symbols::SymbolTable();
            auto typeTable = TypeTableNS::TypeTable();

            auto semanticAnalyzer = SemanticAnalyzer(symbolTable, typeTable);
            auto analyzedAst = semanticAnalyzer.analyze(ast);

            debugPrinter.printAST(*analyzedAst, "Analyzed");
            debugPrinter.printSymbols(symbolTable, "Analyzed");
            debugPrinter.printTypes(typeTable, "Analyzed");

            if (stage == Stage::Validate)
                continue;

            auto tackyGen = TackyGen(symbolTable, typeTable);
            auto tacky = tackyGen.gen(analyzedAst);

            auto optimizedTacky = optimize(_settings, file, *tacky, symbolTable);

            debugPrinter.printSymbols(symbolTable, "Tacky Generation");
            debugPrinter.printTypes(typeTable, "Tacky Generation");
            debugPrinter.printTacky(*tacky, "Initial");
            debugPrinter.printTacky(*optimizedTacky, "Optimized");

            if (stage == Stage::Tacky)
                continue;

            auto backend = Backend(_settings, symbolTable, typeTable);
            auto [asmProgs, asmSymbolTable] = backend.generate(optimizedTacky, file);

            debugPrinter.printSymbols(symbolTable, "Code Generation");
            debugPrinter.printAssembly(*asmProgs[0], "Raw");
            debugPrinter.printAsmSymbols(asmSymbolTable, "Initial");
            debugPrinter.printAssembly(*asmProgs[1], "Register Allocated");
            debugPrinter.printAssembly(*asmProgs[2], "Pseudos Replaced");
            debugPrinter.printAssembly(*asmProgs[3], "Instruction Fixed-up");

            if (stage == Stage::CodeGen)
                continue;

            Emit emitter = Emit(asmSymbolTable);

            std::string asmFile = _settings.replaceExtension(file, ".s");
            emitter.emit(asmProgs[3], asmFile);
            if (stage == Stage::Emit)
                continue;

            assembleAndLink(srcFiles, true, !_settings.getIsDebug());

            if (stage == Stage::Object)
                continue;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Compilation error: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown compilation error" << std::endl;
        return -1;
    }

    return 0;
}

void Compiler::assembleAndLink(const std::vector<std::string> &srcFiles, bool link, bool cleanUp)
{
    std::vector<std::string> objFiles;

    for (const auto &src : srcFiles)
    {
        std::string asmFile = _settings.replaceExtension(src, ".s");
        std::string objFile = _settings.replaceExtension(src, ".o");
        objFiles.push_back(objFile);

        // Assemble each source file into an object file
        _settings.runCommand("gcc", {asmFile, "-c", "-o", objFile});

        if (cleanUp)
        {
            _settings.runCommand("rm", {asmFile});
        }
    }

    if (link)
    {
        // Link all object files into a single executable
        std::string outputFile = _settings.removeExtension(srcFiles[0]); // Use the first file's name for the executable
        std::vector<std::string> gccArgs = objFiles;
        gccArgs.push_back("-o");
        gccArgs.push_back(outputFile);

        _settings.runCommand("gcc", gccArgs);

        if (cleanUp)
        {
            for (const auto &objFile : objFiles)
            {
                _settings.runCommand("rm", {objFile});
            }
        }
    }
}

std::string Compiler::processInputFile(const std::string &src)
{
    std::ifstream inputFile(src);
    if (!inputFile.is_open())
    {
        throw std::runtime_error("Error: Could not open file " + src);
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();

    if (std::remove(src.c_str()) != 0)
    {
        std::cerr << "Warning: Unable to remove temporary file " << src << std::endl;
    }

    return buffer.str();
}
