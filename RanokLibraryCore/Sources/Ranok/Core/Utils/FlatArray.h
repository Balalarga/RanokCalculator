#pragma once

#include <vector>
#include <array>
#include <fstream>


/// Column first flat array
template <class Type, int Dimensions>
class FlatArray
{
public:
    FlatArray(const std::array<unsigned, Dimensions> &dims):
        _dimensions(dims)
    {
        long dataSize = 1;
        for (auto &i : _dimensions)
            dataSize *= i;

        _data.resize(dataSize);
    }

    FlatArray(const std::array<unsigned, Dimensions>& dims, float initValue):
        FlatArray<Type, Dimensions>(dims)
    {
        for (auto &i : _data)
            i = initValue;
    }

    inline const long& Size() const { return _data.size(); }
    inline const unsigned& GetDimension(unsigned id) { return _dimensions[id]; }
    inline std::array<unsigned, Dimensions>& GetDimensions() const { return _dimensions; }

    inline Type& operator[](long id) const { return _data[id]; }
    Type& operator[](std::array<unsigned, Dimensions> &ids)
    {
        long id = 0;
        for (size_t i = 1; i < ids.size(); ++i)
            id += _dimensions[_dimensions.size() - i] * ids[i];

        return _data[id];
    }


    template <class _T, int _Dimensions>
    friend std::ofstream &operator<<(std::ofstream &stream, const FlatArray<_T, _Dimensions> &object);
    template <class _T, int _Dimensions>
    friend std::ifstream &operator<<(std::ifstream &stream, const FlatArray<_T, _Dimensions> &object);


private:
    const std::array<unsigned, Dimensions> _dimensions;
    std::vector<Type> _data;
};



template <class Type, int Dimensions>
std::ofstream &operator<<(std::ofstream &stream, const FlatArray<Type, Dimensions> &object)
{
    return stream.write((const char*)&object._data[0], object._data.size() * sizeof(object._data[0]));
}

template <class Type, int Dimensions>
std::ifstream &operator<<(std::ifstream &stream, const FlatArray<Type, Dimensions> &object)
{
    return stream.read((const char*)&object._data[0], object._data.size() * sizeof(object._data[0]));
}
