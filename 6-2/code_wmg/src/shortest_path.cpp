
/**
 * @file shortest_path.cpp
 * @author wang
 * @date 2016/02/18 23:10:38
 * @brief
 *
 **/

#include "shortest_path.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <stdio.h>
#include <fstream>
#include <xmmintrin.h>
#include <stdint.h>

using namespace std;
ShortestPathUtil::ShortestPathUtil(UndirectedGraph* ptr_ugraph) {
    _ptr_ugraph = ptr_ugraph;
    _ptr_index = NULL;
    _ptr_paths = NULL;
    _num_paths = 0;
}

ShortestPathUtil::~ShortestPathUtil() {
    free_mem();
}

void ShortestPathUtil::reset_graph(UndirectedGraph* ptr_ugraph) {
    free_mem();
    _ptr_ugraph = ptr_ugraph;
}

bool ShortestPathUtil::construct_index() {
    if (_ptr_ugraph == NULL) {
        return false;
    }

    int num_vertices = _ptr_ugraph->get_vertices_num();
    std::vector<bool> vec_used_as_root(num_vertices, false);

    //allocate memory for the index structure
    _ptr_index = (index_t*)calloc(num_vertices, sizeof(index_t));
    if (_ptr_index == NULL) {
        return false;
    }
    for (int i = 0; i < num_vertices; ++i) {
        _ptr_index[i].reset();
    }

    //sort the vertices in descending order of degree
    std::vector<int> vec_degree;
    _ptr_ugraph->sort_vertices_by_degree(&vec_degree);

    std::vector<std::pair<std::vector<int>, std::vector<int> > >
        buf_index(num_vertices, make_pair(std::vector<int>(), std::vector<int>()));
    std::queue<int> queue_vertices;

    for (int v = 0; v < num_vertices; ++v) {
        int vertice = vec_degree[v];
        if (vec_used_as_root[vertice]) {
            continue;
        }

        std::vector<bool> vec_visited(num_vertices, false);
        //std::pair<std::vector<int>, std::vector<int> >& buf_index_v = buf_index[vertice];

        queue_vertices.push(vertice);
        vec_visited[vertice] = true;
        std::vector<int> dst_v(num_vertices, -1);
        /*for (size_t sz = 0; sz < buf_index_v.first.size(); ++sz) {
            dst_v[buf_index_v.first[sz]] = buf_index_v.second[sz];
        }*/
        dst_v[vertice] = 0;

        while (!queue_vertices.empty()) {
            int uid = queue_vertices.front();
            queue_vertices.pop();

            if (vec_used_as_root[uid]) {
                continue;
            }

            std::pair<std::vector<int>, std::vector<int> >& buf_index_u = buf_index[uid];

            //pruning strategy
            bool prune_flag = false;
            for (size_t sz = 0; sz < buf_index_u.first.size(); ++sz) {
                int w = buf_index_u.first[sz];
                if (dst_v[w] == -1) {
                    continue;
                }
                int td = buf_index_u.second[sz] + dst_v[w];
                if (td <= dst_v[uid]) {
                    prune_flag = true;
                    break;
                }
            }

            if (prune_flag) {
                continue;
            }

            //continue traversing
            buf_index_u.first.push_back(vertice);
            buf_index_u.second.push_back(dst_v[uid]);
            const std::vector<int> siblings = (*_ptr_ugraph)[uid];
            for (size_t sz = 0; sz < siblings.size(); ++sz) {
                int w = siblings.at(sz);
                if (vec_visited[w]) {
                    continue;
                }
                dst_v[w] = dst_v[uid] + 1;
                queue_vertices.push(w);
                vec_visited[w] = true;
            }
        }
        vec_used_as_root[vertice] = true;
    }

    //create index structure
    for (int i = 0; i < num_vertices; ++i) {
        int vertice = vec_degree[i];
        size_t size = buf_index[vertice].first.size();
        _ptr_index[vertice].ptr_lv = (int*)calloc(size, sizeof(int));
        _ptr_index[vertice].ptr_ld = (int*)calloc(size, sizeof(int));
        if (_ptr_index[vertice].ptr_lv == NULL
                || _ptr_index[vertice].ptr_ld == NULL) {
            free_mem();
            return false;
        }
        _ptr_index[vertice].size = size;

        //sort the labels(vertices+distances) in ascending order
        std::vector<std::pair<int, int> > vec_pair;
        for (size_t sz = 0; sz < size; ++sz) {
            vec_pair.push_back(std::pair<int,int>(buf_index[vertice].first[sz],sz));
        }
        std::sort(vec_pair.begin(), vec_pair.end(), std::less<std::pair<int, int> >());
        for (size_t sz = 0; sz < size; ++sz) {
            _ptr_index[vertice].ptr_lv[sz] = buf_index[vertice].first[vec_pair[sz].second];
            _ptr_index[vertice].ptr_ld[sz] = buf_index[vertice].second[vec_pair[sz].second];
        }
        //free the memory of vector
        std::vector<int>().swap(buf_index[vertice].first);
        std::vector<int>().swap(buf_index[vertice].second);
    }

    return true;
}

