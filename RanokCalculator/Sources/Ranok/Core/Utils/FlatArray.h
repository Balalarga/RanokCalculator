#pragma once

#include <vector>
#include <array>
#include <fstream>


/// Column first flat array
template <class Type>
class FlatArray
{
public:
    FlatArray(size_t count = 0):
        _dimensions(count)
    {
    }

    FlatArray(const std::vector<size_t> &dims):
        _dimensions(dims)
    {
        Resize(dims);
    }

    FlatArray(const std::vector<size_t>& dims, float initValue):
        FlatArray<Type>(dims)
    {
        for (auto &i : _data)
            i = initValue;
    }
    
    void Resize(const std::vector<size_t>& dims)
    {
        _dimensions = dims;
        long dataSize = 1;
        for (auto &i : dims)
            dataSize *= i;

        _data.resize(dataSize);
    }

    inline void Resize(size_t size)
    {
        _data.resize(size);
    }

    void Clear(const std::vector<size_t>& dims = {})
    {
        if (!dims.empty())
            Resize(dims);
        else
            _data.clear();
    }

    inline size_t Size() const { return _data.size(); }
    inline const size_t& GetDimension(size_t id) { return _dimensions[id]; }
    inline std::vector<size_t>& GetDimensions() { return _dimensions; }

    inline Type& operator[](size_t id) { return _data[id]; }
    Type& operator[](std::vector<size_t> &ids)
    {
        long id = 0;
        for (size_t i = 1; i < ids.size(); ++i)
            id += _dimensions[_dimensions.size() - i] * ids[i];

        return _data[id];
    }

    bool WritePart(std::ofstream& stream, size_t count)
    {
        if (count > _data.size())
            return false;

        stream.write((char*)&_data[0], count * sizeof(_data[0]));

        return true;
    }

    /// Reading count items from stream without metasize
    bool ReadSome(std::ifstream& stream, size_t count)
    {
        if (count > _data.size())
            return false;

        stream.read((char*)&_data[0], count * sizeof(_data[0]));

        return true;
    }

    template <class _T>
    friend std::ofstream &operator<<(std::ofstream &stream, const FlatArray<_T> &object);
    template <class _T>
    friend std::ifstream &operator>>(std::ifstream &stream, FlatArray<_T> &object);


private:
    std::vector<size_t> _dimensions;
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
    stream.read((char*)&object._dimensions[0], dimsCount * sizeof(object._dimensions[0]));

    size_t dataCount;
    stream.read((char*)&dataCount, sizeof(dataCount));
    stream.read((char*)&object._data[0], dataCount * sizeof(object._data[0]));

    return stream;
}
