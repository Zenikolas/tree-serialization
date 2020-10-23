#include "Tree.h"

#include <gtest/gtest.h>


TEST(TreeTest, Print)
{

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

    std::cout << sstream.str() << std::endl;
    std::string line;

//    std::getline(sstream, line);
//    ASSERT_EQ(std::string("8766"), line);
//
//    std::getline(sstream, line);
//    ASSERT_EQ(std::string("100 90.2 qwdqwd 9"), line);
//
//    std::getline(sstream, line);
//    ASSERT_EQ(std::string("786 12.12"), line);
//
//    std::getline(sstream, line);
//    ASSERT_EQ(std::string("12.42 1222ff ddqq22"), line);
}
