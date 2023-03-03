#include <catch.hpp>

#include "trie.h"

TEST_CASE("trie") {
    {
        Trie<int> a(1);
        std::vector<std::pair<int, std::vector<bool>>> answer;

        a.GenerateCodes(answer);

        std::vector<std::pair<int, std::vector<bool>>> wait{
            {1, {}},
        };

        REQUIRE(answer == wait);
    }
    {
        Trie<int> a(1);
        Trie<int> b(2);
        Trie<int> c(std::move(a), std::move(b));

        std::vector<std::pair<int, std::vector<bool>>> answer;

        c.GenerateCodes(answer);

        std::vector<std::pair<int, std::vector<bool>>> wait{
            {1, {false}},
            {2, {true}},
        };

        REQUIRE(answer == wait);
    }
    {
        Trie<int> a(1);
        Trie<int> b(2);
        Trie<int> c(std::move(a), std::move(b));

        Trie<int> d(3);
        Trie<int> e(std::move(d), std::move(c));

        Trie<int> f(4);
        Trie<int> g(5);
        Trie<int> h(std::move(g), std::move(f));

        Trie<int> i(std::move(e), std::move(h));

        std::vector<std::pair<int, std::vector<bool>>> answer;

        i.GenerateCodes(answer);

        std::vector<std::pair<int, std::vector<bool>>> wait{
            {3, {false, false}}, {1, {false, true, false}}, {2, {false, true, true}},
            {5, {true, false}},  {4, {true, true}},
        };

        REQUIRE(answer == wait);
    }
}
