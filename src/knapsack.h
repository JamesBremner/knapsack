// knapsack.h contains declarations for functions and data structures implementing knapsack algorithms

#include <string>
#include <vector>

/// cut orientation codes
enum class eCut
{
    nil,
    vert,
    depth,
    horz,
};

typedef std::vector<std::vector<std::vector<std::vector<int> > > > v4d_t;

/** discretization points ( (positions where guillotine cutting can be performed)
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
    const std::vector<int>& d );

/** Calcculate set of reduced raster points
    @param[in] D the bin size in one dimension
    @param[in[ d the item ypes sizes in same dimension

    This implements https://github.com/JamesBremner/knapsack/issues/1#issuecomment-648373643
*/

std::vector<int> RRP(
    int D,
    const std::vector<int>& d );

/// A problem instance
struct sInstance
{
    std::vector<int> bin;
    std::vector<int> l;
    std::vector<int> w;
    std::vector<int> h;
    std::vector<int> item_values;
    std::string myName;
    int stageCount;                 // the number of stages

    /* read problem instance
            #param[in] fname path to file

        The file format is described
        http://www.loco.ic.unicamp.br/files/instances/3duk/
    */
    void read( const std::string& fname );

    std::string text();
};

/// A solution pattern
struct sPattern
{
    sInstance instance;
    std::vector<int> l_raster, w_raster, h_raster;          /// points where cuts might be made
    v4d_t value;
    v4d_t position;
    v4d_t item;
    v4d_t direction;                         /// cut direction

    std::string text() const;
};

/** dynamic programming for the three-dimensional unbounded knapsack
    @param[in] problem instance
*/
sPattern DP3UK (
    sInstance& problem );

 /** dynamic programming for the staged three-dimensional unbounded knapsack
    @param[in] problem instance

    A k-staged cutting is a sequence of at most k stages of cuts, each
stage of which is a set of parallel guillotine cuts performed on the
objects obtained in the previous stage. Moreover, the cuts in each
stage must be orthogonal to the cuts performed in the previous
stage. We assume, without loss of generality, that the cuts are
infinitely thin ( no kerf! )
*/
    sPattern DPS3UK (
    sInstance& problem );
