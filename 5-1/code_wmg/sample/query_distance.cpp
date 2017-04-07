
 
/**
 * @file undirected_graph.cpp
 * @author wang
 * @date 2016/02/19 00:57:35
 * @brief 
 *  
 **/

#include <stdlib.h>
#include "utils.h"
#include "undirected_graph.h"
#include "shortest_path.h"

int main(int argc, char** argv) {
    /*
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " graph_file w v" << std::endl;
        return 1;
    }*/

    const char* filename = "data/graph1.dat";
    double time_load = -get_current_time_in_mseconds();
    UndirectedGraph ugraph(filename);
    time_load += get_current_time_in_mseconds();

    double time_index = -get_current_time_in_mseconds();
    ShortestPathUtil sp_util(&ugraph);
    sp_util.construct_index_ext();
    time_index += get_current_time_in_mseconds();

    int mem_index = sp_util.compute_index_mem_ext();

    //int w = atoi(argv[2]);
    //int v = atoi(argv[3]);
    int w = 12;
    int v = 1;
    double time_query = -get_current_time_in_mseconds();
    int distance = sp_util.query_distance_ext(w, v);
    time_query += get_current_time_in_mseconds();
    std::cout << "Dist(" << w << ',' << v << ")=" << distance << std::endl;

    std::cout << "Graph Loading Time(msec):" << time_load << std::endl;
    std::cout << "Graph Indexing Time(msec):" << time_index << std::endl;
    std::cout << "Query Time(msec):" << time_query << std::endl;
    std::cout << "Index Structure Mem(bytes):" << mem_index << std::endl;

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
