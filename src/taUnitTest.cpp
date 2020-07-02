#include <iostream>
#include <memory>
#include <cutest.h>
#include "TimberAllocation.h"

using namespace std;

TEST( tid9 )
{
//    i 1000 600 200 1 i1
//d 500 300 200 4 o1

    ta::cInstance I;
    ta::cInventory theInventory;
    ta::timber_t T( new ta::cTimber(1000, 600, 200 ) );
    T->myUserID = "i1";
    theInventory.add( T );
    ta::timber_t O( new ta::cTimber(500, 300, 200 ) );
    O->myUserID = "o1";
    O->myCount = 4;
    I.addOrder( O );
    I.expandCount();


    // sort inventory into stock, sheets and scraps
    theInventory.sortInventory( 100, 100 );
    std::cout << theInventory.textDetails();

    // sort orders into levels of the same height
    auto levels = Levels( I );

    // allocate levels to stock
    LevelsToStock( I, levels, theInventory );

    // cut out orders from stock by level
    LevelCuts( I, levels, theInventory );

    // check cut count and positions
    CHECK_EQUAL( 2, I.myCut.size() );
    CHECK_EQUAL( 300, I.myCut[0].myLocation );
    CHECK_EQUAL( 500, I.myCut[1].myLocation );
    CHECK_EQUAL( 0, I.unPackedCount() );
}

TEST( twoLevels )
{
//    i 1000 600 400 1 i1
//d 500 300 200 8 o1

    ta::cInstance I;
    ta::cInventory theInventory;
    ta::timber_t T( new ta::cTimber(1000, 600, 400 ) );
    T->myUserID = "i1";
    theInventory.add( T );
    ta::timber_t O( new ta::cTimber(500, 300, 200 ) );
    O->myUserID = "o1";
    O->myCount = 8;
    I.addOrder( O );
    I.expandCount();


    // sort inventory into stock, sheets and scraps
    theInventory.sortInventory( 100, 100 );

    // sort orders into levels of the same height
    auto levels = Levels( I );

    // allocate levels to stock
    LevelsToStock( I, levels, theInventory );

    // cut out orders from stock by level
    LevelCuts( I, levels, theInventory );

    //std::cout << I.textSolution();

    // check cut count and positions
    CHECK_EQUAL( 5, I.myCut.size() );
    CHECK_EQUAL( 200, I.myCut[0].myLocation );
    CHECK_EQUAL( 300, I.myCut[1].myLocation );
    CHECK_EQUAL( 500, I.myCut[2].myLocation );
    CHECK_EQUAL( 300, I.myCut[3].myLocation );
    CHECK_EQUAL( 500, I.myCut[4].myLocation );
    CHECK_EQUAL( 0, I.unPackedCount() );
}

TEST( shortStock )
{
//    i 1000 600 400 1 i1
//d 500 300 200 12 o1

    ta::cInstance I;
    ta::cInventory theInventory;
    ta::timber_t T( new ta::cTimber(1000, 600, 400 ) );
    T->myUserID = "i1";
    theInventory.add( T );
    ta::timber_t O( new ta::cTimber(500, 300, 200 ) );
    O->myUserID = "o1";
    O->myCount = 12;
    I.addOrder( O );
    I.expandCount();


    // sort inventory into stock, sheets and scraps
    theInventory.sortInventory( 100, 100 );

    // sort orders into levels of the same height
    auto levels = Levels( I );

    // allocate levels to stock
    LevelsToStock( I, levels, theInventory );

    // cut out orders from stock by level
    LevelCuts( I, levels, theInventory );

    //std::cout << I.textSolution();

    // check cut count and positions
    CHECK_EQUAL( 5, I.myCut.size() );
    CHECK_EQUAL( 200, I.myCut[0].myLocation );
    CHECK_EQUAL( 300, I.myCut[1].myLocation );
    CHECK_EQUAL( 500, I.myCut[2].myLocation );
    CHECK_EQUAL( 300, I.myCut[3].myLocation );
    CHECK_EQUAL( 500, I.myCut[4].myLocation );
    CHECK_EQUAL( 4, I.unPackedCount() );
}

int main(  )
{
    raven::set::UnitTest::RunAllTests();

}
