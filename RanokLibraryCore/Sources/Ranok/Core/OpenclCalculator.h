#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include <iostream>

#include "Space.h"
#include "OpenclSystem.h"
#include "GradientModel.h"
#include "Utils/FlatArray.h"

#include <Ranok/LanguageCore/CodeGenerator.h>


//class OpenclMock
//{
//public:
//    OpenclMock() = delete;
//    static double matrix4x4Det(double* m, int n)
//    {
//          return
//             m[3] * m[1*n+2] * m[2*n+1] * m[3*n+0] - m[0*n+2] * m[1*n+3] * m[2*n+1] * m[3*n+0] -
//             m[3] * m[1*n+1] * m[2*n+2] * m[3*n+0] + m[0*n+1] * m[1*n+3] * m[2*n+2] * m[3*n+0] +
//             m[2] * m[1*n+1] * m[2*n+3] * m[3*n+0] - m[0*n+1] * m[1*n+2] * m[2*n+3] * m[3*n+0] -
//             m[3] * m[1*n+2] * m[2*n+0] * m[3*n+1] + m[0*n+2] * m[1*n+3] * m[2*n+0] * m[3*n+1] +
//             m[3] * m[1*n+0] * m[2*n+2] * m[3*n+1] - m[0*n+0] * m[1*n+3] * m[2*n+2] * m[3*n+1] -
//             m[2] * m[1*n+0] * m[2*n+3] * m[3*n+1] + m[0*n+0] * m[1*n+2] * m[2*n+3] * m[3*n+1] +
//             m[3] * m[1*n+1] * m[2*n+0] * m[3*n+2] - m[0*n+1] * m[1*n+3] * m[2*n+0] * m[3*n+2] -
//             m[3] * m[1*n+0] * m[2*n+1] * m[3*n+2] + m[0*n+0] * m[1*n+3] * m[2*n+1] * m[3*n+2] +
//             m[1] * m[1*n+0] * m[2*n+3] * m[3*n+2] - m[0*n+0] * m[1*n+1] * m[2*n+3] * m[3*n+2] -
//             m[2] * m[1*n+1] * m[2*n+0] * m[3*n+3] + m[0*n+1] * m[1*n+2] * m[2*n+0] * m[3*n+3] +
//             m[2] * m[1*n+0] * m[2*n+1] * m[3*n+3] - m[0*n+0] * m[1*n+2] * m[2*n+1] * m[3*n+3] -
//             m[1] * m[1*n+0] * m[2*n+2] * m[3*n+3] + m[0*n+0] * m[1*n+1] * m[2*n+2] * m[3*n+3];
//    }

//    static char checkZone8(double *values)
//    {
//        bool plus = false;
//        bool zero = false;
//        bool minus = false;
//        for(int i = 0; i < 8; i++)
//        {
//            if(values[i] == 0)
//                zero = true;
//            else if(values[i] < 0)
//                minus = true;
//            else if(values[i] > 0)
//                plus = true;
//        }
//        if(zero || (plus && minus))
//            return 0;
//        if(plus)
//            return 1;
//        return -1;
//    }

//    static void ExecModelTimes(std::function<double(cl_double3)> func,
//                               unsigned times,
//                               char* resultZones,
//                               int startId,
//                               cl_uint3 spaceSize,
//                               cl_double3 startPoint,
//                               cl_double3 pointSize,
//                               cl_double3 halfSize)
//    {
//        for(unsigned i = 0; i < times; ++i)
//            ExecModel(func, i, resultZones, startId, spaceSize, startPoint, pointSize, halfSize);
//    }

//    static void ExecImageTimes(std::function<double(cl_double3)> func,
//                               unsigned times,
//                               double* result,
//                               int startId,
//                               cl_uint3 spaceSize,
//                               cl_double3 startPoint,
//                               cl_double3 pointSize,
//                               cl_double3 halfSize)
//    {
//        for(unsigned i = 0; i < times; ++i)
//            ExecImage(func, i, result, startId, spaceSize, startPoint, pointSize, halfSize);
//    }

//    static void ExecModel(std::function<double(cl_double3)> func,
//                          unsigned id,
//                          char*& resultZones,
//                          int startId,
//                          cl_uint3 spaceSize,
//                          cl_double3 startPoint,
//                          cl_double3 pointSize,
//                          cl_double3 halfSize)
//    {
//        int spaceId = startId + id;
//        cl_double3 point;
//        point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
//        point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
//        point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

