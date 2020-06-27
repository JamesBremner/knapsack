#pragma once

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
    /// rotate so that L, W, H are in decending size order
    void rotateLWH();
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

    std::string text();

    std::string myUserID;
    int myCount;

};

class cInventory
{
public:
    void clear();
    void add( timber_t t );
    void expandCount();
    std::string text();

    /** Sort inventory into stock, sheets and scraps
    @param[in] sheetHeight maximum sheet height
    @param[in] scrapWidth maximum scrap width

    This will allow inentory returns, if present,
    to be optimised.

    tid7
    */
    void sortInventory( int sheetHeight, int scrapWidth );

public:
    timberv_t myInventory;      /// inentory ( all timbers labeled 'i' in the instance file )
    timberv_t myScrap;          /// inventory fpr 1D cutting
    timberv_t mySheet;          /// inventory for 2D cutting
    timberv_t myStock;          /// invemtory for 3D cutting
};

class cInstance
{
public:
    void read(
        cInventory& Inventory,
        const std::string& fname );

    std::string textProblem();
    std::string textSolution();

    /** Expand multiple timbers
        @param[in] tv vector of timbers
    */
    static void expandCount( timberv_t& tv );


    timberv_t myOrder;          /// the timbers that have to be delivered
    std::vector<std::pair<timber_t,timber_t>> myAllocation;

private:

    /// Parse a line in the instance file
    std::vector< int > ParseSpaceDelimited(
        const std::string& l );

    /** Expand multiple timbers
        @param[in] tv vector of timbers
    */
    void expandCount();
};
/** allocate tombers of same height to levels
@param[in] order
@return vector of vectors, each containing timbers of the same height
*/
std::vector< timberv_t >
Levels( timberv_t& order);

std::vector<std::pair<timber_t,timber_t>>
LevelsToStock(
    std::vector< timberv_t >& levels,
    timberv_t& stock );

}

