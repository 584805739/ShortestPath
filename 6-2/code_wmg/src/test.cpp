 
/**
 * @file compare.cpp
 * @author wang
 * @date 2016/03/03 23:29:16
 * @brief 
 *  
 **/

#include <stdlib.h>
#include <iostream>
#include <fstream>


int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage:" << argv[0] << " vertice1 vertice2" << std::endl;
        return 1;
    }
 	int buf[1400000];

    const char* filename = argv[1];
    std::ifstream ifs(filename);
    int vertice0;
    int vertice1;
    for(int i = 0 ; i<1400000; i++)
    {
        buf[i] = i;
    }

    if (ifs.is_open()) {
        while (ifs >> vertice0 >> vertice1) {
            while(vertice0!=buf[vertice0])
            {
            	vertice0 = buf[vertice0];
            }
            while(vertice1 != buf[vertice1])
            {
            	vertice1 = buf[vertice1];
            }
        	buf[vertice0] = vertice0 > vertice1 ? vertice0 : vertice1;
            buf[vertice1] = vertice0 > vertice1 ? vertice0 : vertice1;
            }
    }
    vertice0 = atoi(argv[2]);
    vertice1 = atoi(argv[3]);

    while(vertice0!=buf[vertice0])
    {
    	vertice0 = buf[vertice0];
    }
    while(vertice1 != buf[vertice1])
    {
    	vertice1 = buf[vertice1];
    }
    std::cout<<"vertice0: "<< vertice0<<" "<<"vertice1: "<<vertice1<<std::endl;

    return 0;
    
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
