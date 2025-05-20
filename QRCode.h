
//Important file for all the tables and help functions for the QRcode class
#include "QRCodeHelp.hpp"


class QRcodeHeader{

private:

    //mark a square in the QRcode has occupied and cant be modified
    virtual void OccupySquare() {}

    // place a quare in the QRcode
    virtual void InitialiseBlack(CoordXY Coords){}
    virtual void InitialiseWhite(CoordXY Coords){}

    // initialise the corner without the white borders 
    virtual void InitialiseCorner(CoordXY TopLeftCoords){}



// ////////////////////////////////////////////////////////////////////////////////////////////////

public:

    //normal constructeur that builds all the components of 
    //QRcode(){}

    
    // print the occupied squares of the QRcode
    virtual void printOccupyDebug(){}

    // prints the QRcode to show the coordinates
    virtual void printQRcodeDebug(){}

    //prints the QRcode
    virtual void printQRcode(){}

};//QRcodeHeader



