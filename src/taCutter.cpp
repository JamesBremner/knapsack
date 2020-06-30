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
//    for( auto& c : pack2::CutListEndPoints( E ) )
//    {
//        for( int v : c )
//            std::cout << v << ", ";
//        std::cout << "\n";
//    }
//
//    std::cout << "\nPack2 csv\n";
//    std::cout << pack2::CSV( E );


    // loop over items in level, allocating orders that fit
    int packedCount = 0;
    for( pack2::item_t item : E.items() )
    {
        if( ! item->isPacked() )
            continue;

        packedCount++;
        if( packedCount == 1 )
        {
            /* We are going to cut at least one order for this level from this stock
                first we need to cut the level from the stock.
                so record the level cut in the instance
                unless at top of stock
            */
            CutLevel(
                I,
                level );
        }

        AllocateOrder(
            I,
            level,
            atoi( item->userID().c_str() ),
            item->locX(), item->locY(), h );
    }

    // check for nothing packed
    if( ! packedCount )
    {
        std::cout << "nothing fit\n";
        return false;
    }

    // at least one order was cut for this stock
    // store cuts in the instance

    /* Loop over cuts in level

        The pack2 engine returns a vector of vectors, one for each bin
        There is only one bin, the level,
        so we need only look at the first vector of cuts
    */
    auto cl = CutList( E )[0];
    for( auto& c : cl )
    {
        // check if cut is at stock boundary
        char LW;
        if( c.myIsVertical )
        {
            // vertical cuts in pack2 ( which is 2d ) are orthogonasl to the length dimension in 3D
            if( c.myIntercept == 0 || c.myIntercept == level.myStock->myLength )
                continue;   // unneccesary cut at stock edge
            LW = 'L';
        }
        else
        {
            // horizontal cuts in pack2 ( which is 2d ) are orthogonasl to the width dimension in 3D
            if( c.myIntercept == 0 || c.myIntercept == level.myStock->myWidth )
                continue;   // unneccesary cut at stock edge
            LW = 'W';
        }
        // construct a 3D cut
        cCut cut(
            level.myStock,
            LW,
            c.myIntercept,
            h );

        //std::cout << cut.text() << "\n\n";

        // add it to the instance cut list
        I.myCut.push_back( cut );
    }

    return ( packedCount == level.size() );
}

void AllocateOrder(
    cInstance& I,
    cLevel& level,
    int order,
    int length, int width, int height )
{
    // allocate order to stock
    level.myOrder[ order ]->pack( length, width, height, level.myStock );

    // record allocation
    I.myAllocation.push_back( std::make_pair( level.myOrder[ order ], level.myStock ));

    // mark stock as used
    level.myStock->used();

    // record how much of level used
    level.use( level.myOrder[ order ] );
}
void CutLevel(
    cInstance& I,
    cLevel& level )
{
    timber_t stock = level.myStock;

    stock->level( level.height() );

    if( level.height() == stock->myHeight )
    {
        //no need for a cut, we are at the top of the stock
        return;
    }
    I.myCut.push_back( cCut(
                           stock,
                           'H',
                           level.height(),
                           level.height() ));
}

void ReturnToInventory(
    cInventory& I )
{
    // remove stock timbers whose complete height has been used
    I.myStock.erase(
        remove_if(
            I.myStock.begin(),
            I.myStock.end(),
            [] ( timber_t t )
    {
        std::cout << "remove? " <<t->isUsed() <<" " <<t->level() <<" "<<t->myHeight << "\n";
        return( t->isUsed()  && t->level() == t->myHeight );
    } ),
    I.myStock.end() );

    // adjust height of partially used stock
    for( timber_t t : I.myStock )
    {
        if( ! t->isUsed() )
            continue;

        t->myHeight -= t->level();
        t->level( 0 );
        t->used( false );
    }
}
void DisplayWastage( std::vector<cLevel>& levels )
{
    int wastage = 0;
    for( auto l : levels )
        wastage += l.wastage();
    std::cout << "w " << wastage << "\n";
}
}
