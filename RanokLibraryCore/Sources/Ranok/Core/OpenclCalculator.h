#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include <iostream>

#include "Space.h"
#include "OpenclSystem.h"
#include "GradientModel.h"
#include "Utils/FlatArray.h"

#include <Ranok/LanguageCore/CodeGenerator.h>


class OpenclMock
{
public:
    OpenclMock() = delete;
    static double matrix4x4Det(double* m, int n)
    {
          return
             m[3] * m[1*n+2] * m[2*n+1] * m[3*n+0] - m[0*n+2] * m[1*n+3] * m[2*n+1] * m[3*n+0] -
             m[3] * m[1*n+1] * m[2*n+2] * m[3*n+0] + m[0*n+1] * m[1*n+3] * m[2*n+2] * m[3*n+0] +
             m[2] * m[1*n+1] * m[2*n+3] * m[3*n+0] - m[0*n+1] * m[1*n+2] * m[2*n+3] * m[3*n+0] -
             m[3] * m[1*n+2] * m[2*n+0] * m[3*n+1] + m[0*n+2] * m[1*n+3] * m[2*n+0] * m[3*n+1] +
             m[3] * m[1*n+0] * m[2*n+2] * m[3*n+1] - m[0*n+0] * m[1*n+3] * m[2*n+2] * m[3*n+1] -
             m[2] * m[1*n+0] * m[2*n+3] * m[3*n+1] + m[0*n+0] * m[1*n+2] * m[2*n+3] * m[3*n+1] +
             m[3] * m[1*n+1] * m[2*n+0] * m[3*n+2] - m[0*n+1] * m[1*n+3] * m[2*n+0] * m[3*n+2] -
             m[3] * m[1*n+0] * m[2*n+1] * m[3*n+2] + m[0*n+0] * m[1*n+3] * m[2*n+1] * m[3*n+2] +
             m[1] * m[1*n+0] * m[2*n+3] * m[3*n+2] - m[0*n+0] * m[1*n+1] * m[2*n+3] * m[3*n+2] -
             m[2] * m[1*n+1] * m[2*n+0] * m[3*n+3] + m[0*n+1] * m[1*n+2] * m[2*n+0] * m[3*n+3] +
             m[2] * m[1*n+0] * m[2*n+1] * m[3*n+3] - m[0*n+0] * m[1*n+2] * m[2*n+1] * m[3*n+3] -
             m[1] * m[1*n+0] * m[2*n+2] * m[3*n+3] + m[0*n+0] * m[1*n+1] * m[2*n+2] * m[3*n+3];
    }

    static char checkZone8(float *values)
    {
        bool plus = false;
        bool zero = false;
        bool minus = false;
        for(int i = 0; i < 8; i++)
        {
            if(values[i] == 0)
                zero = true;
            else if(values[i] < 0)
                minus = true;
            else if(values[i] > 0)
                plus = true;
        }
        if(zero || (plus && minus))
            return 0;
        if(plus)
            return 1;
        return -1;
    }

    static void ExecModelTimes(std::function<double(cl_float3)> func,
                               unsigned times,
                               char* resultZones,
                               int startId,
                               cl_uint3 spaceSize,
                               cl_float3 startPoint,
                               cl_float3 pointSize,
                               cl_float3 halfSize)
    {
        for(unsigned i = 0; i < times; ++i)
            ExecModel(func, i, resultZones, startId, spaceSize, startPoint, pointSize, halfSize);
    }

    static void ExecImageTimes(std::function<double(cl_float3)> func,
                               unsigned times,
                               double* result,
                               int startId,
                               cl_uint3 spaceSize,
                               cl_float3 startPoint,
                               cl_float3 pointSize,
                               cl_float3 halfSize)
    {
        for(unsigned i = 0; i < times; ++i)
            ExecImage(func, i, result, startId, spaceSize, startPoint, pointSize, halfSize);
    }

    static void ExecModel(std::function<double(cl_float3)> func,
                          unsigned id,
                          char*& resultZones,
                          int startId,
                          cl_uint3 spaceSize,
                          cl_float3 startPoint,
                          cl_float3 pointSize,
                          cl_float3 halfSize)
    {
        int spaceId = startId + id;
        cl_float3 point;
        point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
        point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
        point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

//        std::cout << "Params: " << "\n";
//        std::cout << "    startId: " << startId << "\n";
//        std::cout << "    spaceSize: "  << spaceSize.x << ", " << spaceSize.y << ", " << spaceSize.z << "\n";
//        std::cout << "    startPoint: " << startPoint.x << ", " << startPoint.y << ", " << startPoint.z << "\n";
//        std::cout << "    pointSize: "  << pointSize.x << ", " << pointSize.y << ", " << pointSize.z << "\n";
//        std::cout << "    halfSize: "   << halfSize.x << ", " << halfSize.y << ", " << halfSize.z << "\n";
//        std::cout << "    point: "   << point.x << ", " << point.y << ", " << point.z << "\n";

        float values[8];
        values[0] = func({point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z});
        values[1] = func({point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z});
        values[2] = func({point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z});
        values[3] = func({point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z});
        values[4] = func({point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z});
        values[5] = func({point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z});
        values[6] = func({point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z});
        values[7] = func({point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z});

        resultZones[id] = checkZone8(values);
    }

