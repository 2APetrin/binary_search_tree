#pragma once

#include "custom_assert.hpp"
#include <functional>
#include <cstring>
#include <iostream>
#include <list>

namespace AVL {

FILE* graphviz_file;
int graphviz_png_count;

template <typename key_t = int, typename comp = std::less<int>>
class avl_tree_t {

    struct node_t {
        using iterator = node_t*;

        key_t key_;
        iterator parent_, left_, right_;

        int height_ = 1;
        int subsz_  = 0;

        node_t(key_t key, iterator parent = nullptr, iterator left = nullptr, iterator right = nullptr) :
        key_(key), parent_(parent), left_(left), right_(right) {}

        void print() const {
            std::cout << "this   = " << this    << std::endl;
            std::cout << "left   = " << left_   << std::endl;
            std::cout << "right  = " << right_  << std::endl;
            std::cout << "parent = " << parent_ << std::endl;
            std::cout << "key    = " << key_    << std::endl;
            std::cout << "height = " << height_ << std::endl;
            std::cout << "subsz_ = " << subsz_  << std::endl;
        }
    };

    using iterator = node_t*;

    iterator root_ = nullptr;
    std::list<node_t> nodes_;

    const iterator nil_ = nullptr;

private:
    void balance_update(iterator node) {
        if (node == nil_) return;

        iterator curr = node;

        while (curr) {
            iterator parent = curr->parent_;

            node_update(curr);

            if (bfactor(curr) == 2) {
                if (bfactor(curr->right_) < 0) {
                    rotate_right(curr->right_);
                }
                curr = rotate_left(curr);
                if (parent == nil_) root_ = curr;

                curr = parent;
                continue;
            }

            if (bfactor(curr) == -2) {
                if (bfactor(curr->left_) > 0) {
                    rotate_left(curr->left_);
                }
                curr = rotate_right(curr);
                if (parent == nil_) root_ = curr;

                curr = parent;
                continue;
            }

            curr = parent;
        }
    }


    iterator rotate_right(iterator node) {
        if (node == nil_) return nullptr;

        iterator new_polus = node->left_;
        iterator parent = node->parent_;
        iterator save = node;

        node->left_ = new_polus->right_;
        if (node->left_) node->left_->parent_ = node;

        new_polus->right_ = node;
        node->parent_ = new_polus;
        new_polus->parent_ = parent;

        if (parent) {
            if (save == parent->right_) parent->right_ = new_polus;
            if (save == parent->left_)  parent->left_  = new_polus;
        }

        node_update(node);
        node_update(new_polus);

        return new_polus;
    }


    iterator rotate_left(iterator node) {
        if (node == nil_) return nullptr;

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
            if (save == parent->left_)  parent->left_  = new_polus;
        }

        node_update(node);
        node_update(new_polus);

        return new_polus;
    }


    void node_update(iterator node) {
        if (node == nil_) return;

        int hl  = 0, hr  = 0;
        int szl = 0, szr = 0;

        if (node->left_) {
            hl  = node->left_->height_;
            szl = node->left_->subsz_ + 1;
        }

        if (node->right_) {
            hr  = node->right_->height_;
            szr = node->right_->subsz_ + 1;
        }

        node->subsz_  = szl + szr;
        node->height_ = ((hl > hr) ? hl : hr) + 1;
    }


    int bfactor(iterator node) {
        int hl = 0;
        int hr = 0;

        if (node->left_)  hl = node->left_->height_;
        if (node->right_) hr = node->right_->height_;

        return hr - hl;
    }

public:
    //avl_tree_t() {}


