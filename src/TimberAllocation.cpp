#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include "TimberAllocation.h"

using namespace std;

namespace ta
{

int cTimber::ParseSpaceDelimited(
    const std::string& l )
{
    std::vector< std::string > token;
    std::stringstream sst(l);
    std::string a;
    while( getline( sst, a, ' ' ) )
        token.push_back( a );
    if( token.size() < 6 )
        throw std::runtime_error(" Error reading: " + l );
    myLength = atoi(token[1].c_str());
    myWidth = atoi(token[2].c_str());
    myHeight = atoi(token[3].c_str());
    myCount = atoi(token[4].c_str());
    myUserID = token[5];

    if( token[0] == "i" )
        return 0;
    else  if( token[0] == "d" )
        return 1;
    else
        throw std::runtime_error(" Error reading: " + l );
}

std::string cTimber::text()
{
     std::stringstream ss;
     ss <<  myLength <<" "<< myWidth <<" "<< myHeight <<" "<< myUserID;
     return ss.str();
}

void cTimber::pack( int l, int w, int h, timber_t stock )
{
    myPacked = true;
    myLocL = l;
    myLocW = w;
    myLocH = h;
    myStock = stock;
}

void cInstance::read(
    cInventory& Inventory,
    const std::string& fname )
{
    std::ifstream f( fname );
    if( ! f.is_open() )
        throw std::runtime_error("Cannot read instance file " + fname );

    Inventory.clear();
    myOrder.clear();

    // loop over lines in file
    std::string line;
    while( std::getline( f, line ) )
    {
        //std::cout << lcount << " " << line << "\n";
        timber_t T( new cTimber() );
        switch ( T->ParseSpaceDelimited( line ) )
        {
        case 0:     // inventory
            Inventory.add( T );
            break;
        case 1:     // demand
            myOrder.push_back( T );
            break;
        }
    }
    Inventory.expandCount();
    expandCount();
}

void cInstance::expandCount()
{
    expandCount( myOrder );
}
void cInstance::expandCount( timberv_t& tv )
{
    timberv_t ex;
    for( auto& t : tv )
    {
        if( t->myCount <= 0 )
            throw std::runtime_error("Bad count for " + t->myUserID );
        for( int k = 0; k < t->myCount-1; k++ )
            ex.push_back( timber_t( new cTimber( *t.get() )));
    }
    tv.insert(
        tv.end(),
        ex.begin(), ex.end() );
}

std::string cInstance::textSolution()
{
    std::stringstream ss;
    for( auto& a : myAllocation ) {
        ss << "a " << a.second->myUserID
            <<" "<< a.first->myUserID << "\n";
    }
    for( auto& c : myCut )
    {
        ss << c.text() << "\n";
    }
    return ss.str();
}

std::string cCut::text()
{
    stringstream ss;
    ss << "c " << myStock->myUserID << " "
        << myDirection <<" "<< myLocation <<" "
        << myLevelHeight;
    return ss.str();
}


}
int main( int argc, char* argv[] )
{
    cout << "TimberAllocation" << endl;

    if( argc != 2 )
    {
        std::cout << "Usage: TimberAllocation <path to instance file>\n";
        exit(1);
    }

    try
    {
        ta::cInstance I;
        ta::cInventory theInventory;

        // read problem instance file
        I.read( theInventory, argv[1] );

        // sort inventory into stock, sheets and scraps
        theInventory.sortInventory( 1000, 100 );

        // sort orders into levels of the same height
        auto levels = Levels( I.myOrder );

        // allocate levels to stock
        LevelsToStock( I, levels, theInventory.myStock );

        LevelCuts( I, levels );

        // display solution
        std::cout << I.textSolution();
    }
    catch( std::runtime_error& e )
    {
        std::cout << "exception: " << e.what() << "\n";
    }
    return 0;
}
