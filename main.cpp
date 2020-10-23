#include <iostream>
#include <memory>

#include "Tree.h"

int main() {
    const char* inputFile = "tree_serialization.in.dat";
    const char* outfileFile = "tree_serialization.out.dat";
    std::shared_ptr<Node> root = Tree::deserialize(inputFile);
    if (!root) {
        std::cerr << "Failed to read tree from the given file: " << inputFile << std::endl;
        return 1;
    }

    Tree::print(root);

    if (!Tree::serialize(outfileFile)) {
        std::cerr << "Failed to serialize tree to file: " << outfileFile << std::endl;
        return 3;
    }

    return 0;
}
