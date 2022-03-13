//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <functional>


template<class T>
struct FunctionInfo: public std::function<T>
{
    FunctionInfo(const std::string& name, std::function<T> function):
        std::function<T>(function),
        name(name)
    {

    }

    const std::string name;
    bool operator<(const FunctionInfo<T>& oth) const 
    {
        return name < oth.name;
    }
};


#endif //RANOKLANGUAGE_FUNCTIONINFO_H
