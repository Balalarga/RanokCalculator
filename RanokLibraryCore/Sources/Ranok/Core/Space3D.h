#pragma once

#include <glm.hpp>


enum class ModelData
{
    Negative = -1,
    Zero,
    Positive
};

struct MImageData
{
    double Cx, Cy, Cz, Cw, Ct;
};


class Space3D
{
public:
    Space3D(const Space3D &oth) = default;
    virtual ~Space3D() = default;

    static Space3D FromStart(const glm::fvec3 &startPoint,
                             const glm::fvec3 &size,
                             const glm::uvec3 partition = glm::uvec3(1, 1, 1));

    static Space3D FromCenter(const glm::fvec3 &centerPoint,
                              const glm::fvec3 &size,
                              const glm::uvec3 partition = glm::uvec3(1, 1, 1));

    inline const glm::fvec3 &GetSize() const { return _size; }
    inline const glm::fvec3 &GetCentral() const { return _centerPoint; }
    inline const glm::uvec3 &GetPartition() const { return _partition; }
    inline const glm::fvec3 &GetStartPoint() const { return _startPoint; }
    inline unsigned GetTotalPartition() const { return _partition.x * _partition.y * _partition.z; }

    void SetPartition(const glm::uvec3& partition);
    void SetPartition(const unsigned& partition);


protected:
    Space3D() = default;
    void UpdateCenterPoint();
    void UpdateStartPoint();


private:
    glm::fvec3 _size;
    glm::fvec3 _centerPoint;
    glm::fvec3 _startPoint;
    glm::uvec3 _partition;
};