/**
 * @brief get the longest path according to the array storing parent vertice
 *
 * @see
 * @note the parent for root vertice is -1
 * @author wang
 * @date 2016/02/20 22:15:34
**/
static int get_longest_path(const int* pre_ver_buf,
        int vertices_num,
        int last,
        std::vector<int>* ptr_vec) {
    if (pre_ver_buf == NULL || ptr_vec == NULL) {
        return 0;
    }
    ptr_vec->clear();

    if (last < 0 || last > vertices_num) {
        return 0;
    }
    std::vector<int> reverse_path;

    while (last != -1) {
        reverse_path.push_back(last);
        last = *(pre_ver_buf + last);
    }
    size_t num = reverse_path.size();
    for(size_t sz = 0; sz < num; ++sz) {
        ptr_vec->push_back(reverse_path.at(num - sz - 1));
    }

    return ptr_vec->size();
}

bool ShortestPathUtil::construct_index_ext() {
    if (_ptr_ugraph == NULL) {
        return false;
    }
    ofstream outfile;
    outfile.open("index_r.txt");
    int num_vertices = _ptr_ugraph->get_vertices_num();
    std::vector<bool> vec_used_as_root(num_vertices, false);
    std::vector<bool> vec_vertice_in_path(num_vertices, false);
    std::vector<unsigned int> vec_degree_rank(num_vertices, -1);


    //allocate memory for the index structure
    _ptr_index = (index_t*)calloc(num_vertices, sizeof(index_t));
    if (_ptr_index == NULL) {
        return false;
    }
    for (int i = 0; i < num_vertices; ++i) {
        _ptr_index[i].reset();
    }

    //sort the vertices in descending order of degree
    std::vector<int> vec_degree;
    _ptr_ugraph->sort_vertices_by_degree(&vec_degree);

    std::vector<std::pair<std::vector<int>, std::vector<int> > >
        buf_index(num_vertices, make_pair(std::vector<int>(), std::vector<int>()));
    std::vector<std::map<int, int> > 
        buf_expand_index(num_vertices);//index store expand labels

    std::map<int, std::vector<int> > map_vertice2paths;//stores vertice->path index
    std::vector<std::vector<int> > vec_paths;
    std::queue<int> queue_vertices;
    std::set<int> set_vertice_in_paths;

    int pre_ver_buf[num_vertices];//record the parent vertice for each vertice
    std::vector<int> vec_longest_path;
    int count_times = 0;
    unsigned long num_label_prune = 0;

    for(int i = 0;i < vec_degree.size();++i)
    {
    	vec_degree_rank[vec_degree[i]] = vec_degree.size() - i;
    }

    //for (int v = 0; v < num_vertices; ++v) {
    for (size_t v = 0; v < vec_degree.size(); ++v) {
        int vertice = vec_degree[v];
        if (vec_used_as_root[vertice]) {
            continue;
        }

        std::vector<bool> vec_visited(num_vertices, false);

        queue_vertices.push(vertice);
        vec_visited[vertice] = true;
        std::vector<int> dst_v(num_vertices, -1);
        dst_v[vertice] = 0;
        
        std::vector<int> dst_r(num_vertices, -1); //store all labels of the root vertice(including expanded labels)
        dst_r[vertice] = 0;

        std::set<int> set_addlabel;
        int uid = -1;
        pre_ver_buf[vertice] = -1;
        int cur_level_num = 1;
        int next_level_num = 0;
        int cnt = 0;
        bool prune_flag = false;
        std::vector<int> vec_last_level_vertices;//stores the visited vertices at the last level
        //std::vector<std::pair<int, int> > ptr_label_u;
        //std::vector<std::pair<int, int> > ptr_label_v;

        std::pair<std::vector<int>, std::vector<int> >& buf_index_v = buf_index[vertice];

        std::map<int, int> &map_expand_index_v = buf_expand_index[vertice]; // store vertice's expand labels
        // Prefetch
        _mm_prefetch(&buf_index_v.first[0], _MM_HINT_T0);
        _mm_prefetch(&buf_index_v.second[0], _MM_HINT_T0);
        _mm_prefetch(&map_expand_index_v, _MM_HINT_T0);
        //_mm_prefetch(&map_expand_index_v, _MM_HINT_T0);

        for (size_t sz = 0; sz < buf_index_v.first.size(); ++sz) 
        {
            if(buf_index_v.first[sz] >= 0)
            {
                dst_r[buf_index_v.first[sz]] = buf_index_v.second[sz];
            }
        }

        std::map<int,int>::iterator iter = map_expand_index_v.begin();

        for(iter = map_expand_index_v.begin(); iter != map_expand_index_v.end(); ++iter)
        {
            dst_r[iter->first] = iter->second;
        }

        //std::cout << "Queue Sequence:";
        while (!queue_vertices.empty()) {
            uid = queue_vertices.front();
            //std::cout << uid << ' ';
            queue_vertices.pop();
            ++cnt;
            prune_flag = false;
            if (vec_used_as_root[uid]) {
                prune_flag = true;
                continue;
            }
            vec_last_level_vertices.push_back(uid);

            std::pair<std::vector<int>, std::vector<int> >& buf_index_u = buf_index[uid];

            std::map<int, int> &map_expand_index_u = buf_expand_index[uid] ;

            //ptr_label_u.clear();
            //ptr_label_v.clear();
            

            //pruning strategy with expanded l label
                      
            for(size_t sz_v = 0; sz_v < buf_index_u.first.size(); ++sz_v) {
                int w = buf_index_u.first[sz_v];
                if(w < 0)
                {
                    continue;
                }
                if (dst_r[w] == -1 ||!vec_used_as_root[w] ) {
                    continue;
                }
                int td = dst_r[w] + buf_index_u.second[sz_v];
                if (td <= dst_v[uid]) {
                        
                    prune_flag = true;
                    goto out;
                }
            }

            //pruning strategy with expanded p label

            for(iter = map_expand_index_u.begin(); iter != map_expand_index_u.end(); ++iter)
            {
                int w = iter->first;
                if (dst_r[w] == -1 || !vec_used_as_root[w]) {
                        continue;
                }
                int td = dst_r[w] + iter->second;
                if (td <= dst_v[uid]) {
                    prune_flag = true;
                    goto out;
                }
            }
            
            
out:;

          if (prune_flag) {
                vec_last_level_vertices.pop_back();
                vec_last_level_vertices.push_back(pre_ver_buf[uid]);
                continue;
            }

            //add l label when current node and root node are not on the same path
            std::vector<int>& vec_path1 = map_vertice2paths[vertice];
            std::vector<int>& vec_path2 = map_vertice2paths[uid];
            size_t path1_len = vec_path1.size();
            size_t path2_len = vec_path2.size();
            std::vector<int> vec_intersect;
            //must allocate enough memory before set_intersection
            vec_intersect.reserve(std::max(path1_len, path2_len));
            std::sort(vec_path1.begin(), vec_path1.end());
            std::sort(vec_path2.begin(), vec_path2.end());
            
            
            if (uid != vertice) {
                std::set_intersection(vec_path1.begin(), vec_path1.end(),
                        vec_path2.begin(), vec_path2.end(), std::back_inserter(vec_intersect));
            }
            /*
            std::cout <<"Intersection Elems\n";
            for (size_t sz = 0; sz < vec_intersect.size();++sz)
                std::cout << vec_intersect.at(sz) << ' ';
            std::cout << std::endl;
            */
            
            if (vertice == uid || vec_intersect.empty()) {
                buf_index_u.first.push_back(vertice);
                buf_index_u.second.push_back(dst_v[uid]);
                set_addlabel.insert(uid);//标识uid新添了L label
            }else
            {
            	num_label_prune ++;
            }

            //continue traversing
            const std::vector<int> siblings = (*_ptr_ugraph)[uid];
            for (size_t sz = 0; sz < siblings.size(); ++sz) {
                int w = siblings.at(sz);
                if (vec_visited[w]) {
                    continue;
                }
                dst_v[w] = dst_v[uid] + 1;
                queue_vertices.push(w);
                vec_visited[w] = true;
                pre_ver_buf[w] = uid;
                ++next_level_num;
            }

            if (cnt == cur_level_num) {
                if (next_level_num > 0) {
                    vec_last_level_vertices.clear();
                }
                cur_level_num = next_level_num;
                cnt = 0;
                next_level_num = 0;
            }
        }
        //std::cout << std::endl;
        vec_used_as_root[vertice] = true;
        count_times = 0;
        int num_times = vec_last_level_vertices.size();        
        //int num_times = 0;

        while(count_times < num_times)//
        {
        //cout<<v<<"  vertice  "<<vertice<<" size "<<vec_last_level_vertices.size()<<endl;
            int moveitem = 0;
            int max_vertices_new = 0;
            unsigned int max_path_score = 0;

        //get the most valuable longest path
            int max_valuable_vertice = 0;
            if (prune_flag && vec_last_level_vertices.empty()) 
            {
                max_valuable_vertice = pre_ver_buf[uid];
            }
            else{
                max_valuable_vertice = vec_last_level_vertices.at(0);
                for (size_t sz = 0; sz < vec_last_level_vertices.size(); ++sz) {
                    int vertice_id = vec_last_level_vertices.at(sz);
                    int cur_vertices_new = 0;
                    unsigned int degree_score = 0;
                    int path_len = 0;
                    unsigned int path_score = 0; 
                    while (vertice_id != -1) {
                        if (vec_vertice_in_path[vertice_id] == false) {
                            ++cur_vertices_new;
                        }
                        degree_score += vec_degree_rank[vertice_id];
                        vertice_id = pre_ver_buf[vertice_id];
                        path_len++;
                    }
                    path_score = degree_score/path_len * cur_vertices_new;
                    if (max_path_score < path_score) {
                        max_path_score = path_score;
                        max_vertices_new = cur_vertices_new;
                        max_valuable_vertice = vec_last_level_vertices.at(sz);
                        moveitem = sz;
                    }
                    // if (max_vertices_new < cur_vertices_new) {
                    //     max_vertices_new = cur_vertices_new;
                    //     max_valuable_vertice = vec_last_level_vertices.at(sz);
                    //     moveitem = sz;
                    // }
                }
            }
            if (!vec_last_level_vertices.empty())
            {
                vec_last_level_vertices.erase(vec_last_level_vertices.begin() + moveitem);
            }
            get_longest_path(pre_ver_buf, num_vertices, max_valuable_vertice, &vec_longest_path);
            if((vec_longest_path.size()>3)&&(max_vertices_new >= vec_longest_path.size()*0.4))//
            {
                cout<<max_vertices_new<<"  "<< vec_longest_path.size()<<endl;
                for (size_t sz = 0; sz < vec_longest_path.size(); ++sz) {
                    //set_vertice_in_paths.insert(vec_longest_path.at(sz));
                     vec_vertice_in_path[vec_longest_path.at(sz)] = true;
                }
                vec_paths.push_back(vec_longest_path);

                std::vector<std::pair<int, int> > vec_expanded_label;
                for(int sz_v = 0;sz_v < vec_longest_path.size();++sz_v)
                {
                    int vertice_id = vec_longest_path.at(sz_v);
                    vec_expanded_label.clear();
                    expand_plabel(vertice_id, vec_longest_path, &vec_expanded_label);
                    for (int sz = 0; sz < vec_expanded_label.size(); ++ sz) {
                        buf_expand_index[vertice_id].insert(vec_expanded_label.at(sz));
                    }
                }   

                //replace the L label with P label
                int path_id = -(int)vec_paths.size();//the id assigned to a path is negative starting from -1 to -n
                for (size_t sz = 0; sz < vec_longest_path.size(); ++sz) {
                    int vertice_id = vec_longest_path.at(sz);
            //if no l label is added to the vertice, there's no need to replace l label into p label
                    if (set_addlabel.find(vertice_id) == set_addlabel.end()) {
                        continue;
                    }
                    map_vertice2paths[vertice_id].push_back(path_id);
                    std::pair<std::vector<int>, std::vector<int> >& buf_pair = buf_index[vertice_id];
                    size_t num = buf_pair.first.size();
                    if(buf_pair.first.at(num - 1)>=0)
                    {
                        buf_pair.first.at(num - 1) = path_id;
                        buf_pair.second.at(num - 1) = sz;
                    }else{
                        buf_index[vertice_id].first.push_back(path_id);
                        buf_index[vertice_id].second.push_back(sz);
                    }
            
                }
                count_times ++;
            }else {
            	count_times ++;
                //break;
            }
        }
    }
    std::cout<<"BFS ended . Begin to creat index structure......"<<std::endl;
    //create index structure
    _num_paths = vec_paths.size();
    _ptr_paths = (path_t*)calloc(_num_paths, sizeof(path_t));
    if (_ptr_paths == NULL) {
        free_mem();
        return false;
    }
    for (int i = 0; i < _num_paths; ++i) {
        //std::cout << "Path:" << i << std::endl;
        outfile << "Path:" << i << endl;

        const std::vector<int>& vec_path = vec_paths.at(i);
        size_t size = vec_path.size();
        _ptr_paths[i].ptr_vertices = (int*)calloc(size, sizeof(int));
        if (_ptr_paths[i].ptr_vertices == NULL) {
            free_mem();
            return false;
        }
        for (size_t sz = 0; sz < size; ++sz) {
            _ptr_paths[i].ptr_vertices[sz] = vec_path.at(sz);
            //std::cout << _ptr_paths[i].ptr_vertices[sz] << ' ';
            outfile << _ptr_paths[i].ptr_vertices[sz] << ' ';

        }
        //std::cout << std::endl;
       outfile<<endl;

        _ptr_paths[i].len = size;
    }
    int labeltotal = 0;
    for (size_t i = 0; i < vec_degree.size(); ++i) {
        int vertice = vec_degree.at(i);

        size_t size = buf_index[vertice].first.size();
        labeltotal += size;
        _ptr_index[vertice].ptr_lv = (int*)calloc(size, sizeof(int));
        _ptr_index[vertice].ptr_ld = (int*)calloc(size, sizeof(int));
        if (_ptr_index[vertice].ptr_lv == NULL
                || _ptr_index[vertice].ptr_ld == NULL) {
            free_mem();
            return false;
        }
        _ptr_index[vertice].size = size;

        //sort the labels(vertices+distances) in ascending order
        std::vector<std::pair<int, int> > vec_pair;
        for (size_t sz = 0; sz < size; ++sz) {
            vec_pair.push_back(std::pair<int,int>(buf_index[vertice].first[sz],sz));
        }
        std::sort(vec_pair.begin(), vec_pair.end(), std::less<std::pair<int, int> >());
        //std::cout << "Vertice:" << vertice <<" label size:"<< size <<std::endl;
        outfile<<"Vertice:" << vertice <<" label size:"<< size <<endl;
        for (size_t sz = 0; sz < size; ++sz) {
            _ptr_index[vertice].ptr_lv[sz] = buf_index[vertice].first[vec_pair[sz].second];
            _ptr_index[vertice].ptr_ld[sz] = buf_index[vertice].second[vec_pair[sz].second];
            //std::cout << '(' << _ptr_index[vertice].ptr_lv[sz] << ',' << _ptr_index[vertice].ptr_ld[sz] << ") ";
             outfile<< '(' << _ptr_index[vertice].ptr_lv[sz] << ',' << _ptr_index[vertice].ptr_ld[sz] << ") ";

        }
        outfile << endl;
        //free the memory of vector
        std::vector<int>().swap(buf_index[vertice].first);
        std::vector<int>().swap(buf_index[vertice].second);
    }
    cout << "Total label size:" << labeltotal <<endl;
    cout << "Average label size:" << labeltotal*1.0/num_vertices <<endl;
    cout << "Pruned label size:" << num_label_prune <<endl;

    outfile.close();
    return true;
}

