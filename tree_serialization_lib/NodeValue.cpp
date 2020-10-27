#include "NodeValue.h"

#include <arpa/inet.h>
#include <iostream>

#include "Util.h"

namespace {
    void serialize(std::ostream& os, int value) {
        value = htonl(value);
        os.write(reinterpret_cast<char*>(&value), sizeof value);
    }

    void serialize(std::ostream& os, double value) {
        uint64_t uintValue = util::htonll(reinterpret_cast<uint64_t&>(value));
        os.write(reinterpret_cast<char*>(&uintValue), sizeof uintValue);
    }

    void serialize(std::ostream& os, const std::string& value) {
        os.write(value.c_str(), value.size());
    }

    bool streamValid(std::ios& os) {
        return !os.bad() && !os.fail();
    }

    NodeValue extractInt(std::istream& istream) {
        uint32_t valueUnsigned;
        istream.read(reinterpret_cast<char*>(&valueUnsigned), sizeof(valueUnsigned));
        if (!streamValid(istream)) {
            return NodeValue();
        }

        valueUnsigned = ntohl(valueUnsigned);
        return NodeValue(static_cast<int>(valueUnsigned));
    }

    NodeValue extractDouble(std::istream& istream) {
        uint64_t valueUnsigned;
        istream.read(reinterpret_cast<char*>(&valueUnsigned), sizeof(valueUnsigned));
        if (!streamValid(istream)) {
            return NodeValue();
        }

        valueUnsigned = util::htonll(valueUnsigned);
        return NodeValue(reinterpret_cast<double&>(valueUnsigned));
    }

    NodeValue extractString(std::istream& istream) {
        uint64_t size;
        istream.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (!istream.good()) {
            return NodeValue();
        }

        size = util::htonll(size);
        char buffer[size];
        istream.read(buffer, size);
        if (!streamValid(istream)) {
            return NodeValue();
        }

        return NodeValue(std::string(buffer, size));
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

void NodeValue::printTypeAndSize(std::ostream& os, Type type) const {
    os.write(reinterpret_cast<char*>(&type), sizeof(type));
    if (type == NodeValue::STRING_TYPE) {
        auto& strRef = std::get<std::string>(m_value);
        uint64_t size = util::htonll(strRef.size());
        os.write(reinterpret_cast<char*>(&size), sizeof(size));
    }
}

void NodeValue::print(std::ostream& os) const {
    if (std::holds_alternative<int>(m_value)) {
        os << std::get<int>(m_value);
    } else if (std::holds_alternative<double>(m_value)) {
        os << std::get<double>(m_value);
    } else if (std::holds_alternative<std::string>(m_value)) {
        os << std::get<std::string>(m_value);
    }
}

void NodeValue::serialize(std::ostream& os) const {
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

NodeValue NodeValue::deserialize(std::istream& istream) {
    if (istream.eof()) {
        std::cerr << "Invalid buffer for representation NodeValue" << std::endl;
        return NodeValue();
    }

    uint8_t type;
    istream.read(reinterpret_cast<char*>(&type), sizeof(type));
    if (!istream.good()) {
        return NodeValue();
    }
    switch (static_cast<Type>(type)) {
        case NodeValue::INT_TYPE:
            return extractInt(istream);
        case NodeValue::DOUBLE_TYPE:
            return extractDouble(istream);
        case NodeValue::STRING_TYPE:
            return extractString(istream);
        default:
            std::cerr << "Unsupported type to extract from string: " << (int) type
                      << std::endl;
            break;
    }

    return NodeValue();
}
