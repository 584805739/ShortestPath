/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file utils.h
 * @author wangyunfei01(com@baidu.com)
 * @date 2016/02/18 19:26:03
 * @brief 
 *  
 **/

#ifndef  __UTILS_H_
#define  __UTILS_H_

#include <iostream>
#include <vector>

/**
 * @brief get current time represented by mseconds
 *
 * @return  double 
 * @retval   
 * @see 
 * @note 
 * @author wangyunfei01
 * @date 2016/02/18 19:37:27
**/
double get_current_time_in_seconds();

/**
 * @brief randomly generate at most num_edges edges for a graph
 *
 * @param [in] num_ver   : int, number of vertices
 * @param [in] num_edges   : int, number of edges
 * @param [out] ptr_vec   : std::vector<std::pair<int, int> >*, pointer to the vector storing edges
 * @return  void 
 * @retval   
 * @see 
 * @note 
 * @author wangyunfei01
 * @date 2016/02/18 19:42:16
**/
void generate_random_graph(int num_ver, int num_edges, std::vector<std::pair<int, int> >* ptr_vec);

/**
 * @brief load graph from file in which each line is composed by two vertices of an edge
 *
 * @param [in] file_name   : const char*
 * @param [out] ptr_vec   : std::vector<std::pair<int, int> >*, pointer to the vector storing edges
 * @return  bool 
 * @retval  true:succ false:fail 
 * @see 
 * @note 
 * @author wangyunfei01
 * @date 2016/02/18 19:38:10
**/
bool load_graph(const char* file_name, std::vector<std::pair<int, int> >* ptr_vec);

#endif  //__UTILS_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
