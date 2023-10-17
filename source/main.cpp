#include "avl_tree.hpp"
#include "avl_graphviz.hpp"


int main()
{
    AVL::tree_t<int> tree;

    tree.insert(1);
    tree.insert(1);
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);
    tree.insert(-5);
    tree.insert(500);
    tree.insert(10);
    tree.insert(1);
    tree.insert(-1);
}
