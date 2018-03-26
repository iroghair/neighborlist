#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <boost/array.hpp>

using namespace std;

// Class to represent points.
class NeighborList {
private:
        int np,pmin,pmax;
        double **pos;
        int NX, NY, NZ;
        double XMAX, YMAX, ZMAX, DX, DY, DZ;
        vector<int> ***cell_list;
        vector<int> *partner_list;
public:
        // Constructor
        NeighborList(int npart, double **pp, double* domain_size, int* number_of_cells, int min_p, int max_p) {
            NX = number_of_cells[0];
            NY = number_of_cells[1];
            NZ = number_of_cells[2];
            XMAX = domain_size[0];
            YMAX = domain_size[1];
            ZMAX = domain_size[2];
            DX = XMAX/NX;
            DY = YMAX/NY;
            DZ = ZMAX/NZ;
            np = npart;
            pos = pp;
            pmin = min_p;
            pmax = max_p;

            // Initialize the cell_list
            cell_list = new vector<int>**[NX];
            for (uint i = 0; i < NX; i++) {
                cell_list[i] = new vector<int>*[NY];
                for (uint j = 0; j < NY; j++) {
                    cell_list[i][j] = new vector<int>[NZ];
                }
            }
            partner_list = new vector<int>[np];
        }

        ~NeighborList() {
            cout << "Clearing NeighborList\n\n";

            clearPartnerList();
            clearCellList();

            for (uint i = 0; i < NX; ++i) {
                for (uint j = 0; j < NY; ++j)
                    delete [] cell_list[i][j];

                delete [] cell_list[i];
            }
            delete [] cell_list;
            delete [] partner_list;

            cout << "Done clearing..." << endl;
        }

        int Numpart() { return np; }

        // Setters
        void setParticles(int npart, double **pp) { np = npart; pos = pp; }

        void createCellList() {
            int cell[3];
            for (unsigned int i = pmin; i < pmax; i++) {
                getCurrentCell(i,cell);
                cell_list[cell[0]][cell[1]][cell[2]].push_back(i); 
            }
        }

        void clearCellList() {
            for (uint i = 0; i < NX; ++i)
                for (uint j = 0; j < NY; ++j)
                    for ( uint k = 0; k < NZ; k++)
                        cell_list[i][j][k].clear();
        }

        void clearPartnerList() {
            for (uint p = pmin; p < pmax; p++)
                partner_list[p].clear();
        }

        void printCellList() {
            vector<int> myVec;
             for (uint i = 0; i < NX; ++i) {
                for (uint j = 0; j < NY; ++j) {
                     for (uint k = 0; k < NZ; ++k) {
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
            unsigned int lo[3], hi[3];
            unsigned int cx, cy, cz, i;
            vector<int>::const_iterator it;

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

        void getNeighborCells(unsigned int idx, unsigned int lo[3], unsigned int hi[3])
        {
            // for (uint i = 0; i < 3; i++)
            lo[0] = (unsigned int) max(0, (int) floor((pos[idx][0])/DX-0.5));
            lo[1] = (unsigned int) max(0, (int) floor((pos[idx][1])/DY-0.5));
            lo[2] = (unsigned int) max(0, (int) floor((pos[idx][2])/DZ-0.5));
            hi[0] = (unsigned int) min(NX-1, (int) floor((pos[idx][0])/DX+0.5));
            hi[1] = (unsigned int) min(NY-1, (int) floor((pos[idx][1])/DY+0.5));
            hi[2] = (unsigned int) min(NZ-1, (int) floor((pos[idx][2])/DZ+0.5));
        }

        void getCurrentCell(int idx, int cell[3]) {
            cell[0] = (int) floor((pos[idx][0]) / DX);
            cell[1] = (int) floor((pos[idx][1]) / DY);
            cell[2] = (int) floor((pos[idx][2]) / DZ);
        }

};