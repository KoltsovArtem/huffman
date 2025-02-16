#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <cstddef>
#include <cstring>

#include "HuffmanTreeNode.h"
#include "HuffmanArchiver.h"
#include "HuffmanDecoder.h"
#include "exceptions.h"

using namespace Huffman;

typedef struct args_t {
    bool mode;
    char* input_path;
    char* output_path;
} Args;

Args parse_args(int argc, char** args) {
    if (argc != 6) {
        throw ArgNumException(5, argc - 1);
    }

    Args t;
    int i = 1;
    int args_read = 0;
    int keys[3] = { 0, 0, 0 };
    while (args_read < 3 && i < argc) {
        if (!strcmp(args[i], "-c")) {
            t.mode = true;
            ++keys[0];
        }
        else if (!strcmp(args[i], "-u")) {
            t.mode = false;
            ++keys[0];
        }
        else if (!strcmp(args[i], "-f") || !strcmp(args[i], "--file")) {
            t.input_path = args[++i];
            ++keys[1];
        }
        else if (!strcmp(args[i], "-o") || !strcmp(args[i], "--output")) {
            t.output_path = args[++i];
            ++keys[2];
        }
        ++args_read;
        ++i;
    }

    if (keys[0] != 1 || keys[1] != 1 || keys[2] != 1) {
        throw ExceptionOutput("Invalid keys");
    }

    return t;
}

int main(int argc, char** argv) {
    Args t;
    try {
        t = parse_args(argc, argv);
    } catch (ExceptionOutput& e) {
        e.out();
        return -1;
    }

    std::ifstream input(t.input_path, std::ios::binary);
    std::ofstream output(t.output_path, std::ios::binary);
    if (!input) {
        std::cout << "cannot open file " << t.input_path << std::endl;
        return -1;
    }
    if (!output) {
        std::cout << "cannot open file " << t.output_path << std::endl;
        return -1;
    }

    if (t.mode) {
        try {
            HuffmanEncoder Encoder((std::istream*)&input, (std::ostream*)&output);
            Encoder.count_chars();
            Encoder.restart_input();
            Encoder.dict_to_tree();
            Encoder.buid_tree();
            Encoder.build_codes();

            Encoder.encode();
            Encoder.print_info();
        } catch (MemException& e) {
            e.out();
            return -1;
        } catch (FIOException& e) {
            e.out();
            return -1;
        }
    }
    else {
        try {
            HuffmanDecoder Decoder((std::istream*)&input, (std::ostream*)&output);

            Decoder.get_key_make_tree();
            Decoder.build_tree();

            Decoder.decode();
            Decoder.print_info();
        } catch (MemException& e) {
            e.out();
            return -1;
        } catch (FIOException& e) {
            e.out();
            return -1;
        }
    }
    return 0;
}