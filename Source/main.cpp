#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Using: Club.exe path_to_input_file" << '\n';
        return 0;
    }

    Parser parser(argv[1]);

    if (!parser.Ready()) {
        return 1;
    }

    parser.Run();

    return 0;
}
