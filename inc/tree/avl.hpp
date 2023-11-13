#pragma once

#include "custom_assert.hpp"
#include <functional>
#include <fstream>
#include <cstring>
#include <list>

//#define DUMP_MODE

namespace AVL {

int graphviz_png_count;

template <typename key_t = int, typename comp = std::less<int>>
class avl_tree_t final {

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

    iterator root_ = nil_;
    std::list<node_t> nodes_;

private:
    void balance_update(iterator node) {
        if (node == nil_) return;

        iterator curr = node;

        while (curr != nil_) {
            iterator parent = curr->parent_;

            node_update(curr);

            int bfact = bfactor(curr);

            if (bfact == 2) {
                iterator right = curr->right_;

                if (bfactor(right) < 0) rotate_right(right);

                curr = rotate_left(curr);
                if (parent == nil_) root_ = curr;
            }

            else if (bfact == -2) {
                iterator left = curr->left_;

                if (bfactor(left) > 0) rotate_left(left);

                curr = rotate_right(curr);
                if (parent == nil_) root_ = curr;
            }

            curr = parent;
        }
    }


    iterator rotate_right(iterator node) {
        //if (node == nil_) return nil_; test without error handler if

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
        //if (node == nil_) return nullptr;

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
        //if (node == nil_) return;

        int hl  = 0, hr  = 0;
        int szl = 0, szr = 0;

        iterator left  = node->left_;
        iterator right = node->right_;

        if (left) {
            hl  = left->height_;
            szl = left->subsz_ + 1;
        }

        if (right) {
            hr  = right->height_;
            szr = right->subsz_ + 1;
        }

        node->subsz_  = szl + szr;
        node->height_ = ((hl > hr) ? hl : hr) + 1;
    }


    int bfactor(iterator node) const {
        int hl = 0;
        int hr = 0;

        if (node->left_)  hl = node->left_->height_;
        if (node->right_) hr = node->right_->height_;

        return hr - hl;
    }


