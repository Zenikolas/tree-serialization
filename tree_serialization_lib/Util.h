/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief useful utility functions
\author Nikolay Zemtsovskiy
*/
#pragma once

#include <utility>

namespace util {
/*!
Takes integral type T of various length and swaps it's bytes to network byte order
using info about endianness of the current architecture
\param[in, out] input integral type
*/
template<typename T>
void htonT(T* input) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    char* ptr = reinterpret_cast<char*>(input);

    for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
        std::swap(ptr[i], ptr[sizeof(T) - 1 - i]);
    }
#endif
}
}
/*! @} */

