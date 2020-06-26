#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "knapsack.h"

bool RK2FFG(
    itemv_t& level,
    const std::vector<int>& bin )
{
    // sort items into decreasing size
    std::sort(
        level.begin(), level.end(),
        []( cItem& a, cItem& b)
    {
        return a.size_horiz() > b.size_horiz();
    });

    // create a vector of empty spaces
    // with one initial space
    // the length and width of the bin and the height of the items
    itemv_t space;
    space.push_back( cItem( bin[0], bin[1], level[0].myHeight, 0 ));
    space.back().myLocL = 0;
    space.back().myLocW = 0;
    space.back().myLocH = 0;

    bool allFitted = true;
    // loop over the items
    for( auto& item : level )
    {
        // loop over the available space
        for( auto& test_space : space )
        {
            // will the item fit into the space?
            // ( this is why the algorithm is called first fit ( FF ) )
            if( ! test_space.fit( item ) )
                continue;

            // the item fits, so pack it in
            item.pack( test_space );

            // divide the remaining space into two remaining rectangular spaces

            // one remainder is created by modifying the used space
            test_space.myLocL += item.myLength;
            test_space.myLength -= item.myLength;

            // and the second remainder is a new space added to the vector
            cItem new_space(
                item.myLength,
                test_space.myWidth-item.myWidth,
                test_space.myHeight, 0 );
            new_space.myLocL = test_space.myLocL;
            new_space.myLocW = test_space.myLocW + item.myWidth;
            new_space.myLocH = 0;
            space.push_back( new_space );

            // sort the new vector of spaces into decreasing size
            // so the next item will first try in the largest space
            // this is why the algorithm is called greedy first fit
            std::sort(
                space.begin(), space.end(),
                []( cItem& a, cItem& b)
            {
                return a.size_horiz() > b.size_horiz();
            });
        }
        if( ! item.isPacked )
            allFitted = false;
    }
    return allFitted;
}

