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
    sstream >> type >> check_value;
    ASSERT_EQ(0, type);
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