    static void ExecImage(std::function<double(cl_float3)> func,
                            unsigned id,
                            double* result,
                            int startId,
                            cl_uint3 spaceSize,
                            cl_float3 startPoint,
                            cl_float3 pointSize,
                            cl_float3 halfSize)
    {
        int spaceId = startId + id;
        cl_float3 point;
        point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
        point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
        point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

        double wv[4];
        wv[0] = func({point.x,             point.y,             point.z            });
        wv[1] = func({point.x+pointSize.x, point.y,             point.z            });
        wv[2] = func({point.x,             point.y+pointSize.y, point.z            });
        wv[3] = func({point.x,             point.y,             point.z+pointSize.z});

        double a[] = {
            point.y,             point.z,             wv[0], 1,
            point.y,             point.z,             wv[1], 1,
            point.y+pointSize.y, point.z,             wv[2], 1,
            point.y,             point.z+pointSize.z, wv[3], 1
        };
        double b[] = {
            point.x,             point.z,             wv[0], 1,
            point.x+pointSize.x, point.z,             wv[1], 1,
            point.x,             point.z,             wv[2], 1,
            point.x,             point.z+pointSize.z, wv[3], 1
        };
        double c[] = {
            point.x,             point.y,             wv[0], 1,
            point.x+pointSize.x, point.y,             wv[1], 1,
            point.x,             point.y+pointSize.y, wv[2], 1,
            point.x,             point.y,             wv[3], 1
        };
        double d[] = {
            point.x,             point.y,             point.z,              1,
            point.x+pointSize.x, point.y,             point.z,              1,
            point.x,             point.y+pointSize.y, point.z,              1,
            point.x,             point.y,             point.z+pointSize.z, 1
        };
        double f[] = {
            point.x,             point.y,             point.z,             wv[0],
            point.x+pointSize.x, point.y,             point.z,             wv[1],
            point.x,             point.y+pointSize.y, point.z,             wv[2],
            point.x,             point.y,             point.z+pointSize.z, wv[3]
        };
        double detA = matrix4x4Det(a, 4);
        double detB = matrix4x4Det(b, 4);
        double detC = matrix4x4Det(c, 4);
        double detD = matrix4x4Det(d, 4);
        double detF = matrix4x4Det(f, 4);
        double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

        result[id*5  ] = detA/div;
        result[id*5+1] = -detB/div;
        result[id*5+2] = -detC/div;
        result[id*5+3] = detD/div;
        result[id*5+4] = detF/div;
    }
};



template<int Dimensions>
class OpenclCalculator
{
public:
    OpenclCalculator():
        _modelColor(ColorFromUint(255, 255, 255, 20)),
        _program(nullptr),
        _codeGenerator(languageDefenition),
        _imageGradient({
                       ColorFromUint(255, 255, 0,   20),
                       ColorFromUint(0,   255, 162, 20),
                       ColorFromUint(0,   0,   255, 20),
                       ColorFromUint(255, 145, 0,   20),
                       ColorFromUint(214, 0,   255, 20),
                       })
    {}

    glm::fvec4 GetImageColor(double value)
    {
        return _imageGradient.GetColor(UINT_MAX * (1.0 + value) / 2.0);
    }
    glm::fvec4 GetModelColor()
    {
        return _modelColor;
    }

    void SetImageGradient(const LinearGradient& imageGradient)
    {
        _imageGradient = imageGradient;
    }

