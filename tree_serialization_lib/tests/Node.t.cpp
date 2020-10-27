#include <gtest/gtest.h>

#include "Node.h"

using namespace treesl;

TEST(NodeTest, AppendChildTest) {
    double firstChildValue = 90.2;
    const char* secondChildValue = "90.2poiuytr";
    Node node(100);

    ASSERT_TRUE(node.getChildes().empty());
    node.appendChild(std::make_unique<Node>(firstChildValue));
    ASSERT_EQ(1, node.getChildes().size());
    node.appendChild(std::make_unique<Node>(secondChildValue));
    ASSERT_EQ(2, node.getChildes().size());
    ASSERT_EQ(Node(firstChildValue), *node.getChildes()[0]);
    ASSERT_EQ(Node(secondChildValue), *node.getChildes()[1]);
}

TEST(NodeTest, SerialiseTest) {
    const std::string expectedString = "100dqwdw90.876";
    Node node(expectedString);

    std::stringstream sstream;
    node.serialize(sstream);
    std::unique_ptr<Node> dvalue = Node::deserialize(sstream);
    ASSERT_TRUE(dvalue);
    ASSERT_EQ(node, *dvalue);
}

TEST(NodeTest, PrintTest) {
    const std::string expectedString = "2.876";
    Node node(expectedString);
    std::stringstream pStream;
    node.print(pStream);
    ASSERT_EQ(expectedString, pStream.str());
}