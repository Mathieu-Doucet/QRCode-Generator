
//Important file for all the tables and help functions for the QRcode class
#include "QRCodeHelp.hpp"


class QRcodeHeader{

public:

    // place 
    virtual void InitialiseBlack(CoordXY Coords){}
    virtual void InitialiseWhite(CoordXY Coords){}

    // initialise the corner without the white borders 
    virtual void InitialiseCorner(CoordXY TopLeftCoords){}



// ////////////////////////////////////////////////////////////////////////////////////////////////

public:

    //normal constructeur that builds all the components of 
    //QRcode(){}



    // prints the QRcode so show the coordantes
    virtual void printQRcodeDebug(){}

    //prints the QRcode
    virtual void printQRcode(){}

};//QRcodeHeader



