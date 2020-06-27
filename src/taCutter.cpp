#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>
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

    for( auto& l : vlevel )
    {
        std::cout <<"\nLEVEL\n";
        for( timber_t t : l )
        {
            std::cout << t->text() << "\n";
        }
    }
    return vlevel;
}

std::vector<std::pair<timber_t,timber_t>>
LevelsToStock(
    std::vector< timberv_t >& levels,
    timberv_t& stock )
{
    std::vector<std::pair<timber_t,timber_t>> allocation;

    for ( auto& level : levels )
    {
        int least_waste = std::numeric_limits<int>::max();
//        int least_stock_height = std::numeric_limits<int>::max();
        timber_t best_stock = stock[0];
        int level_height = level[0]->myHeight;
        for( timber_t t : stock )
        {
            int stockHeight = t->myHeight;
            if( stockHeight < level_height )
                continue;
            if( stockHeight == level_height )
            {
                least_waste = 0;
                best_stock = t;
                break;
            }
            int waste = stockHeight % level_height;
            if( waste < least_waste )
            {
                least_waste = waste;
                best_stock = t;
            }
        }
        std::cout << "level " << level_height << " stock " << best_stock->text() << "\n";

        for( timber_t o : level )
        {
            allocation.push_back( std::make_pair( o, best_stock ));

            //std::cout << "a " << o->myUserID <<" "<< best_stock->myUserID << "\n";
        }
    }
    return allocation;
}
}
