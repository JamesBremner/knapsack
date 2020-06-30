#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include "TimberAllocation.h"

using namespace std;

int main( int argc, char* argv[] )
{
    cout << "TimberAllocation" << endl;

    if( argc != 2 )
    {
        std::cout << "Usage: TimberAllocation <path to instance file>\n";
        exit(1);
    }

    try
    {
        ta::cInstance I;
        ta::cInventory theInventory;

        // read problem instance file
        I.read( theInventory, argv[1] );

        // sort inventory into stock, sheets and scraps
        theInventory.sortInventory( 100, 100 );

        // sort orders into levels of the same height
        auto levels = Levels( I );

        // allocate levels to stock
        LevelsToStock( I, levels, theInventory );

        // cut out orders from stock by level
        LevelCuts( I, levels, theInventory );

        ReturnToInventory( theInventory );

        // display solution
        std::cout << I.textSolution();

        // display depleted inventory
        std::cout << theInventory.textDetails();
    }
    catch( std::runtime_error& e )
    {
        std::cout << "exception: " << e.what() << "\n";
    }
    return 0;
}
