#include <gtest/gtest.h>
#include <netinet/in.h>

#include "NodeValue.h"

using namespace treesl;

//void testBody(NodeValue& value, const std::string& expectedOutput) {
//    std::stringstream sstream;
//    ASSERT_EQ(NodeError::SUCCESS, value.serialize(sstream));
//    auto [dvalue, errorCode] = NodeValue::deserialize(sstream);
//    ASSERT_EQ(errorCode, NodeError::SUCCESS);
//    ASSERT_EQ(value, dvalue);
//
//    std::stringstream pStream;
//    value.print(pStream);
//    ASSERT_EQ(expectedOutput, pStream.str());
//}
//
//TEST(NodeValueTest, IntTest) {
//    NodeValue value(100);
//
//    testBody(value, std::string("100"));
//}
//
//TEST(NodeValueTest, DoubleTest) {
//    double expectedValue = 100.87662;
//    NodeValue value(expectedValue);
//    testBody(value, std::string("100.877")); // by default double precision is 3 digits
//}
//
//TEST(NodeValueTest, StringTest) {
//    const std::string expectedString = "100dqwdw90.876";
//    NodeValue value(expectedString);
//
//    testBody(value, expectedString);
//}