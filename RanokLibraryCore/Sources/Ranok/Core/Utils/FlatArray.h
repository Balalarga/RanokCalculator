#pragma once

#include <vector>
#include <array>
#include <fstream>


/// Column first flat array
template <class Type>
class FlatArray
{
public:
    FlatArray(unsigned count = 0):
        _dimensions(count)
    {
    }

    FlatArray(const std::vector<unsigned> &dims):
        _dimensions(dims)
    {
        Resize(dims);
    }

    FlatArray(const std::vector<unsigned>& dims, float initValue):
        FlatArray<Type>(dims)
    {
        for (auto &i : _data)
            i = initValue;
    }
    
    void Resize(const std::vector<unsigned>& dims)
    {
        _dimensions = dims;
        long dataSize = 1;
        for (auto &i : dims)
            dataSize *= i;

        _data.resize(dataSize);
    }

    void Resize(unsigned size)
    {
        _data.resize(size);
    }

    void Clear(const std::vector<unsigned>& dims = {})
    {
        if (!dims.empty())
            Resize(dims);
        else
            _data.clear();
    }

    inline long Size() const { return _data.size(); }
    inline const unsigned& GetDimension(unsigned id) { return _dimensions[id]; }
    inline std::vector<unsigned>& GetDimensions() { return _dimensions; }

    inline Type& operator[](long id) { return _data[id]; }
    Type& operator[](std::vector<unsigned> &ids)
    {
        long id = 0;
        for (size_t i = 1; i < ids.size(); ++i)
            id += _dimensions[_dimensions.size() - i] * ids[i];

        return _data[id];
    }


    template <class _T>
    friend std::ofstream &operator<<(std::ofstream &stream, const FlatArray<_T> &object);
    template <class _T>
    friend std::ifstream &operator>>(std::ifstream &stream, FlatArray<_T> &object);


private:
    std::vector<unsigned> _dimensions;
    std::vector<Type> _data;
};



template <class Type>
std::ofstream &operator<<(std::ofstream &stream, const FlatArray<Type> &object)
{
    size_t dimsCount = object._dimensions.size();
    stream.write((char*)&dimsCount, sizeof(dimsCount));
    stream.write((char*)&object._dimensions[0], dimsCount * sizeof(object._dimensions[0]));

    size_t dataCount = object._data.size();
    stream.write((char*)&dataCount, sizeof(dataCount));
    stream.write((char*)&object._data[0], dataCount * sizeof(object._data[0]));
    return stream;
}

template <class Type>
std::ifstream &operator>>(std::ifstream &stream, FlatArray<Type> &object)
{
    size_t dimsCount;
    stream.read((char*)&dimsCount, sizeof(dimsCount));
    object._dimensions.resize(dimsCount);
    stream.read((char*)&object._dimensions[0], dimsCount * sizeof(object._dimensions[0]));

    size_t dataCount;
    stream.read((char*)&dataCount, sizeof(dataCount));
    object._data.resize(dataCount);
    stream.read((char*)&object._data[0], dataCount * sizeof(object._data[0]));
    
    return stream;
}