    bool CalculateModel(Program& program, const Space<Dimensions>& space, int batchSize = 0)
    {
        if (_program != &program)
        {
            GenerateCode(program);
            _program = &program;
            if (!OpenclSystem::Get().Compile(_code))
                return false;
        }

        // TODO: make opencl program "templated" for other dimensions
        cl_int startId = 0;
        cl_uint3 spaceSize = {space.GetPartition()[0], space.GetPartition()[1], space.GetPartition()[2]};
        cl_float3 startPoint = {space.GetStartPoint()[0], space.GetStartPoint()[1], space.GetStartPoint()[2]};
        cl_float3 pointSize = {space.GetUnitSize()[0], space.GetUnitSize()[1], space.GetUnitSize()[2]};
        cl_float3 halfSize = {pointSize.x / 2.f, pointSize.y / 2.f, pointSize.z / 2.f};


        if (_imageBuffer.Size() != 0)
            _imageBuffer.Clear();

        _modelBuffer.Resize(space.GetPartition());

        /*
           global char *resultZones,
           const int startId,
           const uint3 spaceSize,
           const float3 startPoint,
           const float3 pointSize,
           const float3 halfSize)
        */
        KernelArguments::Item output(&_modelBuffer[0], sizeof(char), _modelBuffer.Size());
        std::vector<KernelArguments::Item> optional
        {
            {&startId, sizeof(cl_int)},
            {&spaceSize, sizeof(cl_uint3)},
            {&startPoint, sizeof(cl_float3)},
            {&pointSize, sizeof(cl_float3)},
            {&halfSize, sizeof(cl_float3)},
        };

        KernelArguments args(output, optional);

        return OpenclSystem::Get().Run(modelFunctionName, args);
    }

    bool CalculateImage(Program& program, const Space<Dimensions>& space, int batchSize = 0)
    {
        if (_program != &program)
        {
            GenerateCode(program);
            _program = &program;
            if (!OpenclSystem::Get().Compile(_code))
                return false;
        }

        // TODO: make opencl program "templated" for other dimensions
        cl_int startId = 0;
        cl_uint3 spaceSize = {space.GetPartition()[0], space.GetPartition()[1], space.GetPartition()[2]};
        cl_float3 startPoint = {space.GetStartPoint()[0], space.GetStartPoint()[1], space.GetStartPoint()[2]};
        cl_float3 pointSize = {space.GetUnitSize()[0], space.GetUnitSize()[1], space.GetUnitSize()[2]};
        cl_float3 halfSize = {pointSize.x / 2.f, pointSize.y / 2.f, pointSize.z / 2.f};


        if (_modelBuffer.Size() != 0)
            _modelBuffer.Clear();

        _imageBuffer.Resize(space.GetPartition());

        /*
           global char *resultZones,
           const int startId,
           const uint3 spaceSize,
           const float3 startPoint,
           const float3 pointSize,
           const float3 halfSize)
        */
        KernelArguments::Item output(&_imageBuffer[0], sizeof(_imageBuffer[0]), _imageBuffer.Size());
        std::vector<KernelArguments::Item> optional
        {
            {&startId, sizeof(cl_int)},
            {&spaceSize, sizeof(cl_uint3)},
            {&startPoint, sizeof(cl_float3)},
            {&pointSize, sizeof(cl_float3)},
            {&halfSize, sizeof(cl_float3)},
        };

        KernelArguments args(output, optional);

        return OpenclSystem::Get().Run(imageFunctionName, args);
    }

