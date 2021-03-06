#include "OpenclCalculator.h"


std::string OpenclCalculator::modelFunctionName = "__calcualteModel";

std::string OpenclCalculator::imageFunctionName = "__calculateMImage";

CodeGenerator::LanguageDefinition OpenclCalculator::languageDefenition =
        CodeGenerator::LanguageDefinition()
        .Functions({{"abs", "fabs"}})
        .MainFuncName("__resultFunc")
        .ArrayInitialization("{{{2}}}")
        .ArrayReturnAsParam(true)
        .NumberArrayType("double*")
        .FillResultArray([](const std::string& varName, const std::vector<std::string>& params) -> std::string
        {
            std::stringstream stream;

            for (size_t i = 0; i < params.size(); ++i)
            {
                stream << varName << "[" << i << "]" << " = " << params[i];
                if (i + 1 < params.size())
                    stream << ";\n";
            }

            return stream.str();
        });

std::string OpenclCalculator::codeHeader = R"(

struct __attribute__ ((packed)) MImage3D
{
    double c[5];
    char zone;
};
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

char __checkZone8(double *values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
    for(int i = 0; i < 8; i++)
    {
        if(values[i] == 0.0)
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

std::string OpenclCalculator::codeFooter = R"(
kernel void __calculateMImage(global struct MImage3D *result,
                           const int startId,
                           const uint3 spaceSize,
                           const double3 startPoint,
                           const double3 pointSize,
                           const double3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    double3 point;
    point.x = startPoint.x + pointSize.x * (spaceId / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * ((spaceId / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (spaceId % spaceSize.z);

    double wv[4];
    double params1[3] = {point.x, point.y, point.z };
    wv[0] = __resultFunc(params1);
    double params2[3] = {point.x+pointSize.x, point.y, point.z };
    wv[1] = __resultFunc(params2);
    double params3[3] = {point.x, point.y + pointSize.y, point.z };
    wv[2] = __resultFunc(params3);
    double params4[3] = {point.x, point.y, point.z + pointSize.z};
    wv[3] = __resultFunc(params4);

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

    result[id].c[0] = detA/div;
    result[id].c[1] = -detB/div;
    result[id].c[2] = -detC/div;
    result[id].c[3] = detD/div;
    result[id].c[4] = detF/div;


    double values[8];
    double zoneParams1[3] = {point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z};
    values[0] = __resultFunc(zoneParams1);
    double zoneParams2[3] = {point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z};
    values[1] = __resultFunc(zoneParams2);
    double zoneParams3[3] = {point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z};
    values[2] = __resultFunc(zoneParams3);
    double zoneParams4[3] = {point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z};
    values[3] = __resultFunc(zoneParams4);
    double zoneParams5[3] = {point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z};
    values[4] = __resultFunc(zoneParams5);
    double zoneParams6[3] = {point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z};
    values[5] = __resultFunc(zoneParams6);
    double zoneParams7[3] = {point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z};
    values[6] = __resultFunc(zoneParams7);
    double zoneParams8[3] = {point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z};
    values[7] = __resultFunc(zoneParams8);
    result[id].zone = __checkZone8(values);
}
)";

OpenclCalculator::OpenclCalculator():
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

glm::fvec4 OpenclCalculator::GetImageColor(double value)
{
    return _imageGradient.GetColor(UINT_MAX * (1.0 + value) / 2.0);
}

glm::fvec4 OpenclCalculator::GetModelColor()
{
    return _modelColor;
}

void OpenclCalculator::SetImageGradient(const LinearGradient &imageGradient)
{
    _imageGradient = imageGradient;
}

bool OpenclCalculator::Calculate(Program &program,
                                 const Space &space,
                                 const std::function<void (size_t, size_t)> &onDone,
                                 size_t batchSize)
{
    if (_program != &program)
    {
        GenerateCode(program);
        _program = &program;
        if (!OpenclSystem::Get().Compile(_code))
            return false;
    }

    cl_int startId = 0;
    cl_uint3 spaceSize = {static_cast<unsigned>(space.GetPartition()[0]),
                          static_cast<unsigned>(space.GetPartition()[1]),
                          static_cast<unsigned>(space.GetPartition()[2])};
    cl_double3 startPoint = {space.GetStartPoint()[0], space.GetStartPoint()[1], space.GetStartPoint()[2]};
    cl_double3 pointSize = {space.GetUnitSize()[0], space.GetUnitSize()[1], space.GetUnitSize()[2]};
    cl_double3 halfSize = {pointSize.x / 2.f, pointSize.y / 2.f, pointSize.z / 2.f};

    size_t bufferSize = space.GetTotalPartition();
    if (batchSize != 0 && batchSize < bufferSize)
        bufferSize = batchSize;

    _imageBuffer.Resize(bufferSize);
    for (size_t i = 0; i < bufferSize; ++i)
        _imageBuffer[i] = {0, 0, 0, 0, 0};

    /*
       global MImageData *resultZones,
       const int startId,
       const uint3 spaceSize,
       const double3 startPoint,
       const double3 pointSize,
       const double3 halfSize)
    */
    std::vector<KernelArguments::Item> optional
    {
        {&startId, sizeof(cl_int)},
        {&spaceSize, sizeof(cl_uint3)},
        {&startPoint, sizeof(cl_double3)},
        {&pointSize, sizeof(cl_double3)},
        {&halfSize, sizeof(cl_double3)},
    };

    KernelArguments::Item imageOutput(&_imageBuffer[0], sizeof(_imageBuffer[0]), _imageBuffer.Size());
    bool ok = false;
    size_t spaceFlatSize = space.GetTotalPartition();
    for (size_t mstartId = 0; mstartId < spaceFlatSize; mstartId += bufferSize)
    {
        ok = OpenclSystem::Get().Run(imageFunctionName, KernelArguments(imageOutput, optional));

        if (!ok)
            break;

        if (mstartId + bufferSize > spaceFlatSize)
            bufferSize = spaceFlatSize - mstartId;

        onDone(mstartId, bufferSize);
    }

    return ok;
}
