#pragma once
/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class NodeError class which provides list of errorCodes
\author Nikolay Zemtsovskiy

For each error code the user can obtain string representation by calling operator<<
*/

namespace treesl {
enum class NodeError {
    SUCCESS = 0,
    INVALID_STREAM,
    INVALID_NODE_VALUE_TYPE,
    INVALID_FIRST_TOKEN_STREAM,
    INVALID_TREE_CHILD,
    INVALID_TREE_ROOT
};

inline std::ostream& operator<<(std::ostream& os, const NodeError& value) {
    switch (value) {
        case NodeError::SUCCESS:
            os << "SUCCESS";
            break;
        case NodeError::INVALID_STREAM:
            os << "INVALID STREAM";
            break;
        case NodeError::INVALID_NODE_VALUE_TYPE:
            os << "INVALID NODE VALUE TYPE";
            break;
        case NodeError::INVALID_FIRST_TOKEN_STREAM:
            os << "INVALID FIRST TOKEN STREAM";
            break;
        case NodeError::INVALID_TREE_CHILD:
            os << "INVALID TREE CHILD";
            break;
        case NodeError::INVALID_TREE_ROOT:
            os << "INVALID TREE ROOT";
            break;
        default:
            break;
    }

    return os;
}


}  // namespace treesl
/*! @} */

