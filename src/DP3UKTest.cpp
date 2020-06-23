#include <iostream>
#include <vector>
#include "knapsack.h"

using namespace std;


int main()
{
    cout << "DP3UK" << endl;

    sInstance problem;
    int L = 10;
    int W = 10;
    int H = 10;

    // single 5 unit cube item type
    std::vector<int> l  = { 7 };
    std::vector<int> w  = { 7 };
    std::vector<int> h  = { 7 };
    problem.item_values = { 5 };

    auto P = DP3UK(
          L,W,H,
          l,w,h,
          problem );

    std::cout << P.text() << "\n";

    return 0;
}

