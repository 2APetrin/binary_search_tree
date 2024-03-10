#pragma once

#include <functional>
#include <vector>
#include <memory>

namespace AVL {

int graphviz_png_count;

const int right_rotate_factor = 2;
const int left_rotate_factor = -2;
const int zero_rotate_factor =  0;

namespace detail {

class avl_mem_manager { // Rule of zero

struct node_t {
    using iterator = node_t*;

    key_t key_;
    iterator parent_, left_, right_;

    int height_ = 1;
    int subsz_  = 0;

    node_t(key_t key, iterator parent = nullptr, iterator left = nullptr, iterator right = nullptr) :
    key_(key), parent_(parent), left_(left), right_(right) {}
};

using iterator = node_t*;
const iterator nil_ = nullptr;

protected:
    iterator root_ = nil_;
    std::vector<std::unique_ptr<node_t>> nodes_; // if exception it flows up and everything is OK

    avl_mem_manager() = default;

    //---------------------------------------BIG_FIVE---------------------------------------//

    ~avl_mem_manager() = default; // noexcept

    avl_mem_manager(const avl_mem_manager &tree2) : avl_mem_manager() {
        avl_mem_manager tmp_mem;
        tmp_mem.avl_copy(tree2);

    //--------------------Kalb line--------------------//

        nodes_ = std::move(tmp_mem.nodes_);
        root_  = tmp_mem.root_;
    }


    avl_mem_manager(avl_mem_manager &&tree2) noexcept : avl_mem_manager() {
        nodes_ = std::move(tree2.nodes_);
        root_  = tree2.root_;
    }


    avl_mem_manager& operator= (const avl_mem_manager &tree2) {
        if (this == &tree2) return *this;

        avl_mem_manager tmp_mem;
        tmp_mem.avl_copy(tree2);

    //--------------------Kalb line--------------------//

        nodes_ = std::move(tmp_mem.nodes_);
        root_  = tmp_mem.root_;

        return *this;
    }


    avl_mem_manager& operator= (avl_mem_manager &&tree2) noexcept {
        if (this == &tree2) return *this;

        nodes_ = std::move(tree2.nodes_);
        root_  = tree2.root_;

        return *this;
    }
//--------------------------------------------------------------------------------------//

private:
    void avl_copy(const avl_mem_manager &tree2) {
        if (tree2.root_ == nil_) return;

        root_ = add_node(tree2.root_->key_);

        iterator dst = root_;
        iterator src = tree2.root_;

        while (true) {
            if      ((dst->left_  == nil_) && (src->left_  != nil_)) left_node_copy (src, dst);
            else if ((dst->right_ == nil_) && (src->right_ != nil_)) right_node_copy(src, dst);

            else if (dst != root_) {
                dst = dst->parent_;
                src = src->parent_;
            }

            else break;
        }
    }

    void left_node_copy(iterator &src, iterator &dst) {
        iterator sleft = src->left_;

        dst->left_ = add_node(sleft->key_, dst, nullptr, nullptr);

        iterator dleft = dst->left_;

        dleft->height_ = sleft->height_;
        dleft->subsz_  = sleft->subsz_;

        dst = dleft;
        src = sleft;
    }


    void right_node_copy(iterator &src, iterator &dst) {
        iterator sright = src->right_;

        dst->right_ = add_node(sright->key_, dst, nullptr, nullptr);

        iterator dright = dst->right_;

        dright->height_ = sright->height_;
        dright->subsz_  = sright->subsz_;

        dst = dright;
        src = sright;
    }

protected:
    node_t* add_node(key_t key, iterator parent = nullptr, iterator left = nullptr, iterator right = nullptr) {
        auto tmp = new node_t{key, parent, left, right};
        nodes_.push_back(std::move(std::unique_ptr<node_t>(tmp)));
        return nodes_.back().get();
    }
};

}


//---------------------------------------AVL TREE---------------------------------------//
template <typename key_t = int, typename comp = std::less<int>>
class avl_tree_t final : private detail::avl_mem_manager {
    using detail::avl_mem_manager::root_
    using detail::avl_mem_manager::nodes_;

    using detail::avl_mem_manager::node_t;
    using detail::avl_mem_manager::nil_;

public:
    avl_tree_t() = default; // not noexcept cuz of list but it's OK

private:
    void balance_update(iterator node) {
        if (node == nil_) return;

        iterator curr   = node;
        iterator parent = nil_;

        while (curr != nil_) {
            parent = curr->parent_;
            node_update(curr);

            int bfact = bfactor(curr);

            if (bfact == right_rotate_factor) {
                iterator right = curr->right_;

                if (bfactor(right) < zero_rotate_factor)
                    rotate_right(right);

                curr = rotate_left(curr);
                if (parent == nil_) root_ = curr;
            }

            else if (bfact == left_rotate_factor) {
                iterator left = curr->left_;

                if (bfactor(left) > zero_rotate_factor)
                    rotate_left(left);

                curr = rotate_right(curr);
                if (parent == nil_) root_ = curr;
            }

            curr = parent;
        }
    }


