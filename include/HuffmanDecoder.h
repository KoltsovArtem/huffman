#ifndef HW_02_HUFFMAN_HUFFMANDECODER_H
#define HW_02_HUFFMAN_HUFFMANDECODER_H

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

#include "HuffmanTreeNode.h"

std::string reverse(std::string s);
std::string byte_bin(unsigned char c);


namespace Huffman {

    class HuffmanDecoder {
        friend class HuffmanTester;
    public:
        HuffmanDecoder(std::istream* input, std::ostream* output);

        void decode();

        void get_key_make_tree();
        void build_tree();
        void print_info();

    private:
        std::string delete_end_zeros(std::string s);
        HuffmanNode* decode_piece(HuffmanNode* current_node, std::string s, bool special_case);
        int find_input_length();

    private:
        std::istream* in;
        std::ostream* out;
        HuffmanTree T;
        int key_length;
    };
}

#endif //HW_02_HUFFMAN_HUFFMANDECODER_H