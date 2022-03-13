//
// Created by Balalarga on 10.03.2022.
//

#include "CustomFunction.h"

#include "Parser.h"


void CustomFunction::SetRoot(spExpression& root)
{
    _root = root;
}

CustomFunction::CustomFunction(const FunctionInfo<FunctionExpression::FuncType> &info, const std::string &code):
    _info(info)
{
    Parser parser;
    Program program = parser.Parse(Lexer::CreateFrom(code));
    _args = program.Table().Arguments();
    _root = program.Root();
}

PureCustomFunction::PureCustomFunction(const std::string& name, const std::string &code):
    CustomFunction({name, nullptr}, code)
{

}
