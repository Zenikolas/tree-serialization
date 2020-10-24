#ifndef TREE_SERIALIZATION_NODEVALUE_H
#define TREE_SERIALIZATION_NODEVALUE_H

#include <string>
#include <variant>
#include <ostream>

class NodeValue {
    std::variant<bool, int, double, std::string> m_value;

    enum Type : uint8_t {
        INT_TYPE,
        DOUBLE_TYPE,
        STRING_TYPE,
        UNSUPPORTED_TYPE
    };

    Type getType() const;

    void printTypeAndSize(std::ostream &os, Type type) const;

    friend
    bool operator==(const NodeValue &lhs, const NodeValue &rhs);

public:
    NodeValue() = default;

    NodeValue(const NodeValue &) = default;

    NodeValue(NodeValue &&) = default;

    NodeValue& operator=(const NodeValue&) = default;
    NodeValue& operator=(NodeValue&&) = default;

    explicit NodeValue(int value) : m_value(value) {}

    explicit NodeValue(double value) : m_value(value) {}

    explicit NodeValue(const std::string &str) : m_value(str) {}

    void print(std::ostream &os) const;

    void serialize(std::ostream &os) const;

    bool isInitialized() const;

    static NodeValue deserialize(std::istream &stream);
};

inline
bool NodeValue::isInitialized() const {
    return !std::holds_alternative<bool>(m_value);
}

inline
std::ostream &operator<<(std::ostream &os, const NodeValue &value) {
    value.print(os);

    return os;
}

inline
bool operator==(const NodeValue &lhs, const NodeValue &rhs) {
    return lhs.m_value == rhs.m_value;
}

#endif //TREE_SERIALIZATION_NODEVALUE_H
