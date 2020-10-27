/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class TreeUtil which provides method to manipulate trees
\author Nikolay Zemtsovskiy

TreeUtil class provides mechanism to manage trees: serialise, de-serialise, traverse and print
*/

#ifndef TREE_SERIALIZATION_TREEUTIL_H
#define TREE_SERIALIZATION_TREEUTIL_H

#include <memory>
#include <functional>

#include "Node.h"

namespace treesl {
class TreeUtil {
public:
    /*!
    De-serialises the tree from the given input stream
    \param[in] ifs input stream which stores serialised tree
    \returns de-serialised tree
    */
    static std::unique_ptr<Node> deserialize(std::istream& ifs);

    /*!
    Serialises the given tree to the given output stream
    \param[out] os stream to serialise tree
    \param[in] root pointer to root node to serialise
    \returns true on success and false otherwise
    */
    static bool serialize(std::ostream& os, const Node* root);

    /*!
    Prints the given tree to the given output stream
    \param[out] os stream to print tree
    \param[in] root pointer to root node to print
    */
    static void print(std::ostream& os, const Node* root);

    /*!
    Traverse the given tree by NLR way: first visit current node, then from left to the
    right childes
    \param[in] root pointer to root node to traverse
    \param[in] func to apply on each visited Node pointer
    */
    static void traverseNLR(const Node* root,
                            const std::function<void(const Node*)>& func);
};
}

#endif //TREE_SERIALIZATION_TREEUTIL_H
/*! @} */