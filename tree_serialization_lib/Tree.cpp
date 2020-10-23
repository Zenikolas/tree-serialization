#include <queue>
#include <iostream>
#include "Tree.h"

void Tree::traverse(std::shared_ptr<Node> root) {
    std::queue<std::shared_ptr<Node>> qq;
    qq.push(root);
    qq.push(nullptr);

    while (!qq.empty()) {
        std::shared_ptr<Node> current = qq.front();
        qq.pop();

        if (!current) {
            std::cout << "\n";
            continue;
        }

        std::cout << *current;

        for (auto& child : current->m_childs) {
            qq.push(child);
        }
    }
}

std::shared_ptr<Node> Tree::deserialize(std::string_view inputFile) {
    return nullptr;
}

//void Tree::print(std::ostream& os, std::shared_ptr<Node> root) {
//    std::queue<std::shared_ptr<Node>> qq;
//    qq.push(root);
//    qq.push(nullptr);
//
//    while (!qq.empty()) {
//        std::shared_ptr<Node> current = qq.front();
//        qq.pop();
//
//        if (!current) {
//            os << "\n";
//            continue;
//        }
//
//        os << *current << " ";
//
//        for (auto& child : current->m_childs) {
//            qq.push(child);
//        }
//        qq.push(nullptr);
//    }
//}

void Tree::print(std::ostream& os, std::shared_ptr<Node> root) {
    std::queue<std::shared_ptr<Node>> qq;
    os << *root << "\n";
    qq.push(root);
    qq.push(nullptr);

    while (!qq.empty()) {
        std::shared_ptr<Node> current = qq.front();
        qq.pop();

        if (!current) {
            os << std::endl; // todo "\n"
            continue;
        }

        for (auto& child : current->m_childs) {
            os << *child << " ";
            qq.push(child);
        }

        if (qq.front()) {
            os << "\t";
        }
        qq.push(nullptr);
    }
}

bool Tree::serialize(std::string_view outFile) {
    return false;
}
