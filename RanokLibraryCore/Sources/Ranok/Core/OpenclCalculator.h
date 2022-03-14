#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include "Space.h"
#include "GradientModel.h"
#include "Utils/FlatArray.h"

#include <Ranok/LanguageCore/CodeGenerator.h>

template<int Dimensions>
class OpenclCalculator
{
public:
    OpenclCalculator();

    glm::fvec4 GetImageColor(double value);
    glm::fvec4 GetModelColor();

    void SetImageGradient(const LinearGradient& imageGradient);

    void CalculateModel(Program& program, const Space<Dimensions>& space, int batchSize = 0);
    void CalculateImage(Program& program, const Space<Dimensions>& space, int batchSize = 0);


protected:
    void GenerateCode(Program& program);

    static std::string codeHeader;
    static std::string codeFooter;
    static CodeGenerator::LanguageDefinition languageDefenition;


private:
    CodeGenerator _codeGenerator;
    std::string _code;

    LinearGradient _imageGradient;
    glm::fvec4 _modelColor;

    Program* _program;
    std::unique_ptr<FlatArray<class Space<Dimensions>::ModelData, Dimensions>> _modelBuffer;
    std::unique_ptr<FlatArray<class Space<Dimensions>::MImageData, Dimensions>> _imageBuffer;
};

#endif // OPENCLCALCULATOR_H
