  
/**
 * @file shortest_path.h
 * @author wang
 * @date 2016/02/18 23:10:25
 * @brief 
 *  
 **/

#ifndef  __SHORTEST_PATH_H_
#define  __SHORTEST_PATH_H_

#include "undirected_graph.h"

// storing pairs of (vertice,dist) for each vertice
struct index_t {
    index_t():ptr_lv(NULL),ptr_ld(NULL),size(0){}
    void reset() {
        ptr_lv = NULL;
        ptr_ld = NULL;
        size = 0;
    }

    int* ptr_lv;//pointer to the label storing index of vertices
    int* ptr_ld;//pointer to the label storing distances
    int size;//number of vertices
};

struct path_t {
    path_t():ptr_vertices(NULL), len(0) {}
    void reset() {
        ptr_vertices = NULL;
        len = 0;
    }

    int* ptr_vertices;//pointer to the first vertice of the path
    int len;//number of vertices on the path
};


class ShortestPathUtil {
    public:
        explicit ShortestPathUtil() : 
            _ptr_ugraph(NULL), 
            _ptr_index(NULL),
            _ptr_paths(NULL),
            _num_paths(0) {};
        explicit ShortestPathUtil(UndirectedGraph* ptr_ugraph);
        ~ShortestPathUtil();

        /**
         * @brief Reset the undirected-graph to be analysed
         *
         * @param [in] ptr_ugraph   : UndirectedGraph*
         * @return  void 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/20 00:11:18
        **/
        void reset_graph(UndirectedGraph* ptr_ugraph);

        /**
         * @brief Construct index structure for answering exact shortest distance queries
         *
         * @return  bool 
         * @retval  true:succ false:fail 
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/20 00:13:04
        **/
        bool construct_index();

        bool construct_index_ext();

        /**
         * @brief Compute the memory consumed by the index structure
         *
         * @return  int 
         * @retval  memory comsumend by index structue in bytes 
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/20 00:14:05
        **/
        long compute_index_mem() const;
        long compute_index_mem_ext() const;

        /**
         * @brief get the shortest distance between two vertices
         *
         * @param [in] w   : int
         * @param [in] v   : int
         * @return  int 
         * @retval  -1: not reachable non-negative: shortest distance 
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/20 00:14:48
        **/
        int query_distance(int w, int v) const;
        int query_distance_ext(int w, int v) const;

    private:
        /**< Disallow copy and assignment       */
        ShortestPathUtil(const ShortestPathUtil& other);
        ShortestPathUtil& operator= (const ShortestPathUtil& other);

        /**
         * @brief get the shortest distance between two vertices
         *
         * @param [in] vertice_id   : int
         * @param [in] vec_path   : const std::vector<int>& 
         * @param [out] ptr_label   : std::vector<std::pair<int, int> >* 
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/28 00:14:48
        **/
        void expand_plabel(int vertice_id,
                const std::vector<int>& vec_path,
                std::vector<std::pair<int, int> >*ptr_label) const;
        void expand_plabel(int vertice_id,
                const path_t& path,
                std::vector<std::pair<int, int> >* ptr_label) const;
        void expand_plabels(int vertice_id,
                const int* ptr_path_ids,
                int num_paths,
                std::vector<std::pair<int, int> >* ptr_label) const;
        /**
         * @brief Free the memory for the index structure
         *
         * @return  void 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/20 00:16:21
        **/
        void free_mem();

    private:
        UndirectedGraph* _ptr_ugraph;
        index_t* _ptr_index;
        path_t* _ptr_paths;
        int _num_paths;//number of paths saved
};

#endif  //__SHORTEST_PATH_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
