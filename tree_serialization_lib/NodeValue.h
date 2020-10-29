#pragma once
/*! \defgroup tree_serialization_lib TreeUtil serialization library
    @{
*/

/*!
\file
\brief Contains class NodeValue which provides mechanism to store and manipulate
       on Node values
\author Nikolay Zemtsovskiy

The object of NodeValue class can store three different types of values:
integer, floating number, string
*/

#include <ostream>
#include <string>
#include <variant>

#include "NodeError.h"

namespace treesl {
class NodeValue {
    std::variant<bool, int, double, std::string> m_value;

    enum Type : uint8_t {
        INT_TYPE = 0,
        DOUBLE_TYPE,
        STRING_TYPE,
        UNSUPPORTED_TYPE
    };

    Type getType() const;

    void serializeTypeAndSize(std::ostream& os, Type type) const;

    friend bool operator==(const NodeValue& lhs, const NodeValue& rhs);

public:
    NodeValue() = default;

    NodeValue(const NodeValue&) = default;

    NodeValue(NodeValue&&) = default;

    NodeValue& operator=(const NodeValue&) = default;

    NodeValue& operator=(NodeValue&&) = default;

    explicit NodeValue(int value) : m_value(value) {}

    explicit NodeValue(double value) : m_value(value) {}

    explicit NodeValue(const std::string& str) : m_value(str) {}

    /// Prints value to the given stream
    void print(std::ostream& os) const;

    /// Serialise value to the given stream
    NodeError serialize(std::ostream& os) const;

    /*!
    De-serialise the value of the object from the given stream
    \param[in] istream holding serialised NodeValue
    \returns pair of new created NodeValue and NodeError indicating 0 on success and
             non-zero otherwise
    */
    static std::pair<NodeValue, NodeError>
    deserialize(std::istream& istream);
};

inline std::ostream& operator<<(std::ostream& os, const NodeValue& value) {
    value.print(os);

    return os;
}

inline bool operator==(const NodeValue& lhs, const NodeValue& rhs) {
    return lhs.m_value == rhs.m_value;
}
}
/*! @} */
