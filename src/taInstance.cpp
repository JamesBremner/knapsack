#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include "TimberAllocation.h"

using namespace std;

namespace ta
{

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
    isEveryIDUnique();
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

        // construct required number of copies, append copy number to userID
        for( int k = 0; k < t->myCount-1; k++ )
        {
            timber_t newTimber( new cTimber( *t.get() ) );
            newTimber->myUserID = t->myUserID + ":" + std::to_string( k+2 );
            ex.push_back( newTimber );
        }
    }
    tv.insert(
        tv.end(),
        ex.begin(), ex.end() );
}

std::string cInstance::textSolution()
{
    std::stringstream ss;
    for( auto& a : myAllocation )
    {
        ss << "a " << a.second->myUserID
           <<" "<< a.first->myUserID << "\n";
    }
    for( auto& c : myCut )
    {
        ss << c.text() << "\n";
    }
    for( auto& u : myUnpacked )
    {
        ss << "u " << u->myUserID << "\n";
    }
    return ss.str();
}
void cInstance::addOrder( timber_t t )
{
    myOrder.push_back( t );
}
void cInstance::addUnpacked( timberv_t& unpacked )
{
    myUnpacked.insert(
        myUnpacked.end(),
        unpacked.begin(), unpacked.end() );
}
bool cInstance::isEveryIDUnique()
{
    std::set<std::string> IDset, dupIDset;
    bool first = true;
    for( timber_t t: myOrder )
    {
        if( ! IDset.insert( t->myUserID ).second )
        {
            if( dupIDset.insert( t->myUserID ).second )
            {
                if( first )
                {
                    std::cout << "Discarding duplicate IDs ( tid11 ):\n";
                    first = false;
                }
                std::cout << t->myUserID << " ";
            }
        }
    }

    myOrder.erase(
        remove_if(
            myOrder.begin(),
            myOrder.end(),
            [&dupIDset] ( timber_t t )
    {
        return( dupIDset.find(t->myUserID) != dupIDset.end() );
    } ),
    myOrder.end() );

    std::cout <<  "\n" << myOrder.size() << " unique order IDs\n";
    return true;
}
}

