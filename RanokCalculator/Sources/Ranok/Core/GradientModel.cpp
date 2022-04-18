#include "GradientModel.h"


LinearGradient::LinearGradient(const std::vector<glm::fvec4>& colors):
    _stops(colors)
{

}

glm::fvec4 LinearGradient::GetColor(unsigned value)
{
    // Find the "bin" that value falls in
    unsigned range = UINT_MAX;
    float step = range / static_cast<float>(_stops.size() - 1);
    int bin = static_cast<int>(value / step);

    // Normalize value in the interval (0,1]
    float normalized_v = (value - bin * step) / step;

    return Interpolate(_stops[bin], _stops[bin + 1], normalized_v);
}

void LinearGradient::SetColors(const std::vector<glm::fvec4>& colors)
{
    _stops = colors;
}

glm::fvec4 LinearGradient::Interpolate(const glm::fvec4& c1,
                                       const glm::fvec4& c2,
                                       float normalized_value)
{
    if (normalized_value <= 0.0)
        return c1;
    if (normalized_value >= 1.0)
        return c2;

    return {
        (1.0 - normalized_value) * c1.r + normalized_value * c2.r,
        (1.0 - normalized_value) * c1.g + normalized_value * c2.g,
        (1.0 - normalized_value) * c1.b + normalized_value * c2.b,
        (1.0 - normalized_value) * c1.a + normalized_value * c2.a
    };
}
