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


    std::string myUserID;
    int myDemand;

};
class cInstance
{
public:
    void read( const std::string& fname );
    std::string text();
    void sortInventory( int sheetHeight, int scrapWidth );


private:
    timberv_t myInventory;
    timberv_t myOrder;
    timberv_t myScrap;
    timberv_t mySheet;
    std::vector< int > ParseSpaceDelimited(
        const std::string& l );
    void add( std::vector< int > );
    void expandDemand( timberv_t& tv );
};
}

