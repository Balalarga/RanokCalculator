#include "Space.h"

template<int Dimensions>
Space<Dimensions> Space<Dimensions>::FromStart(const std::array<float, Dimensions> &startPoint,
                                               const std::array<float, Dimensions> &size,
                                               const std::array<unsigned, Dimensions> partition)
{
    Space<Dimensions> space;
    space._size = size;
    space._partition = partition;
    space._startPoint = startPoint;
    space.UpdateCenterPoint();
    return space;
}

template<int Dimensions>
Space<Dimensions> Space<Dimensions>::FromStart(const std::array<float, Dimensions> &startPoint,
                                               const unsigned &recursiveDepth,
                                               const std::array<unsigned, Dimensions> partition)
{

    return FromStart(startPoint, std::pow(2, recursiveDepth), partition);
}

template<int Dimensions>
Space<Dimensions> Space<Dimensions>::FromCenter(const std::array<float, Dimensions> &centerPoint,
                                                const std::array<float, Dimensions> &size,
                                                const std::array<unsigned, Dimensions> partition)
{
    Space<Dimensions> space;
    space._size = size;
    space._partition = partition;
    space._centerPoint = centerPoint;
    space.UpdateStartPoint();
    return space;
}

template<int Dimensions>
Space<Dimensions> Space<Dimensions>::FromCenter(const std::array<float, Dimensions> &centerPoint,
                                                const unsigned &recursiveDepth,
                                                const std::array<unsigned, Dimensions> partition)
{
    return FromCenter(centerPoint, std::pow(2, recursiveDepth), partition);
}

template<int Dimensions>
void Space<Dimensions>::SetPartition(const std::array<unsigned, Dimensions>& partition)
{
    _partition = partition;
}

template<int Dimensions>
void Space<Dimensions>::SetPartition(const unsigned& partition)
{
    _partition = { partition, partition, partition };
}

template<int Dimensions>
void Space<Dimensions>::UpdateStartPoint()
{
    _startPoint = _centerPoint - _size / 2.f;
}

template<int Dimensions>
void Space<Dimensions>::UpdateCenterPoint()
{
    _centerPoint = _startPoint + _size / 2.f;
}