    iterator rotate_right(iterator node) {
        iterator new_polus = node->left_;
        iterator parent = node->parent_;
        iterator save = node;

        node->left_ = new_polus->right_;
        if (node->left_ != nil_) node->left_->parent_ = node;

        new_polus->right_ = node;
        node->parent_ = new_polus;
        new_polus->parent_ = parent;

        if (parent) {
            if (save == parent->right_) parent->right_ = new_polus;
            else parent->left_ = new_polus;
        }

        node_update(node);
        node_update(new_polus);

        return new_polus;
    }


    iterator rotate_left(iterator node) {
        iterator new_polus = node->right_;
        iterator parent = node->parent_;
        iterator save = node;

        node->right_ = new_polus->left_;
        if (node->right_) node->right_->parent_ = node;

        new_polus->left_ = node;
        node->parent_ = new_polus;
        new_polus->parent_ = parent;

        if (parent) {
            if (save == parent->right_) parent->right_ = new_polus;
            else parent->left_ = new_polus;
        }

        node_update(node);
        node_update(new_polus);

        return new_polus;
    }


    void node_update(iterator node) {
        int hl  = 0, hr  = 0;
        int szl = 0, szr = 0;

        iterator left  = node->left_;
        iterator right = node->right_;

        if (left != nil_) {
            hl  = left->height_;
            szl = left->subsz_ + 1;
        }

        if (right != nil_) {
            hr  = right->height_;
            szr = right->subsz_ + 1;
        }

        node->subsz_  = szl + szr;
        node->height_ = ((hl > hr) ? hl : hr) + 1;
    }


    int bfactor(iterator node) const {
        int hl = 0;
        int hr = 0;

        if (node->left_  != nil_) hl = node->left_ ->height_;
        if (node->right_ != nil_) hr = node->right_->height_;

        return hr - hl;
    }


public:
    void insert(key_t key) {
        if (root_ == nil_) {
            root_ = add_node(key);
    #ifdef DUMP_MODE
            dump(root_);
    #endif
            return;
        }

        iterator curr = root_;
        while (curr) {
            if (comp()(curr->key_, key)) {
                if (curr->right_) {
                    curr = curr->right_;
                    continue;
                }

                curr->right_ = add_node(key, curr, nullptr, nullptr);
                balance_update(curr);
    #ifdef DUMP_MODE
                dump(root_);
    #endif
            }

            else if (comp()(key, curr->key_)) {
                if (curr->left_) {
                    curr = curr->left_;
                    continue;
                }

                curr->left_ = add_node(key, curr, nullptr, nullptr);
                balance_update(curr);
    #ifdef DUMP_MODE
                dump(root_);
    #endif
            } return;
        }
    }


    iterator lower_bound(key_t key) const {
        iterator curr = root_;
        iterator ans  = nil_;

        while (curr != nil_) {
            if (comp()(key, curr->key_)) {
                ans = curr;
                curr = curr->left_;
                continue;
            }
            else if (comp()(curr->key_, key)) {
                curr = curr->right_;
                continue;
            }

            return curr;
        }

        return ans;
    }


    iterator upper_bound(key_t key) const {
        iterator curr = root_;
        iterator ans  = nil_;

        while (curr != nil_) {
            if (comp()(curr->key_, key)) {
                ans = curr;
                curr = curr->right_;
                continue;
            }
            else if (comp()(key, curr->key_)) {
                curr = curr->left_;
                continue;
            }

            return curr;
        }

        return ans;
    }


    int distance(iterator fst, iterator snd) const {
        int not_fit_cnt = 0;

        if ((fst == nil_) || (snd == nil_)) return 0;
        if (fst == snd) return 1;

        iterator curr = root_;
        key_t lwkey = fst->key_;
        key_t upkey = snd->key_;

        if (lwkey > upkey) return 0;

        while (true) {
            iterator left = curr->left_;

            if (curr == fst) {
                if (left != nil_) not_fit_cnt += left->subsz_ + 1;
                break;
            }

            if (comp()(curr->key_, lwkey)) {
                if (left != nil_) not_fit_cnt += left->subsz_ + 1;
                ++not_fit_cnt;

                curr = curr->right_;
                continue;
            }

            curr = left;
        }

        curr = root_;
        while (true) {
            iterator right = curr->right_;

            if (curr == snd) {
                if (right != nil_) not_fit_cnt += right->subsz_ + 1;
                break;
            }

            if (comp()(upkey, curr->key_)) {
                if (right != nil_) not_fit_cnt += right->subsz_ + 1;
                ++not_fit_cnt;

                curr = curr->left_;
                continue;
            }

            curr = right;
        }

        return root_->subsz_ + 1 - not_fit_cnt;
    }
};

} // namespace AVL