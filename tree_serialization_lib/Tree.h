#ifndef TREE_SERIALIZATION_TREE_H
#define TREE_SERIALIZATION_TREE_H

#include <memory>
#include <functional>

#include "Node.h"

class Tree {
public:
    static std::shared_ptr<Node> deserialize(std::istream& ifs);
    static bool serialize(std::ostream& os, std::shared_ptr<Node> root);

    static void print(std::ostream& os, std::shared_ptr<Node> root);

    static void traverseNLR(std::shared_ptr<Node> root, const std::function<void(Node*)>& func);
};

#endif //TREE_SERIALIZATION_TREE_H
