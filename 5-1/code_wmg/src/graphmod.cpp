#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <sys/time.h>
#include <set>
using namespace std;
int main(int argc, char *argv[]) {
	//const char *filename = "data/graph.dat";
	const char* filename = argv[1];
	int flag = 0;
	ifstream ifs(filename);
	std::ofstream outfile;
	
	int vertice0,vertice1;
	std::vector<std::pair<int, int> > vec_edges;
	std::vector<std::pair<int, int> > vec_edges_norepeat;

	if(ifs.is_open())
	{
		while(ifs >> vertice0 >> vertice1)
		{
			vec_edges.push_back(std::pair<int, int>(vertice0, vertice1));
			//std::cout<< vertice0 << "\t" << vertice1 <<std::endl;

		}
	}
	outfile.open("graph_out.dat");
	for(int i = 0;i < vec_edges.size();i++)
	{
		flag = 0;
		for(int j = i-1;j >= 0;j--)
		{
			if(vec_edges[i].first == vec_edges[j].second && 
				vec_edges[i].second == vec_edges[j].first)
			{
				//std::cout<<i<<" "<<vec_edges[i].first<<" "<<vec_edges[i].second<<" ";
				//std::cout<<j<<" "<<vec_edges[j].first<<" "<<vec_edges[j].second<<std::endl;
				flag = 1;
			}
		}
			if(!flag)
			{
				vec_edges_norepeat.push_back(std::pair<int, int>(vec_edges[i].first, vec_edges[i].second));
				//vertice0 = vec_edges[i].first;
				//vertice1 = vec_edges[i].second;
				//std::cout<<"记录"<<" "<<vec_edges[i].first<<" "<<vec_edges[i].second<<std::endl;

				outfile<< vertice0 << "\t" << vertice1 <<std::endl;
			}
		
	}
	return true;
}