#include "AvailableExpressions.h"
#include "Algorithm.h"

class AvailableExpessionsPass : public llvm::ModulePass
{
public:
    AvailableExpessionsPass() : llvm::ModulePass(AvailableExpessionsPass::ID) {}
    virtual bool runOnModule(llvm::Module &mod)
    {
        llvm::Module *m = &mod;
        AvailableExpessions *data = new AvailableExpessions();

        for (llvm::Module::iterator func = m->begin(); func != m->end(); ++func)
        {
            llvm::Function *f = func;
            data->addFunction(f);
        }

        Algorithm *algorithm = new Algorithm(data);

        algorithm->printInputData();
        algorithm->createEGenEKill();
        algorithm->printEGenEKill();

        //Insert calls to methods class Algorithm
        algorithm->createAvailableExpressions();

        algorithm->printAvailableExpressions();

        delete algorithm;
        delete data;

        return false;
    }
    static char ID;
};

char AvailableExpessionsPass::ID = 'a';

static
llvm::RegisterPass<AvailableExpessionsPass> global_("available_expressions",
                                                    "available expressions",
                                                    false, false);
