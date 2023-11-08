#pragma once

#include "avl.hpp"
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


template <typename key_t, typename comp>
int subtree_dump(detail::node_t<key_t, comp>* node)
{
    std::cout << "graphviz init\n";
    open_grapviz();

    node_print<key_t, comp>(node);
    node_link<key_t, comp>(node);

    close_graphviz();

    char sys_cmd[200] = "dot ../logs/log_graphviz.dot -Tpng -o ../logs/images/tree_dump";
    snprintf(sys_cmd + strlen(sys_cmd), 30, "%d.png", graphviz_png_count);
    system(sys_cmd);

    graphviz_png_count++;

    return 0;
}


template <typename key_t, typename comp>
void node_link(detail::node_t<key_t, comp>* node)
{
    if (node == nullptr) return;

    if (node->get_left() != nullptr)
    {
        link_nodes<key_t, comp>(node, node->get_left());
        node_link<key_t, comp>(node->get_left());
    }

    if (node->get_right() != nullptr)
    {
        link_nodes<key_t, comp>(node, node->get_right());
        node_link<key_t, comp>(node->get_right());
    }
}


template <typename key_t, typename comp>
void link_nodes(detail::node_t<key_t, comp>* node1, detail::node_t<key_t, comp>* node2)
{
    ASSERT(node1);
    ASSERT(node2);
    ASSERT(graphviz_file);

    if (node2 == node1->get_right()) fprintf(graphviz_file, "    node_%p->node_%p [color = \"#1164B4\"];\n", node1, node2);
    if (node2 == node1->get_left())  fprintf(graphviz_file, "    node_%p->node_%p [color = \"#E32636\"];\n", node1, node2);
}


template <typename key_t, typename comp>
void node_print(detail::node_t<key_t, comp>* node)
{
    if (node == nullptr) return;

    graphviz_add_node<key_t, comp>(node);

    node_print<key_t, comp>(node->get_left());

    node_print<key_t, comp>(node->get_right());
}


template <typename key_t, typename comp>
void graphviz_add_node(detail::node_t<key_t, comp>* node)
{
    fprintf(graphviz_file, "    node_%p[shape = Mrecord, label = \"{{%p} | {%d} | {height = %d} | {size = %d}}\", style = \"filled\", fillcolor = \"#EFDECD\"];\n", node, node, node->get_key(), node->get_height(), node->get_size());
}

}