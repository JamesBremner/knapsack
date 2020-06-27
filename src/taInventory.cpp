#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include "TimberAllocation.h"

namespace ta
{
void cInventory::clear()
{
    myInventory.clear();
    myStock.clear();
    mySheet.clear();
    myScrap.clear();
}
void cInventory::add( timber_t t )
{
    myInventory.push_back( t );
}
void cInventory::expandCount()
{
    cInstance::expandCount( myInventory );
}
std::string cInventory::text()
{
    std::stringstream ss;
    ss << "Inventory " << myInventory.size() << " timbers\n"
       << "Stock contains " << myInventory.size()  << " timbers\n"
       << "Sheet inventory " << mySheet.size() << "\n"
       << "Scrap inventory " << myScrap.size() << "\n";
    return ss.str();
}

void cInventory::sortInventory( int sheetHeight, int scrapWidth )
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
