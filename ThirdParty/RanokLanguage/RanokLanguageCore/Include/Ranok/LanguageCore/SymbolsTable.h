//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_SYMBOLSTABLE_H
#define RANOKLANGUAGE_SYMBOLSTABLE_H

#include "Expression.h"

#include <vector>


class SymbolsTable
{
public:
    SymbolsTable() = default;

    spArgumentExpression CreateArgument(const std::string& name, const ArgumentExpression::Range& range);
    spVariableExpression CreateVariable(const std::string& name, spExpression child);
    spNumberExpression   CreateConstant(double value);

    spArgumentExpression FindArgument(const std::string& name);
    spVariableExpression FindVariable(const std::string& name);
    spNumberExpression   FindConstant(const std::string& name);

    inline const std::vector<spArgumentExpression>& Arguments(){ return _arguments; }
    inline const std::vector<spVariableExpression>& Variables(){ return _variables; }
    inline const std::vector<spNumberExpression>&   Constants(){ return _constants; }


private:
    std::vector<spArgumentExpression> _arguments;
    std::vector<spVariableExpression> _variables;
    std::vector<spNumberExpression>   _constants;
};

#endif //RANOKLANGUAGE_SYMBOLSTABLE_H
