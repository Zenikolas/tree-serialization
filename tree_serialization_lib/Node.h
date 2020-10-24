#ifndef TREE_SERIALIZATION_NODE_H
#define TREE_SERIALIZATION_NODE_H

#include <memory>
#include <vector>

#include "NodeValue.h"

class Node {
    NodeValue m_value;

    friend
    bool operator==(const Node &lhs, const Node &rhs);
public:
    Node() = default;

    explicit Node(int value) : m_value(value) {}

    explicit Node(double value) : m_value(value) {}

    explicit Node(const std::string &value) : m_value(value) {}

    std::vector<std::shared_ptr<Node>> m_childs;

    void print(std::ostream &os) const;

    void serialize(std::ostream &os) const;

    static std::unique_ptr<Node> deserialize(std::istream &stream);
};

inline
void Node::print(std::ostream &os) const {
    m_value.print(os);
}

inline
void Node::serialize(std::ostream &os) const {
    m_value.serialize(os);
}

inline
std::unique_ptr<Node> Node::deserialize(std::istream &stream) {
    NodeValue nodeValue = NodeValue::deserialize(stream);
    if (!nodeValue.isInitialized()) {
        return nullptr;
    }

    auto ret = std::make_unique<Node>();
    ret->m_value = std::move(nodeValue);
    return ret;
}

inline
std::ostream &operator<<(std::ostream &os, const Node &value) {
    value.print(os);

    return os;
}

inline
bool operator==(const Node &lhs, const Node &rhs) {
    return lhs.m_value == rhs.m_value;
}


#endif //TREE_SERIALIZATION_NODE_H
