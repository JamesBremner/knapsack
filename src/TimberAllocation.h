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
        , myPacked( false )
    {

    }
    cTimber( int L, int W, int H )
        : cSpace( L, W, H )
    {

    }
    int ParseSpaceDelimited(
        const std::string& l );

    void pack( int l, int w, int h, timber_t stock );

    std::string text();

    std::string myUserID;
    int myCount;

private:
    bool myPacked;
    timber_t myStock;
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
class cCut
{
public:
    timber_t myStock;
    char myDirection;
    int myLocation;
    cCut( timber_t stock, char D, int loc )
        : myStock( stock )
        , myDirection( D )
        , myLocation( loc )
    {

    }
    std::string text();
};

class cLevel
{
public:
    timberv_t myOrder;
    timber_t  myStock;
    int height()
    {
        return myOrder[0]->myHeight;
    }
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

    std::vector< cCut > myCut;

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
@return vector of levels, each containing timbers of the same height
*/
std::vector< cLevel >
Levels( timberv_t& order);

/** allocate levels to stock
    @param[out] I the instance
    @param[in] levels
    @param[in] stock inventory
*/
void
LevelsToStock(
    cInstance& I,
    std::vector< cLevel >& levels,
    timberv_t& stock );

/** cut out the orders
    @param[out] I the instance
    @param[in] levels
*/
void LevelCuts(
    cInstance& I,
    std::vector< cLevel >& levels );

/** Cutting stock 2 dimensional no width
    @param[out] I the instance
    @param[in level
    @param[in] h height in stock

    This is an extremely simple 2D cutting stock algorithm
    to be used as a placeholder in the H3CS algorithm.

    Currently it does not provide an interface allowing
    levels to be stacked, not for a level to be distributed
    over multiple stock timbers.  I will be adding this.

    This makes no effort to stack in the width dimension.

    The effect is that this will fail if all the orders in
    a level cannot be cut in one line along the bottom of the
    allocated stock timber.  Even if it does not fail, the wastage is
    likely to be enormous!
*/
void CS2LNW(
          cInstance& I,
          cLevel& level, int h );
}

