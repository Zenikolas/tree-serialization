#ifndef TREE_SERIALIZATION_TREE_H
#define TREE_SERIALIZATION_TREE_H

#include "Node.h"

#include <memory>

class Tree {
    void traverse(std::shared_ptr<Node> root); //todo fix this
public:
    static std::shared_ptr<Node> deserialize(std::istream& ifs);

    static bool serialize(std::ostream& os, std::shared_ptr<Node> root);

    static void print(std::ostream& os, std::shared_ptr<Node> root);
};

#endif //TREE_SERIALIZATION_TREE_H
