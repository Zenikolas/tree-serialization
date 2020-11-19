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
#include <istream>

#include "NodeError.h"

namespace treesl {

class Node {
public:
    enum Type : uint8_t {
        INT_TYPE = 0,
        DOUBLE_TYPE,
        STRING_TYPE,
        UNSUPPORTED_TYPE
    };
private:
    std::vector<std::unique_ptr<Node>> m_childes;

    virtual NodeError extractValue(std::istream& istream) = 0;

    virtual bool isEqual(const Node& rhs) const = 0;

    friend
    bool operator==(const Node& lhs, const Node& rhs);

public:
    virtual ~Node() = 0;

    /// Returns non-modifiable vector of Node's childes
    const std::vector<std::unique_ptr<Node>>& getChildes() const;

    /// Prints Node to the given stream
    virtual void print(std::ostream& os) const = 0;

    /// Serialise Node to the given stream
    virtual NodeError serialize(std::ostream& os) const = 0;

    /// Append a child to Node by moving pointer
    void appendChild(std::unique_ptr<Node>&& node);

    /*!
    De-serialise the object of Node from the given stream
    \param[in] stream holding serialised NodeValue
    \returns pair of new created pointer to Node object and NodeError indicating 0 on
             success and non-zero otherwise
    */
    static std::pair<std::unique_ptr<Node>, NodeError>
    deserialize(std::istream& stream);

    template<class ValueType, class ... Args>
    static std::unique_ptr<Node> makeNode(Args&& ... args);
};

template<class ValueType>
class NodeSpecialised : public Node {
    ValueType m_value;

    NodeError extractValue(std::istream& istream) override;

    bool isEqual(const Node& rhs) const override;

public:
    NodeSpecialised() = default;

    template<class ForwardValueType>
    explicit NodeSpecialised(ForwardValueType&& value)
            : m_value(std::forward<ForwardValueType>(value)) {}

    void print(std::ostream& os) const override;

    NodeError serialize(std::ostream& os) const override;
};

inline
Node::~Node() {}

inline
const std::vector<std::unique_ptr<Node>>& Node::getChildes() const {
    return m_childes;
}

inline
void Node::appendChild(std::unique_ptr<Node>&& node) {
    m_childes.emplace_back(std::move(node));
}

template<class ValueType, class ... Args>
std::unique_ptr<Node> Node::makeNode(Args&& ... args) {
    if constexpr (std::is_constructible_v<ValueType, Args...>) {
        return std::make_unique<NodeSpecialised<ValueType>>(std::forward<Args>(args)...);
    }

    return nullptr;
}

inline
std::ostream& operator<<(std::ostream& os, const Node& value) {
    value.print(os);

    return os;
}

inline
bool operator==(const Node& lhs, const Node& rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.isEqual(rhs);
}

template<class ValueType>
void NodeSpecialised<ValueType>::print(std::ostream& os) const {
    os << m_value;
}

template<class ValueType>
NodeError NodeSpecialised<ValueType>::extractValue(std::istream& istream) {
    return NodeError::INVALID_NODE_VALUE_TYPE;
}

template<>
NodeError NodeSpecialised<int>::extractValue(std::istream& stream);

template<>
NodeError NodeSpecialised<double>::extractValue(std::istream& stream);

template<>
NodeError NodeSpecialised<std::string>::extractValue(std::istream& stream);

template<class ValueType>
bool NodeSpecialised<ValueType>::isEqual(const Node& rhs) const {
    return m_value == static_cast<const NodeSpecialised<ValueType>&>(rhs).m_value;
}

template<class ValueType>
NodeError NodeSpecialised<ValueType>::serialize(std::ostream& os) const {
    return NodeError::INVALID_NODE_VALUE_TYPE;
}

template<>
NodeError NodeSpecialised<int>::serialize(std::ostream& os) const;

template<>
NodeError NodeSpecialised<double>::serialize(std::ostream& os) const;

template<>
NodeError NodeSpecialised<std::string>::serialize(std::ostream& os) const;
}
/*! @} */
