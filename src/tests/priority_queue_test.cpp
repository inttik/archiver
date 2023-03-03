#include <catch.hpp>

#include "priority_queue.h"

TEST_CASE("Inserting") {
    {
        PriorityQueue<int64_t> queue;

        REQUIRE(queue.ToVector().empty());

        queue.Insert(10);

        REQUIRE(queue.Size() == 1);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{10});

        queue.Insert(20);

        REQUIRE(queue.Size() == 2);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 20});

        queue.Insert(15);

        REQUIRE(queue.Size() == 3);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 15, 20});

        queue.Insert(5);

        REQUIRE(queue.Size() == 4);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{5, 10, 15, 20});
    }

    {
        PriorityQueue<int64_t> queue;

        queue.Insert(1);
        queue.Insert(3);
        queue.Insert(7);
        queue.Insert(10);
        queue.Insert(2);
        queue.Insert(9);
        queue.Insert(8);
        queue.Insert(5);
        queue.Insert(4);
        queue.Insert(6);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
}

TEST_CASE("Extracting") {
    {
        PriorityQueue<int64_t> queue;

        queue.Insert(10);
        queue.Insert(20);
        queue.Insert(30);

        queue.Extract(20);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 30});

        queue.Insert(20);
        queue.Extract(10);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{20, 30});

        queue.Insert(10);
        queue.Extract(30);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 20});

        queue.Extract(20);
        queue.Extract(10);
        REQUIRE(queue.ToVector().empty());
    }
    {
        PriorityQueue<int64_t> queue;

        queue.Insert(1);
        queue.Insert(3);
        queue.Insert(7);
        queue.Insert(10);
        queue.Insert(2);
        queue.Insert(9);
        queue.Insert(8);
        queue.Insert(5);
        queue.Insert(4);
        queue.Insert(6);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        queue.Extract(3);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5, 6, 7, 8, 9, 10});
        queue.Extract(9);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5, 6, 7, 8, 10});
        queue.Extract(1);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{2, 4, 5, 6, 7, 8, 10});
        queue.Extract(2);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{4, 5, 6, 7, 8, 10});
        queue.Extract(6);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{4, 5, 7, 8, 10});
    }
}

TEST_CASE("Extracting by index") {
    {
        PriorityQueue<int64_t> queue;

        queue.Insert(10);
        queue.Insert(20);
        queue.Insert(30);

        auto elem = queue.ExtractByIndex(1);
        REQUIRE(elem == 20);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 30});

        queue.Insert(20);
        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 10);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{20, 30});

        queue.Insert(10);
        elem = queue.ExtractByIndex(2);
        REQUIRE(elem == 30);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{10, 20});

        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 10);
        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 20);

        REQUIRE(queue.ToVector().empty());
    }
    {
        PriorityQueue<int64_t> queue;

        queue.Insert(1);
        queue.Insert(3);
        queue.Insert(7);
        queue.Insert(10);
        queue.Insert(2);
        queue.Insert(9);
        queue.Insert(8);
        queue.Insert(5);
        queue.Insert(4);
        queue.Insert(6);

        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

        auto elem = queue.ExtractByIndex(2);
        REQUIRE(elem == 3);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5, 6, 7, 8, 9, 10});
        elem = queue.ExtractByIndex(7);
        REQUIRE(elem == 9);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5, 6, 7, 8, 10});
        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 1);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{2, 4, 5, 6, 7, 8, 10});
        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 2);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{4, 5, 6, 7, 8, 10});
        elem = queue.ExtractByIndex(2);
        REQUIRE(elem == 6);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{4, 5, 7, 8, 10});
        elem = queue.ExtractByIndex(0);
        REQUIRE(elem == 4);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{5, 7, 8, 10});
    }
}

TEST_CASE("First two extracting") {
    {
        PriorityQueue<int64_t> queue;

        queue.Insert(3);
        queue.Insert(5);
        queue.Insert(33);
        queue.Insert(66);
        queue.Insert(99);

        {
            auto [p1, p2] = queue.ExtractFirstTwo();
            REQUIRE(p1 == 3);
            REQUIRE(p2 == 5);
            REQUIRE(queue.ToVector() == std::vector<int64_t>{33, 66, 99});
        }

        queue.Extract(66);
        REQUIRE(queue.ToVector() == std::vector<int64_t>{33, 99});

        {
            auto [p1, p2] = queue.ExtractFirstTwo();
            REQUIRE(p1 == 33);
            REQUIRE(p2 == 99);
            REQUIRE(queue.ToVector().empty());
        }
    }
}

TEST_CASE("Copy and move") {
    {
        PriorityQueue<int64_t> queue1;

        queue1.Insert(10);
        queue1.Insert(20);
        queue1.Insert(30);

        PriorityQueue<int64_t> queue2(queue1);

        REQUIRE(queue2.Size() == 3);
        REQUIRE(queue1.ToVector() == queue2.ToVector());

        queue2.Insert(40);

        REQUIRE(queue1.ToVector() != queue2.ToVector());

        queue1.Insert(40);

        REQUIRE(queue1.ToVector() == queue2.ToVector());

        queue1.ExtractFirstTwo();

        REQUIRE(queue1.ToVector() != queue2.ToVector());

        queue2.Extract(10);
        queue2.ExtractByIndex(0);

        REQUIRE(queue1.ToVector() == queue2.ToVector());
    }
    {
        PriorityQueue<int64_t> queue1;

        queue1.Insert(10);
        queue1.Insert(20);
        queue1.Insert(30);

        PriorityQueue<int64_t> queue2;

        queue2.Insert(40);

        queue2 = queue1;

        REQUIRE(queue2.Size() == 3);
        REQUIRE(queue1.ToVector() == queue2.ToVector());

        queue2.Insert(40);

        REQUIRE(queue1.ToVector() != queue2.ToVector());
    }
    {
        PriorityQueue<int64_t> queue1;

        queue1.Insert(10);
        queue1.Insert(20);
        queue1.Insert(30);

        PriorityQueue<int64_t> queue2(std::move(queue1));

        REQUIRE(queue2.Size() == 3);
        REQUIRE(queue2.ToVector() == std::vector<int64_t>{10, 20, 30});

        queue2.Insert(1);

        REQUIRE(queue2.Size() == 4);
        REQUIRE(queue2.ToVector() == std::vector<int64_t>{1, 10, 20, 30});
    }
}

TEST_CASE("useless commands cause no error") {
    PriorityQueue<int64_t> queue;

    queue.Extract(10);

    REQUIRE(queue.ToVector().empty());

    queue.Insert(1);
    queue.Insert(2);
    queue.Insert(4);
    queue.Insert(5);

    queue.Insert(2);

    REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5});

    queue.Extract(3);

    REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5});

    queue.ExtractByIndex(4);

    REQUIRE(queue.ToVector() == std::vector<int64_t>{1, 2, 4, 5});

    queue.Insert(3);
    queue.ExtractFirstTwo();
    queue.ExtractFirstTwo();

    queue.ExtractFirstTwo();

    REQUIRE(queue.ToVector() == std::vector<int64_t>{5});
}
