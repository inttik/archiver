#pragma once

#include <algorithm>
#include <vector>

template <typename KeyType>
class MinHeap {
public:
    MinHeap() : data_(0) {
    }
    MinHeap(MinHeap const &other) = default;
    MinHeap& operator=(const MinHeap& other) = default;

    void Insert(KeyType element) {
        data_.push_back(std::move(element));
        fix_up(data_.size() - 1);
    }

    KeyType Extract() {
        KeyType ans = std::move(data_[0]);
        if (data_.size() == 1) {
            data_.pop_back();
            return ans;
        }
        std::swap(data_[0], data_.back());
        data_.pop_back();
        fix_down(0);
        return ans;
    }

    size_t Size() const {
        return data_.size();
    }

    std::vector<KeyType> ToVector() const {
        return data_;
    }

private:
    void fix_up(size_t i) {
        if (i == 0) {
            return;
        }
        size_t pred = (i - 1) / 2;
        if (data_[pred] <= data_[i]) {
            return;
        }
        std::swap(data_[pred], data_[i]);
        fix_up(pred);
    }
    void fix_down(size_t i) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        if (left >= data_.size()) {
            return;
        }
        size_t min_index = left;
        if (right < data_.size() && data_[right] < data_[left]) {
            min_index = right;
        }
        if (data_[i] <= data_[min_index]) {
            return;
        }
        std::swap(data_[i], data_[min_index]);
        fix_down(min_index);
    }

    std::vector<KeyType> data_;
};
