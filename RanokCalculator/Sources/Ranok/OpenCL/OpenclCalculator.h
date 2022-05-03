#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include <iostream>

#include "../Core/Space.h"
#include "OpenclSystem.h"
#include "../Core/GradientModel.h"
#include "../Core/Utils/FlatArray.h"

#include <Ranok/LanguageCore/CodeGenerator.h>


class OpenclCalculator
{
public:
    OpenclCalculator();

    glm::fvec4 GetImageColor(double value);
    glm::fvec4 GetModelColor();

    void SetImageGradient(const LinearGradient& imageGradient);

    bool Calculate(Program& program,
                   const Space& space,
                   const std::function<void(size_t, size_t)>& onDone,
                   size_t batchSize = 0);

    inline FlatArray<MImage3D>& GetImage() { return _imageBuffer; }


protected:
    void GenerateCode(Program& program)
    {
        std::stringstream codeStream;

        codeStream << codeHeader;
//        std::cout << _codeGenerator.Generate(program) << std::endl;
        codeStream << _codeGenerator.Generate(program);
        codeStream << codeFooter;

        _code = codeStream.str();
    }

    static std::string codeHeader;
    static std::string codeFooter;
    static std::string modelFunctionName;
    static std::string imageFunctionName;
    static CodeGenerator::LanguageDefinition languageDefenition;


private:
    CodeGenerator _codeGenerator;
    std::string _code;

    LinearGradient _imageGradient;
    glm::fvec4 _modelColor;

    Program* _program;
    FlatArray<MImage3D> _imageBuffer;
};


#endif // OPENCLCALCULATOR_H
