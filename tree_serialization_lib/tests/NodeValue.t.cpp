#include <gtest/gtest.h>
#include <netinet/in.h>

#include "NodeValue.h"

using namespace treesl;

TEST(NodeValueTest, IntTest) {
    NodeValue value(100);

    std::stringstream sstream;
    ASSERT_EQ(NodeError::SUCCESS, value.serialize(sstream));
    auto [dvalue, errorCode] = NodeValue::deserialize(sstream);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_EQ(value, dvalue);

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ(std::string("100"), pStream.str());
}

TEST(NodeValueTest, DoubleTest) {
    double expectedValue = 100.87662;
    NodeValue value(expectedValue);

    std::stringstream sstream;
    ASSERT_EQ(NodeError::SUCCESS, value.serialize(sstream));
    auto [dvalue, errorCode] = NodeValue::deserialize(sstream);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_EQ(value, dvalue);

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ("100.877", pStream.str()); // by default double precision is 3 digits
}

TEST(NodeValueTest, StringTest) {
    const std::string expectedString = "100dqwdw90.876";
    NodeValue value(expectedString);

    std::stringstream sstream;
    ASSERT_EQ(NodeError::SUCCESS, value.serialize(sstream));
    auto [dvalue, errorCode] = NodeValue::deserialize(sstream);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_EQ(value, dvalue);

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ(expectedString, pStream.str());
}