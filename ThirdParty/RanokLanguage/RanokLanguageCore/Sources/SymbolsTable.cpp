//
// Created by Balalarga on 07.03.2022.
//

#include "SymbolsTable.h"


spArgumentExpression SymbolsTable::CreateArgument(const std::string& name, const ArgumentExpression::Range& range)
{
    _arguments.push_back(std::make_shared<ArgumentExpression>(name, range));
    return _arguments.back();
}

spVariableExpression SymbolsTable::CreateVariable(const std::string& name, spExpression child)
{
    _variables.push_back(std::make_shared<VariableExpression>(name, child));
    return _variables.back();
}

spNumberExpression SymbolsTable::CreateConstant(double value)
{
    _constants.push_back(std::make_shared<NumberExpression>(value));
    return _constants.back();
}

spArgumentExpression SymbolsTable::FindArgument(const std::string& name)
{
    for (auto& var: _arguments)
        if (var->name == name)
            return var;

    return nullptr;
}

spVariableExpression SymbolsTable::FindVariable(const std::string& name)
{
    for (auto& var: _variables)
        if (var->name == name)
            return var;
            
    return nullptr;
}

spNumberExpression SymbolsTable::FindConstant(const std::string& name)
{
    for (auto& var: _constants)
        if (var->name == name)
            return var;
            
    return nullptr;
}