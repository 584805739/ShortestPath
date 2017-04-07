 
 
/**
 * @file utils.cpp
 * @author wang
 * @date 2016/02/18 19:45:12
 * @brief 
 *  
 **/

#include "utils.h"
#include <stdlib.h>
#include <sys/time.h>
#include <fstream>
#include <set>

double get_current_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec  + tv.tv_usec * 1e-6;
}

void generate_random_graph(int num_ver, int num_edges, std::vector<std::pair<int, int> >* ptr_vec) {
    if (ptr_vec == NULL || num_ver <=0 || num_edges <= 0) {
        return;
    }
    ptr_vec->clear();

    const int MAX_EDGES = (num_ver * (num_ver -1))/2;
    num_edges = (num_edges < MAX_EDGES)? num_edges : MAX_EDGES;
    ptr_vec->reserve(num_edges);

    srand(time(NULL));
    int vertice0;
    int vertice1;
    int rand_num;
    const int MAX_PAIRS = num_ver * num_ver;
    std::set<std::pair<int,int> > set_rand;
    for (int i = 0; i < num_edges; ++i) {
        rand_num = rand() % MAX_PAIRS;
        vertice0 = rand_num / num_ver;
        vertice1 = rand_num % num_ver;
        std::pair<int, int> edge(vertice0, vertice1);

        if (vertice0 == vertice1 
                || set_rand.find(edge) != set_rand.end()) {
            i -= 1;
            continue;
        }
        set_rand.insert(edge);
        ptr_vec->push_back(edge);
    }
}

bool load_graph(const char* file_name, std::vector<std::pair<int, int> >* ptr_vec) {
    if (file_name == NULL || ptr_vec == NULL) {
        return false;
    }
    ptr_vec->clear();

    std::ifstream ifs(file_name);
    int vertice0;
    int vertice1;
    if (ifs.is_open()) {
        while (ifs >> vertice0 >> vertice1) {
            ptr_vec->push_back(std::pair<int, int>(vertice0, vertice1));
        }
        return true;
    }
    return false;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
