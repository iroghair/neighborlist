#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int 
main() 
{
    static Array3i myZero = Array3i::Zero();
    Array3d tmp = {-1.0,22.0,3.0};
    Array3d DD = {0.5, 0.5, 0.2};

    // Vector3d DD = {1.0, 1.0, 1.0};

    // floor(pos.row(idx).array()/DD.array() - 0.5).array() 
    Array3i lo = myZero.max( floor(tmp/DD).cast <int> () );

    cout << lo << endl;

}
