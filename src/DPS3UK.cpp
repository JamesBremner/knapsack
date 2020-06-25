#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "knapsack.h"

sPattern DPS3UK (
    sInstance& problem )
{
    // populate variables used by pseudo code
    int L = problem.bin[0];
    int W = problem.bin[1];
    int H = problem.bin[2];
    std::vector<int> l = problem.l;
    std::vector<int> w = problem.w;
    std::vector<int> h = problem.h;
    std::vector<int> v = problem.item_values;
    int k = problem.stageCount;

    eCut previous;

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


    // set up some 4D vectors for storing results
    std::vector<int>                                G1(u,0);
    std::vector<std::vector<int>  >                 G2(s,G1);
    std::vector<std::vector<std::vector<int> > >    G3(m,G2);
    v4d_t    G(k,G3);        // the best value for boxes in this bin
    v4d_t    item(m,G3);     // the item at this location
    v4d_t    guil(m,G3);     // the cut orientation
    v4d_t    pos(m,G3);      // the cut position

    //pseudocode lines 2.3 to 2.8
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
                        if( v[d] > G[0][ilength][iwidth][iheight] )
                        {
                            G[0][ilength][iwidth][iheight] = v[d];
                            item[0][ilength][iwidth][iheight] = d;
                            guil[0][ilength][iwidth][iheight] = (int) eCut::nil;
                        }
                    }
                }
                std::cout << "Most valuable item in ( "
                          << Phat[ilength] <<" "<< Qhat[iwidth] <<" "<< Rhat[iheight] <<" ) has value "<< G[0]  [ilength][iwidth][iheight] << "\n";
            }
        }
    }
    //pseudocode line 2.9
    switch( k % 3 )
    {
    case 1:
        previous = eCut::vert;
        break;
    case 2:
        previous = eCut::depth;
        break;
    default:
        previous = eCut::horz;
        break;
    }

    // loop over stages
    for( int b = 1; b < k; b++ )
    {
        std::cout << "======= STAGE " << b << "============\n";
        for( int ilength = 0; ilength < m; ilength++ )
        {
            for( int iwidth = 0; iwidth < s; iwidth++ )
            {
                for( int iheight = 0; iheight < u; iheight++ )
                {
                    G[b][ilength][iwidth][iheight] = G[b-1][ilength][iwidth][iheight];
                    guil[b][ilength][iwidth][iheight] = (int)eCut::nil;
                    if( previous == eCut::depth )
                    {
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
                            // Does vertical cut at Phat[x] improve value of solution
                            if( G[b][ilength][iwidth][iheight] < G[b][x][iwidth][iheight]+G[b][t][iwidth][iheight] )
                            {
                                std::cout << "vertical cut ";
                                std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[b][ilength][iwidth][iheight] << " ";
                                std::cout << x <<" "<< t <<" "<< G[b][ilength][iwidth][iheight] << "<" << G[b][x][iwidth][iheight] << "+"<<G[b][t][iwidth][iheight] << "\n";

                                G[b][ilength][iwidth][iheight] = G[b][x][iwidth][iheight]+G[b][t][iwidth][iheight];
                                pos[b][ilength][iwidth][iheight] = Phat[t];
                                guil[b][ilength][iwidth][iheight] = (int) eCut::vert;  // Vertical cut; parallel to yz-plane

                            }
                        }
                    }

                    else if ( previous == eCut::vert )
                    {
                        int nn = -1;
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
                            if( G[b][ilength][iwidth][iheight] < G[b][ilength][iwidth][z]+G[b][ilength][iwidth][t] )
                            {
                                std::cout << "horizontal cut ";
                                std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[b][ilength][iwidth][iheight] << " ";
                                std::cout  << z <<" "<< t <<" "<< G[b][ilength][iwidth][iheight] << "<" << G[b][z][iwidth][iheight] << "+"<<G[b][t][iwidth][iheight] << "\n";

                                G[b][ilength][iwidth][iheight] = G[b][ilength][iwidth][z]+G[b][ilength][iwidth][t];
                                pos[b][ilength][iwidth][iheight] = Rhat[t];
                                guil[b][ilength][iwidth][iheight] = (int) eCut::horz;  // Horizontal cut; parallel to xy plane
                            }
                        }
                    }
                    else
                    {
                        int nn = -1;
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
                            if( G[b][ilength][iwidth][iheight] < G[b][ilength][y][iheight]+G[b][ilength][t][iheight] )
                            {
                                std::cout << "depth cut ";
                                std::cout << ilength <<" "<< iwidth <<" "<< iheight  <<" "<< G[b][ilength][iwidth][iheight] << " ";
                                std::cout  << y <<" "<< t <<" "<< G[b][ilength][iwidth][iheight] << "<" << G[b][y][iwidth][iheight] << "+"<<G[b][t][iwidth][iheight] << "\n";

                                G[b][ilength][iwidth][iheight] = G[b][ilength][y][iheight]+G[b][ilength][y][iheight];
                                pos[b][ilength][iwidth][iheight] = Qhat[t];
                                guil[b][ilength][iwidth][iheight] = (int) eCut::depth;  // Depth cut ðvertical; parallel to xy plane
                            }
                        }
                    }
                }
            }
        }
        // next stage uses next cut orientation
        switch( previous )
        {
        case eCut::vert:
            previous = eCut::depth;
            break;
        case eCut::depth:
            previous = eCut::horz;
            break;
        case eCut::horz:
            previous = eCut::vert;
            break;
        default:
            throw std::runtime_error("DPS3UK Bad stage value");
        }

    }   // end loop over stages


    sPattern P;
    P.instance = problem;

    P.value = G ;
    P.position = pos;
    P.direction = guil;
    P.item =item;
    P.l_raster = Phat;
    P.w_raster = Qhat;
    P.h_raster = Rhat;

    return P;
}
