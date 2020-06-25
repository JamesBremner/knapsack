#include <iostream>
#include <sstream>
#include <vector>

#include "knapsack.h"

sPattern DP3UK (
    sInstance& problem )
{
    // force unstaged
    problem.stageCount = 0;

    // populate variables used by pseudo code
    int L = problem.bin[0];
    int W = problem.bin[1];
    int H = problem.bin[2];
    std::vector<int> l = problem.l;
    std::vector<int> w = problem.w;
    std::vector<int> h = problem.h;
    std::vector<int> v = problem.item_values;

    /* Calculate discretization points

    In the Queiro paper reduced raster points are used as an optimization.
    We cannot get this to work correctly, see https://github.com/JamesBremner/knapsack/issues/1
    So we simply use the discretization points
    */
    auto Phat = DDP( L, l );
    auto Qhat = DDP( W, w );
    auto Rhat = DDP( H, h );
    int m = Phat.size();
    int s = Qhat.size();
    int u = Rhat.size();
    std::cout << "length points ( total " << m << " ) " ;
    for( int p : Phat )
        std::cout << p << " ";
    std::cout << "\nwidth points ( total " << s << " ) " ;
    for( int p : Qhat )
        std::cout << p << " ";
    std::cout << "\nheight  points ( total " << u << " ) " ;
    for( int p : Rhat )
        std::cout << p << " ";
    std::cout << "\n";

    // set up some 3D vectors for storing results
    std::vector<int>                                G1(u,0);
    std::vector<std::vector<int>  >                 G2(s,G1);
    std::vector<std::vector<std::vector<int> > >    G(m,G2);        // the best value for boxes in this bin
    std::vector<std::vector<std::vector<int> > >    item(m,G2);     // the item at this location
    std::vector<std::vector<std::vector<int> > >    guil(m,G2);     // the cut orientation
    std::vector<std::vector<std::vector<int> > >    pos(m,G2);      // the cut position

    for( int ilength = 0; ilength < m; ilength++ )
    {
        for( int iwidth = 0; iwidth < s; iwidth++ )
        {
            for( int iheight = 0; iheight < u; iheight++ )
            {
                // loop over the items
                for( int d = 0; d < (int)v.size(); d++ )
                {
                    if( l[d] <= Phat[ilength] &&
                            w[d] <= Qhat[iwidth] &&
                            h[d] <= Rhat[iheight] )
                    {
                        // the item is small enough to fit

                        // does it have greater value than previous fitted items?
                        if( v[d] > G[ilength][iwidth][iheight] )
                        {
                            G[ilength][iwidth][iheight] = v[d];
                            item[ilength][iwidth][iheight] = d;
                            guil[ilength][iwidth][iheight] = (int) eCut::nil;
                        }
                    }
                }
                std::cout << "Most valuable item in ( "
                          << Phat[ilength] <<" "<< Qhat[iwidth] <<" "<< Rhat[iheight] <<" ) has value "<< G[ilength][iwidth][iheight] << "\n";
            }
        }
    }
    for( int ilength = 0; ilength < m; ilength++ )
    {
        for( int iwidth = 0; iwidth < s; iwidth++ )
        {
            for( int iheight = 0; iheight < u; iheight++ )
            {
                //std::cout << "\nijk " << ilength <<" "<< iwidth <<" "<< iheight  << "\n";

                // avoid generating symmetric patterns by considering, in each direction,
                // r-points up to half of the size of the respective bin
                int nn = -1;
                for( int d = 0; d <= ilength; d++ )
                {
                    if( Phat[d] <= Phat[ilength] / 2 )
                    {
                        nn = d;
                    }
                }

                for( int x = 0; x <= nn; x++ )
                {
                    int t = 0;
                    for( int d = 0; d < m; d++ )
                    {
                        if( Phat[d] <= Phat[ilength] - Phat[x] )
                            t = d;
                    }
                    // Does vertical cut at Phat[t] improve value of solution
                    if( G[ilength][iwidth][iheight] < G[x][iwidth][iheight]+G[t][iwidth][iheight] )
                    {
                        std::cout << "vertical cut ";
                        std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[ilength][iwidth][iheight] << " ";
                        std::cout <<"x="<< x <<" t="<< t <<" "<< G[ilength][iwidth][iheight] << "<" << G[x][iwidth][iheight] << "+"<<G[t][iwidth][iheight] << "\n";

                        G[ilength][iwidth][iheight] = G[x][iwidth][iheight]+G[t][iwidth][iheight];
                        pos[ilength][iwidth][iheight] = Phat[t];
                        guil[ilength][iwidth][iheight] = (int) eCut::vert;  // Vertical cut; parallel to yz-plane

                    }
                }

                nn = -1;
                for( int d = 0; d <= iwidth; d++ )
                {
                    if( Qhat[d] <= Qhat[iwidth] / 2 )
                        nn = d;
                }
                for( int y = 0; y <= nn; y++ )
                {
                    int t = 0;
                    for( int d = 0; d < s; d++ )
                    {
                        if( Qhat[d] <= Qhat[iwidth] - Qhat[y] )
                            t = d;
                    }
                    if( G[ilength][iwidth][iheight] < G[ilength][y][iheight]+G[ilength][t][iheight] )
                    {
                        std::cout << "depth cut ";
                        std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[ilength][iwidth][iheight] << " ";
                        std::cout  << y <<" "<< t <<" "<< G[ilength][iwidth][iheight] << "<" << G[y][iwidth][iheight] << "+"<<G[t][iwidth][iheight] << "\n";

                        G[ilength][iwidth][iheight] = G[ilength][y][iheight]+G[ilength][y][iheight];
                        pos[ilength][iwidth][iheight] = Qhat[t];
                        guil[ilength][iwidth][iheight] = (int) eCut::depth;  // Depth cut ðvertical; parallel to xy plane
                    }
                }

                nn = -1;
                for( int d = 0; d <= iheight; d++ )
                {
                    if( Rhat[d] <= Rhat[iheight] / 2 )
                        nn = d;
                }
                for( int z = 0; z <= nn; z++ )
                {
                    int t = 0;
                    for( int d = 0; d < u; d++ )
                    {
                        if( Rhat[d] <= Rhat[iheight] - Rhat[z] )
                            t = d;
                    }
                    if( G[ilength][iwidth][iheight] < G[ilength][iwidth][z]+G[ilength][iwidth][t] )
                    {
                        std::cout << "horizontal cut ";
                        std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[ilength][iwidth][iheight] << " ";
                        std::cout  << z <<" "<< t <<" "<< G[ilength][iwidth][iheight] << "<" << G[z][iwidth][iheight] << "+"<<G[t][iwidth][iheight] << "\n";

                        G[ilength][iwidth][iheight] = G[ilength][iwidth][z]+G[ilength][iwidth][t];
                        pos[ilength][iwidth][iheight] = Rhat[t];
                        guil[ilength][iwidth][iheight] = (int) eCut::horz;  // Horizontal cut; parallel to xy plane
                    }
                }
            }
        }
    }

    // collect results together for return to calling code
    sPattern P;
    P.instance = problem;

    P.value.push_back( G );
    P.position.push_back( pos );
    P.direction.push_back( guil );
    P.item.push_back( item );
    P.l_raster = Phat;
    P.w_raster = Qhat;
    P.h_raster = Rhat;

    return P;
}


