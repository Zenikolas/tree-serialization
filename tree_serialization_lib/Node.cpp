#include "Node.h"

#include <netinet/in.h>
#include <istream>
#include <cstring>

#include "Util.h"

namespace treesl {
namespace {
inline
bool streamValid(std::ios& os) { return !os.bad() && !os.fail(); }
}

std::pair<std::unique_ptr<Node>, NodeError> Node::deserialize(std::istream& istream) {
    if (!istream.good()) {
        return {nullptr, NodeError::INVALID_STREAM};
    }

    uint8_t type;
    istream.read(reinterpret_cast<char*>(&type), sizeof(type));
    if (!istream.good()) {
        return {nullptr, NodeError::INVALID_STREAM};
    }

    std::unique_ptr<Node> ret = nullptr;
    switch (static_cast<Type>(type)) {
        case Type::INT_TYPE: {
            ret = std::make_unique<NodeSpecialised<int>>();
            break;
        }
        case Type::DOUBLE_TYPE: {
            ret = std::make_unique<NodeSpecialised<double>>();
            break;
        }
        case Type::STRING_TYPE: {
            ret = std::make_unique<NodeSpecialised<std::string>>();
            break;
        }
        default:
            break;
    }

    if (!ret) {
        return {nullptr, NodeError::INVALID_NODE_VALUE_TYPE};
    }

    return {std::move(ret), ret->extractValue(istream)};
}

template<>
NodeError NodeSpecialised<int>::extractValue(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&m_value), sizeof(m_value));
    if (!streamValid(stream)) {
        return NodeError::INVALID_STREAM;
    }

    m_value = ntohl(m_value);
    return NodeError::SUCCESS;
}

template<>
NodeError NodeSpecialised<double>::extractValue(std::istream& stream) {
    double value;
    stream.read(reinterpret_cast<char*>(&m_value), sizeof(m_value));
    if (!streamValid(stream)) {
        return NodeError::INVALID_STREAM;
    }

    return NodeError::SUCCESS;
}

template<>
NodeError NodeSpecialised<std::string>::extractValue(std::istream& stream) {
    uint64_t size;
    stream.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!stream.good()) {
        return NodeError::INVALID_STREAM;
    }

    util::htonT(&size);
    m_value.resize(size);

    stream.read(m_value.data(), size);
    if (!streamValid(stream)) {
        return NodeError::INVALID_STREAM;
    }

    return NodeError::SUCCESS;
}

void serializeType(std::ostream& os, Node::Type type) {
    os.write(reinterpret_cast<char*>(&type), sizeof(type));
}

template<>
NodeError NodeSpecialised<int>::serialize(std::ostream& os) const {
    serializeType(os, Node::INT_TYPE);
    int value = htonl(m_value);
    os.write(reinterpret_cast<char*>(&value), sizeof value);

    return NodeError::SUCCESS;
}

template<>
NodeError NodeSpecialised<double>::serialize(std::ostream& os) const {
    serializeType(os, Node::DOUBLE_TYPE);
    char data[sizeof(m_value)];
    memcpy(&data, &m_value, sizeof(m_value));
    os.write(data, sizeof data);

    return NodeError::SUCCESS;
}

template<>
NodeError NodeSpecialised<std::string>::serialize(std::ostream& os) const {
    serializeType(os, Node::STRING_TYPE);
    uint64_t size = m_value.size();
    util::htonT(&size);
    os.write(reinterpret_cast<char*>(&size), sizeof(size));
    os.write(m_value.c_str(), m_value.size());

    return NodeError::SUCCESS;
}

}