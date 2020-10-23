#include "NodeValue.h"

#include <arpa/inet.h>
#include <iostream>
#include <cassert>

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


    void serialize(std::ostream &os, int value) {
        value = htonl(value);
        os.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void serialize(std::ostream &os, double value) {
        uint64_t uintValue = htonll(reinterpret_cast<uint64_t &>(value));
        os.write(reinterpret_cast<char*>(&uintValue), sizeof uintValue);
    }

    void serialize(std::ostream &os, const std::string &value) {
        os << value;
    }

    NodeValue extractInt(std::istream &istream) {
        uint32_t valueUnsigned;
        istream.read(reinterpret_cast<char*>(&valueUnsigned), sizeof(valueUnsigned));
        valueUnsigned = ntohl(valueUnsigned);
        if (istream.bad() || istream.fail()) {
            return NodeValue();
        }

        return NodeValue(static_cast<int>(valueUnsigned));
    }

    NodeValue extractDouble(std::istream &istream) {
        assert(false && "not implemented");
    }

    NodeValue extractString(std::istream &istream) {
        //todo read by bytes count!
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
    os << static_cast<uint8_t>(type);
    if (type == NodeValue::STRING_TYPE) {
        auto &strRef = std::get<std::string>(m_value);
        uint64_t size = htonll(strRef.size());
        os.write(reinterpret_cast<char*>(&size),sizeof(size));
    }
}

void NodeValue::print(std::ostream &os) const {
    if (std::holds_alternative<int>(m_value)) {
        os << std::get<int>(m_value);
    } else if (std::holds_alternative<double>(m_value)) {
        os << std::get<double>(m_value);
    } else if (std::holds_alternative<std::string>(m_value)) {
        os << std::get<std::string>(m_value);
    }
}

void NodeValue::serialize(std::ostream &os) const {
    NodeValue::Type type = getType();
    printTypeAndSize(os, type);

    switch (type) {
        case NodeValue::INT_TYPE:
            ::serialize(os, std::get<int>(m_value));
            break;
        case NodeValue::DOUBLE_TYPE:
            ::serialize(os, std::get<double>(m_value));
            break;
        case NodeValue::STRING_TYPE:
            ::serialize(os, std::get<std::string>(m_value));
            break;
        default:
            std::cerr << "Unsupported type to serialize: " << type << std::endl;
            break;
    }
}

NodeValue NodeValue::deserialize(std::istream &stream) {
    if (stream.eof()) {
        std::cerr << "Invalid buffer for representation NodeValue" << std::endl;
        return NodeValue();
    }

    uint8_t type;
    stream >> type;
    if (stream.bad() || stream.fail()) {
        return NodeValue();
    }
    switch (static_cast<Type>(type)) {
        case NodeValue::INT_TYPE:
            return extractInt(stream);
        case NodeValue::DOUBLE_TYPE:
            return extractDouble(stream);
        case NodeValue::STRING_TYPE:
            return extractString(stream);
        default:
            std::cerr << "Unsupported type to extract from string: " << type << std::endl;
            break;
    }

    return NodeValue();
}
