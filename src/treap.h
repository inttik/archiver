#pragma once

#include <cassert>
#include <exception>
#include <memory>
#include <ostream>
#include <random>
#include <vector>

template <typename KeyType>
class Treap {
public:
    using PriorityType = uint64_t;
    using TreapPtr = std::unique_ptr<Treap<KeyType>>;

    explicit Treap(const KeyType& value)
        : rnd_(seed_), value_(value), priority_(rnd_()), size_(1), left_(nullptr), right_(nullptr) {
    }
    explicit Treap(KeyType&& value)
        : rnd_(seed_), value_(std::move(value)), priority_(rnd_()), size_(1), left_(nullptr), right_(nullptr) {
    }
    Treap(const Treap<KeyType>& other) = delete;
    Treap(Treap<KeyType>&& other) = default;

    Treap<KeyType>& operator=(const Treap<KeyType>& other) & = delete;
    Treap<KeyType>& operator=(Treap<KeyType>&& other) & = default;

    ~Treap() = default;

    static size_t GetSize(const TreapPtr& root) {
        if (root == nullptr) {
            return 0;
        }
        return root->size_;
    }
    static KeyType GetValue(const TreapPtr& root) {
        if (root == nullptr) {
            return KeyType();
        }
        return root->value_;
    }
    static KeyType StealValue(const TreapPtr& root) {
        if (root == nullptr) {
            return KeyType();
        }
        return std::move(root->value_);
    }

