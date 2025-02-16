#include "HuffmanArchiver.h"
#include "exceptions.h"

#define TRESHOLD 400

using namespace Huffman;

unsigned char code_to_int(const std::string* s, int pos) {
    unsigned char res = 0;
    int mult = 1;

    for (int i = pos + 7; i >= pos; i--) {
        res += (int)((char)(*s)[i] == '1') * mult;
        mult *= 2;
    }

    return res;
}

HuffmanEncoder::HuffmanEncoder(std::istream* input, std::ostream* output) {
    in = input;
    out = output;

    buff_size = TRESHOLD;

    dict = std::map<char, int>();

    codes = new std::map<char, std::string>;
    if (!codes) {
        throw MemException("std::map<char, std::string>");
    }

    T = HuffmanTree();
    total = 0;
}

HuffmanEncoder::~HuffmanEncoder() {
    delete codes;
}

void HuffmanEncoder::count_chars() {
    char c;

    while (in->read(&c, 1)) {
        if (in->bad()) {
            throw FIOException("reading");
        }
        auto it = dict.find(c);
        if (it == dict.end()) {
            dict.insert({c, 1});
        }
        else {
            it->second++;
        }
    }

    in->clear(); in->seekg(0, in->end);
    total = in->tellg();
}

void HuffmanEncoder::dict_to_tree() {
    for (auto it = dict.begin(); it != dict.end(); it++) {
        try {
            T.add(new HuffmanNode(it->first, (float)it->second / (float)total));
        } catch (MemException& e) {
            throw e;
        }
    }
}

void HuffmanEncoder::buid_tree() {
    while (T.size() > 1) {
        std::pair<HuffmanNode*, HuffmanNode*> p = T.find_two_min();

        try {
            T.unite(p.first, p.second);
        } catch (MemException& e) {
            throw e;
        }
    }
}

void HuffmanEncoder::build_codes() {
    T.build_codes(codes);
}

void HuffmanEncoder::encode_key() {
    int key_size = dict.size();
    out->write((char*)&key_size, 4);
    if (out->bad()) {
        throw FIOException("writing");
    }

    auto it = dict.begin();
    while (it != dict.end()) {
        float prob = (float)it->second / (float)total;
        out->write(&(it->first), 1);
        if (out->bad()) {
            throw FIOException("writing");
        }
        out->write((char*)&prob, 4);
        if (out->bad()) {
            throw FIOException("writing");
        }
        ++it;
    }
}

void HuffmanEncoder::write_iteration(std::string accum, size_t max_pos) {
    for (size_t pos = 0; pos < max_pos; pos += 8) {
        char code_num = code_to_int(&accum, pos);
        out->write(&code_num, 1);
        if (out->bad()) {
            throw FIOException("writing");
        }
    }
}

void HuffmanEncoder::print_info() {
    in->clear(); in->seekg(0, in->end);
    std::cout << (int)in->tellg() << std::endl;
    std::cout << (int)out->tellp() - dict.size() * 5 - 5 << std::endl;
    std::cout << dict.size() * 5 + 5 << std::endl;
}

int HuffmanEncoder::encode() {
    std::string accum, s;
    char c;
    try {
        encode_key();
    } catch (FIOException& e) {
        throw e;
    }

    while (in->read(&c, 1)) {
        accum += (*codes)[c];
        if (accum.length() > buff_size) {
            for (size_t i = buff_size; i < accum.length(); i++)
                s.push_back(accum[i]);
            try {
                write_iteration(accum, buff_size);
            } catch (FIOException& e) {
                throw e;
            }
            accum = s;
            s.clear();
        }
    }

    char end_zeros = (char)((8 - accum.length() % 8) % 8);
    if (accum.length() % 8 != 0) {
        accum += std::string((int)end_zeros, '0');
    }
    try {
        write_iteration(accum, accum.length());
    } catch (FIOException& e) {
        throw e;
    }

    out->write(&end_zeros, 1);
    if (out->bad()) {
        throw FIOException("writing");
    }

    return 0;
}

void HuffmanEncoder::restart_input() {
    in->clear();
    in->seekg(0, in->beg);
}

int HuffmanEncoder::get_tree_size() {
    return T.size();
}