
/**
 * @file make_graph.cpp
 * @author wang
 * @date 2016/02/18 20:15:07
 * @brief 
 *  
 **/

#include <stdlib.h>
#include <iostream>
#include "utils.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout<< "Usage: " << argv[0] << " number_vertices number_edges" << std::endl;
        std::cout << '\t' << argv[0] << " 10 3" << std::endl;
        return 1;
    }

    int num_vertices = atoi(argv[1]);
    int num_edges = atoi(argv[2]);

    std::vector<std::pair<int, int> > vec_edges;
    generate_random_graph(num_vertices, num_edges, &vec_edges);

    for (size_t sz = 0; sz < vec_edges.size(); ++sz) {
        std::pair<int, int>& edge = vec_edges.at(sz);
        std::cout << edge.first << '\t' << edge.second << std::endl;
    }
    
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
