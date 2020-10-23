#include "NodeValue.h"

#include <arpa/inet.h>
#include <iostream>
#include <assert.h>

namespace {
    unsigned long long htonll(uint64_t src) {
        enum {
            INIT_TYPE = 0,
            LE_TYPE,
            BE_TYPE
        };

        static int type = INIT_TYPE;
        union {
            unsigned long long ull;
            unsigned char c[8];
        } x;
        if (type == INIT_TYPE) {
            x.ull = 0x01;
            type = (x.c[7] == 0x01ULL) ? BE_TYPE : LE_TYPE;
        }
        if (type == BE_TYPE)
            return src;
        x.ull = src;
        std::swap(x.c[0], x.c[7]);
        std::swap(x.c[1], x.c[6]);
        std::swap(x.c[2], x.c[5]);
        std::swap(x.c[3], x.c[4]);
        return x.ull;
    }


    void print(std::ostream &os, int value) {
        os << htonl(value);
    }

    void print(std::ostream &os, double value) {
        os << htonll(reinterpret_cast<uint64_t &>(value));
    }

    void print(std::ostream &os, const std::string &value) {
        os << value;
    }

    NodeValue extractInt(std::string_view buffer) {
        if (buffer.size() != sizeof(int)) {
            return NodeValue();
        }

        uint32_t valueUnsigned = ntohl(*reinterpret_cast<const uint32_t *>(buffer.data()));

        return NodeValue(static_cast<int>(valueUnsigned));
    }

    NodeValue extractDouble(std::string_view buffer) {
        assert(false && "not implemented");
    }

    NodeValue extractString(std::string_view buffer) {
        assert(false && "not implemented");
    }
}

NodeValue::Type NodeValue::getType() const {
    if (std::holds_alternative<int>(m_value)) {
        return NodeValue::Type::INT_TYPE;
    } else if (std::holds_alternative<double>(m_value)) {
        return NodeValue::Type::DOUBLE_TYPE;
    } else if (std::holds_alternative<std::string>(m_value)) {
        return NodeValue::Type::STRING_TYPE;
    }

    return NodeValue::Type::UNSUPPORTED_TYPE;
}

void NodeValue::printTypeAndSize(std::ostream &os, Type type) const {
    os << type;
    if (type == NodeValue::STRING_TYPE) {
        auto &strRef = std::get<std::string>(m_value);
        os << strRef.size();
    }
}

void NodeValue::print(std::ostream &os) const {
    NodeValue::Type type = getType();
    printTypeAndSize(os, type);

    switch (type) {
        case NodeValue::INT_TYPE:
            ::print(os, std::get<int>(m_value));
            break;
        case NodeValue::DOUBLE_TYPE:
            ::print(os, std::get<double>(m_value));
            break;
        case NodeValue::STRING_TYPE:
            ::print(os, std::get<std::string>(m_value));
            break;
        default:
            std::cerr << "Unsupported type to print: " << type << std::endl;
            break;
    }
}

NodeValue NodeValue::fromString(std::string_view str) {
    if (str.empty()) {
        std::cerr << "Invalid buffer for representation NodeValue" << std::endl;
        return NodeValue();
    }

    auto type = static_cast<Type>(str[0]);
    std::string_view valueStr = str.substr(1);
    switch (type) {
        case NodeValue::INT_TYPE:
            return extractInt(valueStr);
        case NodeValue::DOUBLE_TYPE:
            return extractDouble(valueStr);
        case NodeValue::STRING_TYPE:
            return extractString(valueStr);
        default:
            std::cerr << "Unsupported type to extract from string: " << type << std::endl;
            break;
    }

    return NodeValue();
}
