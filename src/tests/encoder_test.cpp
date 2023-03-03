#include <catch.hpp>
#include <fstream>
#include <sstream>

#include "encoder.h"

void IsSame(std::istream& first, std::istream& second) {
    char chl = 0;
    char chr = 0;

    while (first >> chl) {
        if (!(second >> chr)) {
            FAIL("Require same sizes");
        }
        REQUIRE(chl == chr);
    }
    if (second >> chr) {
        FAIL("Require same sizes");
    }
}

TEST_CASE("a") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/a/a", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/a.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "a", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
TEST_CASE("empty") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/empty/empty", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/empty.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "empty", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
TEST_CASE("forest") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/forest/forest.mp4", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/forest.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "forest.mp4", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
TEST_CASE("master") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/master/master_i_margarita.txt", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/master.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "master_i_margarita.txt", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
TEST_CASE("mountains") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/mountains/mountains.jpg", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/mountains.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "mountains.jpg", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
TEST_CASE("multiple_files") {
    std::stringstream output;
    std::ifstream in1("../../src/tests/data/multiple_files/forest.mp4", std::ios_base::binary);
    std::ifstream in2("../../src/tests/data/multiple_files/master_i_margarita.txt", std::ios_base::binary);
    std::ifstream in3("../../src/tests/data/multiple_files/mountains.jpg", std::ios_base::binary);
    std::ifstream in4("../../src/tests/data/multiple_files/shabanov.pdf", std::ios_base::binary);

    std::ifstream correct("../../src/tests/data/multiple_files.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in1.is_open());
    REQUIRE(in2.is_open());
    REQUIRE(in3.is_open());
    REQUIRE(in4.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "forest.mp4", .input = BitReader(in1)}, false);
    encoder.EncodeFile({.name = "master_i_margarita.txt", .input = BitReader(in2)}, false);
    encoder.EncodeFile({.name = "mountains.jpg", .input = BitReader(in3)}, false);
    encoder.EncodeFile({.name = "shabanov.pdf", .input = BitReader(in4)}, true);

    IsSame(output, correct);

    in1.close();
    in2.close();
    in3.close();
    in4.close();
    correct.close();
}
TEST_CASE("new_lines") {
    std::stringstream output;
    std::ifstream in("../../src/tests/data/new_lines/new_lines", std::ios_base::binary);
    std::ifstream correct("../../src/tests/data/new_lines.arc", std::ios_base::binary);
    Encoder encoder({.output = BitWriter(output)});

    REQUIRE(in.is_open());
    REQUIRE(correct.is_open());

    encoder.EncodeFile({.name = "new_lines", .input = BitReader(in)}, true);

    IsSame(output, correct);

    in.close();
    correct.close();
}
