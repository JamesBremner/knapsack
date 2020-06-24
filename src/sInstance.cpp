#include <fstream>
#include <sstream>
#include <iostream>
#include "knapsack.h"

std::vector< int >
ParseSpaceDelimited(
    const std::string& l )
{
    std::vector< int > output;
    std::stringstream sst(l);
    std::string a;
    while( getline( sst, a, ' ' ) )
        output.push_back( atoi(a.c_str()));
    return output;
}

void sInstance::read( const std::string& fname )
{
    std::ifstream f( fname );
    if( ! f.is_open() )
        throw std::runtime_error("Cannot read instance file " + fname );

    l.clear();
    w.clear();
    h.clear();

    int box_type_count;
    int lcount = 0;
    std::string line;
    while( std::getline( f, line ) )
    {
        //std::cout << lcount << " " << line << "\n";
        if( lcount == 0 )
        {
            box_type_count = atoi( line.c_str() );
            lcount++;
            continue;
        }
        if( lcount == 1 )
        {
            bin = ParseSpaceDelimited( line );
            lcount++;
            continue;
        }
        if( lcount < 2 + box_type_count )
        {
            auto lv = ParseSpaceDelimited( line );
            l.push_back( lv[0] );
            w.push_back( lv[1] );
            h.push_back( lv[2] );
            item_values.push_back( lv[0]*lv[1]*lv[2] );
            lcount++;
            continue;
        }
        myName = line;
    }
    std::cout << "read problem " << myName << "\n";
}

    std::string sInstance::text()
    {
        std::stringstream ss;
        for( int k = 0; k < (int)l.size(); k++ )
            ss << l[k] <<" "<< w[k] <<" " << h[k] << "\n";
        return ss.str();
    }
