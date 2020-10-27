/*!
\file
\brief cmd application demonstrating how serialisation for trees works
\author Nikolay Zemtsovskiy

 Application does the following:
  1. reads the input file containing the serialized tree
  2. deserializes the tree
  3. prints the tree to stdout
  4. serializes the tree to the output file
*/

#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>

#include "TreeUtil.h"

void usage(const char* appName) {
    std::cerr << appName << " does the following order of actions:\n"
                            "  1) reads the input file containing the serialized tree\n"
                            "  2) deserializes the tree\n"
                            "  3) prints the tree to stdout\n"
                            "  4) serializes the tree to the output file\n\n"
              << "Usage: " << appName << " -i <inputFile> -o <outputFile>\n"
              << std::flush;
}

int main(int argc, char* argv[]) {
    int c;
    const char* inputFile = nullptr;
    const char* outfileFile = nullptr;

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
        return EXIT_SUCCESS;
    }

    std::unique_ptr<treesl::Node> root;
    {
        std::ifstream ifs(inputFile);
        if (!ifs.good()) {
            std::cerr << "Failed to open for reading input file: " << inputFile <<
                      std::endl;
            return EXIT_FAILURE;
        }
        root = treesl::TreeUtil::deserialize(ifs);
    }

    if (!root) {
        std::cerr << "Failed to read tree from the given file: " << inputFile
                  << std::endl;
        return EXIT_FAILURE;
    }

    treesl::TreeUtil::print(std::cout, root.get());
    std::cout << std::flush;

    {
        std::ofstream ofs(outfileFile);
        if (!ofs.good()) {
            std::cerr << "Failed to open for writing output file: " << outfileFile <<
                      std::endl;
            return EXIT_FAILURE;
        }
        if (!treesl::TreeUtil::serialize(ofs, root.get())) {
            std::cerr << "Failed to serialize tree to file: " << outfileFile << std::endl;
            return EXIT_FAILURE;
        }
    }

    return 0;
}