//    //        std::cout << "Params: " << "\n";
//    //        std::cout << "    startId: " << startId << "\n";
//    //        std::cout << "    spaceSize: "  << spaceSize.x << ", " << spaceSize.y << ", " << spaceSize.z << "\n";
//    //        std::cout << "    startPoint: " << startPoint.x << ", " << startPoint.y << ", " << startPoint.z << "\n";
//    //        std::cout << "    pointSize: "  << pointSize.x << ", " << pointSize.y << ", " << pointSize.z << "\n";
//    //        std::cout << "    halfSize: "   << halfSize.x << ", " << halfSize.y << ", " << halfSize.z << "\n";
//    //        std::cout << "    point: "   << point.x << ", " << point.y << ", " << point.z << "\n";

//        double values[8];
//        values[0] = func({point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z});
//        values[1] = func({point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z});
//        values[2] = func({point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z});
//        values[3] = func({point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z});
//        values[4] = func({point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z});
//        values[5] = func({point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z});
//        values[6] = func({point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z});
//        values[7] = func({point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z});

//        resultZones[id] = checkZone8(values);
//    }

//    static void ExecImage(std::function<double(cl_double3)> func,
//                            unsigned id,
//                            double* result,
//                            int startId,
//                            cl_uint3 spaceSize,
//                            cl_double3 startPoint,
//                            cl_double3 pointSize,
//                            cl_double3 halfSize)
//    {
//        int spaceId = startId + id;
//        cl_double3 point;
//        point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
//        point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
//        point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

//        double wv[4];
//        wv[0] = func({point.x,             point.y,             point.z            });
//        wv[1] = func({point.x+pointSize.x, point.y,             point.z            });
//        wv[2] = func({point.x,             point.y+pointSize.y, point.z            });
//        wv[3] = func({point.x,             point.y,             point.z+pointSize.z});

//        double a[] = {
//            point.y,             point.z,             wv[0], 1,
//            point.y,             point.z,             wv[1], 1,
//            point.y+pointSize.y, point.z,             wv[2], 1,
//            point.y,             point.z+pointSize.z, wv[3], 1
//        };
//        double b[] = {
//            point.x,             point.z,             wv[0], 1,
//            point.x+pointSize.x, point.z,             wv[1], 1,
//            point.x,             point.z,             wv[2], 1,
//            point.x,             point.z+pointSize.z, wv[3], 1
//        };
//        double c[] = {
//            point.x,             point.y,             wv[0], 1,
//            point.x+pointSize.x, point.y,             wv[1], 1,
//            point.x,             point.y+pointSize.y, wv[2], 1,
//            point.x,             point.y,             wv[3], 1
//        };
//        double d[] = {
//            point.x,             point.y,             point.z,              1,
//            point.x+pointSize.x, point.y,             point.z,              1,
//            point.x,             point.y+pointSize.y, point.z,              1,
//            point.x,             point.y,             point.z+pointSize.z, 1
//        };
//        double f[] = {
//            point.x,             point.y,             point.z,             wv[0],
//            point.x+pointSize.x, point.y,             point.z,             wv[1],
//            point.x,             point.y+pointSize.y, point.z,             wv[2],
//            point.x,             point.y,             point.z+pointSize.z, wv[3]
//        };
//        double detA = matrix4x4Det(a, 4);
//        double detB = matrix4x4Det(b, 4);
//        double detC = matrix4x4Det(c, 4);
//        double detD = matrix4x4Det(d, 4);
//        double detF = matrix4x4Det(f, 4);
//        double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

//        result[id*5  ] = detA/div;
//        result[id*5+1] = -detB/div;
//        result[id*5+2] = -detC/div;
//        result[id*5+3] = detD/div;
//        result[id*5+4] = detF/div;
//    }
//};


enum class CalculateTarget: uint8_t
{
    Model,
    Image
};

struct MImage3D
{
    double C0, C1, C2, C3, C4;
};

class OpenclCalculator
{
public:
    OpenclCalculator();

    glm::fvec4 GetImageColor(double value);
    glm::fvec4 GetModelColor();

    void SetImageGradient(const LinearGradient& imageGradient);

    bool Calculate(CalculateTarget target,
                   Program& program,
                   const Space& space,
                   const std::function<void(size_t, size_t)>& onDone,
                   size_t batchSize = 0);

    inline const CalculateTarget& GetLastTarget() { return _lastTarget; }
    inline FlatArray<char>& GetModel() { return _modelBuffer; }
    inline FlatArray<MImage3D>& GetImage() { return _imageBuffer; }


protected:
    void GenerateCode(Program& program)
    {
        std::stringstream codeStream;

        codeStream << codeHeader;
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
    CalculateTarget _lastTarget;
    FlatArray<char> _modelBuffer;
    FlatArray<MImage3D> _imageBuffer;
};


#endif // OPENCLCALCULATOR_H
