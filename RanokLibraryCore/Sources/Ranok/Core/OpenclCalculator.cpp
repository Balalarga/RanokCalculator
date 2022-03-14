#include "OpenclCalculator.h"

#include "OpenclSystem.h"


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
int __checkZone(double *values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
    for(int i = 0; i < 8; i++)
    {
        if(values[i] == 0) zero = true;
        else if(values[i] < 0) minus = true;
        else if(values[i] > 0) plus = true;
    }
    if(zero || (plus && minus)) return 0;
    if(plus) return 1;
    return -1;
}
)";

template<int Dimensions>
std::string OpenclCalculator<Dimensions>::codeFooter = R"(
kernel void __calcualteModel(global int *resultZones,
                             const int startId,
                             const uint3 spaceSize,
                             const float3 startPoint,
                             const float3 pointSize,
                             const float3 halfSize)
{
    int id = get_global_id(0);
    int spaceId = startId + id;
    float3 point;
    point.x=startPoint.x+pointSize.x*(spaceId/(spaceSize.z*spaceSize.y));
    point.y=startPoint.y+pointSize.y*((spaceId/spaceSize.z)%spaceSize.y);
    point.z=startPoint.z+pointSize.z*(spaceId%spaceSize.z);
    double values[8];
    values[0]=__resultFunc(point.x+halfSize.x,point.y+halfSize.y,point.z+halfSize.z);
    values[1]=__resultFunc(point.x+halfSize.x,point.y+halfSize.y,point.z-halfSize.z);
    values[2]=__resultFunc(point.x+halfSize.x,point.y-halfSize.y,point.z+halfSize.z);
    values[3]=__resultFunc(point.x+halfSize.x,point.y-halfSize.y,point.z-halfSize.z);
    values[4]=__resultFunc(point.x-halfSize.x,point.y+halfSize.y,point.z+halfSize.z);
    values[5]=__resultFunc(point.x-halfSize.x,point.y+halfSize.y,point.z-halfSize.z);
    values[6]=__resultFunc(point.x-halfSize.x,point.y-halfSize.y,point.z+halfSize.z);
    values[7]=__resultFunc(point.x-halfSize.x,point.y-halfSize.y,point.z-halfSize.z);
    resultZones[id] = __checkZone(values);
}
kernel void __calculateMImage(global double *result,
                           const int startId,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id=get_global_id(0);
    int spaceId=startId+id;
    float3 point;
    point.x=startPoint.x+pointSize.x*(spaceId/(spaceSize.z*spaceSize.y));
    point.y=startPoint.y+pointSize.y*((spaceId/spaceSize.z)%spaceSize.y);
    point.z=startPoint.z+pointSize.z*(spaceId%spaceSize.z);
    double wv[4];
    wv[0] = __resultFunc(point.x,point.y,point.);
    wv[1] = __resultFunc(point.x+pointSize.x,point.y,point.);
    wv[2] = __resultFunc(point.x,point.y+pointSize.y,point.);
    wv[3] = __resultFunc(point.x,point.y,point.z+pointSize.z);
    double a[]={
        point.y,point.z,wv[0],1,
        point.y,point.z,wv[1],1,
        point.y+pointSize.y,point.z,wv[2],1,
        point.y,point.z+pointSize.z,wv[3],1
    };
    double b[]={
        point.x,point.z,wv[0],1,
        point.x+pointSize.x,point.z,wv[1],1,
        point.x,point.z,wv[2],1,
        point.x,point.z+pointSize.z,wv[3],1
    };
    double c[]={
        point.x,             point.y,             wv[0], 1,
        point.x+pointSize.x, point.y,             wv[1], 1,
        point.x,             point.y+pointSize.y, wv[2], 1,
        point.x,             point.y,             wv[3], 1
    };
    double d[]={
        point.x,point.y,point.z,1,
        point.x+pointSize.x,point.y,point.z,1,
        point.x,point.y+pointSize.y,point.z,1,
        point.x,point.y,point.z+pointSize.z,1
    };
    double f[]={
        point.x,point.y,point.z,wv[0],
        point.x+pointSize.x,point.y,point.z,wv[1],
        point.x,point.y+pointSize.y,point.z,wv[2],
        point.x,point.y,point.z+pointSize.z,wv[3]
    };
    double detA=__matrix4x4Det(a,4);
    double detB=__matrix4x4Det(b,4);
    double detC=__matrix4x4Det(c,4);
    double detD=__matrix4x4Det(d,4);
    double detF=__matrix4x4Det(f,4);
    double div=sqrt(pow(detA,2)+pow(detB,2)+pow(detC,2)+pow(detD,2)+pow(detF,2));
    result[id*5]=detA/div;
    result[id*5+1]=-detB/div;
    result[id*5+2]=-detC/div;
    result[id*5+3]=detD/div;
    result[id*5+4]=detF/div;
}
)";


template<int Dimensions>
OpenclCalculator<Dimensions>::OpenclCalculator():
    _modelColor(ColorFromUint(255, 255, 255, 20)),
    _program(nullptr),
    _imageGradient({
                   ColorFromUint(255, 255, 0,   20),
                   ColorFromUint(0,   255, 162, 20),
                   ColorFromUint(0,   0,   255, 20),
                   ColorFromUint(255, 145, 0,   20),
                   ColorFromUint(214, 0,   255, 20),
                   })
{

}

template<int Dimensions>
void OpenclCalculator<Dimensions>::GenerateCode(Program &program)
{
    std::stringstream codeStream;

    codeStream << codeHeader;
    codeStream << _codeGenerator.Generate(program);
    codeStream << codeFooter;

    _code = codeStream.str();
}

template<int Dimensions>
glm::fvec4 OpenclCalculator<Dimensions>::GetImageColor(double value)
{
    return _imageGradient.GetColor(UINT_MAX * (1.0 + value) / 2.0);
}

template<int Dimensions>
glm::fvec4 OpenclCalculator<Dimensions>::GetModelColor()
{
    return _modelColor;
}

template<int Dimensions>
void OpenclCalculator<Dimensions>::SetImageGradient(const LinearGradient &imageGradient)
{
    _imageGradient = imageGradient;
}

template<int Dimensions>
void OpenclCalculator<Dimensions>::CalculateModel(Program &program, const Space<Dimensions> &space, int batchSize)
{
    if (_program != &program)
    {
        GenerateCode(program);
        _program = &program;
    }

    cl_int startId = 0;
    // TODO: make opencl program "templated" for other dimensions
    cl_uint3 spaceSize = {space.GetSize()[0], space.GetSize()[1], space.GetSize()[2]};
    cl_float3 startPoint = {};
    cl_float3 pointSize = {};
    cl_float3 halfSize = {};

    FlatArray<class Space<Dimensions>::ModelData, Dimensions>* buffer = _modelBuffer.get();

    KernelArguments::Item output(&buffer[0], sizeof(buffer[0]), buffer->Size());
    std::vector<KernelArguments::Item> optional
    {





    };

    KernelArguments args(output);
}

template<int Dimensions>
void OpenclCalculator<Dimensions>::CalculateImage(Program &program, const Space<Dimensions> &space, int batchSize)
{

}

