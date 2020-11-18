#include <gtest/gtest.h>

#include "Node.h"

using namespace treesl;

TEST(NodeTest, AppendChildTest) {
    double firstChildValue = 90.2;
    std::string secondChildValue = "90.2poiuytr";
    auto node = Node::makeNode(100);

    ASSERT_TRUE(node->getChildes().empty());
    node->appendChild(Node::makeNode(firstChildValue));
    ASSERT_EQ(1, node->getChildes().size());
    node->appendChild(Node::makeNode(secondChildValue));
    ASSERT_EQ(2, node->getChildes().size());
    ASSERT_EQ(*Node::makeNode(firstChildValue), *node->getChildes()[0]);
    ASSERT_EQ(*Node::makeNode(secondChildValue), *node->getChildes()[1]);
}

TEST(NodeTest, SerialiseTest) {
    const std::string expectedString = "100dqwdw90.876";
    auto node = Node::makeNode(expectedString);

    std::stringstream sstream;
    ASSERT_EQ(NodeError::SUCCESS, node->serialize(sstream));
    auto [dvalue, errorCode] = Node::deserialize(sstream);
    ASSERT_TRUE(dvalue);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_EQ(*node, *dvalue);
}

TEST(NodeTest, PrintTest) {
    const std::string expectedString = "2.876";
    auto node = Node::makeNode(expectedString);
    std::stringstream pStream;
    node->print(pStream);
    ASSERT_EQ(expectedString, pStream.str());
}

void testBody(Node& value, const std::string& expectedOutput) {
    std::stringstream sstream;
    ASSERT_EQ(NodeError::SUCCESS, value.serialize(sstream));
    auto [dvalue, errorCode] = Node::deserialize(sstream);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_EQ(value, *dvalue);

    std::stringstream pStream;
    value.print(pStream);
    ASSERT_EQ(expectedOutput, pStream.str());
}

TEST(NodeTest, IntTest) {
    auto value = Node::makeNode(100);
    ASSERT_TRUE(value);

    testBody(*value, std::string("100"));
}

TEST(NodeTest, DoubleTest) {
    double expectedValue = 100.87662;
    auto value = Node::makeNode(expectedValue);
    ASSERT_TRUE(value);

    testBody(*value, std::string("100.877")); // by default double precision is 3 digits
}

TEST(NodeTest, StringTest) {
    const std::string expectedString = "100dqwdw90.876";
    auto value = Node::makeNode(expectedString);
    ASSERT_TRUE(value);

    testBody(*value, expectedString);
}