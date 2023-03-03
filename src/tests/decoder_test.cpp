#include <catch.hpp>
#include <fstream>

#include "decoder.h"

void IsSame(const std::string& file_name, const std::string& directory_name) {
    std::string answer_path = "../../src/tests/unzipped/" + file_name;
    std::string correct_path = "../../src/tests/data/" + directory_name + "/" + file_name;

    std::ifstream first(answer_path, std::ios_base::binary);
    std::ifstream second(correct_path, std::ios_base::binary);

    REQUIRE(first.is_open());
    REQUIRE(second.is_open());

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

    first.close();
    second.close();
}

TEST_CASE("a") {
    std::string arc_name = "a";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("a", arc_name);
}
TEST_CASE("empty") {
    std::string arc_name = "empty";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("empty", arc_name);
}
TEST_CASE("forest") {
    std::string arc_name = "forest";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("forest.mp4", arc_name);
}
TEST_CASE("master") {
    std::string arc_name = "master";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("master_i_margarita.txt", arc_name);
}
TEST_CASE("mountains") {
    std::string arc_name = "mountains";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("mountains.jpg", arc_name);
}
TEST_CASE("multiple files") {
    std::string arc_name = "multiple_files";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("forest.mp4", arc_name);
    IsSame("master_i_margarita.txt", arc_name);
    IsSame("mountains.jpg", arc_name);
    IsSame("shabanov.pdf", arc_name);
}
TEST_CASE("new lines") {
    std::string arc_name = "new_lines";
    std::ifstream in("../../src/tests/data/" + arc_name + ".arc", std::ios_base::binary);

    REQUIRE(in.is_open());

    Decoder decoder(BitReader(in), "../../src/tests/unzipped/");
    decoder.Decode();
    in.close();

    IsSame("new_lines", arc_name);
}
