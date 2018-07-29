#include <AvailableExpressions.h>
#include "Algorithm.h"

using namespace std;

void
Algorithm::printInputData()
{
    availableExpessionsInput->printInputData();
}

//Result methods
void
Algorithm::createEGenEKill()
{
    for(auto &function : this->availableExpessionsInput->getFunctions())
    {
        for(auto &node : function.basicBlocks)
        {
            for(auto &instruction : node.block.instructions)
            {
                if(instruction.operationName == "alloca")
                    continue;

                AvailableExpessions::Expression curExpression;
                curExpression.instr = instruction;
                curExpression.operationName = instruction.operationName;
                AvailableExpessions::Value assignableValue;

                if(instruction.operationName == "store")
                {
                    curExpression.operands.push_back(instruction.values[0]);
                    assignableValue = instruction.values[1];
                }
                else
                {
                    curExpression.operands = instruction.values;
                    assignableValue.value = static_cast<llvm::Value *>
                                            (instruction.instructionResult);
                    assignableValue.valueName = instruction.instructionResultName;
                }

                node.e_gen.insert(curExpression);
                function.allExpressions.insert(curExpression);
                for(auto &node2 : function.basicBlocks)
                {
                    for(auto &instruction2 : node2.block.instructions)
                    {
                        if(instruction2.operationName == "alloca"
                           || instruction2 == instruction)
                            continue;

                        if(instruction2.operationName == "store")
                        {
                            if(instruction2.values[0] == assignableValue)
                                node.e_kill.insert(AvailableExpessions::
                                            Expression("store",
                                                       {instruction2.values[0]},
                                                       instruction2));
                        }
                        else
                        {
                            auto iter = find(instruction2.values.begin(),
                                             instruction2.values.end(),
                                             assignableValue);
                            if(iter != instruction2.values.end())
                                node.e_kill.insert(AvailableExpessions::
                                        Expression(instruction2.operationName,
                                                   instruction2.values,
                                                   instruction2));
                        }
                    }
                }
                //Надеюсь, то, что ниже не понадобится
                /*
                for(auto iter = node.e_gen.begin(); iter != node.e_gen.end(); ++iter) {
                    if(find(iter->operands.begin(), iter->operands.end(), assignableValue) != iter->operands.end()) {
                        iter = node.e_gen.erase(iter);
                    }
                }
                 */
            }
        }
    }
}

void
Algorithm::printEGenEKill()
{
    for(auto &function : this->availableExpessionsInput->getFunctions())
    {
        cout << "Function: " + function.fName << endl;
        for(auto &node : function.basicBlocks)
        {
            cout << "Block: " << node.block.name << endl;
            cout << "E_Gen:" << endl;
            copy(node.e_gen.begin(), node.e_gen.end(),
                 ostream_iterator<AvailableExpessions::Expression>(cout, "\n"));
            cout << "E_Kill:" << endl;
            copy(node.e_kill.begin(), node.e_kill.end(),
                 ostream_iterator<AvailableExpessions::Expression>(cout, "\n"));
        }

    }
}


void Algorithm::createAvailableExpressions()
{
    for(auto &f : availableExpessionsInput->getFunctions())
    {
        AvailableExpessions::Node entry;
        set<AvailableExpessions::Expression> allExpressions;
        for(auto &block : f.basicBlocks)
            block.out_b = f.allExpressions;

        bool changed = true;
        while(changed)
        {
            changed = false;
            for(auto &block : f.basicBlocks)
            {
                std::set<AvailableExpessions::Expression> newInB;
                std::set<AvailableExpessions::Expression> newOutB;

                if (!block.pred.empty())
                {
                    newInB = this->getNodeByBlock(f, *block.pred.begin()).out_b;
                    for(auto iter = ++block.pred.begin();
                        iter != block.pred.end(); ++iter)
                    {
                        std::set<AvailableExpessions::Expression> tmp;
                        auto tmpNode = this->getNodeByBlock(f, *iter);
                        set_intersection(newInB.begin(), newInB.end(),
                                         tmpNode.out_b.begin(),
                                         tmpNode.out_b.end(),
                                         inserter(tmp, tmp.end()));
                        newInB = tmp;
                    }

                    if(newInB != block.in_b)
                        block.in_b = newInB;

                    set_difference(block.in_b.begin(), block.in_b.end(),
                                   block.e_kill.begin(), block.e_kill.end(),
                                   inserter(newOutB, newOutB.end()));
                }
                else
                {
                    block.in_b.clear();
                }
                
                for(auto &expr : block.e_gen)
                    newOutB.insert(expr);

                if(newOutB != block.out_b)
                {
                    changed = true;
                    block.out_b = newOutB;
                }
            }
        }

    }
}


AvailableExpessions::Node Algorithm::getNodeByBlock(AvailableExpessions::Function &f,
                                        AvailableExpessions::BasicBlock &block)
{
    vector<AvailableExpessions::Node>::iterator iter =
            find_if(f.basicBlocks.begin(), f.basicBlocks.end(),
            [&block](const AvailableExpessions::Node &node)
            {return node.block.ID == block.ID;});

    return *iter;
}

void Algorithm::printAvailableExpressions()
{
    for(auto &function : this->availableExpessionsInput->getFunctions())
    {
        cout << "Function " << function.fName << endl;
        for(auto &block : function.basicBlocks)
        {
            cout << "Block " << block.block.name << endl;
            copy(block.in_b.begin(), block.in_b.end(),
                 ostream_iterator<AvailableExpessions::Expression>(cout, "\n"));
            cout << endl;
        }
    }
}
