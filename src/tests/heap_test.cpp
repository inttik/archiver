#include <catch.hpp>

#include "heap.h"

#include <random>

TEST_CASE("heap insert/extract simple") {
    MinHeap<int64_t> heap;
    heap.Insert(1);
    REQUIRE(heap.Extract() == 1);
    heap.Insert(2);
    REQUIRE(heap.Extract() == 2);
    heap.Insert(3);
    heap.Insert(3);
    REQUIRE(heap.Extract() == 3);
    heap.Insert(5);
    heap.Insert(4);
    REQUIRE(heap.Extract() == 3);
    REQUIRE(heap.Extract() == 4);
    REQUIRE(heap.Extract() == 5);
}

void CheckHeapInsert(std::vector<int64_t> order) {
    std::vector<int64_t> expect = order;
    std::sort(expect.begin(), expect.end());
    {
        CAPTURE(order);
        MinHeap<int64_t> heap;
        for (const auto& e : order) {
            heap.Insert(e);
        }
        CAPTURE(heap.ToVector());
        auto data = heap.ToVector();
        std::sort(data.begin(), data.end());
        REQUIRE(data == expect);
    }
    {
        CAPTURE(order);
        std::reverse(order.begin(), order.end());
        MinHeap<int64_t> heap;
        while (!order.empty()) {
            heap.Insert(std::move(order.back()));
            order.pop_back();
        }
        CAPTURE(heap.ToVector());
        auto data = heap.ToVector();
        std::sort(data.begin(), data.end());
        REQUIRE(data == expect);
    }
}

void CheckHeapSort(std::vector<int64_t> order) {
    std::vector<int64_t> expect = order;
    std::sort(expect.begin(), expect.end());
    {
        CAPTURE(order);
        MinHeap<int64_t> heap;
        for (const auto& e : order) {
            heap.Insert(e);
        }
        CAPTURE(heap.ToVector());
        size_t i = 0;
        while (heap.Size() > 0) {
            CAPTURE(heap.ToVector());
            int64_t current = heap.Extract();
            REQUIRE(current == expect[i]);
            ++i;
        }
    }
    {
        CAPTURE(order);
        std::reverse(order.begin(), order.end());
        MinHeap<int64_t> heap;
        while (!order.empty()) {
            heap.Insert(std::move(order.back()));
            order.pop_back();
        }
        CAPTURE(heap.ToVector());
        size_t i = 0;
        while (heap.Size() > 0) {
            CAPTURE(heap.ToVector());
            int64_t current = heap.Extract();
            REQUIRE(current == expect[i]);
            ++i;
        }
    }
}

TEST_CASE("heap insert") {
    const size_t TEST = 20;
    const size_t REP_COUNT = 3;

    std::mt19937 rnd(1231);
    std::vector<int64_t> fill_order{1, 2, 3, 3, 6, 0, -1};
    size_t n = fill_order.size();

    for (size_t rep = 0; rep < REP_COUNT; ++rep) {
        for (size_t test_case = 0; test_case < TEST; ++test_case) {
            std::shuffle(fill_order.begin(), fill_order.end(), rnd);
            CheckHeapInsert(fill_order);
        }
        fill_order.reserve(n * (rep + 2));
        fill_order.insert(fill_order.end(), fill_order.begin(), fill_order.begin() + n);
    }
}

TEST_CASE("heap sort") {
    const size_t TEST = 20;
    const size_t REP_COUNT = 3;

    std::mt19937 rnd(1231);
    std::vector<int64_t> fill_order{1, 2, 3, 3, 6, 0, -1};
    size_t n = fill_order.size();

    for (size_t rep = 0; rep < REP_COUNT; ++rep) {
        for (size_t test_case = 0; test_case < TEST; ++test_case) {
            std::shuffle(fill_order.begin(), fill_order.end(), rnd);
            CheckHeapSort(fill_order);
        }
        fill_order.reserve(n * (rep + 2));
        fill_order.insert(fill_order.end(), fill_order.begin(), fill_order.begin() + n);
    }
}
