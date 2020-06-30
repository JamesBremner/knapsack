#pragma once

//#define USE_CS2LNW
#define USE_CS2Pack2

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
        , myUsed( false )
    {

    }
    cTimber( int L, int W, int H )
        : cSpace( L, W, H )
    {

    }
    int ParseSpaceDelimited(
        const std::string& l );

    /** Cut the order
        @param[in] l length position
        @param[in] w width position
        @param[in] k height position
        @param[in] stock timber from which order is cut
    */
    void pack( int l, int w, int h, timber_t stock );

    std::string text();

    bool isPacked() const
    {
        return myPacked;
    }

    void setUsed()
    {
        myUsed = true;
    }
    bool isUsed() const
    {
        return myUsed;
    }

    std::string myUserID;
    int myCount;

private:
    bool myPacked;
    bool myUsed;
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
    int myLevelHeight;
    cCut( timber_t stock, char D, int loc, int levelHeight )
        : myStock( stock )
        , myDirection( D )
        , myLocation( loc )
        , myLevelHeight( levelHeight )
    {

    }
    std::string text();
};

/// orders that have the same height
class cLevel
{
public:
    timberv_t myOrder;      // orders
    timber_t  myStock;      // stock allocated to level
    void removePacked();    // remove orders that have been packed
    std::string text() const;
    int height() const
    {
        return myOrder[0]->myHeight;
    }
    int size() const
    {
        return (int) myOrder.size();
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

    void addUnpacked( timberv_t& unpacked );


    timberv_t myOrder;          /// the timbers that have to be delivered

    std::vector<std::pair<timber_t,timber_t>> myAllocation;

    std::vector< cCut > myCut;

private:

    timberv_t myUnpacked;

    /// Parse a line in the instance file
    std::vector< int > ParseSpaceDelimited(
        const std::string& l );

    /** Expand multiple timbers
        @param[in] tv vector of timbers
    */
    void expandCount();
};
/** allocate tombers of same height to levels
@param[in] I the problem instance
@return vector of levels, each containing timbers of the same height
*/
std::vector< cLevel >
Levels( cInstance& I );

/** allocate levels to stock
    @param[out] I the instance
    @param[in] levels
    @param[in] stock inventory
*/
void
LevelsToStock(
    cInstance& I,
    std::vector< cLevel >& levels,
    cInventory& inventory );

/** allocate a level to a stock
    @param[out] I the instance
    @param[in] level
    @param[in] stock inventory
    @return true if allocation successful
*/
bool
LevelToStock(
    cInstance& I,
    cLevel& level,
    timberv_t& stock );

/** cut out the orders
    @param[out] I the instance
    @param[in] levels
*/
void LevelCuts(
    cInstance& I,
    std::vector< cLevel >& levels,
    cInventory& inventory );

/** Cutting stock 2 dimensional no width
    @param[out] I the instance
    @param[in level
    @param[in] h height in stock
    @return true if all timbers in level were packed

    This is an extremely simple 2D cutting stock algorithm
    to be used as a placeholder in the H3CS algorithm.

    This makes no effort to stack in the width dimension!

    The effect is that the orders will be cut from each level
    in one line along the side of the stock timbers.
    The wastage is likely to be enormous!
*/
bool CS2LNW(
    cInstance& I,
    cLevel& level, int h );

bool CS2Pack2(
    cInstance& I,
    cLevel& level, int h );

/** Allocate an order
    @param[in] I the instance
    @param[in] stock order is allocated to
    @param[in] order being allocated
    @param[[in] length position
    @param[[in] width position
    @param[[in] height position
*/
void AllocateOrder(
    cInstance& I,
    timber_t& stock,
    timber_t& order,
    int length, int width, int height );

/** Record the V cut for a level in the instance
    @param[in] I the instance
    @param[in] level
*/
void CutLevel(
    cInstance& I,
    cLevel& level );
}

