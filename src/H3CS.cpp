
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "knapsack.h"

/** allocate items of same height to levels
@param[in] all_items
@return vector of levels
*/
std::vector< itemv_t >
items2levels( itemv_t& all_items )
{
    std::sort( all_items.begin(), all_items.end(),[]( cItem& a, cItem& b)
    {
        return a.myHeight < b.myHeight;
    });

    // allocate items of same height to a level
    std::vector< itemv_t > vlevel;
    int h_level = -1;
    for( auto& item : all_items )
    {
        if( item.myHeight == h_level )
        {
            // add to current level
            vlevel.back().push_back( item );
        }
        else
        {
            // create a new level
            h_level = item.myHeight;
            itemv_t vl;
            vl.push_back( item );
            vlevel.push_back( vl );
        }
    }
    return vlevel;
}

sPattern H3CS (
    sInstance& problem )
{
    // construct vector of all items in problem instance
    itemv_t all_items;
    for( int kItem = 0; kItem < (int) problem.l.size(); kItem++ )
    {
        for( int k = 0; k < problem.demand[kItem]; k++ )
            all_items.push_back( cItem(
                                     problem.l[kItem],
                                     problem.w[kItem],
                                     problem.h[kItem],
                                     1 ));
    }

    // allocate items to levels
    auto theLevels = items2levels( all_items );

    // pack items into their level
    for( auto& level : theLevels )
        RK2FFG( level, problem.bin );

    // stack the levels
    int height = 0;
    for( auto& level : theLevels )
    {
        for( auto& item : level )
        {
            item.myLocH = height;
            std::cout << item.myLocL << " "<<
                      item.myLocW << " "<<
                      item.myLocH << "\n";
        }
        height += level[0].myHeight;
    }

    sPattern P;
    P.instance = problem;

    return P;
}
