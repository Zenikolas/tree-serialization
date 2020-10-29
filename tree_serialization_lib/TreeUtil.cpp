#include <queue>
#include <iostream>
#include <stack>
#include <cassert>

#include "TreeUtil.h"

namespace treesl {
std::pair<std::unique_ptr<Node>, NodeError> getNodeFromStream(std::istream& ifs) {
    char message;
    ifs.read(&message, sizeof(message));
    if (!ifs.good()) {
        return {nullptr, NodeError::INVALID_STREAM};
    }

    if (message == '/') {
        return {nullptr, NodeError::SUCCESS};
    }

    return Node::deserialize(ifs);
}

std::pair<std::unique_ptr<Node>, NodeError> TreeUtil::deserialize(std::istream& ifs) {
    if (!ifs.good()) {
        return {nullptr, NodeError::INVALID_STREAM};
    }
    std::stack<Node*> stack;
    auto[root, errorCode] = getNodeFromStream(ifs);
    if (errorCode != NodeError::SUCCESS) {
        return {nullptr, errorCode};
    }

    if (!root) {
        return {nullptr, NodeError::INVALID_FIRST_TOKEN_STREAM};
    }

    stack.push(root.get());
    while (!stack.empty()) {
        auto& top = stack.top();
        assert(top && "Stack node pointers must be not null");

        auto[child, childErrorCode] = getNodeFromStream(ifs);
        if (childErrorCode != NodeError::SUCCESS) {
            return {nullptr, childErrorCode};
        }

        if (!child) {
            stack.pop();
            continue;
        }

        top->appendChild(std::move(child));
        Node* lastChildren = top->getChildes().back().get();
        stack.push(lastChildren);
    }

    return {std::move(root), NodeError::SUCCESS};
}

void TreeUtil::print(std::ostream& os, const Node* root) {
    if (!root) {
        std::cerr << "invalid root in the given tree!" << std::endl;
        return;
    }

    struct TreeNode {
        const Node* node;
        std::string intend;
        bool last;
    };

    std::stack<TreeNode> stack;
    stack.push({.node = root, .intend = "", .last = true});
    while (!stack.empty()) {
        TreeNode top = stack.top();
        stack.pop();

        os << top.intend << "+ " << *top.node << "\n";
        auto& childs = top.node->getChildes();
        if (childs.empty()) {
            continue;
        }

        std::string intend;
        if (top.last) {
            intend = top.intend + "\t";
        } else {
            intend = top.intend + "|\t";
        }

        stack.push({.node = childs.back().get(), .intend = intend, .last = true});
        const size_t lastElemIndex = childs.size() - 1;
        const size_t preLastElemIndex = lastElemIndex - 1;
        for (size_t i = 0; i < lastElemIndex; ++i) {
            const Node* currentChild = childs[preLastElemIndex - i].get();
            stack.push({.node = currentChild, .intend = intend, .last =false});
        }
    }
}

NodeError TreeUtil::serialize(std::ostream& os, const Node* root) {
    if (!root) {
        return NodeError::INVALID_TREE_ROOT;
    }

    struct TreeNode {
        const Node* node;
        bool visited;
    };

    std::stack<TreeNode> stack;
    stack.push({.node = root, .visited = false});
    while (!stack.empty()) {
        auto& top = stack.top();
        if (!top.node) {
            return NodeError::INVALID_TREE_CHILD;
        }

        if (top.visited) {
            os.write("/", 1);
            stack.pop();
            continue;
        } else {
            top.visited = true;
        }

        os.write("+", 1);
        NodeError errorCode = top.node->serialize(os);
        if (errorCode != NodeError::SUCCESS) {
            return errorCode;
        }

        auto& childs = top.node->getChildes();
        for (auto it = childs.rbegin(); it != childs.rend(); ++it) {
            stack.push({.node = it->get(), .visited = false});
        }
    }

    return NodeError::SUCCESS;
}


void TreeUtil::traverseNLR(const Node* root,
                           const std::function<void(const Node*)>& func) {
    if (!root) {
        std::cerr << "invalid root in the given tree!" << std::endl;
        return;
    }
    std::queue<const Node*> qq;
    qq.push(root);

    while (!qq.empty()) {
        auto current = qq.front();
        qq.pop();

        func(current);

        for (auto& child : current->getChildes()) {
            qq.push(child.get());
        }
    }
}
}