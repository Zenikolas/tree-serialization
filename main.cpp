#include <iostream>
#include <memory>
#include <fstream>

#include "Tree.h"

int main() {
    const char* inputFile = "tree_serialization.in.dat";
    const char* outfileFile = "tree_serialization.out.dat";

    std::shared_ptr<Node> root;
    {
        std::ifstream ifs(inputFile);
        root = Tree::deserialize(ifs);
    }

    if (!root) {
        std::cerr << "Failed to read tree from the given file: " << inputFile << std::endl;
        return 1;
    }
    Tree::print(std::cout, root);

    {
        std::ofstream ofs(outfileFile);
        if (!Tree::serialize(ofs, root)) {
            std::cerr << "Failed to serialize tree to file: " << outfileFile << std::endl;
            return 3;
        }
    }

    return 0;
}
