#include <gtest/gtest.h>

#include "Node.h"

using namespace treesl;

TEST(NodeTest, AppendChildTest) {
    Node node(100);
    auto firstChild = std::make_shared<Node>(90.2);
    auto secondChild = std::make_shared<Node>("90.2poiuytr");

    ASSERT_TRUE(node.getChildes().empty());
    node.appendChild(firstChild);
    ASSERT_EQ(1, node.getChildes().size());
    node.appendChild(secondChild);
    ASSERT_EQ(2, node.getChildes().size());
    ASSERT_EQ(firstChild, node.getChildes()[0]);
    ASSERT_EQ(secondChild, node.getChildes()[1]);
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