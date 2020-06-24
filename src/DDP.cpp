#include <vector>
#include <iostream>

std::vector<int> DDP(
    int D,
    const std::vector<int>& d )
{
    /*
    Algorithm 2.2 DDP
    Input: D, d1, . . ., dm.
    Output: a set P of discretization points.
    P = {0}.
    For j = 0 to D do cj = 0.
    For i = 1 to m do
    For j = di to D
    If cj < cj-di + di then cj = cj-di + di.
    For j = 1 to D
    If c
    j = j then P = P ? {j}.
    Return P.
    */

    std::vector<int> P;          // calculated discretization points

    std::vector<int> c( D+1 );

    for( int i = 0; i < (int)d.size(); i++ )
    {
        for( int j = d[i]; j <= D; j++ )
        {
            if( c[ j ] < c[ j - d[i] ] + d[ i ] )
                c[ j ] = c[ j - d[i] ] + d[ i ];
        }
    }
    for( int j = 1; j <= D; j++ )
    {
        if( c[ j ] == j )
            P.push_back( j );
    }

    return P;
}
