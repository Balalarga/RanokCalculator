//
// Created by Balalarga on 09.03.2022.
//

#include "CodeGenerator.h"
#include "Functions.h"
#include "Operations.h"

#include <fmt/format.h>

#include <iostream>
#include <set>

std::map<std::string, std::string> CodeGenerator::defaultFunctionsMappings
{
    { "abs", "fabs" },
    { "sqrt", "sqrt" },
    { "sin", "sin" },
    { "cos", "cos" },
    { "tan", "tan" },
    { "arctan", "atan" },
    { "arcsin", "asin" },
    { "arccos", "acos" },
    { "cosh", "cosh" },
    { "sinh", "sinh" },
    { "tanh", "tanh" },
    { "exp", "exp" },
    { "ln", "log" },
    { "log", "log" },
    { "log10", "log10" },
    { "log2", "log2" },
    { "ceil", "ceil" },
    { "floor", "floor" }
};

std::map<std::string, std::string> CodeGenerator::defaultUnaryOperationsMappings
{
    {"-", "-{0}"},
};

std::map<std::string, std::string> CodeGenerator::defaultBinaryOperationsMappings
{
        {"+", "{0} + {1}"},
        {"-", "{0} - {1}"},
        {"/", "{0} / {1}"},
        {"*", "{0} * {1}"},
        {"^", "pow({0}, {1})"},
        {"|", "{0} + {1} + sqrt(pow({0}, 2) + pow({1}, 2))"},
        {"&", "{0} + {1} - sqrt(pow({0}, 2) + pow({1}, 2))"},
};

CodeGenerator::CodeGenerator()
{
    _languageDefinition.functionsMapping = std::move(defaultFunctionsMappings);
    _languageDefinition.unaryOperationsMapping = std::move(defaultUnaryOperationsMappings);
    _languageDefinition.binaryOperationsMapping = std::move(defaultBinaryOperationsMappings);
}

CodeGenerator::CodeGenerator(const LanguageDefinition& langDef):
    _languageDefinition(std::move(langDef))
{
    _languageDefinition.functionsMapping.merge(defaultFunctionsMappings);
    _languageDefinition.unaryOperationsMapping.merge(defaultUnaryOperationsMappings);
    _languageDefinition.binaryOperationsMapping.merge(defaultBinaryOperationsMappings);
    
    CheckMappings();
}

void CodeGenerator::CheckMappings()
{
    for (auto& func: Functions::GetAll())
    {
        if (_languageDefinition.functionsMapping.find(func.name) == _languageDefinition.functionsMapping.end())
            throw std::runtime_error("Couldn't find code for " + func.name + " function");
    }

    for (auto& oper : Operations::GetUnaries())
    {
        if (_languageDefinition.unaryOperationsMapping.find(oper.first) == _languageDefinition.unaryOperationsMapping.end())
            throw std::runtime_error("Couldn't find code for " + oper.first + " unary operation");
    }

    for (auto& oper : Operations::GetBinaries())
    {
        if (_languageDefinition.binaryOperationsMapping.find(oper.first) == _languageDefinition.binaryOperationsMapping.end())
            throw std::runtime_error("Couldn't find code for " + oper.first + " binary function");
    }
}

std::string CodeGenerator::Generate(Program& program)
{
    std::stringstream code;
    std::queue<std::pair<int, Expression*>> nodes;
    program.Root()->Visit(nodes);
    
    std::vector<std::string> argsDef;
    const auto& args = program.Table().Arguments();
    for (auto& a: args)
        argsDef.push_back(fmt::format("{0} {1}", _languageDefinition.numberType, a->name));

    code << DefineFunctions(program);
    code << EnterFunction(_languageDefinition.numberType, _languageDefinition.mainFuncName, fmt::format("{}", fmt::join(argsDef, ", ")));
    code << DefineVariables(program);
    code << fmt::format(_languageDefinition.returnDef, GetExpressionCode(program.Root().get())) << _languageDefinition.endLineDef;
    code << LeaveFunction();

    return code.str();
}