    FlatArray<char, Dimensions>& GetModel() { return _modelBuffer; }
    FlatArray<std::array<double, Dimensions+2>, Dimensions>& GetImage() { return _imageBuffer; }


protected:
    void GenerateCode(Program& program)
    {
        std::string generatedCode = _codeGenerator.Generate(program);
        std::cout << generatedCode << std::endl;
        std::stringstream codeStream;

        codeStream << codeHeader;
        codeStream << generatedCode;
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
    FlatArray<char, Dimensions> _modelBuffer;
    FlatArray<std::array<double, Dimensions+2>, Dimensions> _imageBuffer;
};


template<int Dimensions>
std::string OpenclCalculator<Dimensions>::modelFunctionName = "__calcualteModel";

template<int Dimensions>
std::string OpenclCalculator<Dimensions>::imageFunctionName = "__calculateMImage";

template<int Dimensions>
CodeGenerator::LanguageDefinition OpenclCalculator<Dimensions>::languageDefenition =
        CodeGenerator::LanguageDefinition()
        .MainFuncName("__resultFunc");

template<int Dimensions>
std::string OpenclCalculator<Dimensions>::codeHeader = R"(
double __matrix4x4Det(double* m, int n)
{
      return
         m[3] * m[1*n+2] * m[2*n+1] * m[3*n+0] - m[0*n+2] * m[1*n+3] * m[2*n+1] * m[3*n+0] -
         m[3] * m[1*n+1] * m[2*n+2] * m[3*n+0] + m[0*n+1] * m[1*n+3] * m[2*n+2] * m[3*n+0] +
         m[2] * m[1*n+1] * m[2*n+3] * m[3*n+0] - m[0*n+1] * m[1*n+2] * m[2*n+3] * m[3*n+0] -
         m[3] * m[1*n+2] * m[2*n+0] * m[3*n+1] + m[0*n+2] * m[1*n+3] * m[2*n+0] * m[3*n+1] +
         m[3] * m[1*n+0] * m[2*n+2] * m[3*n+1] - m[0*n+0] * m[1*n+3] * m[2*n+2] * m[3*n+1] -
         m[2] * m[1*n+0] * m[2*n+3] * m[3*n+1] + m[0*n+0] * m[1*n+2] * m[2*n+3] * m[3*n+1] +
         m[3] * m[1*n+1] * m[2*n+0] * m[3*n+2] - m[0*n+1] * m[1*n+3] * m[2*n+0] * m[3*n+2] -
         m[3] * m[1*n+0] * m[2*n+1] * m[3*n+2] + m[0*n+0] * m[1*n+3] * m[2*n+1] * m[3*n+2] +
         m[1] * m[1*n+0] * m[2*n+3] * m[3*n+2] - m[0*n+0] * m[1*n+1] * m[2*n+3] * m[3*n+2] -
         m[2] * m[1*n+1] * m[2*n+0] * m[3*n+3] + m[0*n+1] * m[1*n+2] * m[2*n+0] * m[3*n+3] +
         m[2] * m[1*n+0] * m[2*n+1] * m[3*n+3] - m[0*n+0] * m[1*n+2] * m[2*n+1] * m[3*n+3] -
         m[1] * m[1*n+0] * m[2*n+2] * m[3*n+3] + m[0*n+0] * m[1*n+1] * m[2*n+2] * m[3*n+3];
}

char __checkZone8(float *values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
    for(int i = 0; i < 8; i++)
    {
        if(values[i] == 0)
            zero = true;
        else if(values[i] < 0)
            minus = true;
        else if(values[i] > 0)
            plus = true;
    }
    if(zero || (plus && minus))
        return 0;
    if(plus)
        return 1;
    return -1;
}
)";

template<int Dimensions>
std::string OpenclCalculator<Dimensions>::codeFooter = R"(
kernel void __calcualteModel(global char *resultZones,
                           const int startId,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    float3 point;
    point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);


    float values[8];
    values[0] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[1] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[2] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[3] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z);
    values[4] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[5] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[6] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[7] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z);

    resultZones[id] = __checkZone8(values);
}

kernel void __calculateMImage(global double *result,
                           const int startId,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    float3 point;
    point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

    double wv[4];
    wv[0] = __resultFunc(point.x,             point.y,             point.z            );
    wv[1] = __resultFunc(point.x+pointSize.x, point.y,             point.z            );
    wv[2] = __resultFunc(point.x,             point.y+pointSize.y, point.z            );
    wv[3] = __resultFunc(point.x,             point.y,             point.z+pointSize.z);

    double a[] = {
        point.y,             point.z,             wv[0], 1,
        point.y,             point.z,             wv[1], 1,
        point.y+pointSize.y, point.z,             wv[2], 1,
        point.y,             point.z+pointSize.z, wv[3], 1
    };
    double b[] = {
        point.x,             point.z,             wv[0], 1,
        point.x+pointSize.x, point.z,             wv[1], 1,
        point.x,             point.z,             wv[2], 1,
        point.x,             point.z+pointSize.z, wv[3], 1
    };
    double c[] = {
        point.x,             point.y,             wv[0], 1,
        point.x+pointSize.x, point.y,             wv[1], 1,
        point.x,             point.y+pointSize.y, wv[2], 1,
        point.x,             point.y,             wv[3], 1
    };
    double d[] = {
        point.x,             point.y,             point.z,              1,
        point.x+pointSize.x, point.y,             point.z,              1,
        point.x,             point.y+pointSize.y, point.z,              1,
        point.x,             point.y,             point.z+pointSize.z, 1
    };
    double f[] = {
        point.x,             point.y,             point.z,             wv[0],
        point.x+pointSize.x, point.y,             point.z,             wv[1],
        point.x,             point.y+pointSize.y, point.z,             wv[2],
        point.x,             point.y,             point.z+pointSize.z, wv[3]
    };
    double detA = __matrix4x4Det(a, 4);
    double detB = __matrix4x4Det(b, 4);
    double detC = __matrix4x4Det(c, 4);
    double detD = __matrix4x4Det(d, 4);
    double detF = __matrix4x4Det(f, 4);
    double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

    result[id*5  ] = detA/div;
    result[id*5+1] = -detB/div;
    result[id*5+2] = -detC/div;
    result[id*5+3] = detD/div;
    result[id*5+4] = detF/div;
}
)";


#endif // OPENCLCALCULATOR_H
