#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>
#include "TimberAllocation.h"

namespace ta
{
std::vector< cLevel >
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
    std::vector< cLevel > levels;
    int h_level = -1;
    for( timber_t t : order )
    {
        if( t->myHeight != h_level )
        {
            // create a new level
            h_level = t->myHeight;
            cLevel level;
            levels.push_back( level );
        }

        // add to current level
        levels.back().myOrder.push_back( t );
    }

//    for( cLevel& l : levels )
//        std::cout << l.text();

    return levels;
}

void LevelsToStock(
    cInstance& I,
    std::vector< cLevel >& levels,
    timberv_t& stock )
{


    for ( auto& level : levels )
    {
        int least_waste = std::numeric_limits<int>::max();
//        int least_stock_height = std::numeric_limits<int>::max();
        timber_t best_stock = stock[0];
        int level_height = level.height();
        bool found = false;
        for( timber_t t : stock )
        {
            int stockHeight = t->myHeight;
            if( stockHeight < level_height )
                continue;
            if( stockHeight == level_height )
            {
                found = true;
                least_waste = 0;
                best_stock = t;
                break;
            }
            int waste = stockHeight % level_height;
            if( waste < least_waste )
            {
                found = true;
                least_waste = waste;
                best_stock = t;

                //std::cout << best_stock->myUserID << " " << waste << "\n";
            }
        }
        if( ! found )
            throw std::runtime_error("Cannot allocate");

        //std::cout << "level " << level_height << " stock " << best_stock->text() << "\n";

        level.myStock = best_stock;
        for( timber_t o : level.myOrder )
        {
            I.myAllocation.push_back( std::make_pair( o, best_stock ));
        }
        for( int cut = level_height; cut < best_stock->myHeight; cut += level_height )
        {
            I.myCut.push_back( cCut(
                                   best_stock,
                                   'H',
                                   cut,
                                   cut ));
        }
    }   // end loop over levels
}
void LevelCuts(
    cInstance& I,
    std::vector< cLevel >& levels )
{
    // loop over levels
    for( cLevel& level : levels )
    {
        /* stack levels

        If not all the orders in a level can be fitted into the stock timber
        at one level, perhaps the stock can be cut into several levels to fit them

        */
        for(
            int h = 0;                      // start at the bottom
            h < level.myStock->myHeight;    // does stock have enough height to stack another level?
            h += level.height() )           // up one level
        {
            // Use 2D cutting algorithm to cut orders from level.
            if(  CS2LNW( I, level, h ) )
            {
                // all timbers in this level are packed
                break;
            }

            // remove timbers from level that have been packed
            level.removePacked();

        }
    }
}
bool CS2LNW(
    cInstance& I,
    cLevel& level, int h )
{
    std::sort(
        level.myOrder.begin(), level.myOrder.end(),
        []( timber_t a, timber_t b )
    {
        return ( a->myLength < b->myLength );
    });

    int L = level.myStock->myLength;
    bool allPacked = true;
    for( timber_t t : level.myOrder )
    {
        //std::cout << L <<" "<< t->myLength << "\n";
        if( L < t->myLength )
        {
            allPacked = false;
            break;
        }
        if( t->myWidth > level.myStock->myLength )
            throw std::runtime_error( "CS2LNW failed to width pack" );

        // order will fit into remainder of stock
        t->pack( L, 0, h, level.myStock  );

        // cut position, separating order from remainder of stock
        int loc = level.myStock->myLength - L + t->myLength;

        // check if cut is needed
        if( loc < level.myStock->myLength )
        {
            // add to cutting list
            cCut cut(
                level.myStock,
                'L',
                loc,
                h );
            //std::cout << cut.text() << "\n\n";
            I.myCut.push_back( cut );
        }

        L -= t->myLength;
    }
    return allPacked;
}
}
