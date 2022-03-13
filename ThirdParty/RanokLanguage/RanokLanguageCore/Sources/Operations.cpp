//
// Created by Balalarga on 07.03.2022.
//

#include "Operations.h"
#include <cmath>
#include <iostream>


double Operations::RvCross(double a, double b)
{
    return a + b - std::sqrt(std::pow(a, 2) + std::pow(b, 2));
};
double Operations::RvUnion(double a, double b)
{
    return a + b + std::sqrt(std::pow(a, 2) + std::pow(b, 2));
};

std::map<std::string, Operations::Unary> Operations::_unaryOperations
{
    {"-", [](double v) { return -v; }},
};

std::map<std::string, Operations::Binary> Operations::_binaryOperations
{
    {"+", std::plus<double>()},
    {"-", std::minus<double>()},
    {"/", std::divides<double>()},
    {"*", std::multiplies<double>()},
    {"^", [](double a, double b) { return std::pow(a, b); }},
    {"|", Operations::RvUnion},
    {"&", Operations::RvCross},
};

Operations::Unary Operations::UnaryFromString(const std::string& name)
{
    auto it = _unaryOperations.find(name);
    if (it == _unaryOperations.end())
        return nullptr;
    return it->second;
}

Operations::Binary Operations::BinaryFromString(const std::string& name)
{
    auto it = _binaryOperations.find(name);
    if (it != _binaryOperations.end())
        return it->second;
    std::cout << "No "<<name<<" operation\n";
    return nullptr;
}


const std::map<std::string, Operations::Binary>& Operations::GetBinaries()
{
    return _binaryOperations;
}

const std::map<std::string, Operations::Unary>& Operations::GetUnaries()
{
    return _unaryOperations;
}
