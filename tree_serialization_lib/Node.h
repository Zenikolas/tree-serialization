#ifndef TREE_SERIALIZATION_NODE_H
#define TREE_SERIALIZATION_NODE_H

#include <memory>
#include <vector>

#include "NodeValue.h"

class Node {
    NodeValue m_value;

    std::shared_ptr<Node> m_parent;
    std::vector<std::shared_ptr<Node>> m_childs;
public:
};

#endif //TREE_SERIALIZATION_NODE_H
