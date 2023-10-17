#pragma once

#include "avl_tree.hpp"
#include "custom_assert.hpp"
#include <cstring>
#include <iostream>

namespace AVL {

//! @brief graphviz file
//!
FILE* graphviz_file;


//! @brief extern counter for graphviz pngs
//!
int graphviz_png_count;


int open_grapviz()
{
    if ((graphviz_file = fopen("../logs/log_graphviz.dot", "w")) == nullptr)
    {
        printf("Cannot open graphviz file. Programm shutdown\n");
        return 1;
    }

    fprintf(graphviz_file, "digraph\n{\n");

    return 0;
}


void close_graphviz(void)
{
    ASSERT(graphviz_file);

    fprintf(graphviz_file, "}");

    fclose(graphviz_file);
}


template <typename key_t>
int subtree_dump(node_t<key_t>* node)
{
    std::cout << "graphviz init\n";
    open_grapviz();

    node_print<key_t>(node);
    node_link<key_t>(node);

    close_graphviz();

    char sys_cmd[200] = "dot ../logs/log_graphviz.dot -Tpng -o ../logs/images/tree_dump";
    snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count);
    system(sys_cmd);

    graphviz_png_count++;

    return 0;
}


template <typename key_t>
void node_link(node_t<key_t>* node)
{
    if (node == nullptr) return;

    if (node->get_left() != nullptr)
    {
        link_nodes<key_t>(node, node->get_left());
        node_link<key_t>(node->get_left());
    }

    if (node->get_right() != nullptr)
    {
        link_nodes<key_t>(node, node->get_right());
        node_link<key_t>(node->get_right());
    }
}


template <typename key_t>
void link_nodes(node_t<key_t>* node1, node_t<key_t>* node2)
{
    ASSERT(node1);
    ASSERT(node2);
    ASSERT(graphviz_file);

    if (node2 == node1->get_right()) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#1164B4\"];\n", node1, node2);
    if (node2 == node1->get_left())  fprintf(graphviz_file, "    node_%p->node_%p [color = \"#E32636\"];\n", node1, node2);
}


template <typename key_t>
void node_print(node_t<key_t>* node)
{
    if (node == nullptr) return;

    graphviz_add_node(node);

    node_print<key_t>(node->get_left());

    node_print<key_t>(node->get_right());
}


template <typename key_t>
void graphviz_add_node(node_t<key_t>* node)
{
    fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%d}}\", style = \"filled\", fillcolor = \"#EFDECD\"];\n", node, node, node->get_key());
}

}