#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>
#include <Eigen/Dense>
#include "NeighborList.h"
// #include "mpi.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace Eigen;
using namespace std;

int main(int argc, char* argv[])
{
    int npart;
    double** pos;
    double *tmp_pos;
    MatrixX3d ppos;

    cout.precision(numeric_limits<double>::max_digits10);
    cout << "Trying to open file " << argv[1] << " ..." << endl;
    ifstream file(argv[1], ios::binary);

    if (file.is_open())
    {   
        // Read number of particles
        file.read((char*) &npart, sizeof(int));
        cout << "Trying to read " << npart << " positions from file..." << endl;
        
        // Allocate memory for temporary array
        tmp_pos = new double[npart*3];

        // Read all particle positions into array
        file.read((char*) tmp_pos, npart*sizeof(double[3]));
        ppos = Map<MatrixX3d>(tmp_pos, npart, 3);

        delete [] tmp_pos;

        file.close();
        
        // Create neighborlist
        double dsize[3] = {1.0, 1.0, 1.0};
        int ncell[3] = {100,100,100};
        NeighborList nblist(npart, ppos, dsize, ncell, 0, npart);

        auto t1 = Clock::now();
        nblist.createCellList();
        nblist.createPartnerList();
        auto t2 = Clock::now();
        std::cout << "Delta t2-t1: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms" << std::endl;
    }
    else cout << "Error opening file " << argv[1] << ".\n\n" << endl;

    cout << "Exiting normally..." << endl;
}