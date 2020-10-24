#include "Tree.h"

#include <gtest/gtest.h>


TEST(TreeTest, Print) {

    auto root = std::make_shared<Node>("8766");
    root->m_childs.emplace_back(std::make_shared<Node>(100));
    root->m_childs.emplace_back(std::make_shared<Node>(90.2));
    root->m_childs.emplace_back(std::make_shared<Node>("qwdqwd"));
    root->m_childs.emplace_back(std::make_shared<Node>(9));

    root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(786));
    root->m_childs[0]->m_childs.emplace_back(std::make_shared<Node>(12.12));

    root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>(12.42));
    root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("1222ff"));
    root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("ddqq22"));

    root->m_childs[2]->m_childs.emplace_back(std::make_shared<Node>("29.23"));

    root->m_childs[1]->m_childs.emplace_back(std::make_shared<Node>("1222ff"));

    std::stringstream sstream;
    Tree::print(sstream, root);

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
