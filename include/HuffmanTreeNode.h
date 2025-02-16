#ifndef HW_02_HUFFMAN_HUFFMANTREENODE_H
#define HW_02_HUFFMAN_HUFFMANTREENODE_H

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

#define UNUSED(x) (void)(x)

namespace Huffman {
    class HuffmanNode {
        friend class HuffmanTester;
    public:
        HuffmanNode(std::string str, float p, HuffmanNode* r, HuffmanNode* l);
        HuffmanNode(char c, float p);
        ~HuffmanNode();

        const std::string& get_s() const
        {
            return s;
        }
        const float& get_prob() const
        {
            return prob;
        }

        HuffmanNode* right;
        HuffmanNode* left;

        bool operator<(const HuffmanNode& other) const;
        void print(int depth) const;
        void build_codes(std::map<char, std::string>* codes, std::string s) const;
    private:
        std::string s;
        float prob;
    };

    struct HuffmanComparator {
        bool operator()( const HuffmanNode* a, const HuffmanNode* b ) const {
            return a->get_s() < b->get_s();
        }
    };

    class HuffmanTree {
        friend class HuffmanTester;
    public:
        HuffmanTree();
        ~HuffmanTree();

        void print() const;
        int size() const;
        void add(HuffmanNode* node);
        void unite(HuffmanNode* first, HuffmanNode* second);
        std::pair<HuffmanNode*, HuffmanNode*> find_two_min() const;
        void build_codes(std::map<char, std::string>* codes) const;

        const std::set<HuffmanNode*, HuffmanComparator>& get_nodes() const
        {
            return nodes;
        }
    private:
        std::set<HuffmanNode*, HuffmanComparator> nodes;
    };
}

#endif //HW_02_HUFFMAN_HUFFMANTREENODE_H