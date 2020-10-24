#include "Tree.h"

#include <gtest/gtest.h>

class TreeTest : public ::testing::Test {
protected:
    std::shared_ptr<Node> m_root;

    TreeTest() {
//        m_root = std::make_shared<Node>("8766");
//        m_root->m_childs.emplace_back(std::make_shared<Node>(100));
//        m_root->m_childs.emplace_back(std::make_shared<Node>(90.2));
//        m_root->m_childs.emplace_back(std::make_shared<Node>("qwdqwd"));
//        m_root->m_childs.emplace_back(std::make_shared<Node>(9));
//
//        m_root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(786));
//        m_root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(12.12));
//
//        m_root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>(12.42));
//        m_root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("1222ff"));
//        m_root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("ddqq22"));
//
//        m_root->m_childs[2]->m_childs.emplace_back(std::make_shared<Node>("29.23"));
//
//        m_root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("1222ff"));

        m_root = std::make_shared<Node>(1);
        m_root->m_childs.emplace_back(std::make_shared<Node>(2));
        m_root->m_childs.emplace_back(std::make_shared<Node>(3));

        m_root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(4));
        m_root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(5));

        m_root->m_childs[0]->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>(6));
    }
};

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
            "\t|\t+ 1222ff",
            "\t+ qwdqwd",
            "\t|\t+ 29.23",
            "\t+ 9"
    };
    std::string line;
    size_t i = 0;
    while (std::getline(sstream, line)) {
        ASSERT_LE(i , lines.size());
        ASSERT_EQ(lines[i], line);
        ++i;
    }
}

TEST_F(TreeTest, Serialize) {
    std::stringstream sstream;

    Tree::print(std::cout, m_root);
    Tree::serialize(sstream, m_root);

    auto root = Tree::deserialize(sstream);
    Tree::print(std::cout, root);
}