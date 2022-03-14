#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>


/// Column first flat array
template <class T>
class FlatArray
{
public:
    FlatArray(const std::vector<unsigned> &dims) : _dimensions(dims)
    {
        long dataSize = 0;
        for (auto &i : _dimensions)
            dataSize += i;

        _data.resize(dataSize);
    }
    FlatArray(const std::vector<unsigned> &dims, float initValue) : FlatArray<T>(dims)
    {
        for (auto &i : _data)
            i = rand() % 100;
    }

    long Size() const { return _data.size(); }

    unsigned GetDimension(unsigned id)
    {
        return _dimensions.at(id);
    }
    const std::vector<unsigned> &GetDimensions() const { return _dimensions; }

    T& operator[](long id)
    {
        return _data.at(id);
    }

    T& operator[](std::vector<unsigned> &ids)
    {
        assert(ids.size() == _dimensions.size());

        long id = 0;
        for (size_t i = 1; i < ids.size(); ++i)
            id += _dimensions.at(_dimensions.size() - i) * ids.at(i);

        return _data.at(id);
    }

    template <class _T>
    friend std::ostream &operator<<(std::ostream &stream, const FlatArray<_T> &object)
    {
        for(size_t i = 0; i < object._data.size(); ++i)
        {
            stream << object._data[i] << " ";
            if ((i+1) % object._dimensions[0] == 0)
                stream << "\n";
        }
        return stream;
    }

    template <class _T>
    friend std::istream &operator>>(std::istream &stream, FlatArray<_T> &object)
    {
        return stream;
    }

    template <class _T>
    friend std::ofstream &operator<<(std::ofstream &stream, const FlatArray<_T> &object)
    {
        stream.write((const char*)&object._data[0], object._data.size() * sizeof(object._data[0]));
        return stream;
    }

    template <class _T>
    friend std::ifstream &operator>>(std::ifstream &stream, FlatArray<_T> &object)
    {
        stream.read(object._data.begin(), object._data.size() * sizeof(object._data[0]));
        return stream;
    }


private:
    const std::vector<unsigned> _dimensions;
    std::vector<T> _data;
};