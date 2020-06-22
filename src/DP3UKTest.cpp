#include <iostream>
#include <vector>
#include "knapsack.h"

using namespace std;


int main()
{
    cout << "DP3UK" << endl;

    int L = 10;
    int W = 10;
    int H = 10;
    std::vector<int> l  = { 3,7 };
    std::vector<int> w  = { 3,7 };
    std::vector<int> h  = { 3,7 };
    std::vector<int> v  = { 3,7 };

    DP3UK(
          L,W,H,
          l,w,h,
          v );



    return 0;
}

