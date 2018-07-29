#ifndef AVAILABLEEXPRESSIONS_H_INCLUDED
#define AVAILABLEEXPRESSIONS_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/Casting.h"

class AvailableExpessions
{
public:
    struct Value
    {
        llvm::Value *value;
        std::string valueName;
        bool operator==(const Value &rhs) const
        {
            return rhs.value == this->value && rhs.valueName == this->valueName;
        }
        bool operator<(const Value &rhs) const
        {
            if(this->value != rhs.value)
                return this->value < rhs.value;
            else
                return this->valueName < rhs.valueName;
        }
    };

    struct Instruction
    {
        llvm::Instruction *instructionResult;
        std::string instructionResultName;
        std::string operationName;
        std::vector<Value> values;
        bool operator==(const Instruction &rhs)
        {
            return this->instructionResult == rhs.instructionResult
                   && this->instructionResultName == rhs.instructionResultName
                   && this->operationName == rhs.operationName
                   && this->values == rhs.values;
        }
    };

    struct BasicBlock
    {
        llvm::BasicBlock *ID;
        std::string name;
        std::vector<Instruction> instructions;
    };

    struct Expression {
        std::string operationName;
        std::vector<Value> operands;
        Instruction instr;
        Expression(const std::string &operationName,
                   const std::vector<Value> &operands,
                   const Instruction instr)
                   : operationName(operationName), operands(operands),
                     instr(instr) {}
        Expression() {}

        friend std::ostream &operator<<(std::ostream &os, const Expression &ex)
        {
            os << ex.operationName ;
            for(auto &value : ex.operands)
                os << ", (" << value.valueName << ":" << value.value << ")";
            return os;
        }

        bool operator==(const Expression &rhs) const
        {
            return this->operationName == rhs.operationName
                   && this->operands == rhs.operands;
        }

        bool operator!=(Expression &expr)
        {
            return !(*this == expr);
        }

        bool operator<(const Expression &rhs) const
        {
            if(this->operationName != rhs.operationName)
                return this->operationName < rhs.operationName;
            else
                return this->operands < rhs.operands;
        }
    };

    struct Node
    {
        BasicBlock block;
        std::vector<BasicBlock> pred;
        std::vector<BasicBlock> succ;
        std::set<Expression> e_kill;
        std::set<Expression> e_gen;
        std::set<Expression> in_b;
        std::set<Expression> out_b;
        bool operator==(const Node &rhs) const
        {
            return this->block.ID == rhs.block.ID;
        }
    };

    struct Function
    {
        llvm::Function *fID;
        std::string fName;
        std::vector<Node> basicBlocks;
        std::set<Expression> allExpressions;
    };

    AvailableExpessions() {}

    ~AvailableExpessions() {}

    Node addBasicBlock(llvm::BasicBlock *block, unsigned &unnamedNo);

    void addFunction(llvm::Function *f);

    void printInputData();

    std::vector<Function>& getFunctions();
private:
    std::vector<Function> functions;

};

#endif
