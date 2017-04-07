GXX = g++

INCLUDES = -I./include \
		   -I../pruned-landmark-labeling-master/src

CXXFLAGS = -O3 -g -Wall $(INCLUDES)

LDFLAGS = -L.

src = $(wildcard ./src/*.cpp)
obj = $(src:%.cpp=%.o)

%.o : %.cpp
	$(GXX) -c -o $@ $^ $(CXXFLAGS)

LIB = libshortest_path_alg.a
RAND_GRAPH_EXE = bin/rand_graph
MAKE_GRAPH_EXE = bin/make_graph
QUERY_DIST_EXE = bin/query_distance
COMPARE = bin/compare

default : all
	mkdir -p output
	mkdir -p output/lib
	mkdir -p output/include
	mkdir -p output/bin
	cp -rf $(LIB) output/lib
	cp include/*.h output/include
	cp $(MAKE_GRAPH_EXE) output/bin

all : $(LIB) $(RAND_GRAPH_EXE) $(MAKE_GRAPH_EXE) $(QUERY_DIST_EXE) $(COMPARE)

$(RAND_GRAPH_EXE) : sample/rand_graph.o
	$(GXX) -o $@ $^ $(CXXFLAGS) -L. -lshortest_path_alg

$(MAKE_GRAPH_EXE) : sample/make_graph.o
	$(GXX) -o $@ $^ $(CXXFLAGS) -L. -lshortest_path_alg

$(QUERY_DIST_EXE) : sample/query_distance.o
	$(GXX) -o $@ $^ $(CXXFLAGS) -L. -lshortest_path_alg

$(COMPARE) : sample/compare.o
	$(GXX) -o $@ $^ $(CXXFLAGS) -L. -lshortest_path_alg

$(LIB) : $(obj)
	ar rcv $@ $^

clean:
	rm -rf bin/*
	rm -rf src/*.o
	rm -rf sample/*.o
	rm -rf output
	rm -rf $(LIB)
