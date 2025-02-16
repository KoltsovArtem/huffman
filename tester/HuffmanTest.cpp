#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <utility>
#include <sstream>
#include <string>
#include <vector>

#include "HuffmanTest.h"

TEST_CASE("HuffmanNode tests.") {
    Huffman::HuffmanNode n1('p', 0.25);
    Huffman::HuffmanNode n2('s', 0.35);
    Huffman::HuffmanNode n3('g', 0.1);
    Huffman::HuffmanNode n4(' ', 0.1);

    CHECK(!(n2 < n1));
    CHECK(!(n4 < n3));
}


TEST_CASE("HuffmanArchiver and HuffmanDecoder tests.") {
    std::vector<std::string> strings;

    std::string str1("Huffman was a man!");
    std::string str2("Chtoby ne vylitet' is vuza \n nado postarat'sa");
    std::string str3("абвгдеёжзиклмнопрстуфхцчшщъыьэюя");
    std::string str4("Тут мог быть текст песни 'Лесник' КиШа");

    strings.push_back(str1);
    strings.push_back(str2);
    strings.push_back(str3);
    strings.push_back(str4);

    for (auto it = strings.begin(); it != strings.end(); ++it) {
        std::string str = *it;
        std::istringstream in(str);
        std::ostringstream out;

        Huffman::HuffmanTester Tester;

        Huffman::HuffmanEncoder Encoder((std::istream*)&in, (std::ostream*)&out);
        Tester.add_encoder(&Encoder);

        Encoder.count_chars();
        Encoder.restart_input();

        Encoder.dict_to_tree();
        Tester.save_enc_order();

        Encoder.buid_tree();
        REQUIRE(Encoder.get_tree_size() == 1);
        Encoder.build_codes();
        Encoder.encode();

        std::string result = out.str();
        std::istringstream in2(result);
        std::ostringstream out2;

        Huffman::HuffmanDecoder Decoder((std::istream*)&in2, (std::ostream*)&out2);
        Tester.add_decoder(&Decoder);

        Decoder.get_key_make_tree();
        Tester.save_dec_order();

        bool nodes_in_the_same_order = Tester.check_nodes_order();
        CHECK(nodes_in_the_same_order);

        Decoder.build_tree();

        bool check_trees = Tester.check_full_trees();
        CHECK(check_trees);

        Decoder.decode();

        CHECK(str == out2.str());
    }
}


TEST_CASE("HuffmanArchiver and HuffmanDecoder tests.(2)") {
    std::string str("");

    std::istringstream in(str);
    std::ostringstream out;

    Huffman::HuffmanEncoder Encoder((std::istream*)&in, (std::ostream*)&out);
    Encoder.count_chars();
    Encoder.restart_input();
    Encoder.dict_to_tree();

    Encoder.buid_tree();
    REQUIRE(Encoder.get_tree_size() == 0);

    Encoder.build_codes();
    Encoder.encode();

    std::string result = out.str();
    std::istringstream in2(result);
    std::ostringstream out2;

    Huffman::HuffmanDecoder Decoder((std::istream*)&in2, (std::ostream*)&out2);

    Decoder.get_key_make_tree();
    Decoder.build_tree();
    Decoder.decode();

    CHECK(str == out2.str());
}