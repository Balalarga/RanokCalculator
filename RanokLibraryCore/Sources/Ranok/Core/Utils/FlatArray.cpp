#include "FlatArray.h"

#include <assert.h>


// template<class T>
// FlatArray<T>::FlatArray(const std::vector<unsigned>& dims):
//     _dimensions(dims)
// {
//     long dataSize = 0;
//     for (auto& i: _dimensions)
//         dataSize += i;

//     _data.resize(dataSize);
// }

// template<class T>
// FlatArray<T>::FlatArray(const std::vector<unsigned>& dims, float initValue):
//     FlatArray<T>(dims)
// {
//     for (auto& i: _data)
//         i = initValue;
// }

// template<class T>
// unsigned FlatArray<T>::GetDimension(unsigned id)
// {
//     return _dimensions.at(id);
// }

// template<class _T>
// std::ifstream& operator>>(std::ifstream& stream, FlatArray<_T>& object)
// {
//     return stream;
// }

// template<class _T>
// std::ofstream& operator<<(std::ofstream& stream, const FlatArray<_T>& object)
// {
//     return stream;
// }

// template<class _T>
// std::istream& operator>>(std::istream& stream, FlatArray<_T>& object)
// {
//     return stream;
// }

// template<class _T>
// std::ostream& operator<<(std::ostream& stream, const FlatArray<_T>& object)
// {
//     return stream;
// }

// template<class T>
// T& FlatArray<T>::operator[](std::vector<unsigned>& ids)
// {
//     assert(ids.size() == _dimensions.size());
    
//     long id = 0;
//     for (size_t i = 1; i < ids.size(); ++i)
//         id += _dimensions.at(_dimensions.size()-i) * ids.at(i);

//     return _data.at(id);
// }

// template<class T>
// T& FlatArray<T>::operator[](long id)
// {
//     return _data.at(id);
// }
