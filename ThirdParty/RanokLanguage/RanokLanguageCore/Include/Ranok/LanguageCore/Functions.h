//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONS_H
#define RANOKLANGUAGE_FUNCTIONS_H

#include <vector>

#include "CustomFunction.h"


class Functions
{
public:
    Functions() = delete;

    static FunctionInfo<FunctionExpression::FuncType>* Find(const std::string& name);
    static const std::vector<FunctionInfo<FunctionExpression::FuncType>>& GetAll();

    static CustomFunction* FindCustom(const std::string& name);
    static const std::vector<std::shared_ptr<CustomFunction>>& GetAllCustoms();
    static void AddCustom(const std::shared_ptr<CustomFunction>& function);

    static CheckedResult<double> Cut(const std::vector<spExpression>& params);


private:
    static std::vector<FunctionInfo<FunctionExpression::FuncType>> _functions;
    static std::vector<std::shared_ptr<CustomFunction>> _customFunctions;
};

#endif //RANOKLANGUAGE_FUNCTIONS_H
