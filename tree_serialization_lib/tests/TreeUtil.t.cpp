#include "TreeUtil.h"

#include <gtest/gtest.h>

#include <fstream>

using namespace treesl;

/// @cond DEV
class TreeTest : public ::testing::Test {
protected:
    std::unique_ptr<Node> m_root;

    TreeTest() {
        m_root = std::make_unique<Node>("8766");
        m_root->appendChild(std::make_unique<Node>(100));
        m_root->appendChild(std::make_unique<Node>(90.2));
        m_root->appendChild(std::make_unique<Node>("qwdqwd"));
        m_root->appendChild(std::make_unique<Node>(9));

        m_root->getChildes()[0]->appendChild(std::make_unique<Node>(786));
        m_root->getChildes()[0]->appendChild(std::make_unique<Node>(12.12));

        m_root->getChildes()[1]->appendChild(std::make_unique<Node>(12.42));
        m_root->getChildes()[1]->appendChild(std::make_unique<Node>("1222ff"));
        m_root->getChildes()[1]->appendChild(std::make_unique<Node>("ddqq22"));

        m_root->getChildes()[2]->appendChild(std::make_unique<Node>("29.23"));
    }
};
/// @endcond

TEST_F(TreeTest, Print) {
    std::stringstream sstream;
    TreeUtil::print(sstream, m_root.get());

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
    std::vector<const Node*> expectedNodes;
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

    std::vector<const Node*> resultNodes;
    TreeUtil::traverseNLR(m_root.get(),
                          [&resultNodes](const Node* node) {
                              resultNodes.emplace_back
                                      (node);
                          });
    ASSERT_EQ(expectedNodes, resultNodes);
}

TEST_F(TreeTest, NullTest) {
    std::stringstream sstream;
    ASSERT_NE(TreeUtil::serialize(sstream, nullptr), NodeError::SUCCESS);
    ASSERT_TRUE(sstream.good());
    ASSERT_TRUE(sstream.str().empty());

    TreeUtil::print(sstream, nullptr);
    ASSERT_TRUE(sstream.good());
    ASSERT_TRUE(sstream.str().empty());

    std::vector<const Node*> resultNodes;
    TreeUtil::traverseNLR(nullptr,
                          [&resultNodes](const Node* node) {
                              resultNodes.emplace_back
                                      (node);
                          });
    ASSERT_TRUE(resultNodes.empty());

    std::ifstream ifs;
    auto [root, errorCode] = TreeUtil::deserialize(ifs);
    ASSERT_NE(errorCode, NodeError::SUCCESS);
    ASSERT_FALSE(root);
}

TEST_F(TreeTest, Serialize) {
    std::stringstream sstream;

    std::vector<const Node*> expectedNodes;
    TreeUtil::traverseNLR(m_root.get(),
                          [&expectedNodes](const Node* node) {
                              expectedNodes.emplace_back
                                      (node);
                          });

    TreeUtil::serialize(sstream, m_root.get());
    {
        std::ofstream ofs("input.dat");
        TreeUtil::serialize(ofs, m_root.get());
    }
    auto [root, errorCode] = TreeUtil::deserialize(sstream);
    ASSERT_EQ(errorCode, NodeError::SUCCESS);
    ASSERT_TRUE(root);

    std::vector<const Node*> resultNodes;
    TreeUtil::traverseNLR(root.get(),
                          [&resultNodes](const Node* node) {
                              resultNodes.emplace_back
                                      (node);
                          });

    ASSERT_EQ(expectedNodes.size(), resultNodes.size());
    for (size_t i = 0; i < expectedNodes.size(); ++i) {
        ASSERT_EQ(*expectedNodes[i], *resultNodes[i]);
    }
}