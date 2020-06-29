#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>
#include "TimberAllocation.h"
#include "pack2.h"

namespace ta
{

std::string cCut::text()
{
    std::stringstream ss;
    ss << "c " << myStock->myUserID << " "
       << myDirection <<" "<< myLocation <<" "
       << myLevelHeight;
    return ss.str();
}

std::vector< cLevel >
Levels( cInstance& I )
{
    timberv_t& order = I.myOrder;

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

    for( cLevel& l : levels )
        std::cout << l.text() <<"\n";

    return levels;
}

void LevelsToStock(
    cInstance& I,
    std::vector< cLevel >& levels,
    cInventory& inventory )
{
    for ( auto& level : levels )
    {
        LevelToStock( I, level, inventory.myStock );
    }
}

bool
LevelToStock(
    cInstance& I,
    cLevel& level,
    timberv_t& stock )
{
    if( ! stock.size() )
        throw std::runtime_error("No stock");

    int least_waste = std::numeric_limits<int>::max();
//        int least_stock_height = std::numeric_limits<int>::max();
    timber_t best_stock = stock[0];
    int level_height = level.height();
    bool found = false;
    for( timber_t t : stock )
    {
        if( t->isUsed() )
            continue;

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

    // check for no stock available
    if( ! found )
    {
        std::cout << "level " << level_height << " no stock available\n";
        return false;
    }

    std::cout << "level " << level_height << " order count " << level.size()
              << " allocated stock " << best_stock->text() << "\n";

    level.myStock = best_stock;

    return true;
}

void LevelCuts(
    cInstance& I,
    std::vector< cLevel >& levels,
    cInventory& inventory )
{
    bool allPacked;
    // loop over levels
    for( cLevel& level : levels )
    {
        std::cout << "cutting level " << level.text() << "\n";

        do
        {
            /* stack levels

            If not all the orders in a level can be fitted into the stock timber
            at one level, perhaps the stock can be cut into several levels to fit them

            */
            allPacked = false;
            for(
                int h = 0;                      // start at the bottom
                h < level.myStock->myHeight;    // does stock have enough height to stack another level?
                h += level.height() )           // up one level
            {
                // Use 2D cutting algorithm to cut orders from level.

#ifdef USE_CS2LNW
                allPacked = CS2LNW( I, level, h );
#endif // USE_CS2LNW
#ifdef USE_CS2Pack2
                allPacked = CS2Pack2( I, level, h );
#endif // CS2Pack2

                if(  allPacked )
                {
                    // all timbers in this level are packed
                    std::cout << "all timbers in this level are packed\n";
                    allPacked = true;
                    break;
                }

                // remove timbers from level that have been packed
                level.removePacked();

                // flag the stock has been used
                level.myStock->setUsed();

                // TODO: return unused remainders to inventory

                if( ! allPacked )
                {
                    // stock timber exhausted, need to allocate another
                    if( ! LevelToStock( I, level, inventory.myStock ) )
                    {
                        // no suitable stock available
                        std::cout << " no suitable stock available\n";
                        I.addUnpacked( level.myOrder );
                        allPacked = true;
                    }
                }

            }
        }
        while( ! allPacked );
    }
    if( allPacked )
    {
        // success
        return;
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

bool CS2Pack2(
    cInstance& I,
    cLevel& level, int h )
{
    // load level into pack2 engine
    pack2::cPackEngine E;

    E.add( pack2::bin_t( new pack2::cBin( "stock",
                                          level.myStock->myLength,
                                          level.myStock->myWidth )) );
    int k = 0;
    for( timber_t& t : level.myOrder )
    {
        E.addItem(
            std::to_string( k++ )+"_"+t->myUserID,
            t->myLength, t->myWidth );
    }

    // run the Pack2 engine
    Pack( E );

//    std::cout << "Pack2 cutlist\n";
//    for( auto& c : pack2::CutList( E ) )
//    {
//        for( int v : c )
//            std::cout << v << ", ";
//        std::cout << "\n";
//    }
//
//    std::cout << "\nPack2 csv\n";
//    std::cout << pack2::CSV( E );

    int unpackedCount = 0;
    for( pack2::item_t item : E.items() )
    {
        if( ! item->isPacked() )
        {
            unpackedCount++;
            if( unpackedCount == 1 )
            {
                CutLevel(
                    I,
                    level );
            }
            continue;
        }
        // order  cut

        CutOrder(
            I,
            level.myStock,
            level.myOrder[ atoi( item->userID().c_str() ) ],
            item->locX(), item->locY(), h );
    }

    // check for nothing packed
    if( unpackedCount == (int)level.myOrder.size() )
    {
        std::cout << "nothing fit\n";
        return false;
    }

    auto binList = RawCutList( E )[0];
    for( auto& c : binList )
    {
        // add to cutting list
        char LW;
        if( c.myIsVertical )
        {
            LW = 'L';
        }
        else
        {
            LW = 'W';
        }

        cCut cut(
            level.myStock,
            LW,
            c.myIntercept,
            h );
        //std::cout << cut.text() << "\n\n";
        I.myCut.push_back( cut );
    }

    return ( unpackedCount == 0 );
}

void CutOrder(
    cInstance& I,
    timber_t& stock,
    timber_t& order,
    int length, int width, int height )
{
    order->pack( length, width, height, stock );
    I.myAllocation.push_back( std::make_pair( order, stock ));
}
void CutLevel(
    cInstance& I,
    cLevel& level )
{
    I.myCut.push_back( cCut(
                           level.myStock,
                           'H',
                           level.height(),
                           level.height() ));
}
}
