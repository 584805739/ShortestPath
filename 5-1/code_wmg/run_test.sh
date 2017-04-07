#!/bin/bash

if [ $# -ne 2 ];then
	echo "Usage: $0 graph_data num_test" 
	echo "For example: $0 ./data/graph.dat 1000"
	exit 1
fi

graph_data="$1"
if [ ! -f "${graph_data}" ];then
	echo "${graph_data} does not exist!"
	exit 1
fi

v_max=0
while read v1 v2;do
	if [ ${v1} -gt ${v_max} ];then
		v_max=${v1}
	fi
	if [ ${v2} -gt ${v_max} ];then
		v_max=${v2}
	fi
done <${graph_data}
vertices=$((${v_max}+10))

benchmark_dir="../pruned-landmark-labeling-master"
if [ ! -d "${benchmark_dir}" ];then
	echo "${benchmark_dir} does not exist"
	exit 1
fi

benchmark_exe="${benchmark_dir}/bin/query_distance"
if [ ! -f "${benchmark_exe}" ];then
	cd "${benchmark_dir}" && make clean && make && cd -
fi

current_exe="./bin/query_distance"
if [ ! -f "${current_exe}" ];then
	 make clean && make
fi

rm -rf index_file
./${benchmark_dir}/bin/construct_index "${graph_data}" index_file &>/dev/null

num_test=$2
for idx in $(seq ${num_test});do
	v1=$((${RANDOM} % ${vertices}))
	v2=$((${RANDOM} % ${vertices}))
	benchmark_res=$(${benchmark_exe} index_file <<< "${v1} ${v2}")
	current_res=$(${current_exe} ${graph_data} ${v1} ${v2}|tail -5 |head -1|sed -e "s/.*=//g")
	echo "${benchmark_res} ${current_res} (${v1},${v2})"
	if [ ${current_res} -ne -1 ] && [ ${benchmark_res} -ne ${current_res} ] ;then
		echo "Query Distance Error for vertices:(${v1},${v2})"
		exit 1
	fi
done
