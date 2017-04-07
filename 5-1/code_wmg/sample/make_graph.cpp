
/**
 * @file undirected_graph.cpp
 * @author wang
 * @date 2016/02/19 00:57:35
 * @brief 
 *  
 **/

#include "undirected_graph.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " graph_file" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    UndirectedGraph ugraph(filename);
    ugraph.print(std::cout);

    std::vector<int> vec_degree_order;
    ugraph.sort_vertices_by_degree(&vec_degree_order);
    for (size_t sz = 0; sz < vec_degree_order.size(); ++sz) {
        std::cout << vec_degree_order[sz] << ' ';
    }
    std::cout << std::endl;

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
