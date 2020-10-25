#include <queue>
#include <iostream>
#include <stack>
#include <cassert>

#include "Tree.h"

std::unique_ptr<Node> getNodeFromStream(std::istream &ifs) {
    char message;
    ifs.read(reinterpret_cast<char*>(&message), sizeof(message));
    if (!ifs.good() || message == '/') {
        return nullptr;
    }

    return Node::deserialize(ifs);
}

std::shared_ptr<Node> Tree::deserialize(std::istream &ifs) {
    std::stack<std::shared_ptr<Node>> stack;
    std::shared_ptr<Node> root = getNodeFromStream(ifs);
    if (!root) {
        return nullptr;
    }
    stack.push(root);

    while (!stack.empty()) {
        auto &top = stack.top();
        assert(top);

        std::shared_ptr<Node> child = getNodeFromStream(ifs);
        if (!child) {
            stack.pop();
            continue;
        }

        top->appendChild(child);
        stack.push(child);
    }

    return root;
}

void Tree::print(std::ostream &os, const std::shared_ptr<Node>& root) {
    struct TreeNode {
        std::shared_ptr<Node> node;
        std::string intend;
        bool last;
    };

    std::stack<TreeNode> stack;
    stack.push({root, "", true});
    while (!stack.empty()) {
        TreeNode top = stack.top();
        stack.pop();

        os << top.intend << "+ " << *top.node << "\n";
        auto &childs = top.node->getChilds();
        if (childs.empty()) {
            continue;
        }

        std::string intend;
        if (top.last) {
            intend = top.intend + "\t";
        } else {
            intend = top.intend + "|\t";
        }

        stack.push({childs.back(), intend, true});
        for (size_t i = 0; i < childs.size() - 1; ++i) {
            stack.push({childs[childs.size() - 2 - i], intend, false});
        }
    }
}

bool Tree::serialize(std::ostream &os, const std::shared_ptr<Node>& root) {
    if (!root) {
        std::cerr << "invalid root in the given tree!" << std::endl;
        return false;
    }

    struct TreeNode {
        std::shared_ptr<Node> node;
        bool visited;
    };

    std::stack<TreeNode> stack;
    stack.push({root, false});
    while (!stack.empty()) {
        auto &top = stack.top();
        if (!top.node) {
            stack.pop();
            std::cerr << "invalid node in the given tree!" << std::endl;
            continue;
        }

        if (top.visited) {
            os.write("/", 1);
            stack.pop();
            continue;
        } else {
            top.visited = true;
        }

        os.write("+", 1);
        top.node->serialize(os);
        auto &childs = top.node->getChilds();
        for (auto it = childs.rbegin(); it != childs.rend(); ++it) {
            stack.push({*it, false});
        }
    }

    return true;
}


void Tree::traverseNLR(const std::shared_ptr<Node>& root, const std::function<void(Node *)> &func) {
    std::queue<std::shared_ptr<Node>> qq;
    qq.push(root);

    while (!qq.empty()) {
        std::shared_ptr<Node> current = qq.front();
        qq.pop();

        func(current.get());

        for (auto &child : current->getChilds()) {
            qq.push(child);
        }
    }
}