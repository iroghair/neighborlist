#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

int main(int argc, char* argv[])
{
    int npart;
    double** pos;

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


        cout << "Waiting for key press...." << endl << endl;
        getchar();

        for (int i = 0; i < npart; i++)
            delete [] pos[i];
        
        delete [] pos;
    }
    else cout << "Error opening file " << argv[1] << ".\n\n" << endl;
}