#pragma once

#include <iostream>

namespace AVL {

template <typename key_t>
class node_t
{
    using iterator = node_t*;

    key_t key_;

    node_t* parent_;
    node_t* left_;
    node_t* right_;

    int height_ = 1;

    public:

    node_t(key_t key, node_t* parent = nullptr, node_t* left = nullptr, node_t* right = nullptr) : key_(key), parent_(parent), left_(left), right_(right) {}

    node_t(const node_t &node) = delete;
    node_t& operator= (const node_t &node) = delete;


    //modificators
    iterator insert(key_t key)
    {
        if (key == key_) return this;

        if (key < key_)
        {
            if (!left_) left_ = new node_t{key};
            left_ = left_->insert(key);
        }

        if (key > key_)
        {
            if (!right_) right_ = new node_t{key};
            right_ = right_->insert(key);
        }

        return balanced();
    }

        int hl = 0;
        int hr = 0;

    int height() const
    {
        if (this == nullptr) return 0;
        return height_;
    }


    int bfactor() const
    {
        int hl = 0;
        int hr = 0;
        if (left_  != nullptr) hl = left_->height();
        if (right_ != nullptr) hr = right_->height();

        return hr - hl;
    }


    void fixheight()
    {
        int hl = 0;
        int hr = 0;
        if (left_  != nullptr) hl = left_->height();
        if (right_ != nullptr) hr = right_->height();

        height_ = ((hl > hr) ? hl : hr) + 1;
    }


    iterator rotateright()
    {
        iterator new_polus = left_;
        left_ = new_polus->right_;
        new_polus->right_ = this;

        fixheight();
        new_polus->fixheight();

        return new_polus;
    }


    iterator rotateleft()
    {
        iterator new_polus = right_;
        right_ = new_polus->left_;
        new_polus->left_ = this;

        fixheight();
        new_polus->fixheight();

        return new_polus;
    }


    iterator balanced()
    {
        fixheight();

        if (bfactor() == 2)
        {
            if (!right_) return rotateleft();
            if (right_->bfactor() < 0)
                right_ = right_->rotateright();

            return rotateleft();
        }

        if (bfactor() == -2)
        {
            if (!left_) return rotateright();
            if (left_->bfactor() > 0) 
                left_ = left_->rotateleft();

            return rotateright();
        }

        return this;
    }

    iterator get_left()  const { return left_; }
    iterator get_right() const { return right_; }
    key_t get_key() const { return key_; }
};

template <typename key_t>
class tree_t
{
    using iterator = node_t<key_t>*;

    iterator root_ = nullptr;


    public:

    iterator lower_bound(key_t key) const;

    iterator upper_bound(key_t key) const;

    int distance(iterator fst, iterator snd);


    //modificators
    void insert(key_t key)
    {
        if (!root_)
        {
            root_ = new node_t{key};
            subtree_dump(root_);
            return;
        }
        root_ = root_->insert(key);
        subtree_dump(root_);
    }
};

}