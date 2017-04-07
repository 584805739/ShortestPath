 
/**
 * @file undirected_graph.h
 * @author wang
 * @date 2016/02/18 23:19:36
 * @brief 
 *  
 **/

#ifndef  __UNDIRECTED_GRAPH_H_
#define  __UNDIRECTED_GRAPH_H_

#include <iostream>
#include "utils.h"

class UndirectedGraph {
    public:
        explicit UndirectedGraph();
        explicit UndirectedGraph(const std::vector<std::pair<int, int> >& vec_edges);
        explicit UndirectedGraph(const char* filename);

        virtual ~UndirectedGraph();

        /**
         * @brief get the number of vertices of the graph
         *
         * @return  int 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 10:55:55
        **/
        int get_vertices_num() const {
            return _num_vertices;
        }

        /**
         * @brief get the number of edges of the graph
         *
         * @return  int 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 10:56:17
        **/
        int get_edges_num() const {
            return _num_edges;
        }

        /**
         * @brief get the the adjacent vertices for the i-th vertice
         *
         * @param [in] vertice   : int
         * @return  const std::vector<int>&
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 10:56:33
        **/
        const std::vector<int>& operator[] (int vertice);

        /**
         * @brief sort the vertices in decreasing order of degree
         *
         * @param [out] vec_deg   : std::vector<int>*
         * @return  bool
         * @retval  true:succ false:fail
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 10:57:20
        **/
        bool sort_vertices_by_degree(std::vector<int>* vec_deg);

        /**
         * @brief Output the adjacent table for the graph
         *
         * @param [in] os   : std::ostream&
         * @return  void 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 10:59:02
        **/
        void print(std::ostream& os) const;

    private:
        /**< Disallow copy and assignment       */
        UndirectedGraph(const UndirectedGraph& other);
        UndirectedGraph& operator= (const UndirectedGraph& other);

        /**
         * @brief Sort the adjacent vertices in ascending order
         *
         * @return  void 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 11:00:46
        **/
        void sort_vertices_in_order();

        /**
         * @brief Fill the adjacent table for the graph
         *
         * @param [in] std::vector<std::pair<int, int> >&   : const
         * @return  void 
         * @retval   
         * @see 
         * @note 
         * @author wang
         * @date 2016/02/19 11:02:36
        **/
        void construct_graph(const std::vector<std::pair<int, int> >& vec_pairs);

    private:
        int _num_vertices;//number of vertices
        int _num_edges;//number of edges
        std::vector<int>* _pp_buf;//pointer to the adjacent table    
};

#endif  //__UNDIRECTED_GRAPH_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
