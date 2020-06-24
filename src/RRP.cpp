#include <iostream>
#include <vector>
#include <algorithm>
#include "knapsack.h"

int LargestInVSmallerThanX(
    std::vector<int> V,
    int X
)
{
    int R = -1;        // assume all +ve
    for( int t : V )
    {
        if( t <= X )   // smaller or equal
            if( t > R ) // largest
                R = t;
    }
    return R;
}
/** Calulate set of reduced raster points
    @param[in] D the bin size in one dimension
    @param[in[ d the item ypes sizes in same dimension

    This implements https://github.com/JamesBremner/knapsack/issues/1#issuecomment-648373643
*/
std::vector<int> RRP(
    int D,
    const std::vector<int>& d )
{
    std::vector<int> reduced_raster_points;

    auto S = DDP( D, d );   // set of conic combinations

    for( auto p : S )       // loop over discretization points
    {
        int r = LargestInVSmallerThanX( S, D - p );
        if( r >= 0 )
            reduced_raster_points.push_back( r );
    }

    // arrange in ascending size
    std::reverse(reduced_raster_points.begin(),reduced_raster_points.end());

    return reduced_raster_points;
}
