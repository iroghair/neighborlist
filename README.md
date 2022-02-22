# Neighborlists 

## Intro
Finding the nearest neighbors in a cloud of particles can be an expensive
operation. This repository sets up an environment that allows for dedicated 
testing of various neighborlist-generating algorithms. Currently, it implements
a cell-list structure class.

## Components
* Python script `generateBinaryPositions.py`: Generates a binary file that 
contains the number of particles (1 double) and consequently their (x,y,z) 
positions.
* A main file `main.cpp` that allows to load the binary position file, puts the particles 
in a neighborlist creating algorithm and obtains a partner list. 
* A class file `NeighborList.cpp` and header `NeighborList.h` which define the
cell-list type neighborlist.

## Structure
Set up the output configuration and open the file that was provided as an 
argument to the executable:

```cpp
    cout.precision(numeric_limits<double>::max_digits10);
    cout << "Trying to open file " << argv[1] << " ..." << endl;
    ifstream file(argv[1], ios::binary);
```

Read the particles one-by-one (contiguous memory should also be able to read at 
once a block of `npart*sizeof(double[3])`):
```cpp
        for (int i = 0; i < npart; i++) {
            file.read((char*) pos[i], sizeof(double[3]));
        }

```

Create a neighborlist:
```cpp
NeighborList nblist(npart, pos, dsize, ncell, 0, npart);
```

Finally, timing of creating a cell list and a partner list:
```cpp
        auto t1 = Clock::now();
        nblist.createCellList();
        nblist.createPartnerList();
        auto t2 = Clock::now();
```

## To do
Creating the cell-list is a functionality of this particular algorithm; other 
algorithms (e.g. brute-force, tree-based) do not do this. One way to overcome 
this is to make a virtual base class that makes the internal algorithm fully 
transparent to the main program. The idea is to have a few public functions for 
any algorithm, apart from the constructor/destructor:
* `reset`: Clears any arrays from previous iterations
* `initialise`: Sets the scene for making partner lists
* `setSearchRadius`: Set the search radius
* `getPartnerList(a)`: Returns a partner list for particle `a`

