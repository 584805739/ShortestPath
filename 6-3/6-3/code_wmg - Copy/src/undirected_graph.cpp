
 
/**
 * @file undirected_graph.cpp
 * @author wang
 * @date 2016/02/18 23:38:37
 * @brief 
 *  
 **/

#include "undirected_graph.h"
#include <assert.h>
#include <algorithm>
#include "utils.h"

UndirectedGraph::UndirectedGraph():_num_vertices(0), _num_edges(0), _pp_buf(NULL) {
}

UndirectedGraph::UndirectedGraph(const std::vector<std::pair<int, int> >& vec_pairs) {
    construct_graph(vec_pairs);
}

UndirectedGraph::UndirectedGraph(const char* filename) {
    std::vector<std::pair<int, int> > vec_pairs;
    bool load_res = load_graph(filename, &vec_pairs);
    if (load_res) {
        construct_graph(vec_pairs);
    }
}

UndirectedGraph::~UndirectedGraph() {
    delete[] _pp_buf;
}

const std::vector<int>& UndirectedGraph::operator[] (int vertice) {
    assert(_pp_buf != NULL && vertice >= 0 && vertice < _num_vertices);
    return *(_pp_buf + vertice);
}

void UndirectedGraph::sort_vertices_in_order() {
    if (_pp_buf == NULL) {
        return;
    }

    for (int i = 0; i < _num_vertices; ++i) {
        std::vector<int>& vec = *(_pp_buf + i);
        std::sort(vec.begin(), vec.end(), std::less<int>());
    }
}

bool UndirectedGraph::sort_vertices_by_degree(std::vector<int>* vec_degrees) {
    if (_pp_buf == NULL || vec_degrees == NULL) {
        return false;
    }

    vec_degrees->clear();
    vec_degrees->reserve(_num_vertices);
    std::vector<std::pair<int, int> > vec_deg_pair;

    for (int i = 0; i < _num_vertices; ++i) {
        int degree = (_pp_buf + i)->size();
        if (degree == 0) {
            continue;
        }
        vec_deg_pair.push_back(std::pair<int, int>(degree, i));
    }

    //sort the vertices in descreasing order of degree
    std::sort(vec_deg_pair.begin(), vec_deg_pair.end(), std::greater<std::pair<int,int> >());
    for (size_t i = 0; i < vec_deg_pair.size(); ++i) {
        vec_degrees->push_back(vec_deg_pair[i].second);
    }

    return true;
}

void UndirectedGraph::construct_graph(const std::vector<std::pair<int, int> >& vec_pairs) {
    _num_edges = (int)vec_pairs.size();

    int max_vertices_idx = 0;
    for (size_t sz = 0; sz < vec_pairs.size(); ++sz) {
        max_vertices_idx = std::max(max_vertices_idx, 
                std::max(vec_pairs[sz].first, vec_pairs[sz].second));
    }
    _num_vertices = max_vertices_idx + 1;

    _pp_buf = new std::vector<int>[_num_vertices];

    int vertices0;
    int vertices1;
    for (size_t sz = 0; sz < vec_pairs.size(); ++sz) {
        const std::pair<int, int>& edge = vec_pairs.at(sz);
        vertices0 = edge.first;
        vertices1 = edge.second;
        if (vertices0 == vertices1) {
            continue;
        }
        _pp_buf[vertices0].push_back(vertices1);
        _pp_buf[vertices1].push_back(vertices0);
    }
}

void UndirectedGraph::print(std::ostream& os) const {
    for (int i = 0; i< _num_vertices; ++i) {
        os << '[' << i << "]:";
        std::vector<int>& vec = *(_pp_buf + i);
        for (size_t sz = 0; sz < vec.size(); ++sz) {
            os << ' ' << vec.at(sz);
        }
        os << std::endl;
    }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
