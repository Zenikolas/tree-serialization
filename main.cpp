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
#include <cassert>

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

    std::unique_ptr<treesl::Node> root = nullptr;
    treesl::NodeError errorCode = treesl::NodeError::SUCCESS;
    {
        std::ifstream ifs(inputFile);
        if (!ifs.good()) {
            std::cerr << "Failed to open for reading input file: " << inputFile <<
                      std::endl;
            return EXIT_FAILURE;
        }
        std::tie(root, errorCode) = treesl::TreeUtil::deserialize(ifs);
    }

    if (errorCode != treesl::NodeError::SUCCESS) {
        std::cerr << "Failed to read tree from the given file: " << inputFile
                  << " - " << errorCode << std::endl;
        return EXIT_FAILURE;
    }

    assert(root && "Root must be not null after deserialization");

    treesl::TreeUtil::print(std::cout, root.get());
    std::cout << std::flush;

    {
        std::ofstream ofs(outfileFile);
        if (!ofs.good()) {
            std::cerr << "Failed to open for writing output file: " << outfileFile <<
                      std::endl;
            return EXIT_FAILURE;
        }

        errorCode = treesl::TreeUtil::serialize(ofs, root.get());
        if (errorCode != treesl::NodeError::SUCCESS) {
            std::cerr << "Failed to serialize tree to file: " << outfileFile << " - " << errorCode << std::endl;
            return EXIT_FAILURE;
        }
    }

    return 0;
}
