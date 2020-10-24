#include <queue>
#include <iostream>
#include <stack>
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

        for (auto &child : current->m_childs) {
            qq.push(child);
        }
    }
}

std::shared_ptr<Node> Tree::deserialize(std::string_view inputFile) {
    return nullptr;
}

namespace {
    struct TreeNode {
        std::shared_ptr<Node> node;
        std::string intend;
        bool last;
    };
}

void Tree::print(std::ostream &os, std::shared_ptr<Node> root) {
    std::stack<TreeNode> stack;
    stack.push({root, "", true});
    while (!stack.empty()) {
        TreeNode top = stack.top();
        stack.pop();

        os << top.intend << "+ " << *top.node << "\n";
        std::vector<std::shared_ptr<Node>> &childrens = top.node->m_childs;
        if (childrens.empty()) {
            continue;
        }

        std::string intend;
        if (top.last) {
            intend = top.intend + "\t";
        } else {
            intend = top.intend + "|\t";
        }

        stack.push({childrens.back(), intend, true});
        for (size_t i = 0; i < childrens.size() - 1; ++i) {
            stack.push({childrens[childrens.size() - 2 - i], intend, false});
        }
    }

}

bool Tree::serialize(std::string_view outFile) {
    return false;
}
