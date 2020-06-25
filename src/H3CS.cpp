
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "knapsack.h"


class cItem
{
public:
    int l, w, h;    // dimensions
    int demand;

    cItem( int L, int W, int H, int D )
        : l( L ), w( W ), h{ H }
        , demand( D )
    {

    }
};
typedef std::vector< cItem > itemv_t;

/** allocate items of same height to levels
@param[in] all_items
@return vector of levels
*/
std::vector< itemv_t >
items2levels( itemv_t& all_items )
{
    std::sort( all_items.begin(), all_items.end(),[]( cItem& a, cItem& b)
    {
        return a.h < b.h;
    });

    // allocate items of same height to a level
    std::vector< itemv_t > vlevel;
    int h_level = -1;
    for( auto& item : all_items )
    {
        if( item.h == h_level )
        {
            // add to current level
            vlevel.back().push_back( item );
        }
        else
        {
            // create a new level
            h_level = item.h;
            itemv_t vl;
            vl.push_back( item );
            vlevel.push_back( vl );
        }
    }
    for( auto& level : vlevel )
    {
        std::cout <<  "LEVEL\n";
        for( auto& item : level )
        {
            std::cout << item.l <<" "<< item.w <<" "<< item.h << "\n";
        }
    }
    return vlevel;
}
/** 2D packer for a level
    @param[in] level the items to go in this level
    @param[in] bin the bin being packed
*/
void CS2( itemv_t& level, std::vector<int>& bin )
{
    throw std::runtime_error("No miracles today!");
}

sPattern H3CS (
    sInstance& problem )
{
    // construct vector of all items in problem instance
    itemv_t all_items;
    for( int kItem = 0; kItem < (int) problem.l.size(); kItem++ )
    {
        all_items.push_back( cItem(
                                 problem.l[kItem],
                                 problem.w[kItem],
                                 problem.h[kItem],
                                 problem.demand[kItem]) );
    }

    // allocate items to levels
    auto theLevels = items2levels( all_items );

    // pack items into their level using CS2 algorithm
    for( auto& level : theLevels )
        CS2( level, problem.bin );

    sPattern P;
    P.instance = problem;

    return P;
}
