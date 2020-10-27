/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class Node which is used for building nodes of the tree
\author Nikolay Zemtsovskiy
*/
#ifndef TREE_SERIALIZATION_NODE_H
#define TREE_SERIALIZATION_NODE_H

#include <memory>
#include <vector>

#include "NodeValue.h"

class Node {
    NodeValue m_value;
    std::vector<std::shared_ptr<Node>> m_childes;

    friend
    bool operator==(const Node& lhs, const Node& rhs);

public:
    Node() = default;

    explicit Node(int value) : m_value(value) {}

    explicit Node(double value) : m_value(value) {}

    explicit Node(const std::string& value) : m_value(value) {}

    /// Returns non-modifiable vector of Node's childes
    const std::vector<std::shared_ptr<Node>>& getChildes() const;

    /// Prints Node to the given stream
    void print(std::ostream& os) const;

    /// Serialise Node to the given stream
    void serialize(std::ostream& os) const;

    /// Append a child to Node by copying pointer
    void appendChild(const std::shared_ptr<Node>& node);

    /// Append a child to Node by moving pointer
    void appendChild(std::shared_ptr<Node>&& node);

    /*!
    De-serialise the object of Node from the given stream
    \param[in] stream holding serialised NodeValue
    \returns new created Node object
    */
    static std::unique_ptr<Node> deserialize(std::istream& stream);
};

inline const std::vector<std::shared_ptr<Node>>& Node::getChildes() const {
    return m_childes;
}

inline
void Node::print(std::ostream& os) const {
    m_value.print(os);
}

inline
void Node::serialize(std::ostream& os) const {
    m_value.serialize(os);
}

inline
void Node::appendChild(const std::shared_ptr<Node>& node) {
    m_childes.emplace_back(node);
}

inline
void Node::appendChild(std::shared_ptr<Node>&& node) {
    m_childes.emplace_back(std::move(node));
}

inline
std::unique_ptr<Node> Node::deserialize(std::istream& stream) {
    NodeValue nodeValue = NodeValue::deserialize(stream);
    if (!nodeValue.isInitialized()) {
        return nullptr;
    }

    auto ret = std::make_unique<Node>();
    ret->m_value = std::move(nodeValue);
    return ret;
}

inline
std::ostream& operator<<(std::ostream& os, const Node& value) {
    value.print(os);

    return os;
}

inline
bool operator==(const Node& lhs, const Node& rhs) {
    return lhs.m_value == rhs.m_value;
}


#endif //TREE_SERIALIZATION_NODE_H
/*! @} */