long ShortestPathUtil::compute_index_mem() const {
    long mem_sum = 0;
    if (_ptr_index == NULL) {
        return mem_sum;
    }

    int num_vertices = _ptr_ugraph->get_vertices_num();
    for (int i = 0; i < num_vertices; ++i) {
        if (_ptr_index[i].ptr_lv == NULL || _ptr_index[i].ptr_ld == NULL) {
            continue;
        }
        mem_sum += 2 * (sizeof(int) * _ptr_index[i].size);
    }

    return mem_sum;
}

long ShortestPathUtil::compute_index_mem_ext() const {
    long mem_sum = 0;
    long path_label = 0;
    if (_ptr_index == NULL) {
        return mem_sum;
    }

    for (int i = 0; i < _num_paths; ++i) {

        mem_sum += (_ptr_paths[i].len * sizeof(uint8_t));
        mem_sum += sizeof(int);
        path_label += _ptr_paths[i].len;
    }
    int num_vertices = _ptr_ugraph->get_vertices_num();
    std::cout<<"path_memory: "<< mem_sum<<"  path label: "<<path_label<<"  Average path label"<<path_label*0.1/num_vertices<<std::endl;

    for (int i = 0; i < num_vertices; ++i) {
        if (_ptr_index[i].ptr_lv == NULL || _ptr_index[i].ptr_ld == NULL) {
            continue;
        }
        mem_sum += (sizeof(int)+sizeof(uint8_t)) * _ptr_index[i].size;
        mem_sum += sizeof(int);
    }

    return mem_sum;
}

