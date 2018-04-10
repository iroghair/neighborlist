#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

// Class to represent points.
class NeighborList {
private:
        int np, pmin, pmax;
        MatrixX3d pos;
        vector<int> ***cell_list;
        vector<int> *partner_list;

        Array3d domainSize, dCells;
        Array3i nCells;
public:
        // Constructor
        NeighborList(const int npart, const MatrixX3d& pp, double* domain_size, int* number_of_cells, const int min_p, const int max_p) {
            nCells = Map<Array3i>(number_of_cells);
            domainSize = Map<Array3d>(domain_size); 
            dCells = domainSize / nCells.cast<double> ();
            np = npart;
            pos = pp;
            pmin = min_p;
            pmax = max_p;

            // Initialize the cell_list
            cell_list = new vector<int>**[nCells[0]];
            for (uint i = 0; i < nCells[0]; i++) {
                cell_list[i] = new vector<int>*[nCells[1]];
                for (uint j = 0; j < nCells[1]; j++) {
                    cell_list[i][j] = new vector<int>[nCells[2]];
                }
            }
            partner_list = new vector<int>[np];
        }

        ~NeighborList() {
            cout << "Clearing NeighborList\n\n";

            clearPartnerList();
            clearCellList();

            for (uint i = 0; i < nCells[0]; ++i) {
                for (uint j = 0; j < nCells[1]; ++j)
                    delete [] cell_list[i][j];

                delete [] cell_list[i];
            }
            delete [] cell_list;
            delete [] partner_list;

            cout << "Done clearing..." << endl;
        }

        int getNumberOfParticles() { return np; }

        // Setters
        // void setParticles(int npart, double **pp) { np = npart; pos = pp; }

        void createCellList() {
            Array3i cell;
            #pragma omp parallel for private(cell)
            for (unsigned int i = pmin; i < pmax; i++) {
                getCurrentCell(i,cell);
                #pragma omp critical
                cell_list[cell[0]][cell[1]][cell[2]].push_back(i); 
            }
        }

        void clearCellList() {
            for (uint i = 0; i < nCells[0]; ++i)
                for (uint j = 0; j < nCells[1]; ++j)
                    for ( uint k = 0; k < nCells[2]; k++)
                        cell_list[i][j][k].clear();
        }

        void clearPartnerList() {
            for (uint p = pmin; p < pmax; p++)
                partner_list[p].clear();
        }

        void printCellList() {
            vector<int> myVec;
             for (uint i = 0; i < nCells[0]; ++i) {
                for (uint j = 0; j < nCells[1]; ++j) {
                     for (uint k = 0; k < nCells[2]; ++k) {
                        myVec = cell_list[i][j][k];
                        if (myVec.size() > 0) {
                            cout << "Cell [" << i << "][" << j << "][" << k << "] = {";
                            for (vector<int>::const_iterator i = myVec.begin(); 
                            i != myVec.end(); 
                            ++i)
                            std::cout << *i << ", ";
                        cout << "}" << endl;
                        }
                    }
                }
            }
        }

        void createPartnerList() {
            Array3i lo, hi;
            unsigned int cx, cy, cz, i;
            vector<int>::const_iterator it;

            #pragma omp parallel for private(lo,hi,cx,cy,cz,it)
            for ( i = pmin; i < pmax; i++) {
                getNeighborCells(i,lo,hi);

                // Loop over all nearest neighboring cells
                for ( cx = lo[0]; cx <= hi[0]; cx++) {
                    for ( cy = lo[1]; cy <= hi[1]; cy++) {
                        for ( cz = lo[2]; cz <= hi[2]; cz++) {
                            for (   it = cell_list[cx][cy][cz].begin(); 
                                    it != cell_list[cx][cy][cz].end(); 
                                    ++it) {
                                    partner_list[i].push_back(*it);
                            }
                        }
                    }
                }
            }
        }

        void printPartnerList() {
            cout << "Printing partner list..." << endl << endl;
            for (unsigned int p = pmin; p < pmax; p++) {
                cout << "Particle " << p << " = { ";
                for (vector<int>::const_iterator i = partner_list[p].begin(); i != partner_list[p].end(); ++i)
                    cout << *i << ", ";
                cout << " }" << endl;
            }
        }

        void getNeighborCells(const unsigned int idx, Array3i& lo, Array3i& hi)
        {
            static const Array3i myZero = Array3i::Zero(3);
            static const Array3i nCells_sub_1 = nCells - Array3i::Ones(3);

            lo = myZero.max( floor(Array3d(pos.row(idx))/dCells - 0.5).cast <int> ());
            hi = nCells_sub_1.min( floor( Array3d(pos.row(idx))/dCells + 0.5).cast <int> () );
        }

        void getCurrentCell(const unsigned int idx, Array3i& cell) {
            cell = floor(Array3d(pos.row(idx))/dCells.array()).cast <int> ();
        }
};