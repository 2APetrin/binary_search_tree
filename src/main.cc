#include "avl.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <set>

int main() {
    AVL::avl_tree_t tree;
    std::set<int> test_set;

    std::string cmd;
    int lwkey = 0;
    int upkey = 0;
    int key   = 0;

    while (std::cin >> cmd) {
        if (!cmd.compare("k")) {
            std::cout << "K\n";
            if (!(std::cin >> key)) std::cerr << "bad key\n";
            tree.insert(key);
            test_set.insert(key);
        }

        else if (!cmd.compare("q")) {
            if (!(std::cin >> lwkey >> upkey)) std::cerr << "bad ranges\n";
            int anst = AVL::get_tree_distance(tree, lwkey, upkey);
            int anss = std::distance(test_set.lower_bound(lwkey), test_set.upper_bound(upkey));

            std::cout << anst << " " << anss;
            if (anst != anss) std::cout << " ERROR\n";
            else std::cout << " correct\n";
        }

        else std::cerr << "bad ranges\n";
    }
}