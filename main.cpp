#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>
#include "NeighborList.h"
// #include "mpi.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace std;

int main(int argc, char* argv[])
{
    int npart;
    double** pos;

    // // Initialise MPI
    // int world_size, world_rank;
    // MPI_Init(&argc, &argv);
    // MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    cout.precision(numeric_limits<double>::max_digits10);
    cout << "Trying to open file " << argv[1] << " ..." << endl;
    ifstream file(argv[1], ios::binary);

    if (file.is_open())
    {   
        // Read number of particles
        file.read((char*) &npart, sizeof(int));
        cout << "Trying to read " << npart << " positions from file..." << endl;
        
        // Allocate memory
        pos = new double*[npart];
        for (int i = 0; i < npart; i++)
           pos[i] = new double[3];
        
        // Read all particle positions into array
        for (int i = 0; i < npart; i++) {
            file.read((char*) pos[i], sizeof(double[3]));
        }
        file.close();

        // Print array when only few particles are used
        if (npart < 100)
            for (unsigned int i = 0; i < npart; i++)
                cout << "Found position: " << pos[i][0] << ", " << pos[i][1] << ", " << pos[i][2] << endl;

        // getchar();

        // Create neighborlist
        double dsize[3] = {1.0, 1.0, 1.0};
        int ncell[3] = {100,100,100};
        NeighborList nblist(npart, pos, dsize, ncell, 0, npart);

        auto t1 = Clock::now();
        nblist.createCellList();
        nblist.createPartnerList();
        auto t2 = Clock::now();
        std::cout << "Delta t2-t1: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms" << std::endl;
        
        // Cleanup
        for (int i = 0; i < npart; i++) delete [] pos[i];
        delete pos;
    }
    else cout << "Error opening file " << argv[1] << ".\n\n" << endl;

    cout << "Exiting normally..." << endl;
}