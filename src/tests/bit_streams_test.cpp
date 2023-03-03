#include <catch.hpp>
#include <sstream>

#include "bit_reader.h"
#include "bit_writer.h"

std::string ToBin(BitReader& bit_reader, std::vector<size_t> size_order, size_t wait_correct_reads = 0) {
    std::string answer;
    size_t min_size = *std::min_element(size_order.begin(), size_order.end());

    size_t correct_reads = 0;
    bool is_correct_read = true;
    size_t i = 0;
    while (true) {
        if (i >= size_order.size()) {
            i -= size_order.size();
        }
        auto [current_value, result] = bit_reader.ReadSome(size_order[i]);
        if (!result) {
            if (size_order[i] == 1) {
                break;
            }
            if (size_order[i] == min_size) {
                is_correct_read = false;
                size_order = std::vector<size_t>(1, 1);
                i = 0;
                continue;
            }
            ++i;
            continue;
        }
        if (is_correct_read) {
            ++correct_reads;
        }
        std::string current;
        for (size_t j = 0; j < size_order[i]; ++j) {
            current += std::to_string(current_value % 2);
            current_value /= 2;
        }
        std::reverse(current.begin(), current.end());
        answer += current;

        ++i;
    }

    REQUIRE(correct_reads >= wait_correct_reads);

    return answer;
}
void ToText(BitWriter& bit_writer, BitReader& bit_reader, std::vector<size_t> size_order,
            size_t wait_correct_reads = 0) {
    size_t min_size = *std::min_element(size_order.begin(), size_order.end());

    size_t correct_reads = 0;
    bool is_correct_read = true;
    size_t i = 0;
    while (true) {
        if (i >= size_order.size()) {
            i -= size_order.size();
        }
        auto [current_value, result] = bit_reader.ReadSome(size_order[i]);
        if (!result) {
            if (size_order[i] == 1) {
                break;
            }
            if (size_order[i] == min_size) {
                is_correct_read = false;
                size_order = std::vector<size_t>(1, 1);
                i = 0;
                continue;
            }
            ++i;
            continue;
        }
        if (is_correct_read) {
            ++correct_reads;
        }
        bit_writer.WriteSome(current_value, size_order[i]);
    }

    REQUIRE(correct_reads >= wait_correct_reads);

    bit_writer.Flush();
}

TEST_CASE("Bit reader") {
    {
        char input_chars[] = {
            static_cast<char>(0b01110101),
            static_cast<char>(0b10101101),
            static_cast<char>(0b01101000),
            static_cast<char>(0b10001101),
        };

        std::string answer =
            "01110101"
            "10101101"
            "01101000"
            "10001101";

        for (size_t sz = 1; sz <= 32; ++sz) {
            std::string input_string(input_chars);
            std::istringstream input(input_string);
            BitReader bit_reader(input);
            std::string current = ToBin(bit_reader, {sz}, answer.size() / sz);

            REQUIRE(answer == current);
        }
    }
    {
        std::string text = "Я текст!";
        std::string right_answer =
            "110100001010111100100000110100011000001011010000101101011101000010111010110100011000000111010001100000"
            "1000100001";

        std::istringstream input(text);
        BitReader bit_reader(input);
        std::string answer = ToBin(bit_reader, {1}, right_answer.size());

        REQUIRE(answer == right_answer);
    }
}

TEST_CASE("Bit writer") {
    {
        char text[] = {
            static_cast<char>(0b10101110),
            static_cast<char>(0b10101110),
        };

        std::string input_string(text);
        std::istringstream input(input_string);
        BitReader bit_reader(input);

        std::ostringstream output;
        BitWriter bit_writer(output);

        ToText(bit_writer, bit_reader, {3, 3, 3, 7}, 4);

        REQUIRE(input_string == output.str());
    }
    {
        std::string text =
            "1101000010101111001000001101000110000010110100001011010111010000101110101101000110000001110100011000001000"
            "100001";
        std::string right_answer = "Я текст!";

        std::ostringstream output;
        BitWriter bit_writer(output);

        for (size_t i = 0; i < text.size(); ++i) {
            bit_writer.WriteSome(text[i] - '0', 1);
        }
        bit_writer.Flush();

        REQUIRE(output.str() == right_answer);
    }
    {
        std::string text =
            "Это действительно достаточно большой текст, который должен покрыть все частные случаи. For example some "
            "words of it are in english. 12340#(^*(!@$#59!*)";

        std::istringstream input(text);
        BitReader bit_reader(input);

        std::ostringstream output;
        BitWriter bit_writer(output);

        ToText(bit_writer, bit_reader, {3, 1, 8, 9, 7, 20, 32});

        REQUIRE(output.str() == text);
    }
}
