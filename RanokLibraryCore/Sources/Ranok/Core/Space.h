#pragma once

#include <glm.hpp>
#include <array>
#include <fstream>


template<int Dimensions>
class Space
{
public:
    Space(const std::array<float, Dimensions> &centerPoint,
          const std::array<float, Dimensions> &size,
          const unsigned& recursiveDepth)
    {
        _size = size;
        _centerPoint = centerPoint;
        _partition.fill(std::pow(2, recursiveDepth));
        UpdateStartPoint();
    }

    Space(const std::array<float, Dimensions> &centerPoint,
          const std::array<float, Dimensions> &size,
          const std::array<unsigned, Dimensions> partition = std::array<unsigned, Dimensions>(1))
    {
        _size = size;
        _partition = partition;
        _centerPoint = centerPoint;
        UpdateStartPoint();
    }

    Space(const Space &oth) = default;
    virtual ~Space() = default;


    inline const std::array<float, Dimensions>& GetSize() const { return _size; }
    inline const std::array<float, Dimensions>& GetCentral() const { return _centerPoint; }
    inline const std::array<unsigned, Dimensions>& GetPartition() const { return _partition; }
    inline const std::array<float, Dimensions>& GetStartPoint() const { return _startPoint; }
    inline unsigned GetTotalPartition() const { return _partition[0] * _partition[1] * _partition[2]; }


    std::array<float, Dimensions> GetUnitSize() const
    {
        std::array<float, Dimensions> unitSizes;

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

    void SetStartPoint(const std::array<float, Dimensions>& point)
    {
        _startPoint = point;
        UpdateCenterPoint();
    }
    void SetCenterPoint(const std::array<float, Dimensions>& point)
    {
        _centerPoint = point;
        UpdateStartPoint();
    }
    void SetSize(const std::array<float, Dimensions>& size)
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
    std::array<float, Dimensions> _size;
    std::array<float, Dimensions> _centerPoint;
    std::array<float, Dimensions> _startPoint;
    std::array<unsigned, Dimensions> _partition;
};

template <int Dimensions>
std::ofstream &operator<<(std::ofstream &stream, const Space<Dimensions>& space)
{
    if (stream.flags() & std::ios_base::binary)
    {
        unsigned dims = Dimensions;
        stream.write((char*)&dims, sizeof(dims));
        stream.write((char*)&space._centerPoint, sizeof(space._centerPoint));
    }
    else
    {

    }
    return stream;
}


template <int Dimensions>
std::ifstream &operator>>(std::ifstream &stream, Space<Dimensions>& space)
{
    if (stream.flags() & std::ios_base::binary)
    {
    }
    else
    {
    }
    return stream;
}
