//
// Created by Balalarga on 05.03.2022.
//

#include "Expression.h"

#include <iostream>

#define Debug(tag) std::cout << __FUNCTION__ << tag << "\n"

Expression::Expression(const std::string& name):
    name(name)
{

}

void Expression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    container.push({depth, this});
}

void Expression::SetValue(double val)
{
    value = val;
    computed = true;
}

double Expression::GetValue()
{
    return value;
}

void Expression::Reset()
{
    computed = false;
}

NumberExpression::NumberExpression(double value):
    Expression(std::to_string(value))
{
    SetValue(value);
}

void NumberExpression::Reset()
{

}

ArgumentExpression::ArgumentExpression(const std::string& name, const Range& range):
    Expression(name),
    range(range)
{
    
}

VariableExpression::VariableExpression(const std::string& name, spExpression child):
    Expression(name),
    child(child)
{
    
}

double VariableExpression::GetValue()
{
    if (!Computed())
        SetValue(child->GetValue());

    return Expression::GetValue();
}

void VariableExpression::Reset()
{
    child->Reset();
    Expression::Reset();
}

void VariableExpression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    child->Visit(container, depth+1);
}


UnaryOperation::UnaryOperation(const FunctionInfo<double(double)>& operation, spExpression child):
    Expression(operation.name),
    operation(operation),
    child(child)
{
    
}

void UnaryOperation::Reset()
{
    child->Reset();
    Expression::Reset();
}

double UnaryOperation::GetValue()
{
    if (!Computed())
        operation(child->GetValue());

    return Expression::GetValue();
}

void UnaryOperation::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    child->Visit(container, depth+1);
}

BinaryOperation::BinaryOperation(const FunctionInfo<double(double, double)>& operation,
        spExpression leftChild,
        spExpression rightChild):
    Expression(operation.name),
    operation(operation),
    leftChild(leftChild),
    rightChild(rightChild)
{
    
}

void BinaryOperation::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    leftChild->Visit(container, depth+1);
    rightChild->Visit(container, depth+1);
}

double BinaryOperation::GetValue()
{
    if (!Computed())
        SetValue(operation(leftChild->GetValue(), rightChild->GetValue()));
    return Expression::GetValue();
}

void BinaryOperation::Reset()
{
    leftChild->Reset();
    rightChild->Reset();
    Expression::Reset();
}

FunctionExpression::FunctionExpression(const FunctionInfo<FuncType>& function, const std::vector<spExpression>& args):
    Expression(function.name),
    function(function),
    params(args)
{
    
}

double FunctionExpression::GetValue()
{
    if (!Computed() && function)
    {
        CheckedResult<double> result = function(params);
        if (!result.Ok())
            Reset();
        else
            SetValue(result.Get());
    }
    return Expression::GetValue();
}

void FunctionExpression::Reset()
{
    for(auto& i: params)
        i->Reset();
    Expression::Reset();
}

void FunctionExpression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    for(auto& i: params)
        i->Visit(container, depth+1);
}

CustomFunctionExpression::CustomFunctionExpression(const FunctionInfo<FuncType>& function,
                                                   spExpression root,
                                                   const std::vector<spExpression>& args):
    FunctionExpression(function, args),
    root(std::move(root))
{

}
