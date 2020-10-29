#pragma once
/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class Node which is used for building nodes of the tree
\author Nikolay Zemtsovskiy
*/

#include <memory>
#include <vector>

#include "NodeValue.h"

namespace treesl {
class Node {
    NodeValue m_value;
    std::vector<std::unique_ptr<Node>> m_childes;

    friend
    bool operator==(const Node& lhs, const Node& rhs);

public:
    Node() = default;

    explicit Node(int value) : m_value(value) {}

    explicit Node(double value) : m_value(value) {}

    explicit Node(const std::string& value) : m_value(value) {}

    /// Returns non-modifiable vector of Node's childes
    const std::vector<std::unique_ptr<Node>>& getChildes() const;

    /// Prints Node to the given stream
    void print(std::ostream& os) const;

    /// Serialise Node to the given stream
    NodeError serialize(std::ostream& os) const;

    /// Append a child to Node by moving pointer
    void appendChild(std::unique_ptr<Node>&& node);

    /*!
    De-serialise the object of Node from the given stream
    \param[in] stream holding serialised NodeValue
    \returns pair of new created pointer to Node object and NodeError indicating 0 on
             success and non-zero otherwise
    */
    static std::pair<std::unique_ptr<Node>, NodeError> deserialize(std::istream& stream);
};

inline const std::vector<std::unique_ptr<Node>>& Node::getChildes() const {
    return m_childes;
}

inline
void Node::print(std::ostream& os) const {
    m_value.print(os);
}

inline
NodeError Node::serialize(std::ostream& os) const {
    return m_value.serialize(os);
}

inline
void Node::appendChild(std::unique_ptr<Node>&& node) {
    m_childes.emplace_back(std::move(node));
}

inline
std::pair<std::unique_ptr<Node>, NodeError> Node::deserialize(std::istream& stream) {
    auto [nodeValue, errorCode] = NodeValue::deserialize(stream);
    if (errorCode != NodeError::SUCCESS) {
        return {nullptr, errorCode};
    }

    auto ret = std::make_unique<Node>();
    ret->m_value = std::move(nodeValue);

    return {std::move(ret), NodeError::SUCCESS};
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
}
/*! @} */
