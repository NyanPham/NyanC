# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Isrc -Wall -g

# Executable name
TARGET = NyanC.exe

# Source files
SRCS = src/CFG.cpp \
       src/backend/AssemblySymbols.cpp \
       src/backend/Backend.cpp \
       src/backend/CodeGen.cpp \
       src/backend/InstructionFixup.cpp \
       src/backend/RegAlloc.cpp \
       src/backend/ReplacePseudos.cpp \
       src/BackwardDataflow.cpp \
       src/Compiler.cpp \
       src/Emit.cpp \
       src/Lexer.cpp \
       src/main.cpp \
       src/optimizations/AddressTaken.cpp \
       src/optimizations/ConstantFolding.cpp \
       src/optimizations/CopyPropa.cpp \
       src/optimizations/DeadStoreElim.cpp \
       src/optimizations/Optimize.cpp \
       src/optimizations/OptimizeUtils.cpp \
       src/optimizations/UnreachableCodeElim.cpp \
       src/Parser.cpp \
       src/semantic_analysis/CollectSwitchCases.cpp \
       src/semantic_analysis/IdentifierResolution.cpp \
       src/semantic_analysis/LoopLabeling.cpp \
       src/semantic_analysis/SemanticAnalyzer.cpp \
       src/semantic_analysis/TypeChecker.cpp \
       src/semantic_analysis/ValidateLabels.cpp \
       src/Settings.cpp \
       src/Symbols.cpp \
       src/TackyGen.cpp \
       src/Token.cpp \
       src/Types.cpp \
       src/TypeTable.cpp \
       src/UniqueIds.cpp \
       src/utils/DebugPrinter.cpp

# Generate object file names from source file names
OBJS = $(SRCS:.cpp=.o)

# Default rule: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
