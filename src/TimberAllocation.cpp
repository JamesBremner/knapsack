#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

using namespace std;

namespace ta
{

class cTimber;
typedef std::shared_ptr< cTimber > timber_t;
typedef std::vector< timber_t > timberv_t;


/// A 3D object with dimension and location
class cSpace
{
public:
    int myLength, myWidth, myHeight;    // dimensions
    int myLocL, myLocW, myLocH;


    cSpace( int L, int W, int H )
        : myLength( L ), myWidth( W ), myHeight{ H }
    {

    }
    bool fit( const cSpace& squeeze ) const
    {
        return ( squeeze.myLength <= myLength &&
                 squeeze.myLength <= myLength &&
                 squeeze.myHeight <= myHeight );
    }

    int size_horiz()
    {
        return myLength * myWidth;
    }
};

class cTimber : public cSpace
{
public:
    cTimber()
    : cSpace( 0, 0, 0 )
    {

    }
    cTimber( int L, int W, int H )
        : cSpace( L, W, H )
    {

    }
    int ParseSpaceDelimited(
        const std::string& l );
private:
    std::string myUserID;
    int myDemand;

};
class cInstance
{
public:
    void read( const std::string& fname );

private:
    timberv_t myInventory;
    timberv_t myOrder;
    std::vector< int > ParseSpaceDelimited(
        const std::string& l );
    void add( std::vector< int > );
};

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
    myDemand = atoi(token[4].c_str());
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

    ta::cInstance I;
    I.read( argv[1] );

    return 0;
}
