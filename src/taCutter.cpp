#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include "TimberAllocation.h"

namespace ta
{
std::vector< timberv_t >
Levels( timberv_t& order)
{
    // rotate, if neccesary, so L > W > H
    for( auto t : order )
    {
        t->rotateLWH();
    }

    // sort by height
    std::sort( order.begin(), order.end(),
               []( timber_t a, timber_t b)
    {
        return a->myHeight < b->myHeight;
    });

    // allocate items of same height to a level
    std::vector< timberv_t > vlevel;
    int h_level = -1;
    for( timber_t t : order )
    {
        if( t->myHeight != h_level )
        {
            // create a new level
            h_level = t->myHeight;
            timberv_t level;
            vlevel.push_back( level );
        }

        // add to current level
        vlevel.back().push_back( t );
    }

    for( auto& l : vlevel ) {
            std::cout <<"\nLEVEL\n";
            for( timber_t t : l )
            {
                std::cout << t->text() << "\n";
            }
    }
    return vlevel;
}
}