std::string CodeGenerator::DefineFunctions(Program& program)
{
    std::stringstream code;
    std::set<std::string> namesBuffer;
    std::vector<CustomFunction*> usedCustomFuncs;
    for (CustomFunctionExpression*& funcExpr :program.GetAllOf<CustomFunctionExpression>())
    {
        if (namesBuffer.find(funcExpr->name) == namesBuffer.end())
        {
            namesBuffer.insert(funcExpr->name);
            usedCustomFuncs.push_back(Functions::FindCustom(funcExpr->name));
        }
    }

    std::vector<std::string> argsDef;
    for (auto& func: usedCustomFuncs)
    {
        argsDef.clear();
        for (auto& a: func->Args())
            argsDef.push_back(fmt::format("{0} {1}", _languageDefinition.numberType, a->name));

        code << EnterFunction(_languageDefinition.numberType, func->Info().name, fmt::format("{}", fmt::join(argsDef, ", ")));
        code << fmt::format(_languageDefinition.returnDef, GetExpressionCode(func->Root().get())) << _languageDefinition.endLineDef;
        code << LeaveFunction();
    }
    return code.str();
}

std::string CodeGenerator::DefineVariables(Program& program)
{
    std::stringstream varCode;
    for (auto& var: program.Table().Variables())
    {
        varCode << fmt::format(_languageDefinition.varDefinition, _languageDefinition.numberType, var->name, GetExpressionCode(var->child.get()));
        varCode << _languageDefinition.endLineDef;
    }
    return varCode.str();
}

const std::string& CodeGenerator::GetFunctionCode(const std::string& name) const
{
    return _languageDefinition.functionsMapping.at(name);
}

const std::string& CodeGenerator::GetUnaryOperationCode(const std::string& name) const
{
    return _languageDefinition.unaryOperationsMapping.at(name);
}

const std::string& CodeGenerator::GetBinaryOperationCode(const std::string& name) const
{
    return _languageDefinition.binaryOperationsMapping.at(name);
}

std::string CodeGenerator::GetExpressionCode(Expression* expression)
{
    if (auto* expr = dynamic_cast<NumberExpression*>(expression))
    {
        return expr->name;
    }
    else if (auto* expr = dynamic_cast<ArgumentExpression*>(expression))
    {
        return expr->name;
    }
    else if (auto* expr = dynamic_cast<VariableExpression*>(expression))
    {
        return expr->name;
    }
    else if (auto* expr = dynamic_cast<UnaryOperation*>(expression))
    {
        return fmt::format(GetUnaryOperationCode(expr->operation.name), GetExpressionCode(expr->child.get()));
    }
    else if (auto* expr = dynamic_cast<BinaryOperation*>(expression))
    {
        return fmt::format(GetBinaryOperationCode(expr->operation.name),
            GetExpressionCode(expr->leftChild.get()),
            GetExpressionCode(expr->rightChild.get()));
    }
    else if (auto* expr = dynamic_cast<FunctionExpression*>(expression))
    {
        std::vector<std::string> params;
        
        for (auto& i: expr->params)
            params.push_back(GetExpressionCode(i.get()));

        return fmt::format("{0}({1})", expr->function.name, fmt::join(params, ", "));
    }
    else if (auto* expr = dynamic_cast<CustomFunctionExpression*>(expression))
    {
        std::vector<std::string> params;

        for (auto& i: expr->params)
            params.push_back(i->name);

        return fmt::format("{0}({1})", expr->name, fmt::join(params, ", "));
    }
    return "";
}

std::string CodeGenerator::LeaveFunction()
{
    return _languageDefinition.codeBlock.second;
}

std::string CodeGenerator::EnterFunction(const std::string& returnType,
                                         const std::string& name,
                                         const std::string& params)
{
    return fmt::format(_languageDefinition.funcSignature, returnType, name, params) +
           _languageDefinition.codeBlock.first;
}

