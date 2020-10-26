/*! \defgroup tree_serialization_lib Tree serialization library
    @{
*/

/*!
\file
\brief Contains class Tree which provides method to manipulate trees
\author Nikolay Zemtsovskiy

Tree class provides mechanism to manage trees: serialise, de-serialise, traverse and print
*/

#ifndef TREE_SERIALIZATION_TREE_H
#define TREE_SERIALIZATION_TREE_H

#include <memory>
#include <functional>

#include "Node.h"

class Tree {
public:
    /*!
    De-serialises the tree from the given input stream
    \param[in] ifs input stream which stores serialised tree
    \returns de-serialised tree
    */
    static std::shared_ptr<Node> deserialize(std::istream &ifs);

    /*!
    Serialises the given tree to the given output stream
    \param[out] os stream to serialise tree
    \param[in] root pointer to root node to serialise
    \returns true on success and false otherwise
    */
    static bool serialize(std::ostream &os, const std::shared_ptr<Node> &root);

    /*!
    Prints the given tree to the given output stream
    \param[out] os stream to print tree
    \param[in] root pointer to root node to print
    */
    static void print(std::ostream &os, const std::shared_ptr<Node> &root);

    /*!
    Traverse the given tree by NLR way: first visit current node, then from left to the
    right childes
    \param[in] root pointer to root node to traverse
    \param[in] func to apply on each visited Node pointer
    */
    static void traverseNLR(const std::shared_ptr<Node> &root,
                            const std::function<void(Node *)> &func);
};

#endif //TREE_SERIALIZATION_TREE_H
/*! @} */