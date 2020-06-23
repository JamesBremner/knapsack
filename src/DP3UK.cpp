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
    std::vector<std::vector<std::vector<int> > >    pos(m,G2);

    // cut orientation codes
    const int nil = 0;
    const int Vert = 1;
    const int Depth = 2;
    const int Horz = 3;

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
    for( int i = 0; i < m; i++ )
    {
        for( int j = 0; j < s; j++ )
        {
            for( int k = 0; k < u; k++ )
            {
                // avoid generating symmetric patterns by considering, in each direction,
                // r-points up to half of the size of the respective bin
                int nn;
                for( int d = 0; d <= i; d++ )
                {
                    if( Phat[d] <= Phat[i] / 2 )
                        nn = d;
                }
                for( int x = 0; x <= nn; x++ )
                {
                    int t = 0;
                    for( int d = 0; d <= m; d++ )
                    {
                        if( Phat[d] <= Phat[i] - Phat[x] )
                            t = d;
                    }

                    if( G[i][j][k] < G[x][j][k]+G[t][j][k] )
                    {
                        G[i][j][k] = G[x][j][k]+G[t][j][k];
                        pos[i][j][k] = Phat[x];
                        guil[i][j][k] = Vert;  // Vertical cut; parallel to yz-plane
                    }
                }


                for( int d = 0; d <= j; d++ )
                {
                    if( Qhat[d] <= Qhat[j] / 2 )
                        nn = d;
                }
                for( int y = 0; y <= nn; y++ )
                {
                    int t = 0;
                    for( int d = 0; d <= s; d++ )
                    {
                        if( Qhat[d] <= Qhat[j] - Qhat[y] )
                            t = d;
                    }

                    if( G[i][j][k] < G[i][y][k]+G[i][t][k] )
                    {
                        G[i][j][k] = G[i][y][k]+G[i][y][k];
                        pos[i][j][k] = Qhat[y];
                        guil[i][j][k] = Depth;  // Depth cut ðvertical; parallel to xy plane
                    }
                }

                for( int d = 0; d <= k; d++ )
                {
                    if( Rhat[d] <= Rhat[k] / 2 )
                        nn = d;
                }
                for( int z = 0; z <= nn; z++ )
                {
                    int t = 0;
                    for( int d = 0; d <= u; d++ )
                    {
                        if( Rhat[d] <= Rhat[k] - Rhat[z] )
                            t = d;
                    }

                    if( G[i][j][k] < G[i][j][z]+G[i][j][t] )
                    {
                        G[i][j][k] = G[i][j][z]+G[i][j][t];
                        pos[i][j][k] = Rhat[z];
                        guil[i][j][k] = Horz;  // Horizontal cut; parallel to xy plane
                    }
                }
            }
        }
    }
}