    void insert(key_t key) {
        if (root_ == nil_) {
            nodes_.emplace_back(key);
            root_ = &nodes_.back();
            subtree_dump(root_);
            return;
        }

        iterator curr = root_;
        while (curr) {
            if (comp()(curr->key_, key)) {
                if (curr->right_) {
                    curr = curr->right_;
                    continue;
                }

                nodes_.emplace_back(key, curr, nullptr, nullptr);
                curr->right_ = &nodes_.back();
                balance_update(curr);
                subtree_dump(root_);
                return;
            }

            if (comp()(key, curr->key_)) {
                if (curr->left_) {
                    curr = curr->left_;
                    continue;
                }

                nodes_.emplace_back(key, curr, nullptr, nullptr);
                curr->left_ = &nodes_.back();
                balance_update(curr);
                subtree_dump(root_);
                return;
            }

            //std::cout << "eq elements\n";
            return;
        }

        std::cout << "while drop\n";
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

        if (!fst || !snd) return 0;

        iterator curr = root_;
        key_t lwkey = fst->key_;
        key_t upkey = snd->key_;

        while (true) {
            iterator left = curr->left_;

            if (curr == fst) {
                if (left != nil_) not_fit_cnt += left->subsz_ + 1;
                break;
            }

            if (comp()(curr->key_, lwkey)) {
                if (left != nil_) not_fit_cnt += left->subsz_ + 2;
                curr = curr->right_;
                continue;
            }

            curr = left;
        }
        //std::cout << "not fit1 = " << not_fit_cnt << std::endl;

        curr = root_;
        while (true) {
            iterator right = curr->right_;

            if (curr == snd) {
                if (right != nil_) not_fit_cnt += right->subsz_ + 1;
                break;
            }

            if (comp()(upkey, curr->key_)) {
                if (right != nil_) not_fit_cnt += right->subsz_ + 2;
                curr = curr->left_;
                continue;
            }

            curr = right;
        }
        //std::cout << "not fit2 = " << not_fit_cnt << std::endl;

        return root_->subsz_ + 1 - not_fit_cnt;
    }


//---------------------------------------GRAPHVIZ---------------------------------------//

    int subtree_dump(iterator node) {
        open_grapviz();

        node_print(node);
        node_link(node);

        close_graphviz();

        char sys_cmd[200] = "dot ../logs/log_graphviz.dot -Tpng -o ../logs/images/tree_dump";
        snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count);
        system(sys_cmd);

        std::cout << graphviz_png_count << " dump made\n";
        graphviz_png_count++;

        return 0;
    }


    int open_grapviz() {
        if ((graphviz_file = fopen("../logs/log_graphviz.dot", "w")) == nullptr) {
            printf("Cannot open graphviz file. Programm shutdown\n");
            return 1;
        }

        fprintf(graphviz_file, "digraph\n{\n");
        return 0;
    }


    void close_graphviz(void) {
        ASSERT(graphviz_file);

        fprintf(graphviz_file, "}");

        fclose(graphviz_file);
    }


    void node_link(iterator node) {
        if (node == nil_) return;

        if (node->left_) {
            link_nodes(node, node->left_);
            node_link(node->left_);
        }

        if (node->right_) {
            link_nodes(node, node->right_);
            node_link(node->right_);
        }
    }


    void link_nodes(iterator node1, iterator node2) {
        ASSERT(node1);
        ASSERT(node2);
        ASSERT(graphviz_file);

        if (node2 == node1->right_)  fprintf(graphviz_file, "    node_%p->node_%p [color = \"#E32636\"];\n", node1, node2);
        if (node2 == node1->left_)   fprintf(graphviz_file, "    node_%p->node_%p [color = \"#1164B4\"];\n", node1, node2);
        if (node2 == node1->parent_) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#140F0B\", style = dotted];\n", node1, node2);
        if (node1 == node2->parent_) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#140F0B\", style = dotted];\n", node2, node1);
    }


    void node_print(iterator node) {
        if (node == nil_) return;

        graphviz_add_node(node);

        node_print(node->left_);

        node_print(node->right_);
    }


    void graphviz_add_node(iterator node) {
        fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%d} | {height = %d} | {size = %d}}\", style = \"filled\", fillcolor = \"#EFDECD\"];\n", node, node, node->key_, node->height_, node->subsz_);
    }
};


template <typename key_t = int, typename comp = std::less<int>>
int get_tree_distance(avl_tree_t<key_t, comp> &tr, key_t lower, key_t upper) {
    if (lower > upper) return 0;

    auto fst = tr.lower_bound(lower),
         snd = tr.upper_bound(upper);

    return tr.distance(fst, snd);
}

}