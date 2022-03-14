#include "Space3D.h"


Space3D Space3D::FromStart(const glm::fvec3 &startPoint,
                           const glm::fvec3 &size,
                           const glm::uvec3 partition)
{
    Space3D space;
    space._size = size;
    space._partition = partition;
    space._startPoint = startPoint;
    space.UpdateCenterPoint();
    return space;
}

Space3D Space3D::FromCenter(const glm::fvec3 &centerPoint,
                            const glm::fvec3 &size,
                            const glm::uvec3 partition)
{
    Space3D space;
    space._size = size;
    space._partition = partition;
    space._centerPoint = centerPoint;
    space.UpdateStartPoint();
    return space;
}

void Space3D::SetPartition(const glm::uvec3& partition)
{
    _partition = partition;
}

void Space3D::SetPartition(const unsigned& partition)
{
    _partition = {partition, partition, partition};
}

void Space3D::UpdateStartPoint()
{
    _startPoint = _centerPoint - _size / 2.f;
}

void Space3D::UpdateCenterPoint()
{
    _centerPoint = _startPoint + _size / 2.f;
}
