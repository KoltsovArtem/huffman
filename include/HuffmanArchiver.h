#ifndef HW_02_HUFFMAN_HUFFMANARCHIVER_H
#define HW_02_HUFFMAN_HUFFMANARCHIVER_H

#include <set>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>

#include "HuffmanTreeNode.h"

unsigned char code_to_int(const std::string* s, int pos);

namespace Huffman {

    class HuffmanEncoder {
        friend class HuffmanTester;
    public:
        HuffmanEncoder(std::istream* input, std::ostream* output);
        ~HuffmanEncoder();

        void count_chars();
        void dict_to_tree();
        void buid_tree();
        void build_codes();

        int encode();
        void restart_input();
        void print_info();

        int get_tree_size();

    private:
        void encode_key();
        void write_iteration(std::string accum, size_t max_pos);

    private:
        std::istream* in;
        std::ostream* out;
        size_t buff_size;

    private:
        std::map<char, int> dict;
        HuffmanTree T;
        std::map<char, std::string>* codes;
        int total;
    };
}

#endif //HW_02_HUFFMAN_HUFFMANARCHIVER_H