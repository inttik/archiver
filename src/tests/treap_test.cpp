#include <catch.hpp>

#include "treap.h"

namespace {
using Iptr = Treap<uint64_t>::TreapPtr;
using Itreap = Treap<uint64_t>;

bool IsSame(const Iptr& root, const std::vector<uint64_t>& test) {
    std::vector<uint64_t> treap_vector;
    treap_vector.reserve(test.size());
    Treap<uint64_t>::ToVector(root, treap_vector);
    return treap_vector == test;
}
}  // namespace

TEST_CASE("Inserting") {
    {
        Iptr root(nullptr);

        REQUIRE(IsSame(root, {}));
    }
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), 20);

        REQUIRE(IsSame(root, {20}));
    }
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), 20);
        root = Itreap::Insert(std::move(root), 10);
        root = Itreap::Insert(std::move(root), 30);

        std::vector<uint64_t> wait{10, 20, 30};
        REQUIRE(IsSame(root, {10, 20, 30}));
    }
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), {20, 1});
        root = Itreap::Insert(std::move(root), {10, 1});
        root = Itreap::Insert(std::move(root), {30, 1});

        REQUIRE(IsSame(root, {10, 20, 30}));
    }
    {
        Iptr root(nullptr);

        for (int64_t i = 0; i < 10; ++i) {
            root = Itreap::Insert(std::move(root), 10 - i);
        }

        REQUIRE(IsSame(root, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }
    {
        Iptr root(nullptr);

        for (int64_t i = 0; i < 10; ++i) {
            root = Itreap::Insert(std::move(root), {10 - i, 1});
        }

        REQUIRE(IsSame(root, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }
}

TEST_CASE("Extracting") {
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), 20);
        root = Itreap::Insert(std::move(root), 10);
        root = Itreap::Insert(std::move(root), 30);

        root = Itreap::Extract(std::move(root), 10);

        REQUIRE(IsSame(root, {20, 30}));
    }
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), 20);
        root = Itreap::Insert(std::move(root), 10);
        root = Itreap::Insert(std::move(root), 30);

        root = Itreap::Extract(std::move(root), 30);

        REQUIRE(IsSame(root, {10, 20}));
    }
    {
        Iptr root(nullptr);

        root = Itreap::Insert(std::move(root), 20);
        root = Itreap::Insert(std::move(root), 10);
        root = Itreap::Insert(std::move(root), 30);

        root = Itreap::Extract(std::move(root), 30);
        root = Itreap::Extract(std::move(root), 10);
        root = Itreap::Extract(std::move(root), 20);

        REQUIRE(IsSame(root, {}));
    }
    {
        Iptr root(nullptr);

        for (int64_t i = 0; i < 10; ++i) {
            root = Itreap::Insert(std::move(root), 10 - i);
        }

        root = Itreap::Extract(std::move(root), 1);
        root = Itreap::Extract(std::move(root), 10);
        root = Itreap::Extract(std::move(root), 2);
        root = Itreap::Extract(std::move(root), 9);
        root = Itreap::Extract(std::move(root), 3);

        REQUIRE(IsSame(root, {4, 5, 6, 7, 8}));
    }
    {
        Iptr root(nullptr);

        for (int64_t i = 0; i < 10; ++i) {
            root = Itreap::Insert(std::move(root), {10 - i, 1});
        }

        root = Itreap::Extract(std::move(root), 1);
        root = Itreap::Extract(std::move(root), 10);
        root = Itreap::Extract(std::move(root), 2);
        root = Itreap::Extract(std::move(root), 9);
        root = Itreap::Extract(std::move(root), 6);

        REQUIRE(IsSame(root, {3, 4, 5, 7, 8}));
    }
}

TEST_CASE("Extracting by index") {
    {
        Iptr root(nullptr);
        root = Itreap::Insert(std::move(root), 2);
        root = Itreap::Insert(std::move(root), 5);
        root = Itreap::Insert(std::move(root), 11);
        root = Itreap::Insert(std::move(root), 7);
        root = Itreap::Insert(std::move(root), 3);

        REQUIRE(IsSame(root, {2, 3, 5, 7, 11}));

        auto ans = Itreap::ExtractByIndex(std::move(root), 2);
        root = std::move(ans.first);
        auto elem = std::move(ans.second);

        REQUIRE(IsSame(root, {2, 3, 7, 11}));
        REQUIRE(Itreap::GetValue(elem) == 5);

        ans = Itreap::ExtractByIndex(std::move(root), 0);
        root = std::move(ans.first);
        elem = std::move(ans.second);

        REQUIRE(IsSame(root, {3, 7, 11}));
        REQUIRE(Itreap::GetValue(elem) == 2);

        ans = Itreap::ExtractByIndex(std::move(root), 2);
        root = std::move(ans.first);
        elem = std::move(ans.second);

        REQUIRE(IsSame(root, {3, 7}));
        REQUIRE(Itreap::GetValue(elem) == 11);

        ans = Itreap::ExtractByIndex(std::move(root), 1);
        root = std::move(ans.first);
        elem = std::move(ans.second);

        REQUIRE(IsSame(root, {3}));
        REQUIRE(Itreap::GetValue(elem) == 7);

        ans = Itreap::ExtractByIndex(std::move(root), 0);
        root = std::move(ans.first);
        elem = std::move(ans.second);

        REQUIRE(IsSame(root, {}));
        REQUIRE(Itreap::GetValue(elem) == 3);
    }
}

TEST_CASE("useless commands cause no error") {
    Iptr root(nullptr);
    root = Itreap::Extract(std::move(root), 0);

    REQUIRE(IsSame(root, {}));

    root = Itreap::Insert(std::move(root), 2);
    root = Itreap::Insert(std::move(root), 3);
    root = Itreap::Insert(std::move(root), 5);
    root = Itreap::Insert(std::move(root), 6);

    root = Itreap::Insert(std::move(root), 2);

    REQUIRE(IsSame(root, {2, 3, 5, 6}));

    root = Itreap::Extract(std::move(root), 4);

    REQUIRE(IsSame(root, {2, 3, 5, 6}));

    root = Itreap::ExtractByIndex(std::move(root), 4).first;

    REQUIRE(IsSame(root, {2, 3, 5, 6}));
}