    void tree_copy(const avl_tree_t &tree2) {
        if (tree2.root_ == nil_) return;

        nodes_.push_back(node_t{tree2.root_->key_});
        root_ = &nodes_.back();

        iterator dst = root_;
        iterator src = tree2.root_;

        while (true) {
            if ((dst->left_ == nil_) && (src->left_ != nil_)) {
                iterator sleft = src->left_;

                nodes_.push_back(node_t{sleft->key_, dst, nullptr, nullptr});
                dst->left_ = &nodes_.back();
                iterator dleft = dst->left_;

                dleft->height_ = sleft->height_;
                dleft->subsz_  = sleft->subsz_;

                dst = dleft;
                src = sleft;
            }

            else if ((dst->right_ == nil_) && (src->right_ != nil_)) {
                iterator sright = src->right_;

                nodes_.push_back(node_t{sright->key_, dst, nullptr, nullptr});
                dst->right_ = &nodes_.back();
                iterator dright = dst->right_;

                dright->height_ = sright->height_;
                dright->subsz_  = sright->subsz_;

                dst = dright;
                src = sright;
            }

            else if (dst != root_) {
                dst = dst->parent_;
                src = src->parent_;
            }

            else break;
        }
    }


public:
    void insert(key_t key) {
        if (root_ == nil_) {
            nodes_.emplace_back(node_t{key});
            root_ = &nodes_.back();
    #ifdef DUMP_MODE
            subtree_dump(root_);
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

                nodes_.emplace_back(node_t{key, curr, nullptr, nullptr});
                curr->right_ = &nodes_.back();
                balance_update(curr);
    #ifdef DUMP_MODE
                subtree_dump(root_);
    #endif
            }

            else if (comp()(key, curr->key_)) {
                if (curr->left_) {
                    curr = curr->left_;
                    continue;
                }

                nodes_.emplace_back(node_t{key, curr, nullptr, nullptr});
                curr->left_ = &nodes_.back();
                balance_update(curr);
    #ifdef DUMP_MODE
                subtree_dump(root_);
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

//---------------------------------------BIG_FIVE---------------------------------------//

    avl_tree_t()  = default;

    ~avl_tree_t() = default;

    avl_tree_t(const avl_tree_t &tree2) : avl_tree_t() { tree_copy(tree2); }

    avl_tree_t(avl_tree_t &&tree2) noexcept : avl_tree_t() {
        nodes_ = std::move(tree2.nodes_);
        root_ = tree2.root_;
    }


    avl_tree_t& operator= (const avl_tree_t &tree2) {
        if (this == &tree2) return *this;

        nodes_.clear();
        tree_copy(tree2);
        return *this;
    }


    avl_tree_t& operator= (avl_tree_t &&tree2) noexcept {
        if (this == &tree2) return *this;

        nodes_ = std::move(tree2.nodes_);
        root_ = tree2.root_;

        return *this;
    }

//---------------------------------------GRAPHVIZ---------------------------------------//

private:
    void subtree_dump(iterator node) const {
        std::ofstream out;
        open_grapviz(out);

        node_print(node, out);
        node_link(node, out);

        close_graphviz(out);

        char sys_cmd[200] = "dot ../logs/log_graphviz.dot -Tpng -o ../logs/images/tree_dump";
        snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count);
        system(sys_cmd);

        std::cout << graphviz_png_count << " dump made\n";
        graphviz_png_count++;
    }


    int open_grapviz(std::ofstream &out) const {
        out.open("../logs/log_graphviz.dot");
        if (!out.is_open()) {
            std::cerr << "Cannot open graphviz file. Programm shutdown\n";
            return 1;
        }

        out << "digraph\n{\n";
        return 0;
    }


    void close_graphviz(std::ofstream &out) const {
        ASSERT(out.is_open());

        out << "}";
        out.close();
    }


    void node_link(iterator node, std::ofstream &out) const {
        if (node == nil_) return;

        if (node->left_ != nil_) {
            link_nodes(node, node->left_, out);
            node_link(node->left_, out);
        }

        if (node->right_ != nil_) {
            link_nodes(node, node->right_, out);
            node_link(node->right_, out);
        }
    }


    void link_nodes(iterator node1, iterator node2, std::ofstream &out) const {
        ASSERT(node1 != nil_);
        ASSERT(node2 != nil_);
        ASSERT(out.is_open());

        if (node2 == node1->right_)  out << "    node_" << node1 << "->node_" << node2 << " [color = \"#E32636\"];\n";
        if (node2 == node1->left_)   out << "    node_" << node1 << "->node_" << node2 << " [color = \"#1164B4\"];\n";
        if (node2 == node1->parent_) out << "    node_" << node1 << "->node_" << node2 << " [color = \"#140F0B\", style = dotted];\n";
        if (node1 == node2->parent_) out << "    node_" << node2 << "->node_" << node1 << " [color = \"#140F0B\", style = dotted];\n";
    }


    void node_print(iterator node, std::ofstream &out) const {
        if (node == nil_) return;

        graphviz_add_node(node, out);

        node_print(node->left_, out);

        node_print(node->right_, out);
    }


    void graphviz_add_node(iterator node, std::ofstream &out) const {
        out << "    node_" << node << "[shape = Mrecord, label = \"{{" << node << "} | {" << node->key_ << "} | {height = " << node->height_ << "} | {size = " << node->subsz_ << "}}\", style = \"filled\", fillcolor = \"#EFDECD\"];\n";
    }
};


template <typename key_t = int, typename comp = std::less<int>>
int get_tree_distance(avl_tree_t<key_t, comp> &tr, key_t lower, key_t upper) {
    auto fst = tr.lower_bound(lower),
         snd = tr.upper_bound(upper);

    return tr.distance(fst, snd);
}

}