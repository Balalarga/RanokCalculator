#pragma once

#include <glm.hpp>
#include <array>
#include <fstream>


template<int Dimensions>
class Space
{
public:
    Space(const std::array<double, Dimensions> &centerPoint,
          const std::array<double, Dimensions> &size,
          const unsigned& recursiveDepth)
    {
        _size = size;
        _centerPoint = centerPoint;
        _partition.fill(std::pow(2, recursiveDepth));
        UpdateStartPoint();
    }

    Space(const std::array<double, Dimensions> &centerPoint,
          const std::array<double, Dimensions> &size,
          const std::array<unsigned, Dimensions> partition = std::array<unsigned, Dimensions>(1))
    {
        _size = size;
        _partition = partition;
        _centerPoint = centerPoint;
        UpdateStartPoint();
    }

    Space(const Space &oth) = default;
    virtual ~Space() = default;


    inline const std::array<double, Dimensions>& GetSize() const { return _size; }
    inline const std::array<double, Dimensions>& GetCentral() const { return _centerPoint; }
    inline const std::array<unsigned, Dimensions>& GetPartition() const { return _partition; }
    inline const std::array<double, Dimensions>& GetStartPoint() const { return _startPoint; }
    inline unsigned GetTotalPartition() const { return _partition[0] * _partition[1] * _partition[2]; }


    std::array<double, Dimensions> GetUnitSize() const
    {
        std::array<double, Dimensions> unitSizes;

        for (size_t i = 0; i < Dimensions; ++i)
            unitSizes[i] = _size[i] / _partition[i];

        return unitSizes;
    }


    void SetPartition(const std::array<unsigned, Dimensions>& partition)
    {
        _partition = partition;
    }
    void SetPartition(const unsigned& partition)
    {
        _partition = { partition, partition, partition };
    }

    void SetStartPoint(const std::array<double, Dimensions>& point)
    {
        _startPoint = point;
        UpdateCenterPoint();
    }
    void SetCenterPoint(const std::array<double, Dimensions>& point)
    {
        _centerPoint = point;
        UpdateStartPoint();
    }
    void SetSize(const std::array<double, Dimensions>& size)
    {
        _size = size;
    }

    template <int _Dimensions>
    friend std::ofstream &operator<<(std::ofstream &stream, const Space<_Dimensions>& space);
    template <int _Dimensions>
    friend std::ifstream &operator>>(std::ifstream &stream, Space<_Dimensions>& space);


protected:
    void UpdateCenterPoint()
    {
        for(size_t i = 0; i < Dimensions; ++i)
            _centerPoint[i] = _startPoint[i] + _size[i] / 2.f;
    }

    void UpdateStartPoint()
    {
        for(size_t i = 0; i < Dimensions; ++i)
            _startPoint[i] = _centerPoint[i] - _size[i] / 2.f;
    }


private:
    std::array<double, Dimensions> _size;
    std::array<double, Dimensions> _centerPoint;
    std::array<double, Dimensions> _startPoint;
    std::array<unsigned, Dimensions> _partition;
};

template <int Dimensions>
std::ofstream &operator<<(std::ofstream &stream, const Space<Dimensions>& space)
{
    stream.write((char*)&space._size, sizeof(space._size));
    stream.write((char*)&space._centerPoint, sizeof(space._centerPoint));
    stream.write((char*)&space._startPoint, sizeof(space._startPoint));
    stream.write((char*)&space._partition, sizeof(space._partition));
    return stream;
}


template <int Dimensions>
std::ifstream &operator>>(std::ifstream &stream, Space<Dimensions>& space)
{
    stream.read((char*)&space._size, sizeof(space._size));
    stream.read((char*)&space._centerPoint, sizeof(space._centerPoint));
    stream.read((char*)&space._startPoint, sizeof(space._startPoint));
    stream.read((char*)&space._partition, sizeof(space._partition));
    return stream;
}
