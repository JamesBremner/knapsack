// knapsack.h contains declarations for functions implementing knapsack algorithms

/** discretization points
    @param[in] D knapsack capacity
    @param[in] d vector of item weights

    Implemented from the psuedo code in Cintra GF, Miyazawa FK, Wakabayashi Y, Xavier EC. Algorithms for twodimensional cutting stock and strip packing problems using dynamic programming and column generation. European Journal of Operational Research
2008;191:59–83

In what follows we describe the algorithm DDP. The basic idea of this algorithm is to solve
a knapsack problem in which every item i has weight and value di (i = 1, . . . , m), and the
knapsack has capacity D. The well-known dynamic programming technique for the knapsack
problem (see [23]) finds optimal values of knapsacks with (integer) capacities taking values from
1 to D. It is easy to see that j is a discretization point if and only if the knapsack with capacity
j has optimal value j.

 I am not clear on what constitues an "optimal knapsack".  It looks like one that can contain a subset of
 items, and possibly duplicate items, without wasted space.  However the knapsack that can carry
 every item is excluded.

*/
std::vector<int> DDP(
    int D,
    std::vector<int>& d );

