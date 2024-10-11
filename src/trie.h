#pragma once

#include <memory>
#include <vector>
#include <cassert>

template <typename Key>
class RawTrie {
public:
    using TriePtr = std::shared_ptr<RawTrie>;

    RawTrie() : value_(), left_(nullptr), right_(nullptr) {
    }
    explicit RawTrie(const Key& value) : value_(value), left_(nullptr), right_(nullptr) {
    }
    explicit RawTrie(Key&& value) : value_(value), left_(nullptr), right_(nullptr) {
    }
    RawTrie(TriePtr&& left, TriePtr&& right) : left_(std::move(left)), right_(std::move(right)) {
        assert(left_ != nullptr || right_ != nullptr);
        if (left_ == nullptr) {
            value_ = right_->value_;
        } else if (right_ == nullptr) {
            value_ = left_->value_;
        } else {
            value_ = std::min(right_->value_, left_->value_);
        }
    }

    TriePtr GetLeft() {
        return left_;
    }
    TriePtr GetRight() {
        return right_;
    }
    Key GetValue() const {
        return value_;
    }

    void SetLeft(TriePtr&& left) {
        left_ = left;
    }
    void SetRight(TriePtr&& right) {
        right_ = right;
    }
    void SetValue(const Key& value) {
        value_ = value;
    }

    bool IsTerminal() {
        return left_ == nullptr && right_ == nullptr;
    }

    static Key GetValue(const TriePtr& trie) {
        if (trie == nullptr) {
            return Key();
        }
        return trie->value_;
    }
    static void GenerateCodes(const TriePtr& trie, std::vector<bool>& current_code,
                              std::vector<std::pair<Key, std::vector<bool>>>& answer) {
        if (trie == nullptr) {
            return;
        }
        if (trie->IsTerminal()) {
            answer.push_back({trie->value_, current_code});
        } else {
            auto copy = current_code;
            current_code.push_back(false);
            copy.push_back(true);
            GenerateCodes(trie->left_, current_code, answer);
            GenerateCodes(trie->right_, copy, answer);
        }
    }

private:
    Key value_;

    TriePtr left_;
    TriePtr right_;
};

template <typename Key>
std::vector<std::pair<Key, std::vector<bool>>> ToCodes(const RawTrie<Key>& trie) {
    std::vector<std::pair<Key, std::vector<bool>>> codes;
    std::vector<bool> current_code;
    RawTrie<Key>::GenerateCodes(trie, current_code, codes);
    return codes;
}

template <typename Key> 
class Trie {
public:
    using TriePtr = RawTrie<Key>::TriePtr;

    Trie() : current_(new RawTrie<Key>) {
    }
    explicit Trie(const Key& value) : current_(new RawTrie(value)) {
    }
    explicit Trie(Key&& value) : current_(new RawTrie(std::move(value))) {
    }
    Trie(Trie&& left, Trie&& right) : current_(new RawTrie(std::move(left.current_), std::move(right.current_))) {
    }

    auto operator<=>(const Trie<Key>& other) const {
        return RawTrie<Key>::GetValue(current_) <=> RawTrie<Key>::GetValue(other.current_);
    }

    Key GetValue() {
        return RawTrie<Key>::GetValue(current_);
    }
    void GenerateCodes(std::vector<std::pair<Key, std::vector<bool>>>& answer) {
        std::vector<bool> current_code;
        RawTrie<Key>::GenerateCodes(current_, current_code, answer);
    }

    TriePtr GetRaw() & {
        return current_;
    }
    void SetRaw(TriePtr&& current) {
        current_.reset(current.release());
    }

private:
    TriePtr current_;
};
