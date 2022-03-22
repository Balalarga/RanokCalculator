#pragma once

#include <glm.hpp>
#include <vector>
#include <fstream>


class Space
{
public:
    Space(const std::vector<double> &centerPoint,
          const std::vector<double> &size,
          const unsigned& recursiveDepth)
    {
        assert(centerPoint.size() == size.size());

        _size = size;
        _centerPoint = centerPoint;
        _partition.resize(centerPoint.size(), std::pow(2, recursiveDepth));
        _startPoint.resize(centerPoint.size());
        UpdateStartPoint();
    }

    Space(const std::vector<double> &centerPoint,
          const std::vector<double> &size,
          const std::vector<unsigned> partition)
    {
        assert(centerPoint.size() == size.size());
        assert(partition.size() == size.size());

        _size = size;
        _partition = partition;
        _centerPoint = centerPoint;
        _startPoint.resize(centerPoint.size());
        UpdateStartPoint();
    }

    Space(const Space &oth) = default;
    virtual ~Space() = default;


    inline const std::vector<double>& GetSize() const { return _size; }
    inline const std::vector<double>& GetCentral() const { return _centerPoint; }
    inline const std::vector<unsigned>& GetPartition() const { return _partition; }
    inline const std::vector<double>& GetStartPoint() const { return _startPoint; }
    inline unsigned GetTotalPartition() const { return _partition[0] * _partition[1] * _partition[2]; }


    std::vector<double> GetUnitSize() const
    {
        std::vector<double> unitSizes(_size.size());

        for (size_t i = 0; i < _size.size(); ++i)
            unitSizes[i] = _size[i] / _partition[i];

        return unitSizes;
    }


    void SetPartition(const std::vector<unsigned>& partition)
    {
        _partition = partition;
    }
    void SetPartition(const unsigned& partition)
    {
        _partition = { partition, partition, partition };
    }

    void SetStartPoint(const std::vector<double>& point)
    {
        _startPoint = point;
        UpdateCenterPoint();
    }
    void SetCenterPoint(const std::vector<double>& point)
    {
        _centerPoint = point;
        UpdateStartPoint();
    }
    void SetSize(const std::vector<double>& size)
    {
        _size = size;
    }

    friend std::ofstream &operator<<(std::ofstream &stream, const Space& space);
    friend std::ifstream &operator>>(std::ifstream &stream, Space& space);


protected:
    void UpdateCenterPoint()
    {
        for(size_t i = 0; i < _size.size(); ++i)
            _centerPoint[i] = _startPoint[i] + _size[i] / 2.f;
    }

    void UpdateStartPoint()
    {
        for(size_t i = 0; i < _size.size(); ++i)
            _startPoint[i] = _centerPoint[i] - _size[i] / 2.f;
    }


private:
    std::vector<double> _size;
    std::vector<double> _centerPoint;
    std::vector<double> _startPoint;
    std::vector<unsigned> _partition;
};

std::ofstream &operator<<(std::ofstream &stream, const Space& space)
{
    unsigned dims = space._size.size();
    stream.write((char*)&dims, sizeof(dims));
    stream.write((char*)&space._size, sizeof(space._size[0]) * space._size.size());
    stream.write((char*)&space._centerPoint, sizeof(space._centerPoint[0]) * space._centerPoint.size());
    stream.write((char*)&space._startPoint, sizeof(space._startPoint[0]) * space._startPoint.size());
    stream.write((char*)&space._partition, sizeof(space._partition[0]) * space._partition.size());
    return stream;
}


std::ifstream &operator>>(std::ifstream &stream, Space& space)
{
    unsigned dims;
    stream.read((char*)&dims, sizeof(dims));

    space._size.resize(dims);
    space._centerPoint.resize(dims);
    space._startPoint.resize(dims);
    space._partition.resize(dims);

    stream.read((char*)&space._size, sizeof(space._size[0]) * dims);
    stream.read((char*)&space._centerPoint, sizeof(space._centerPoint[0]) * dims);
    stream.read((char*)&space._startPoint, sizeof(space._startPoint[0]) * dims);
    stream.read((char*)&space._partition, sizeof(space._partition[0]) * dims);

    return stream;
}
