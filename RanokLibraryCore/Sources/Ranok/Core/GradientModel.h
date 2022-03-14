#ifndef GRADIENTMODEL_H
#define GRADIENTMODEL_H

#include <vector>
#include <glm.hpp>


static glm::fvec4 ColorFromUint(unsigned char r,unsigned char g,
                                unsigned char b,unsigned char a)
{
    return glm::fvec4(r/255.f, g/255.f,
                      b/255.f, a/255.f);
}

static glm::fvec4 ColorFromHex(unsigned int color)
{
    return ColorFromUint(((color >> 24) & 0xFF),
                         ((color >> 16) & 0xFF),
                         ((color >> 8) & 0xFF),
                         ((color) & 0xFF));
}


class LinearGradient
{
public:
    LinearGradient(const std::vector<glm::fvec4>& colors = {});

    glm::fvec4 GetColor(unsigned value);

    void SetColors(const std::vector<glm::fvec4>& colors);


protected:
    glm::fvec4 Interpolate(const glm::fvec4& c1,
                           const glm::fvec4& c2,
                           float normalized_value);


private:
    std::vector<glm::fvec4> _stops;
};

#endif // GRADIENTMODEL_H
