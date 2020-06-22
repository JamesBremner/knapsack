#include <iostream>
#include <vector>
#include "knapsack.h"

void DP3UK (
    int L, int W, int H,
    std::vector<int>& l,
    std::vector<int>& w,
    std::vector<int>& h,
    std::vector<int>& v)
{
    auto Phat = RRP( L, l );
    auto Qhat = RRP( W, w );
    auto Rhat = RRP( H, h );

    /* Store in G[i, j, k] for each bin of dimension (pi, qj, rk),
     with pi AP ~ , qj AQ ~ and rk AR ~ , the maximum value of a box
    that can be cut in such a bin. */
    int m = Phat.size();
    int s = Qhat.size();
    int u = Rhat.size();
    std::vector<int>                                G1(u,0);
    std::vector<std::vector<int>  >                 G2(s,G1);
    std::vector<std::vector<std::vector<int> > >    G(m,G2);
    std::vector<std::vector<std::vector<int> > >    item(m,G2);
    std::vector<std::vector<std::vector<int> > >    guil(m,G2);

    // cut orientation codes
    const int nil = 0;
    const int V = 1;
    const int D = 2;
    const int H = 3;

    for( int i = 0; i < m; i++ )
    {
        for( int j = 0; j < s; j++ )
        {
            for( int k = 0; k < u; k++ )
            {
                // loop over the items
                for( int d = 0; d < (int)v.size(); d++ )
                {
                    if( l[d] <= Phat[i] &&
                            w[d] <= Qhat[j] &&
                            h[d] <= Rhat[k] )
                    {
                        // the item is small enough to fit

                        // does it have greater value than previous fitted items?
                        if( v[d] > G[i][j][k] )
                        {
                            G[i][j][k] = v[d];
                            item[i][j][k] = d;
                            guil[i][j][k] = nil;
                        }
                    }
                }
                std::cout << "Most valuable item in ( "
                    << Phat[i] <<" "<< Qhat[j] <<" "<< Rhat[k] <<" ) has value "<< G[i][j][k] << "\n";
            }
        }
    }
}
