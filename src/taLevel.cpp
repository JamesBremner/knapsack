#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include "TimberAllocation.h"

namespace ta
{
void cLevel::removePacked()
{
    myOrder.erase(
        remove_if(
            myOrder.begin(),
            myOrder.end(),
            [] ( timber_t t )
    {
        return( t->isPacked() );
    } ),
    myOrder.end() );
}
std::string cLevel::text() const
{
    std::stringstream ss;
    ss <<"\nLEVEL " << height() <<" ";
    for( timber_t t : myOrder )
    {
        ss << t->myUserID << "\n";
    }
    return ss.str();
}
}


