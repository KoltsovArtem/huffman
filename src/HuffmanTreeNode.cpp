#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

#include "HuffmanTreeNode.h"
#include "exceptions.h"

#define UNUSED(x) (void)(x)
#define TRESHOLD 400

using namespace Huffman;

HuffmanNode::HuffmanNode(std::string str, float p, HuffmanNode* r, HuffmanNode* l) {
    s = str;
    prob = p;
    right = r;
    left = l;
}

HuffmanNode::HuffmanNode(char c, float p) {
    s = std::string(1, c);
    prob = p;
    right = left = nullptr;
}

HuffmanNode::~HuffmanNode() {
    if (right != nullptr)
        delete right;
    if (left != nullptr)
        delete left;
}

bool HuffmanNode::operator<(const HuffmanNode& other) const {
    return prob < other.prob;
}

void HuffmanNode::print(int depth) const {
    if (right != nullptr)
        right->print(depth + 1);
    std::string shift = std::string(depth * 15, ' ');
    std::cout << shift << "(" << s << ": " << prob << ")" << std::endl;
    if (left != nullptr)
        left->print(depth + 1);
}

void HuffmanNode::build_codes(std::map<char, std::string>* codes, std::string str) const {
    if (right == nullptr && left == nullptr) {
        if (str.length() == 0) {
            codes->insert({s[0], std::string("1")});
        }
        else {
            codes->insert({s[0], str});
        }
    }

    if (right != nullptr)
        right->build_codes(codes, str + std::string("1"));

    if (left != nullptr)
        left->build_codes(codes, str + std::string("0"));
}

HuffmanTree::HuffmanTree() {
    nodes = std::set<HuffmanNode*, HuffmanComparator>();
}

HuffmanTree::~HuffmanTree() {
    auto it = nodes.begin();
    while (it != nodes.end()) {
        delete *it++;
    }
}

void HuffmanTree::add(HuffmanNode* node) {
    if (!node) {
        throw MemException("HuffmanNode*");
    }
    nodes.insert(nodes.begin(), node);
}

void HuffmanTree::unite(HuffmanNode* first, HuffmanNode* second) {
    auto it1 = nodes.find(first);
    auto it2 = nodes.find(second);
    if (it1 == nodes.end() || it2 == nodes.end())
        return;

    try {
        add(new HuffmanNode((*it1)->get_s() + (*it2)->get_s(), (*it1)->get_prob() + (*it2)->get_prob(), *it1, *it2));
    } catch (MemException& e) {
        throw e;
    }

    nodes.erase(*it1);
    nodes.erase(*it2);
}

std::pair<HuffmanNode*, HuffmanNode*> HuffmanTree::find_two_min() const {
    std::pair<HuffmanNode*, HuffmanNode*> p;
    auto it = nodes.begin();

    p.first = *it++;
    p.second = *it++;
    if (*(p.second) < *(p.first)) {
        auto tmp = p.first;
        p.first = p.second;
        p.second = tmp;
    }

    for (; it != nodes.end(); it++) {
        if (**it < *(p.first)) {
            p.second = p.first;
            p.first = *it;
        }
        else if (**it < *(p.second)) {
            p.second = *it;
        }
    }
    return p;
}

void HuffmanTree::print() const {
    auto it = --(nodes.end());
    do {
        (*it)->print(0);
    } while (it-- != nodes.begin());
}

int HuffmanTree::size() const {
    return nodes.size();
}

void HuffmanTree::build_codes(std::map<char, std::string>* codes) const{
    if (nodes.size() != 1)
        return;

    auto it = nodes.begin();
    HuffmanNode* node = *it;
    node->build_codes(codes, std::string(""));
}