#ifndef TREE_SERIALIZATION_NODEVALUE_H
#define TREE_SERIALIZATION_NODEVALUE_H

#include <string>
#include <variant>
#include <ostream>

class NodeValue {
    std::variant<int, double, std::string> m_value;

    enum Type : char {
        INT_TYPE,
        DOUBLE_TYPE,
        STRING_TYPE,
        UNSUPPORTED_TYPE
    };

    Type getType() const;
    void printTypeAndSize(std::ostream& os, Type type) const;
public:
    NodeValue() = default;

    NodeValue(int value) : m_value(value) {}
    NodeValue(double value) : m_value(value) {}
    NodeValue(std::string_view str) : m_value(str.data()) {}

    void print(std::ostream& os) const;

    static NodeValue fromString(std::string_view str);
};

inline
std::ostream& operator<<(std::ostream& os, const NodeValue& value)
{
    value.print(os);

    return os;
}

#endif //TREE_SERIALIZATION_NODEVALUE_H
