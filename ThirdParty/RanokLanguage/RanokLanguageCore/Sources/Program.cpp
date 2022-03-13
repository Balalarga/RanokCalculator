//
// Created by Balalarga on 06.03.2022.
//

#include "Program.h"

#include <iostream>


double Program::Process(const std::vector<double>& args)
{
    auto& programArgs = _symbolsTable.Arguments();
    if (programArgs.size() > args.size())
    {
        std::cout<< "Not all args have value ("<<args.size()<<"/"<<programArgs.size()<<")\n";
        return 0;
    }

    for (int i = 0; i < programArgs.size(); ++i)
        programArgs[i]->SetValue(args[i]);

    return _root->GetValue();
}

double Program::Process()
{
    return _root->GetValue();
}

void Program::Init(const spExpression& root)
{
    _root = root;
}

spFunctionExpression Program::ToFunctionExpression()
{
    return spFunctionExpression();
}
