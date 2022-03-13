//
// Created by Balalarga on 07.03.2022.
//

#include "Functions.h"
#include "Operations.h"

#include <iostream>
#include <cmath>

using namespace std;

#define CheckParams(params, count) \
    if (params.size() != count) \
    { \
        cout << __FUNCTION__ << ": parameters error(must be " << count << ")\n"; \
        return false; \
    }

CheckedResult<double> Sqrt(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sqrt(params[0]->GetValue());
}
CheckedResult<double> Abs(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::fabs(params[0]->GetValue());
}
CheckedResult<double> Sin(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sin(params[0]->GetValue());
}
CheckedResult<double> Cos(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::cos(params[0]->GetValue());
}
CheckedResult<double> Tan(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::tan(params[0]->GetValue());
}
CheckedResult<double> ArcTan(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::atan(params[0]->GetValue());
}
CheckedResult<double> ArcSin(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::asin(params[0]->GetValue());
}
CheckedResult<double> ArcCos(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::acos(params[0]->GetValue());
}
CheckedResult<double> Cosh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::cosh(params[0]->GetValue());
}
CheckedResult<double> Sinh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sinh(params[0]->GetValue());
}
CheckedResult<double> Tanh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::tanh(params[0]->GetValue());
}
CheckedResult<double> Exp(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::exp(params[0]->GetValue());
}
CheckedResult<double> Ln(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log(params[0]->GetValue());
}
CheckedResult<double> Log(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log(params[0]->GetValue());
}
CheckedResult<double> Log10(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log10(params[0]->GetValue());
}
CheckedResult<double> Log2(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log2(params[0]->GetValue());
}
CheckedResult<double> Ceil(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::ceil(params[0]->GetValue());
}
CheckedResult<double> Floor(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::floor(params[0]->GetValue());
}
CheckedResult<double> Functions::Cut(const std::vector<spExpression>& params)
{
    CheckParams(params, 4)
    auto& var = params[0];
    auto& axis = params[1];
    auto& start = params[2];
    auto& end = params[3];

    /*
        var s1 = -(z - 0.5); // Верх
        var s2 = z + 0.5; // Снизу
        RETURN W & s1 & s2;
    */
    auto plane1 = (-axis->GetValue() + end->GetValue());
    auto plane2 = (axis->GetValue() - start->GetValue());
    return Operations::RvCross(var->GetValue(), Operations::RvCross(plane1, plane2));
}


std::vector<FunctionInfo<FunctionExpression::FuncType>> Functions::_functions
{
        { "abs" , &Abs },
        { "sqrt" , &Sqrt },
        { "sin", &Sin },
        { "cos" , &Cos },
        { "tan" , &Tan },
        { "arctan" , &ArcTan },
        { "arcsin" , &ArcSin },
        { "arccos" , &ArcCos },
        { "cosh" , &Cosh },
        { "sinh" , &Sinh },
        { "tanh" , &Tanh },
        { "exp" , &Exp },
        { "ln" , &Ln },
        { "log" , &Log },
        { "log10" , &Log10 },
        { "log2" , &Log2 },
        { "ceil" , &Ceil },
        { "floor" , &Floor }
};

std::vector<std::shared_ptr<CustomFunction>> Functions::_customFunctions
{

};


FunctionInfo<FunctionExpression::FuncType>* Functions::Find(const std::string &name)
{
    for (auto& func : _functions)
        if (func.name == name)
            return &func;
    return nullptr;
}

const std::vector<FunctionInfo<FunctionExpression::FuncType>>& Functions::GetAll()
{
    return _functions;
}

CustomFunction* Functions::FindCustom(const std::string& name)
{
    for (auto& func : _customFunctions)
        if (func->Info().name == name)
            return func.get();
    return nullptr;
}

const std::vector<std::shared_ptr<CustomFunction>>& Functions::GetAllCustoms()
{
    return _customFunctions;
}

void Functions::AddCustom(const std::shared_ptr<CustomFunction>& function)
{
    _customFunctions.push_back(function);
}
