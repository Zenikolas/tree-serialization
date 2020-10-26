#include "Tree.h"

#include <gtest/gtest.h>

#include <fstream>

/// @cond DEV
class TreeTest : public ::testing::Test {
protected:
    std::shared_ptr<Node> m_root;

    TreeTest() {
        m_root = std::make_shared<Node>("8766");
        m_root->appendChild(std::make_shared<Node>(100));
        m_root->appendChild(std::make_shared<Node>(90.2));
        m_root->appendChild(std::make_shared<Node>("qwdqwd"));
        m_root->appendChild(std::make_shared<Node>(9));

        m_root->getChildes()[0]->appendChild(std::make_shared<Node>(786));
        m_root->getChildes()[0]->appendChild(std::make_shared<Node>(12.12));

        m_root->getChildes()[1]->appendChild(std::make_shared<Node>(12.42));
        m_root->getChildes()[1]->appendChild(std::make_shared<Node>("1222ff"));
        m_root->getChildes()[1]->appendChild(std::make_shared<Node>("ddqq22"));

        m_root->getChildes()[2]->appendChild(std::make_shared<Node>("29.23"));
    }
};
/// @endcond

TEST_F(TreeTest, Print) {
    std::stringstream sstream;
    Tree::print(sstream, m_root);

    std::vector<std::string> lines = {
            "+ 8766",
            "\t+ 100",
            "\t|\t+ 786",
            "\t|\t+ 12.12",
            "\t+ 90.2",
            "\t|\t+ 12.42",
            "\t|\t+ 1222ff",
            "\t|\t+ ddqq22",
            "\t+ qwdqwd",
            "\t|\t+ 29.23",
            "\t+ 9"
    };
    std::string line;
    size_t i = 0;
    while (std::getline(sstream, line)) {
        ASSERT_LE(i, lines.size());
        ASSERT_EQ(lines[i], line);
        ++i;
    }
}

TEST_F(TreeTest, Traverse) {
    std::vector<Node *> expectedNodes;
    expectedNodes.emplace_back(m_root.get());
    expectedNodes.emplace_back(m_root->getChildes()[0].get());
    expectedNodes.emplace_back(m_root->getChildes()[1].get());
    expectedNodes.emplace_back(m_root->getChildes()[2].get());
    expectedNodes.emplace_back(m_root->getChildes()[3].get());
    expectedNodes.emplace_back(m_root->getChildes()[0]->getChildes()[0].get());
    expectedNodes.emplace_back(m_root->getChildes()[0]->getChildes()[1].get());
    expectedNodes.emplace_back(m_root->getChildes()[1]->getChildes()[0].get());
    expectedNodes.emplace_back(m_root->getChildes()[1]->getChildes()[1].get());
    expectedNodes.emplace_back(m_root->getChildes()[1]->getChildes()[2].get());
    expectedNodes.emplace_back(m_root->getChildes()[2]->getChildes()[0].get());

    std::vector<Node *> resultNodes;
    Tree::traverseNLR(m_root,
                      [&resultNodes](Node *node) { resultNodes.emplace_back(node); });
    ASSERT_EQ(expectedNodes, resultNodes);
}

TEST_F(TreeTest, NullTest) {
    std::stringstream sstream;
    ASSERT_FALSE(Tree::serialize(sstream, nullptr));
    ASSERT_TRUE(sstream.good());
    ASSERT_TRUE(sstream.str().empty());

    Tree::print(sstream, nullptr);
    ASSERT_TRUE(sstream.good());
    ASSERT_TRUE(sstream.str().empty());

    std::vector<Node *> resultNodes;
    Tree::traverseNLR(nullptr,
                      [&resultNodes](Node *node) { resultNodes.emplace_back(node); });
    ASSERT_TRUE(resultNodes.empty());

    std::ifstream ifs;
    ASSERT_FALSE(Tree::deserialize(ifs));
}

TEST_F(TreeTest, Serialize) {
    std::stringstream sstream;

    std::vector<Node *> expectedNodes;
    Tree::traverseNLR(m_root,
                      [&expectedNodes](Node *node) { expectedNodes.emplace_back(node); });

    Tree::serialize(sstream, m_root);
    auto root = Tree::deserialize(sstream);

    std::vector<Node *> resultNodes;
    Tree::traverseNLR(root,
                      [&resultNodes](Node *node) { resultNodes.emplace_back(node); });

    ASSERT_EQ(expectedNodes.size(), resultNodes.size());
    for (size_t i = 0; i < expectedNodes.size(); ++i) {
        ASSERT_EQ(*expectedNodes[i], *resultNodes[i]);
    }
}