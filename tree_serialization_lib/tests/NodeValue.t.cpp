#include <gtest/gtest.h>
#include <netinet/in.h>

#include "NodeValue.h"

TEST(NodeValueTest, IntTest)
{
    NodeValue value(100);

    std::stringstream sstream;
    value.serialize(sstream);

    uint8_t type;
    int check_value;
    sstream.read(reinterpret_cast<char*>(&type), sizeof(type));
    ASSERT_EQ(0, type);
    sstream.read(reinterpret_cast<char*>(&check_value), sizeof(check_value));
    ASSERT_EQ(check_value, htonl(100));

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ(std::string("100"), pStream.str());

    sstream = std::stringstream();
    value.serialize(sstream);
    NodeValue dvalue = NodeValue::deserialize(sstream);
    ASSERT_TRUE(dvalue.isInitialized());

    std::stringstream dStream;
    dvalue.print(dStream);
    ASSERT_EQ(std::string("100"), dStream.str());

}

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
}

TEST(NodeValueTest, StringTest)
{
    const std::string expectedString = "100dqwdw90.876";
    NodeValue value(expectedString);

    std::stringstream sstream;
    value.serialize(sstream);
    uint8_t type;
    uint64_t size;
    auto temp = sstream.str();
    sstream.read(reinterpret_cast<char*>(&type), sizeof(type));
    ASSERT_EQ(2, type);

    sstream.read(reinterpret_cast<char*>(&size), sizeof(size));
    size = htonll(size);
    ASSERT_EQ(expectedString.size(), size);

    char buffer[size];
    sstream.read(buffer, size);
    ASSERT_STREQ(buffer, expectedString.c_str());

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ(expectedString, pStream.str());

// todo uncomment this when deserialize will be implemented
//    sstream = std::stringstream();
//    value.serialize(sstream);
//    NodeValue dvalue = NodeValue::deserialize(sstream);
//    ASSERT_TRUE(dvalue.isInitialized());
//
//    std::stringstream dStream;
//    dvalue.print(dStream);
//    ASSERT_EQ(expectedString, dStream.str());

}