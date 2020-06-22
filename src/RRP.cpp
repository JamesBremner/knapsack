#include <vector>
#include "knapsack.h"

std::vector<int> RRP(
    int D,
    std::vector<int>& d )
    {
        return DDP( D, d );
    }
