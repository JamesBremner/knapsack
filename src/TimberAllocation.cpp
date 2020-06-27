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

void cInstance::read(
    const std::string& fname )
{
    std::ifstream f( fname );
    if( ! f.is_open() )
        throw std::runtime_error("Cannot read instance file " + fname );

    myInventory.clear();
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
            myInventory.push_back( T );
            break;
        case 1:     // demand
            myOrder.push_back( T );
            break;
        }
    }
    expandCount();
}

void cInstance::expandCount()
{
    expandCount( myInventory );
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

std::string cInstance::text()
{
    std::stringstream ss;
    ss << "Stock contains " << myInventory.size()  << " timbers\n"
       << "Sheet inventory " << mySheet.size() << "\n"
       << "Scrap inventory " << myScrap.size() << "\n"
       << "Order demands " << myOrder.size() << " timbers\n";
    return ss.str();
}

void cInstance::sortInventory( int sheetHeight, int scrapWidth )
{
    // rotate, if neccesary, so L > W > H
    for( auto t : myInventory )
    {
        t->rotateLWH();
    }

    // assign inventory timbers to sub inventories
    for( auto t : myInventory )
    {
        if( t->myWidth > scrapWidth && t->myHeight > sheetHeight )
            myStock.push_back( t );
        else if ( t->myWidth > scrapWidth )
            mySheet.push_back( t );
        else
            myScrap.push_back( t );
    }
}

}
int main( int argc, char* argv[] )
{
    cout << "TimberAllocation1" << endl;

    if( argc != 2 )
    {
        std::cout << "Usage: TimberAllocation <path to instance file>\n";
        exit(1);
    }

    try
    {
        ta::cInstance I;
        I.read( argv[1] );
        std::cout << I.text();
        I.sortInventory( 1000, 100 );
        std::cout << I.text();
    }
    catch( std::runtime_error& e )
    {
        std::cout << "exception: " << e.what() << "\n";
    }
    return 0;
}
