#include "NodeValue.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>

#include "Util.h"

namespace treesl {
namespace {
void serialize(std::ostream& os, int value) {
    value = htonl(value);
    os.write(reinterpret_cast<char*>(&value), sizeof value);
}

void serialize(std::ostream& os, double value) {
    char data[sizeof(value)];
    memcpy(&data, &value, sizeof(value));
    os.write(data, sizeof data);
}

void serialize(std::ostream& os, const std::string& value) {
    os.write(value.c_str(), value.size());
}

bool streamValid(std::ios& os) { return !os.bad() && !os.fail(); }

std::pair<NodeValue, NodeError> extractInt(std::istream& istream) {
    uint32_t valueUnsigned;
    istream.read(reinterpret_cast<char*>(&valueUnsigned), sizeof(valueUnsigned));
    if (!streamValid(istream)) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }

    valueUnsigned = ntohl(valueUnsigned);
    return {NodeValue(static_cast<int>(valueUnsigned)), NodeError::SUCCESS};
}

std::pair<NodeValue, NodeError> extractDouble(std::istream& istream) {
    double value;
    istream.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (!streamValid(istream)) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }

    return {NodeValue(value), NodeError::SUCCESS};
}

std::pair<NodeValue, NodeError> extractString(std::istream& istream) {
    uint64_t size;
    istream.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!istream.good()) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }

    util::htonT(&size);
    std::string value(size, 0);

    istream.read(value.data(), size);
    if (!streamValid(istream)) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }

    return {NodeValue(value), NodeError::SUCCESS};
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

void NodeValue::serializeTypeAndSize(std::ostream& os, Type type) const {
    os.write(reinterpret_cast<char*>(&type), sizeof(type));
    if (type == NodeValue::STRING_TYPE) {
        auto& strRef = std::get<std::string>(m_value);
        uint64_t size = strRef.size();
        util::htonT(&size);
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

NodeError NodeValue::serialize(std::ostream& os) const {
    NodeValue::Type type = getType();
    serializeTypeAndSize(os, type);

    switch (type) {
        case NodeValue::INT_TYPE:
            treesl::serialize(os, std::get<int>(m_value));
            break;
        case NodeValue::DOUBLE_TYPE:
            treesl::serialize(os, std::get<double>(m_value));
            break;
        case NodeValue::STRING_TYPE:
            treesl::serialize(os, std::get<std::string>(m_value));
            break;
        default:
            return NodeError::INVALID_NODE_VALUE_TYPE;
    }

    return NodeError::SUCCESS;
}

std::pair<NodeValue, NodeError>
NodeValue::deserialize(std::istream& istream) {
    if (!istream.good()) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }

    uint8_t type;
    istream.read(reinterpret_cast<char*>(&type), sizeof(type));
    if (!istream.good()) {
        return {NodeValue(), NodeError::INVALID_STREAM};
    }
    switch (static_cast<Type>(type)) {
        case NodeValue::INT_TYPE:
            return extractInt(istream);
        case NodeValue::DOUBLE_TYPE:
            return extractDouble(istream);
        case NodeValue::STRING_TYPE:
            return extractString(istream);
        default:
            break;
    }

    return {NodeValue(), NodeError::INVALID_NODE_VALUE_TYPE};
}
}