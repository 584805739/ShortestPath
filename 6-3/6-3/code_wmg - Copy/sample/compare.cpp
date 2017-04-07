 
/**
 * @file compare.cpp
 * @author wang
 * @date 2016/03/03 23:29:16
 * @brief 
 *  
 **/

#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "undirected_graph.h"
#include "shortest_path.h"
#include "pruned_landmark_labeling.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage:" << argv[0] << " graph_data_file max_vertice test_num" << std::endl;
        return 1;
    }

    const char* filename = argv[1];
    PrunedLandmarkLabeling<> pll;
    if (!pll.ConstructIndex(filename)) {
        std::cerr << "failed to construct index." << std::endl;
        return 1;
    }
    
    double time_index = -get_current_time_in_seconds();
    UndirectedGraph ugraph(filename);
    ShortestPathUtil sp_util(&ugraph);
    sp_util.construct_index_ext();
    time_index += get_current_time_in_seconds();
    long mem_index = sp_util.compute_index_mem_ext();

    int max_vertice = atoi(argv[2]);
    int num_cases = atoi(argv[3]);
    srand(time(NULL));
    double query_time1 = .0;
    double query_time2 = .0;
    int i = 1;
    for ( ; i <= num_cases; ++i) {
        int w = rand() % max_vertice + 1;
        int v = rand() % max_vertice + 1;
        query_time1 -= get_current_time_in_seconds();
        int dist_benchmark = pll.QueryDistance(w, v);
        query_time1 += get_current_time_in_seconds();
        query_time2 -= get_current_time_in_seconds();
        int dist_ours = sp_util.query_distance_ext(w, v);
        query_time2 += get_current_time_in_seconds();
        std::cout << "Distance for (" << w << ',' << v << ')' << std::endl;
        std::cout << dist_benchmark << '\t' << dist_ours << std::endl;
        if (dist_benchmark == dist_ours
                ||( dist_benchmark == INT_MAX
                && dist_ours == -1)) {
                    ;
        }else{
            std::cout << "Distance error for (" << w << ',' << v << ')' << std::endl;
            break;
        }
    }
    query_time1 /= i;
    query_time2 /= i;

    std::cout << "===========BEGIN=============" << std::endl;
    pll.PrintStatistics();
    std::cout << "=============================" << std::endl;
    std::cout << "Graph Loading+Indexing Time(sec):" << time_index << std::endl;
    std::cout << "Index Structure Mem(bytes):" << mem_index << std::endl;
    std::cout << "Average Query Time(sec):" << query_time1 << "(benchmark):"
        << query_time2 << "(ours)" << std::endl;
    std::cout << "===========END==============" << std::endl;

    return 0;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
