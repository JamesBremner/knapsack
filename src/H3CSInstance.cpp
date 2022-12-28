/** Application to read THREE-DIMENSIONAL UNBOUNDED KNAPSACK PROBLEM - INSTANCES
    from http://www.loco.ic.unicamp.br/files/instances/3duk/
    apply code Implemented from the psuedo code in
    Cintra GF, Miyazawa FK, Wakabayashi Y, Xavier EC. Algorithms for twodimensional cutting stock and strip packing problems using dynamic programming and column generation. European Journal of Operational Research
2008;191:59�83
    algorithm 3: H3CS
*/


#include <iostream>
#include <vector>
#include "knapsack.h"


int main( int argc, char* argv[] )
{
    std::cout << "H3CS\n";

    if( argc != 2 )
    {
        std::cout << "Usage: H3CSInstance <path to instance file>\n";
        exit(1);
    }

    // read the instance file
    sInstance problem;
    problem.read( argv[1] );

    problem.stageCount = 3;

    // run the algorithm
    auto P = H3CS( problem );

    // display the solution
    std::cout << P.text() << "\n";

    return 0;
}

