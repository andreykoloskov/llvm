#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "AvailableExpressions.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <set>

class Algorithm
{
private:
    AvailableExpessions *availableExpessionsInput;

    //struct Result

public:
    Algorithm() {}
    Algorithm(AvailableExpessions *a) : availableExpessionsInput(a) {}
    ~Algorithm() {}

    void printInputData();

    //Result methods
    void createEGenEKill();
    void printEGenEKill();
    void createAvailableExpressions();
    void printAvailableExpressions();
    AvailableExpessions::Node getNodeByBlock(AvailableExpessions::Function &function,
                                             AvailableExpessions::BasicBlock &block);
};

#endif
