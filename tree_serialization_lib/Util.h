/*! \defgroup tree_serialization_lib Tree serialization library
    @{
*/

/*!
\file
\brief useful utility functions
\author Nikolay Zemtsovskiy
*/
#ifndef TREE_SERIALIZATION_UTIL_H
#define TREE_SERIALIZATION_UTIL_H

#include <cstdint>

namespace util {
    /*!
    Takes uint64_t number and swap it's bytes to network order checking at first
    endianness of current architecture
    \param[in] src input 8-byte number
    \returns input number converted to network byte order
    */
    unsigned long long htonll(uint64_t src);
}

#endif //TREE_SERIALIZATION_UTIL_H
/*! @} */
