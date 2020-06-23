#include <iostream>
#include <sstream>
#include <vector>
#include "knapsack.h"

sPattern DP3UK (
    std::vector<int>& l,
    std::vector<int>& w,
    std::vector<int>& h,
    sInstance& problem )
{
    // populate variables used by pseudo code
    int L = problem.bin[0];
    int W = problem.bin[1];
    int H = problem.bin[2];
    std::vector<int> v = problem.item_values;

    // Calulate raster points
    auto Phat = RRP( L, l );
    auto Qhat = RRP( W, w );
    auto Rhat = RRP( H, h );
    std::cout << "length raster points ";
    for( int p : Phat )
        std::cout << p << " ";
    std::cout << "\nwidth raster points ";
    for( int p : Qhat )
        std::cout << p << " ";
    std::cout << "\nheight raster points ";
    for( int p : Rhat )
        std::cout << p << " ";
    std::cout << "\n";

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
                std::cout << "\nijk " << i <<" "<< j <<" "<< k  << "\n";
                // avoid generating symmetric patterns by considering, in each direction,
                // r-points up to half of the size of the respective bin
                int nn = -1;
                for( int d = 0; d <= i; d++ )
                {
                    std::cout << "Phat " << i <<" " << d <<" "<< Phat[i] <<" " << Phat[d] << "\n";
                    if( Phat[d] <= Phat[i] / 2 )
                    {
                        nn = d;
                        std::cout << "=> nn " << nn << "\n";
                    }
                }
                for( int x = 0; x <= nn; x++ )
                {
                    int t = 0;
                    for( int d = 0; d < m; d++ )
                    {
                        if( Phat[d] <= Phat[i] - Phat[x] )
                            t = d;
                    }
                    // Does vertical cut at Phat[x] improve value of solution
                    if( pos[x][j][k] != nil )
                        continue;               // there is already a cut here

                    if( G[i][j][k] < G[x][j][k]+G[t][j][k] )
                    {
                        std::cout << "vertical cut\n";
                        std::cout << i <<" "<< j <<" "<< k  <<" "<< G[i][j][k] << "\n";
                        std::cout << x <<" "<< t <<" "<< G[i][j][k] << "<" << G[x][j][k] << "+"<<G[t][j][k] << "\n";

                        G[i][j][k] = G[x][j][k]+G[t][j][k];
                        pos[i][j][k] = Phat[x];
                        guil[i][j][k] = Vert;  // Vertical cut; parallel to yz-plane

                    }
                }

                nn = -1;
                for( int d = 0; d <= j; d++ )
                {
                    if( Qhat[d] <= Qhat[j] / 2 )
                        nn = d;
                }
                for( int y = 0; y <= nn; y++ )
                {
                    int t = 0;
                    for( int d = 0; d < s; d++ )
                    {
                        if( Qhat[d] <= Qhat[j] - Qhat[y] )
                            t = d;
                    }
                    if( pos[i][t][k] != nil )
                        continue;               // there is already a cut here
                    if( G[i][j][k] < G[i][y][k]+G[i][t][k] )
                    {
                        std::cout << "depth cut\n";
                        std::cout << i <<" "<< j <<" "<< k  <<" "<< G[i][j][k] << "\n";
                        std::cout  << y <<" "<< t <<" "<< G[i][j][k] << "<" << G[y][j][k] << "+"<<G[t][j][k] << "\n";

                        G[i][j][k] = G[i][y][k]+G[i][y][k];
                        pos[i][j][k] = Qhat[y];
                        guil[i][j][k] = Depth;  // Depth cut ðvertical; parallel to xy plane
                    }
                }

                nn = -1;
                for( int d = 0; d <= k; d++ )
                {
                    if( Rhat[d] <= Rhat[k] / 2 )
                        nn = d;
                }
                for( int z = 0; z <= nn; z++ )
                {
                    int t = 0;
                    for( int d = 0; d < u; d++ )
                    {
                        if( Rhat[d] <= Rhat[k] - Rhat[z] )
                            t = d;
                    }
                    if( pos[i][j][t] != nil )
                        continue;               // there is already a cut here
                    if( G[i][j][k] < G[i][j][z]+G[i][j][t] )
                    {
                        std::cout << "horizontal cut\n";
                        std::cout << i <<" "<< j <<" "<< k  <<" "<< G[i][j][k] << "\n";
                        std::cout  << z <<" "<< t <<" "<< G[i][j][k] << "<" << G[z][j][k] << "+"<<G[t][j][k] << "\n";

                        G[i][j][k] = G[i][j][z]+G[i][j][t];
                        pos[i][j][k] = Rhat[z];
                        guil[i][j][k] = Horz;  // Horizontal cut; parallel to xy plane
                    }
                }
            }
        }
    }

    // collect results together for return to calling code
    sPattern P;
    P.instance = problem;
    P.lCount = m;
    P.wCount = s;
    P.hCount = u;
    P.value = G;
    P.position = pos;
    P.direction = guil;
    P.item = item;
    P.l_raster = Phat;
    P.w_raster = Qhat;
    P.h_raster = Rhat;

    return P;
}

std::string sPattern::text() const
{
    std::stringstream ss;

//    ss << "Total value of solution "
//       << value[lCount-1][wCount-1][hCount-1] << "\n";

    int totalCuts = 0;
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                if( direction[il][iw][ih]  )
                    totalCuts++;
            }
        }
    }
    if( ! totalCuts )
    {
        ss << "\nno cuts found\n"
           << "Probably means that items were too big for bin\n";
        return ss.str();
    }

    ss << "Vertical cuts at ";
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                if( direction[il][iw][ih] == 1 )
                    ss << position[il][iw][ih] << " ";
            }
        }
    }
    ss << "\nDepth Cuts at ";
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                if( direction[il][iw][ih] == 2 )
                    ss << position[il][iw][ih] << " ";
            }
        }
    }
    ss << "\nHorizontal Cuts at ";
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                if( direction[il][iw][ih] == 3 )
                    ss << position[il][iw][ih] << " ";
            }
        }
    }

    int itemCount = 0;
    int totalValue = 0;
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                itemCount++;
                totalValue += instance.item_values[ item[il][iw][ih] ];
                std::cout
                        << "item type " << item[il][iw][ih]
                        << " value " << instance.item_values[ item[il][iw][ih] ]
                        << " at " << l_raster[il] <<" "<< w_raster[iw] <<" "<< h_raster[ih] << "\n";
            }
        }
    }
    std::cout << itemCount << " items, total value " << totalValue << "\n\n";


    return ss.str();
}


