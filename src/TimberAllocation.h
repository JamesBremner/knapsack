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
    int myCount;

};
class cInstance
{
public:
    void read( const std::string& fname );
    std::string text();

    /** Sort inventory into stock, sheets and scraps
        @param[in] sheetHeight maximum sheet height
        @param[in] scrapWidth maximum scrap width
    */
    void sortInventory( int sheetHeight, int scrapWidth );


private:
    timberv_t myInventory;      /// inentory ( all timbers labeled 'i' in the instance file )
    timberv_t myOrder;          /// the timbers that have to be delivered
    timberv_t myScrap;          /// inventory fpr 1D cutting
    timberv_t mySheet;          /// inventory for 2D cutting
    timberv_t myStock;          /// invemtory for 3D cutting

    /// Parse a line in the instance file
    std::vector< int > ParseSpaceDelimited(
        const std::string& l );

    /** Expand multiple timbers
        @param[in] tv vector of timbers
    */
    void expandCount();
    void expandCount( timberv_t& tv );
};
}

