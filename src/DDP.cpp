#include <iostream>
#include <vector>

using namespace std;

std::vector<int> DDP(
    int D,
    std::vector<int>& d )
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

    D = 10;
    d  = { 1,2,3,4 };

    std::vector<int> c( d.size() );

    for( int i = 0; i < (int)d.size(); i++ )
    {
        for( int j = d[i]; j < D; j++ )
        {
            if( c[ j ] < c[ j - d[i] ] + d[ i ] )
                c[ j ] = c[ j - d[i] ] + d[ i ];
        }
    }
    for( int j = 0; j < D; j++ )
    {
        if( c[ j ] == j )
            P.push_back( j );
    }

    return P;
}

int main()
{
    cout << "DDP" << endl;

    int D = 10;
    std::vector<int> d  = { 1,2,3,4 };

    std::vector<int> P = DDP( D, d );

    std::cout << "D = " << D << "\ndi = ";
    for( int di : d )
        std::cout << di << " ";
    std::cout << "\nP = ";
    for( int p : P )
        std::cout << p << " ";
    std::cout << "\n";

    return 0;
}
