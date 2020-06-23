#include <iostream>
#include <vector>
#include "knapsack.h"

void testRRP()
{
    std::cout << "DDP\n";
    auto r = DDP( 10, { 5, 7 } );
    for( int x : r )
        std::cout << x<< " ";
    std::cout << "\n";

    std::cout << "RRP\n";
    r = RRP( 10, { 5, 7 } );
    for( int x : r )
        std::cout << x<< " ";
    std::cout << "\n";
}

void test1()
{
     sInstance problem;

    // single 7 unit cube item type
    std::vector<int> l  = { 7 };
    std::vector<int> w  = { 7 };
    std::vector<int> h  = { 7 };

    // 10 unit cube bin
    problem.bin = std::vector<int>(3,10);

    problem.item_values = { 5 };

    auto P = DP3UK(
          l,w,h,
          problem );

    std::cout << P.text() << "\n";
}

void test2()
{
    sInstance problem;

    // single 5 unit cube item type
    std::vector<int> l  = { 4 };
    std::vector<int> w  = { 4 };
    std::vector<int> h  = { 4 };

    // 10 unit cube bin
    problem.bin = std::vector<int>(3,12);

    problem.item_values = { 5 };

    auto P = DP3UK(
          l,w,h,
          problem );

    std::cout << P.text() << "\n";
}

void test3()
{
    sInstance problem;

    // two item types, a 8 unit cube and a 2 unit cube
    std::vector<int> l  = { 8, 2 };
    std::vector<int> w  = { 8, 2 };
    std::vector<int> h  = { 8, 2 };

    // 10 unit cube bin
    problem.bin = std::vector<int>(3,10);

    // make sure the 8 unit cube is included
    problem.item_values = { 1000, 1 };

    auto P = DP3UK(
          l,w,h,
          problem );

    std::cout << P.text() << "\n";
}
int main()
{
    std::cout << "DP3UK\n";

    //testRRP();
    // test1();
    test2();
    //test3();

    return 0;
}

