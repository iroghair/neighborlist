all:
	g++ -o neighborlist -O3 -fopenmp -std=c++14 main.cpp NeighborList.cpp -I /usr/include/eigen3/ -DEIGEN_DONT_PARALLELIZE

debug:
	g++ -o neighborlist -O0 -g3 -fmax-errors=1  -std=c++14 main.cpp NeighborList.cpp -I /usr/include/eigen3/
test:
	g++ test.cpp -o test -I /usr/include/eigen3/
	