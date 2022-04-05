#pragma once

#include <glm.hpp>
#include <vector>
#include <fstream>


class Space
{
public:
    Space() = default;

    Space(const std::vector<double> &centerPoint,
          const std::vector<double> &size,
          const size_t& recursiveDepth)
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
          const std::vector<size_t> partition)
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
    inline const std::vector<size_t>& GetPartition() const { return _partition; }
    inline const std::vector<double>& GetStartPoint() const { return _startPoint; }
    inline size_t GetTotalPartition() const
    {
        size_t total = 1;
        for (auto& i: _partition)
            total *= i;
        return total;
    }


    std::vector<double> GetUnitSize() const
    {
        std::vector<double> unitSizes(_size.size());

        for (size_t i = 0; i < _size.size(); ++i)
            unitSizes[i] = _size[i] / _partition[i];

        return unitSizes;
    }

    std::vector<float> GetPoint(size_t id) const
    {
        auto unitSize = GetUnitSize();
        std::vector<float> point(3);
        point[0] = _startPoint[0] + unitSize[0] * (id / ( _partition[2] * _partition[1] ));
        point[1] = _startPoint[1] + unitSize[1] * ((id / _partition[2] ) % _partition[1]);
        point[2] = _startPoint[2] + unitSize[2] * (id % _partition[2]);
        return point;
    }

    void SetPartition(const std::vector<size_t>& partition)
    {
        _partition = partition;
    }
    void SetPartition(const size_t& partition)
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

    friend std::ofstream& operator << (std::ofstream &stream, Space& space)
    {
        size_t dims = space._size.size();
        stream.write((char*)&dims, sizeof(dims));
        stream.write((char*)space._size.data(), sizeof(space._size[0]) * space._size.size());
        stream.write((char*)space._centerPoint.data(), sizeof(space._centerPoint[0]) * space._centerPoint.size());
        stream.write((char*)space._partition.data(), sizeof(space._partition[0]) * space._partition.size());
        return stream;
    }

    friend std::ifstream& operator >> (std::ifstream& stream, Space &space)
    {
        size_t dims;
        stream.read((char*)&dims, sizeof(dims));

        space._size.resize(dims);
        space._centerPoint.resize(dims);
        space._startPoint.resize(dims);
        space._partition.resize(dims);

        stream.read((char*)space._size.data(), sizeof(space._size[0]) * dims);
        stream.read((char*)space._centerPoint.data(), sizeof(space._centerPoint[0]) * dims);
        stream.read((char*)space._partition.data(), sizeof(space._partition[0]) * dims);
        space.UpdateStartPoint();

        return stream;
    }


protected:
    void UpdateCenterPoint()
    {
        _centerPoint.resize(_size.size());
        for(size_t i = 0; i < _size.size(); ++i)
            _centerPoint[i] = _startPoint[i] + _size[i] / 2.f;
    }

    void UpdateStartPoint()
    {
        _startPoint.resize(_size.size());
        for(size_t i = 0; i < _size.size(); ++i)
            _startPoint[i] = _centerPoint[i] - _size[i] / 2.f;
    }


private:
    std::vector<double> _size;
    std::vector<double> _centerPoint;
    std::vector<double> _startPoint;
    std::vector<size_t> _partition;
};
