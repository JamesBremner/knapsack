#include <iostream>
#include <vector>
#include "knapsack.h"

using namespace std;

void text(
    int D,
    std::vector<int>& d,
    std::vector<int>& P )
{
    std::cout << "D = " << D << "\ndi = ";
    for( int di : d )
        std::cout << di << " ";
    std::cout << "\nP = ";
    for( int p : P )
        std::cout << p << " ";
    std::cout << "\n\n";
}

int main()
{
    cout << "DDP" << endl;

    int D = 10;
    std::vector<int> d  = { 1,2,3,4 };

    std::vector<int> P = DDP( D, d );

    text( D, d, P );

    int D2 = 10;
    std::vector<int> d2  = { 3,7 };

    P = DDP( D2, d2 );

    text( D2, d2, P );

    return 0;
}

