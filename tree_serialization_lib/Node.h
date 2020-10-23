#ifndef TREE_SERIALIZATION_NODE_H
#define TREE_SERIALIZATION_NODE_H

#include <memory>
#include <vector>

#include "NodeValue.h"

class Node {
    NodeValue m_value;
public:
    Node() = default;
    explicit Node(int value) : m_value(value) {}
    explicit Node(double value) : m_value(value) {}
    explicit Node(const std::string& value) : m_value(value) {}

    std::vector<std::shared_ptr<Node>> m_childs;

    void print(std::ostream& os) const;
};

inline
void Node::print(std::ostream &os) const {
    m_value.print(os);
}

inline
std::ostream& operator<<(std::ostream& os, const Node& value) {
    value.print(os);

    return os;
}

#endif //TREE_SERIALIZATION_NODE_H
