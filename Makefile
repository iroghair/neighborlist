all:
	g++ -o neighborlist -O3 -fopenmp -std=c++14 main.cpp NeighborList.cpp
