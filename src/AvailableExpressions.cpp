#include "AvailableExpressions.h"


AvailableExpessions::Node
AvailableExpessions::addBasicBlock(llvm::BasicBlock *blk, unsigned &unnamedNo)
{
    //BasicBlock
    BasicBlock block;
    block.ID = blk;
    block.name = std::string(blk->getName());

    Node node;

    //Pred
    for (llvm::pred_iterator pred_block = pred_begin(blk);
         pred_block != pred_end(blk); ++pred_block)
    {
        llvm::BasicBlock *pred = *pred_block;
        BasicBlock prd;
        prd.ID = pred;
        prd.name = std::string(pred->getName());
        node.pred.push_back(prd);
    }

    //Succ
    for (llvm::succ_iterator succ_block = succ_begin(blk);
         succ_block != succ_end(blk); ++succ_block)
    {
        llvm::BasicBlock *succ = *succ_block;
        BasicBlock scc;
        scc.ID = succ;
        scc.name = std::string(succ->getName());
        node.succ.push_back(scc);
    }

    //instructions
    for (llvm::BasicBlock::iterator instruction = blk->begin();
         instruction != blk->end(); ++instruction)
    {
        llvm::Instruction *instr = instruction;
        Instruction ins;
        ins.instructionResult = instr;
        ins.instructionResultName = instr->getName();
        ins.operationName = std::string(instr->getOpcodeName());

        //values for instruction
        for (llvm::User::value_op_iterator vOp = instruction->value_op_begin();
             vOp != instruction->value_op_end(); ++vOp)
        {
            llvm::Value *v = *vOp;
            Value val;
            val.value = v;
            val.valueName = v->getName();
            ins.values.push_back(val);
        }
        block.instructions.push_back(ins);
    }

    node.block = block;

    return node;
}

void
AvailableExpessions::addFunction(llvm::Function *f)
{
    Function function;
    function.fID = f;
    function.fName = std::string(f->getName());

    for (llvm::Function::iterator block = f->begin();
         block != f->end(); ++block)
    {
        unsigned unnamedNo = 0;
        llvm::BasicBlock *blk = block;
        Node node = addBasicBlock(blk, unnamedNo);
        function.basicBlocks.push_back(node);
    }

    functions.push_back(function);
}

void
AvailableExpessions::printInputData()
{
    for (std::vector<Function>::iterator f = functions.begin();
         f != functions.end(); ++f)
    {
        std::cout << "Function: " << std::endl;
        std::cout << "(" << f->fID << ":" << f->fName << ")" << std::endl;
        std::cout << std::endl;

        std::cout << " BasicBlocks: " << std::endl;
        for (std::vector<Node>::iterator b = f->basicBlocks.begin();
             b != f->basicBlocks.end(); ++b)
        {
            std::cout << " Block: ";
            std::cout << "(" << b->block.ID << ":" << b->block.name << ");";

            std::cout << " Pred: ";
            for (std::vector<BasicBlock>::iterator p = b->pred.begin();
                 p != b->pred.end(); ++p)
                std::cout << "(" << p->ID << ":" << p->name << ") ";

            std::cout << "; Succ: ";
            for (std::vector<BasicBlock>::iterator s = b->succ.begin();
                 s != b->succ.end(); ++s)
                std::cout << "(" << s->ID << ":" << s->name << ") ";

            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "  Instructions:" << std::endl;
        for (std::vector<Node>::iterator b = f->basicBlocks.begin();
             b != f->basicBlocks.end(); ++b)
        {
            std::cout << "----------------" << std::endl;
            std::cout << "  Block: ";
            std::cout << "(" << b->block.ID << ":" << b->block.name << ");"
                      << std::endl;
            std::cout << std::endl;

            std::vector<Instruction>::iterator i;
            for (i = b->block.instructions.begin();
                 i != b->block.instructions.end(); ++i)
            {
                i->instructionResult->dump();
                std::cout << "  (" << i->instructionResult << ":"
                          << i->instructionResultName << ":"
                          << i->operationName << ") <- ";

                for (std::vector<Value>::iterator v = i->values.begin();
                     v != i->values.end(); ++v)
                    std::cout << "(" << v->value << ":" << v->valueName << ") ";

                std::cout << std::endl;
                std::cout << std::endl;
            }
        }
    }
}

std::vector<AvailableExpessions::Function>& AvailableExpessions::getFunctions()
{
    return this->functions;
}
