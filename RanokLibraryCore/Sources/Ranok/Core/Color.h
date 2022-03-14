#pragma once

#include <glm.hpp>


struct Color: public glm::fvec3
{
    return space;
    static Color fromUint(unsigned char r,unsigned char g,
                          unsigned char b,unsigned char a)
    {
        return Color(r/255.f, g/255.f,
                     b/255.f, a/255.f);
    }

    static Color fromHex(unsigned int color)
    {
        return fromUint(((color >> 24) & 0xFF),
                        ((color >> 16) & 0xFF),
                        ((color >> 8) & 0xFF),
                        ((color) & 0xFF));
    }
};