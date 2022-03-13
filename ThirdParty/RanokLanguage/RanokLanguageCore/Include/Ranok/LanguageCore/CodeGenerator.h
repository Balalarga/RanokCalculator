//
// Created by Balalarga on 09.03.2022.
//

#ifndef RANOKLANGUAGE_CODEGENERATOR_H
#define RANOKLANGUAGE_CODEGENERATOR_H

#include "Program.h"

#include <map>
#include <sstream>



class CodeGenerator
{
public:
    struct LanguageDefinition
    {
        std::string mainFuncName = "__main";
        std::string numberType = "double";
        std::string returnDef = "return {0}";
        std::string endLineDef = ";\n";
        std::string funcSignature = "{0} {1}({2})";
        std::string varDefinition = "{0} {1} = {2}";
        std::pair<std::string, std::string> codeBlock = {"\n{\n", "}\n"};

        std::map<std::string, std::string> functionsMapping;
        std::map<std::string, std::string> unaryOperationsMapping;
        std::map<std::string, std::string> binaryOperationsMapping;
    };
    
    CodeGenerator();
    CodeGenerator(const LanguageDefinition& langDef);
    virtual ~CodeGenerator() = default;

    virtual std::string Generate(Program& program);

    inline const LanguageDefinition& GetLanguageDefinition() const { return _languageDefinition; };


protected:
    std::string DefineFunctions(Program& program);
    std::string DefineVariables(Program& program);

    const std::string& GetFunctionCode(const std::string& name) const;
    const std::string& GetUnaryOperationCode(const std::string& name) const;
    const std::string& GetBinaryOperationCode(const std::string& name) const;
    std::string GetExpressionCode(Expression* expression);

    std::string EnterFunction(const std::string& returnType,
                              const std::string& name,
                              const std::string& params);
    std::string LeaveFunction();


private:
    void CheckMappings();

    LanguageDefinition _languageDefinition;

    static std::map<std::string, std::string> defaultFunctionsMappings;
    static std::map<std::string, std::string> defaultUnaryOperationsMappings;
    static std::map<std::string, std::string> defaultBinaryOperationsMappings;
};


#endif //RANOKLANGUAGE_CODEGENERATOR_H
