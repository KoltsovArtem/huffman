#include "HuffmanDecoder.h"
#include "exceptions.h"

using namespace Huffman;

std::string reverse(std::string s) {
    std::string res;
    auto it = s.rbegin();
    while (it != s.rend()) {
        res.push_back(*it++);
    }
    return res;
}

std::string byte_bin(unsigned char c) {
    std::string s;
    for (int i = 0; i < 8; i++) {
        int bit = c % 2;
        if (bit == 0)
            s.push_back('0');
        else
            s.push_back('1');
        c /= 2;
    }
    return reverse(s);
}

HuffmanDecoder::HuffmanDecoder(std::istream* input, std::ostream* output) {
    in = input;
    out = output;

    T = HuffmanTree();
    key_length = 0;
}

void HuffmanDecoder::get_key_make_tree() {
    char c;
    float prob;

    in->read((char*)&key_length, 4);
    if (in->bad()) {
        throw FIOException("reading");
    }

    for (int i = 0; i < key_length; i++) {
        in->read(&c, 1);
        if (in->bad()) {
            throw FIOException("reading");
        }
        in->read((char*)&prob, 4);
        if (in->bad()) {
            throw FIOException("reading");
        }
        try {
            T.add(new HuffmanNode(c, prob));
        } catch (MemException& e) {
            throw e;
        }
    }
}

void HuffmanDecoder::build_tree() {
    while (T.size() > 1) {
        std::pair<HuffmanNode*, HuffmanNode*> p = T.find_two_min();

        try {
            T.unite(p.first, p.second);
        } catch (MemException& e) {
            throw e;
        }
    }
}

int HuffmanDecoder::find_input_length() {
    int pos = in->tellg();
    in->seekg(0, in->end);
    int end_pos = in->tellg();
    in->seekg(pos, in->beg);

    return end_pos;
}

HuffmanNode* HuffmanDecoder::decode_piece(HuffmanNode* current_node, std::string s, bool special_case) {
    auto it = s.begin();
    while (it != s.end()) {
        while(current_node->get_s().length() > 1 && it != s.end()) {
            if (*it == '1')
                current_node = current_node->right;
            else
                current_node = current_node->left;
            ++it;
        }
        if (current_node->get_s().length() == 1) {
            out->write(&(current_node->get_s()[0]), 1);
            if (out->bad()) {
                throw FIOException("writing");
            }

            current_node = *(T.get_nodes().begin());
            if (special_case)
                ++it;
        }
    }
    return current_node;
}

std::string HuffmanDecoder::delete_end_zeros(std::string s) {
    int l = s.length();
    int end_zeros = (s[l - 1] - 48) + 2 * (s[l - 2] - 48) + 4 * (s[l - 3] - 48);
    for (int i = 0; i < 8 + end_zeros; i++)
        s.pop_back();
    return s;
}

void HuffmanDecoder::decode() {
    if (T.size() == 0)
        return;

    int stop_pos = find_input_length() - 2;

    char c;
    std::string s;
    HuffmanNode* current_node = *(T.get_nodes().begin());
    bool special_case = current_node->get_s().length() == 1;

    while (in->tellg() < stop_pos && in->read(&c, 1)) {
        if (in->bad()) {
            throw FIOException("reading");
        }
        s = byte_bin((unsigned char) c);
        try {
            current_node = decode_piece(current_node, s, special_case);
        } catch (FIOException& e) {
            throw e;
        }
    }

    s.clear();
    while (in->read(&c, 1)) {
        if (in->bad()) {
            throw FIOException("reading");
        }
        s += byte_bin((unsigned char) c);
    }

    s = delete_end_zeros(s);

    try {
        current_node = decode_piece(current_node, s, special_case);
    } catch (FIOException& e) {
        throw e;
    }
}

void HuffmanDecoder::print_info() {
    in->clear(); in->seekg(0, in->end);
    std::cout << (int)in->tellg() - key_length * 5 - 5 << std::endl;
    std::cout << (int)out->tellp() << std::endl;
    std::cout << key_length * 5 + 5 << std::endl;
}