int ShortestPathUtil::query_distance(int w, int v) const {
    int num_vertices = _ptr_ugraph->get_vertices_num();
    if (w >= num_vertices || v >= num_vertices) {
        return -1;
    }
    if (w == v) {
        return 0;
    }

    const index_t& index_t_w = _ptr_index[w];
    const index_t& index_t_v = _ptr_index[v];
    int distance = INT_MAX;
    int i = 0;
    int j = 0;
    while (i < index_t_w.size && j < index_t_v.size) {
        int v1 = index_t_w.ptr_lv[i];
        int v2 = index_t_v.ptr_lv[j];
        if (v1 == v2) {
            int td = index_t_w.ptr_ld[i] + index_t_v.ptr_ld[j];
            distance = (td < distance)? td: distance;
            ++i;
            ++j;
        } else {
            i += (v1 < v2)? 1 : 0;
            j += (v1 < v2)? 0 : 1;
        }
    }

    return (distance == INT_MAX)? -1 : distance;
}

int ShortestPathUtil::query_distance_ext(int w, int v) const {
    if (w == v) {
        return 0;
    }

    int num_vertices = _ptr_ugraph->get_vertices_num();
    if (w >= num_vertices || v >= num_vertices) {
        return -1;
    }

    const index_t& index_t_w = _ptr_index[w];
    const index_t& index_t_v = _ptr_index[v];
    int distance = INT_MAX;

size_t size = index_t_w.size;
/*
    std::cout << "Vertice:" << w <<" label size:"<< size <<std::endl;
    for (size_t sz = 0; sz < size; ++sz) {
        std::cout << '(' << _ptr_index[w].ptr_lv[sz] << ',' << _ptr_index[w].ptr_ld[sz] << ") ";
    }
std::cout << std::endl;

 size = index_t_v.size;

    std::cout << "Vertice:" << v <<" label size:"<< size <<std::endl;
    for (size_t sz = 0; sz < size; ++sz) {
        std::cout << '(' << _ptr_index[v].ptr_lv[sz] << ',' << _ptr_index[v].ptr_ld[sz] << ") ";
    }
    std::cout << std::endl;
    */

    //query based on p label
    int i = 0;
    int j = 0;
    int w_p_end = 0;//position of the last p label
    int v_p_end = 0;
    while (i < index_t_w.size && j < index_t_v.size) {
        int v1 = index_t_w.ptr_lv[i];
        int v2 = index_t_v.ptr_lv[j];
        if (v1 < 0) {
            w_p_end = i;
        }
        if (v2 < 0) {
            v_p_end = j;
        }

        if (v1 == v2) {
            if (v1 < 0) {//p label
            //std::cout << "P query:" << v1 <<": "<< index_t_w.ptr_ld[i] <<" "<< v2 << ": "<<index_t_v.ptr_ld[j]<<std::endl;

                return abs(index_t_w.ptr_ld[i] - index_t_v.ptr_ld[j]);
            } else {//l label
                int td = index_t_w.ptr_ld[i] + index_t_v.ptr_ld[j];
                /*
                if(td<distance)
                {
                    std::cout << "l query:" << v1 <<": "<< index_t_w.ptr_ld[i] <<" "<< v2 << ": "<<index_t_v.ptr_ld[j]<<std::endl;

                }*/
                distance = (td < distance)? td: distance;
            }
            ++i;
            ++j;
        } else {
            i += (v1 < v2)? 1 : 0;
            j += (v1 < v2)? 0 : 1;
        }
    }

    //expand p labels into l labels
    std::vector<std::pair<int, int> > vec_w_labels;
    std::vector<std::pair<int, int> > vec_v_labels;
    expand_plabels(w, index_t_w.ptr_lv, w_p_end +1 ,&vec_w_labels);
    expand_plabels(v, index_t_v.ptr_lv, v_p_end +1 ,&vec_v_labels);
   /*
     for(int i =0;i<vec_w_labels.size();i++){
        std::cout<<w<<":"<<vec_w_labels[i].first<<":"<<vec_w_labels[i].second<<"\n";
        } 
        for(int i =0;i<vec_v_labels.size();i++){
            std::cout<<v<<":"<<vec_v_labels[i].first<<":"<<vec_v_labels[i].second<<"\n";
            } */
    //sort the expanded l labels according to the vertice id
    std::vector<std::pair<int, int> > vec_w_pair;
    std::vector<std::pair<int, int> > vec_v_pair;
    for (size_t sz = 0; sz < vec_w_labels.size(); ++sz) {
        vec_w_pair.push_back(std::pair<int,int>(vec_w_labels[sz].first, sz));
    }
    for (size_t sz = 0; sz < vec_v_labels.size(); ++sz) {
        vec_v_pair.push_back(std::pair<int,int>(vec_v_labels[sz].first, sz));
    }
    std::sort(vec_w_pair.begin(), vec_w_pair.end(), std::less<std::pair<int, int> >());
    std::sort(vec_v_pair.begin(), vec_v_pair.end(), std::less<std::pair<int, int> >());

    for (int i = 0, j = 0; i < (int)vec_w_labels.size() && j < (int)vec_v_labels.size(); ) {
        int v1_idx = vec_w_pair.at(i).second;
        int v2_idx = vec_v_pair.at(j).second;
        int v1 = vec_w_labels.at(v1_idx).first;
        int v2 = vec_v_labels.at(v2_idx).first;
        if (v1 == v2) {
            int td = vec_w_labels.at(v1_idx).second + vec_v_labels.at(v2_idx).second;
            
            distance = (distance > td)? td : distance;
            ++i;
            ++j;
        } else {
            i += (v1 < v2)? 1 : 0;
            j += (v1 < v2)? 0 : 1;
        }
    }
    for (int i = 0, j = v_p_end ; i < (int)vec_w_labels.size() && j < index_t_v.size; ) {
        int v1_idx = vec_w_pair.at(i).second;
        int v1 = vec_w_labels.at(v1_idx).first;
        int v2 = index_t_v.ptr_lv[j];
        if (v1 == v2) {
            int td = vec_w_labels.at(v1_idx).second + index_t_v.ptr_ld[j];
            
            distance = (distance > td)? td : distance;
            
            ++i;
            ++j;
        } else {
            i += (v1 < v2)? 1 : 0;
            j += (v1 < v2)? 0 : 1;
        }
    }
    for (int i = w_p_end , j = 0; i < index_t_w.size && j < (int)vec_v_labels.size(); ) {
        int v1 = index_t_w.ptr_lv[i];
        int v2_idx = vec_v_pair.at(j).second;
        int v2 = vec_v_labels.at(v2_idx).first;
        if (v1 == v2) {
            int td = vec_v_labels.at(v2_idx).second + index_t_w.ptr_ld[i];
            distance = (distance > td)? td : distance;
            
            ++i;
            ++j;
        } else {
            i += (v1 < v2)? 1 : 0;
            j += (v1 < v2)? 0 : 1;
        }
    }

    return (distance == INT_MAX)? -1 : distance;
}

