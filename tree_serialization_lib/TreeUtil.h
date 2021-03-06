#pragma once
/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class TreeUtil which provides method to manipulate trees
\author Nikolay Zemtsovskiy

TreeUtil class provides mechanism to manage trees: serialise, de-serialise, traverse and print
*/

#include <memory>
#include <functional>

#include "Node.h"

namespace treesl {
class TreeUtil {
public:
    /*!
    De-serialises the tree from the given input stream
    \param[in] ifs input stream which stores serialised tree
    \returns pair of de-serialised tree and NodeError indicating 0 on success and
             non-zero otherwise
    */
    static std::pair<std::unique_ptr<Node>, NodeError> deserialize(std::istream& ifs);

    /*!
    Serialises the given tree to the given output stream
    \param[out] os stream to serialise tree
    \param[in] root pointer to root node to serialise
    \returns 0 on success and false otherwise
    */
    static NodeError serialize(std::ostream& os, const Node* root);

    /*!
    Prints the given tree to the given output stream
    \param[out] os stream to print tree
    \param[in] root pointer to root node to print
    \returns 0 on success and false otherwise
    */
    static NodeError print(std::ostream& os, const Node* root);

    /*!
    Traverse the given tree by NLR way: first visit current node, then from left to the
    right childes
    \param[in] root pointer to root node to traverse
    \param[in] func to apply on each visited Node pointer
    \returns 0 on success and false otherwise
    */
    static NodeError traverseNLR(const Node* root,
                            const std::function<void(const Node*)>& func);
};
}
/*! @} */
