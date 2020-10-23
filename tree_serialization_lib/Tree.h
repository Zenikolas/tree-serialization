#ifndef TREE_SERIALIZATION_TREE_H
#define TREE_SERIALIZATION_TREE_H

#include "Node.h"

#include <memory>

class Tree {
    void traverse(std::shared_ptr<Node> root);
public:
    static std::shared_ptr<Node> deserialize(std::string_view inputFile);

    static bool serialize(std::string_view outFile);

    static void print(std::ostream& os, std::shared_ptr<Node> root);
};

#endif //TREE_SERIALIZATION_TREE_H