void ShortestPathUtil::free_mem() {
    if (_ptr_index == NULL) {
        return;
    }

    int num_vertices = _ptr_ugraph->get_vertices_num();
    for (int i = 0; i < num_vertices; ++i) {
        if (_ptr_index[i].ptr_lv != NULL) {
            free(_ptr_index[i].ptr_lv);
            _ptr_index[i].ptr_lv = NULL;
        }
        if (_ptr_index[i].ptr_ld != NULL) {
            free(_ptr_index[i].ptr_ld);
            _ptr_index[i].ptr_ld = NULL;
        }
    }
    free(_ptr_index);
    _ptr_index = NULL;
}

void ShortestPathUtil::expand_plabel(int vertice_id,
        const std::vector<int>& vec_path,
        std::vector<std::pair<int, int> >* ptr_label) const {
    if (ptr_label == NULL) {
        return;
    }
    int pos = -1;
    ptr_label->clear();
    std::vector<int>::const_iterator iter = std::find(vec_path.begin(), vec_path.end(), vertice_id);
    if (iter == vec_path.end()) {
        return;
    }else
    {
       pos = iter - vec_path.begin(); 
    }    
    
    for (int sz = 0; sz < vec_path.size(); ++sz) {
        ptr_label->push_back(std::pair<int, int>(vec_path.at(sz), abs(pos - sz)));
    }
}

