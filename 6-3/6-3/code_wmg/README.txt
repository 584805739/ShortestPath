1.目录说明
	include/ 包含项目相关的头文件
	src/ 包含项目相关的实现文件
	sample/ 生成相应的可执行文件的.cpp文件
	data/ 包含测试程序所需的测试数据
	bin/ 可执行文件所在的目录
	output/ 该项目的产出,包括头文件和静态链接库

2.编译运行方法
	进入工程所在目录执行make clean && make

3.生成的可执行文件说明
	3.1 bin/rand_graph 生成随机的图,输出为多行边上的节点对
		使用说明 ./bin/rand_graph 30 10 表示生成10条边,所有的节点编号不大于30
	3.2 bin/make_graph 根据节点对生成无向图,输出邻接矩阵和节点按度排序的结果
		使用说明 ./bin/make_graph data/graph.dat 表示根据graph.dat中的节点的构建无向图
	3.3 bin/query_distance 计算两个节点间的最短距离,并统计构建无向图r/构建索引/索引大小等信息
		使用说明 ./bin/query_distance data/graph.dat 17 11 表示计算节点17和11间的最短距离(-1表示节点不连通)
	3.4 bin/compare 比较两个算法的统计xinx
		使用说明 ./bin/compare data/graph5k.dat 500 100000 表示测试100000组测试用例,最大的节点编号为500
