#pragma once

#include "treap.h"

template <typename KeyType>
class PriorityQueue {
public:
    using TreapPtr = typename Treap<KeyType>::TreapPtr;

    PriorityQueue() : queue_(nullptr) {
    }
    PriorityQueue(const PriorityQueue<KeyType>& other) : queue_(nullptr) {
        *this = other;
    };
    PriorityQueue(PriorityQueue<KeyType>&& other) = default;

    PriorityQueue<KeyType>& operator=(const PriorityQueue<KeyType>& other) & {
        if (this == &other) {
            return *this;
        }
        queue_.release();
        for (const auto& element : other.ToVector()) {
            Insert(element);
        }
        return *this;
    }
    PriorityQueue<KeyType>& operator=(PriorityQueue<KeyType>&& other) & = default;

    ~PriorityQueue() {
        queue_.release();
    }

    void Insert(const KeyType& element) {
        queue_ = Treap<KeyType>::Insert(std::move(queue_), std::move(element));
    }
    void Insert(KeyType&& element) {
        queue_ = Treap<KeyType>::Insert(std::move(queue_), std::move(element));
    }

    void Extract(const KeyType& element) {
        queue_ = Treap<KeyType>::Extract(std::move(queue_), element);
    }
    KeyType ExtractByIndex(size_t index) {
        auto ans = Treap<KeyType>::ExtractByIndex(std::move(queue_), index);
        queue_ = std::move(ans.first);
        return Treap<KeyType>::StealValue(std::move(ans.second));
    }
    std::pair<KeyType, KeyType> ExtractFirstTwo() {
        if (Size() < 2) {
            return {KeyType(), KeyType()};
        }

        auto [left, right] = Treap<KeyType>::SplitByIndex(std::move(queue_), 1);
        queue_ = std::move(right);
        auto [first, second] = Treap<KeyType>::SplitByIndex(std::move(left), 0);
        assert(first != nullptr);
        assert(second != nullptr);

        return {std::move(Treap<KeyType>::StealValue(first)), std::move(Treap<KeyType>::StealValue(second))};
    };

    size_t Size() const {
        return Treap<KeyType>::GetSize(queue_);
    }
    std::vector<KeyType> ToVector() const {
        std::vector<KeyType> elements;
        elements.reserve(Size());
        Treap<KeyType>::ToVector(queue_, elements);

        return elements;
    }

private:
    TreapPtr queue_;
};