void ShortestPathUtil::expand_plabel(int vertice_id,
        const path_t& path,
        std::vector<std::pair<int, int> >* ptr_label) const {
    if (ptr_label == NULL) {
        return;
    }

    ptr_label->clear();
    int pos = -1;
    for (int i = 0; i < path.len; ++i) {
        if (*(path.ptr_vertices + i) == vertice_id) {
            pos = i;
            break;
        }
    }
    if (pos == -1) { //the vertice is not on the path
        return;
    }
    for (int i = 0; i < path.len; ++i) {
        int dist = abs(pos - i);
        ptr_label->push_back(std::pair<int, int>(*(path.ptr_vertices + i), dist));
    }
}

void ShortestPathUtil::expand_plabels(int vertice_id,
        const int* ptr_path_ids,
        int num_paths,
        std::vector<std::pair<int, int> >*ptr_vec) const {
    if (ptr_path_ids == NULL || ptr_vec == NULL) {
        return;
    }
    ptr_vec->clear();

    std::vector<std::pair<int, int> > vec;
    for (int i = 0; i < num_paths; ++i) {
        if (ptr_path_ids[i] >= 0) {
            continue;
        }
        int path_id = abs(ptr_path_ids[i]) - 1;
        expand_plabel(vertice_id, _ptr_paths[path_id], &vec);
        for (size_t sz = 0; sz < vec.size(); ++sz) {
            //std::cout<<vertice<<":"<<ptr_label_v[i].first<<":"<<ptr_label_v[i].second<<"\n";

            ptr_vec->push_back(vec.at(sz));
        }
    }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
