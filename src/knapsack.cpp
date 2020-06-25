#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
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

std::string text( eCut cut )
{
    switch( cut )
    {
    case eCut::vert:
        return "Vertical";
    case eCut::depth:
        return "Depth";
    case eCut::horz:
        return "Horizontal";
    default:
        return "nil";
    }
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


std::string sPattern::text() const
{
    std::stringstream ss;

    int lCount = l_raster.size();
    int wCount = w_raster.size();
    int hCount = h_raster.size();

    int totalCuts = 0;
    if( instance.stageCount )
    {
        // staged solution
        for( int kstage = 1; kstage < instance.stageCount; kstage++ )
        {
            int stageCuts = 0;
            for( int il = 0; il < lCount; il++ )
            {
                for( int iw = 0; iw < wCount; iw++ )
                {
                    for( int ih = 0; ih < hCount; ih++ )
                    {
                        if( direction[kstage][il][iw][ih]  )
                            stageCuts++;
                    }
                }
            }
            if( instance.stageCount )
            {
                ss << textCuts( kstage, eCut::vert);
                ss << textCuts( kstage, eCut::depth);
                ss << textCuts( kstage, eCut::horz);
                ss << "\n";
            }
            totalCuts += stageCuts;
        }
    }
    else
    {
        // unstaged
        for( int il = 0; il < lCount; il++ )
        {
            for( int iw = 0; iw < wCount; iw++ )
            {
                for( int ih = 0; ih < hCount; ih++ )
                {
                    if( direction[0][il][iw][ih]  )
                        totalCuts++;
                }
            }
        }
        ss << textCuts( 0, eCut::vert);
        ss << textCuts( 0, eCut::depth);
        ss << textCuts( 0, eCut::horz);
        ss << "\n";
    }

    if( ! totalCuts )
    {
        ss << "\nno cuts found\n"
           << "Probably means that items were too big for bin\n";
        return ss.str();
    }

    int itemCount = 0;
    int totalValue = 0;
    for( int il = 0; il < lCount; il++ )
    {
        for( int iw = 0; iw < wCount; iw++ )
        {
            for( int ih = 0; ih < hCount; ih++ )
            {
                itemCount++;
                totalValue += instance.item_values[ item[0][il][iw][ih] ];
                std::cout
                        << "item type " << item[0][il][iw][ih]
                        << " value " << instance.item_values[ item[0][il][iw][ih] ]
                        << " at " << l_raster[il] <<" "<< w_raster[iw] <<" "<< h_raster[ih] << "\n";
            }
        }
    }

    std::cout << itemCount << " items, total value " << totalValue << "\n\n";


    return ss.str();
}
std::string sPattern::textCuts( int stage, eCut cut ) const
{
    std::stringstream ss;
    std::set<int> cutset;
    for( int il = 0; il < (int)l_raster.size(); il++ )
    {
        for( int iw = 0; iw < (int)w_raster.size(); iw++ )
        {
            for( int ih = 0; ih < (int)h_raster.size(); ih++ )
            {
                if( direction[stage][il][iw][ih] == (int)cut )
                {
                    cutset.insert( position[stage][il][iw][ih] );
                }
            }
        }
    }
    if( cutset.size() )
    {
        if( stage )
            ss << "Stage " << stage << " ";

        ss << cutset.size() << " " << ::text( cut ) << " cuts at ";
        for( int c : cutset )
        {
            ss << c << " ";
        }
        ss << "\n";
    }
    return ss.str();
}
