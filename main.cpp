#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>

#include "Tree.h"

void usage(const char *appName) {
    std::cerr << appName << " does the following order of actions:\n"
                 "  1) reads the input file containing the serialized tree\n"
                 "  2) deserializes the tree\n"
                 "  3) prints the tree to stdout\n"
                 "  4) serializes the tree to the output file\n\n"
              << "Usage: " << appName << " -i <inputFile> -o <outputFile>\n"
              << std::flush;
}

int main(int argc, char *argv[]) {
    int c;
    const char *inputFile = nullptr;
    const char *outfileFile = nullptr;

    while ((c = getopt(argc, argv, "i:o:h::")) != -1) {
        switch (c) {
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outfileFile = optarg;
                break;
            case 'h':
            case '?':
                usage(argv[0]);
                return 1;
            default:
                break;
        }
    }

    if (!inputFile || !outfileFile) {
        usage(argv[0]);
        return 1;
    }

    std::shared_ptr<Node> root;
    {
        std::ifstream ifs(inputFile);
        root = Tree::deserialize(ifs);
    }

    if (!root) {
        std::cerr << "Failed to read tree from the given file: " << inputFile << std::endl;
        return 2;
    }

    Tree::print(std::cout, root);
    std::cout << std::flush;

    {
        std::ofstream ofs(outfileFile);
        if (!Tree::serialize(ofs, root)) {
            std::cerr << "Failed to serialize tree to file: " << outfileFile << std::endl;
            return 3;
        }
    }

    return 0;
}
