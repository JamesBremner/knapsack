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

//    for( auto& l : vlevel )
//    {
//        std::cout <<"\nLEVEL\n";
//        for( timber_t t : l )
//        {
//            std::cout << t->text() << "\n";
//        }
//    }
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

        std::cout << "level " << level_height << " stock " << best_stock->text() << "\n";

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
                                   cut ));
        }
    }   // end loop over levels
}
void LevelCuts(
    cInstance& I,
    std::vector< cLevel >& levels )
{
    for( cLevel& level : levels )
    {
        for( int h = 0; h < level.myStock->myHeight; h += level.height() )
        {
            CS2LNW( I, level, h );
        }
    }
}
void CS2LNW(
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
        if( L < t->myLength )
        {
            allPacked = false;
            break;
        }
        if( t->myWidth > level.myStock->myLength )
            throw std::runtime_error( "CS2LNW failed to width pack" );

        // order will fit into remainder of stock
        t->pack( L, 0, h, level.myStock  );
        int loc = level.myStock->myLength - L;
        if( 0 < loc && loc < level.myStock->myLength )
            I.myCut.push_back( cCut(
                                   level.myStock,
                                   'L',
                                   level.myStock->myLength - L ));

        L -= t->myLength;
    }
    if( ! allPacked )
        throw std::runtime_error( "CS2LNW failed to stack levels" );
}
}
