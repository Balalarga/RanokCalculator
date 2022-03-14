#pragma once

#include <glm.hpp>
#include <array>


template<int Dimensions>
class Space
{
public:
    enum class ModelData
    {
        Negative = -1,
        Zero,
        Positive
    };
    using MimageData = std::array<double, Dimensions+2>;


    Space(const Space &oth) = default;
    virtual ~Space() = default;


    static Space FromStart(const std::array<float, Dimensions> &startPoint,
                           const std::array<float, Dimensions> &size,
                           const std::array<unsigned, Dimensions> partition = std::array<float, Dimensions>(1));
    static Space FromStart(const std::array<float, Dimensions> &startPoint,
                           const unsigned& recursiveDepth,
                           const std::array<unsigned, Dimensions> partition = std::array<unsigned, Dimensions>(1));

    static Space FromCenter(const std::array<float, Dimensions> &centerPoint,
                            const std::array<float, Dimensions> &size,
                            const std::array<unsigned, Dimensions> partition = std::array<unsigned, Dimensions>(1));
    static Space FromCenter(const std::array<float, Dimensions> &centerPoint,
                            const unsigned& recursiveDepth,
                            const std::array<unsigned, Dimensions> partition = std::array<unsigned, Dimensions>(1));


    inline const std::array<float, Dimensions>& GetSize() const { return _size; }
    inline const std::array<float, Dimensions>& GetCentral() const { return _centerPoint; }
    inline const std::array<unsigned, Dimensions>& GetPartition() const { return _partition; }
    inline const std::array<float, Dimensions>& GetStartPoint() const { return _startPoint; }
    inline unsigned GetTotalPartition() const { return _partition.x * _partition.y * _partition.z; }


    void SetPartition(const std::array<unsigned, Dimensions>& partition);
    void SetPartition(const unsigned& partition);


protected:
    Space() = default;
    void UpdateCenterPoint();
    void UpdateStartPoint();


private:
    std::array<float, Dimensions> _size;
    std::array<float, Dimensions> _centerPoint;
    std::array<float, Dimensions> _startPoint;
    std::array<unsigned, Dimensions> _partition;
};