    static TreapPtr Merge(TreapPtr&& left, TreapPtr&& right) {
        if (left == nullptr) {
            return std::move(right);
        }
        if (right == nullptr) {
            return std::move(left);
        }

        if (left->priority_ > right->priority_) {
            left->right_ = Merge(std::move(left->right_), std::move(right));
            left->Recalculate();
            return std::move(left);
        } else {
            right->left_ = Merge(std::move(left), std::move(right->left_));
            right->Recalculate();
            return std::move(right);
        }
    }
    //                <= key     > key
    static std::pair<TreapPtr, TreapPtr> Split(TreapPtr&& root, const KeyType& target) {
        if (root == nullptr) {
            return std::pair<TreapPtr, TreapPtr>(nullptr, nullptr);
        }

        if (root->value_ > target) {
            auto ans = Split(std::move(root->left_), target);
            root->left_ = std::move(ans.second);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(ans.first), std::move(root));
        } else {
            auto ans = Split(std::move(root->right_), target);
            root->right_ = std::move(ans.first);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(root), std::move(ans.second));
        }
    }
    static std::pair<TreapPtr, TreapPtr> SplitByIndex(TreapPtr&& root, size_t index) {
        if (root == nullptr) {
            return std::pair<TreapPtr, TreapPtr>(nullptr, nullptr);
        }

        if (GetSize(root->left_) > index) {
            auto ans = SplitByIndex(std::move(root->left_), index);
            root->left_ = std::move(ans.second);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(ans.first), std::move(root));
        }
        index -= GetSize(root->left_);

        if (index == 0) {
            auto right = std::move(root->right_);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(root), std::move(right));
        }
        index -= 1;

        auto ans = SplitByIndex(std::move(root->right_), index);
        root->right_ = std::move(ans.first);
        root->Recalculate();
        return std::pair<TreapPtr, TreapPtr>(std::move(root), std::move(ans.second));
    }

    //                < key     >= key
    static std::pair<TreapPtr, TreapPtr> RSplit(TreapPtr&& root, const KeyType& target) {
        if (root == nullptr) {
            return std::pair<TreapPtr, TreapPtr>(nullptr, nullptr);
        }

        if (root->value_ >= target) {
            auto ans = RSplit(std::move(root->left_), target);
            root->left_ = std::move(ans.second);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(ans.first), std::move(root));
        } else {
            auto ans = RSplit(std::move(root->right_), target);
            root->right_ = std::move(ans.first);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(root), std::move(ans.second));
        }
    }
    static std::pair<TreapPtr, TreapPtr> RSplitByIndex(TreapPtr&& root, size_t index) {
        if (root == nullptr) {
            return std::pair<TreapPtr, TreapPtr>(nullptr, nullptr);
        }

        if (GetSize(root->left_) > index) {
            auto ans = RSplitByIndex(std::move(root->left_), index);
            root->left_ = std::move(ans.second);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(ans.first), std::move(root));
        }
        index -= GetSize(root->left_);

        if (index == 0) {
            auto left = std::move(root->left_);
            root->Recalculate();
            return std::pair<TreapPtr, TreapPtr>(std::move(left), std::move(root));
        }
        index -= 1;

        auto ans = RSplitByIndex(std::move(root->right_), index);
        root->right_ = std::move(ans.first);
        root->Recalculate();
        return std::pair<TreapPtr, TreapPtr>(std::move(root), std::move(ans.second));
    }

    static TreapPtr InsertTreapPtr(TreapPtr&& root, TreapPtr&& target) {
        if (root == nullptr) {
            return std::move(target);
        }

        auto [leq, greater] = Split(std::move(root), target->value_);
        auto [less, equal] = RSplit(std::move(leq), target->value_);

        equal = std::move(target);

        leq = Merge(std::move(less), std::move(equal));
        return Merge(std::move(leq), std::move(greater));
    }

    static TreapPtr Insert(TreapPtr&& root, const KeyType& value) {
        TreapPtr target(new Treap(value));
        return InsertTreapPtr(std::move(root), std::move(target));
    }
    static TreapPtr Insert(TreapPtr&& root, KeyType&& value) {
        TreapPtr target(new Treap(std::move(value)));
        return InsertTreapPtr(std::move(root), std::move(target));
    }
    static TreapPtr Insert(TreapPtr&& root, const std::pair<KeyType, PriorityType>& vertex) {
        TreapPtr target(new Treap(vertex.first));
        target->priority_ = vertex.second;
        return InsertTreapPtr(std::move(root), std::move(target));
    }
    static TreapPtr Extract(TreapPtr&& root, const KeyType& target) {
        if (root == nullptr) {
            return nullptr;
        }

        auto [leq, greater] = Split(std::move(root), target);
        auto [less, equal] = RSplit(std::move(leq), target);

        equal.reset(nullptr);

        leq = Merge(std::move(less), std::move(equal));
        return Merge(std::move(leq), std::move(greater));
    }
    //               root      element
    static std::pair<TreapPtr, TreapPtr> ExtractByIndex(TreapPtr&& root, size_t index) {
        if (GetSize(root) <= index) {
            return std::pair<TreapPtr, TreapPtr>{std::move(root), nullptr};
        }

        auto [leq, greater] = SplitByIndex(std::move(root), index);
        if (index == 0) {
            return std::pair<TreapPtr, TreapPtr>(std::move(greater), std::move(leq));
        }
        auto [less, equal] = RSplitByIndex(std::move(leq), index);
        assert(equal != nullptr);

        auto new_root = Merge(std::move(less), std::move(greater));
        return std::pair<TreapPtr, TreapPtr>(std::move(new_root), std::move(equal));
    }

    static void Print(const TreapPtr& root, std::ostream& out) {
        if (root == nullptr) {
            return;
        }
        Print(root->left_, out);
        if (root->left_ != nullptr) {
            out << " ";
        }
        out << root->value_ << "(" << root->size_ << ")";
        if (root->right_ != nullptr) {
            out << " ";
        }
        Print(root->right_, out);
    }
    static void ToVector(const TreapPtr& root, std::vector<KeyType>& answer) {
        if (root == nullptr) {
            return;
        }
        ToVector(root->left_, answer);
        answer.push_back(root->value_);
        ToVector(root->right_, answer);
    }

private:
    void Recalculate() {
        size_ = GetSize(left_) + 1 + GetSize(right_);
    }

    const size_t seed_ = 21436576;
    std::mt19937_64 rnd_;

    KeyType value_;
    PriorityType priority_;

    size_t size_;

    TreapPtr left_;
    TreapPtr right_;
};
