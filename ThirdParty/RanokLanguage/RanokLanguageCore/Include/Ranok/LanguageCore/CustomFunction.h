//
// Created by Balalarga on 10.03.2022.
//

#ifndef RANOKLANGUAGE_CUSTOMFUNCTION_H
#define RANOKLANGUAGE_CUSTOMFUNCTION_H

#include "Expression.h"
#include "FunctionInfo.h"


class CustomFunction
{
public:
    CustomFunction(const FunctionInfo<FunctionExpression::FuncType>& info, const std::string &code);
    virtual ~CustomFunction() = default;

    void SetRoot(spExpression& root);

    inline const FunctionInfo<FunctionExpression::FuncType>& Info() const { return _info; }
    inline spExpression Root() { return _root; }
    inline const std::vector<spArgumentExpression>& Args() const { return _args; }


private:
    FunctionInfo<FunctionExpression::FuncType> _info;
    spExpression _root;
    std::vector<spArgumentExpression> _args;
};

/** Used for function without c++ representation */
class PureCustomFunction: public CustomFunction
{
public:
    PureCustomFunction(const std::string& name, const std::string &code);
};

#endif //RANOKLANGUAGE_CUSTOMFUNCTION_H
