#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Important file for all the tables and help functions for the QRcode class
#include "QRCodeHelp.hpp"


class QRcode{

public:

string URL;

private:

// Go look QRCodeHelp.hpp or the documentation on the Variables
short DimensionQRcode= 41;
unsigned short Size : 6; // (max version is 40)
Errorcorrection ErrorCorrection = L; // Light for now
QRcodeMode Mode = Byte; // Byte is the standard for urls


// allocate an dynamic array initialized for version 6 QRcode for the standard url
vector<vector<bool>> QRcodeArray = vector<vector<bool>>(DimensionQRcode,(vector<bool>(DimensionQRcode, 0)));


// ////////////////////////////////////////////////////////////////////////////////////////////////


// double cout the char to make the the QRcode in the terminal a square
void printBlack(){
    cout << "░" << "░";
}
void printWhite(){
    cout << "▓" << "▓";
}

// place 
void InitialiseBlack(CoordXY Coords){
    QRcodeArray[Coords.Y][Coords.X] = 1;
}
void InitialiseWhite(CoordXY Coords){
    QRcodeArray[Coords.Y][Coords.X] = 0;
}

// initialise the corner without the white borders 
void InitialiseCorner(CoordXY TopLeftCoords){

    // top left corner always the same (0,0) -> (7,7)
    for(int Row = 0; Row<=6; Row++){
        
        CoordXY Cursor(TopLeftCoords.X, TopLeftCoords.Y + Row);
        
        // Always starts with black
        InitialiseBlack(Cursor);
        Cursor.X++;

        // column [1,5]

        // all black row 
        if (Row == 0 || Row == 6){
                
            for (int Column = Cursor.X; Column < (TopLeftCoords.X + 6); Column++) {
                InitialiseBlack(Cursor);
                Cursor.X++;
            }
                
        }
        // all white row
        else if (Row == 1 || Row == 5){
                
            for (int Column = Cursor.X; Column < (TopLeftCoords.X +6); Column++) {
                InitialiseWhite(Cursor);
                Cursor.X++;
            }
                
        }

        // White and black row
        else {

            InitialiseWhite(Cursor);
            Cursor.X++;

            for (int Column = Cursor.X; Column< (TopLeftCoords.X +5); Column++) {
                InitialiseBlack(Cursor);
                Cursor.X++;
            }
            InitialiseWhite(Cursor);
            Cursor.X++;
        }

        // Finishes with Black White
        InitialiseBlack(Cursor);

    }

}



// ////////////////////////////////////////////////////////////////////////////////////////////////

public:

QRcode(){

   
    CoordXY Topleft(0,0);
    CoordXY TopRight(DimensionQRcode-7, 0);
    CoordXY Bottomleft(0, DimensionQRcode-7);
    
    // finding patterns
    InitialiseCorner(Topleft);
    InitialiseCorner(TopRight);
    InitialiseCorner(Bottomleft);
    
    //timing patterns

    // starts form the bottomright finding pattern and between the separators
    CoordXY CursorVertical = {6,8};
    CoordXY CursorHorizontal = {8,6};

    //                   (DimensionQRcode - 8) -8 -1 = length between the two white separators (-1 for first index = 0)
    for (int i = 0 ; i <= DimensionQRcode - 17 ; i++){

        if ( i % 2 == 0){
            InitialiseBlack(CursorHorizontal);
            InitialiseBlack(CursorVertical);
        }
        else{
            InitialiseWhite(CursorHorizontal);
            InitialiseWhite(CursorVertical);
        }
        CursorHorizontal.X++;
        CursorVertical.Y++;
    }
    

}//QRcode

void printQRcodeDebug(){
    
    // counters for dimensions
    int Rows = 0;
    int Columns = 0;
    
    // iteration for the columns
    cout << "   ";
    for (int i = 0; i< QRcodeArray.size() ; i++ ){
        cout << Columns;

        // aligment
        cout << (Columns <= 9 ? " " : "");
        Columns++;

    }
    cout << endl;

    // Normal PrintQRcode()
    for (int i = 0; i< QRcodeArray.size() ; i++ ){
        
        // Iteration for the rows
        cout << Rows << (Rows <= 9 ? "  " : " ");
        Rows++;

        for (int j = 0; j< QRcodeArray[i].size() ; j++ ){


            QRcodeArray[i][j] == 1 ? printBlack() : printWhite();
        
        }
        cout << endl;

        

    }


}// PrintQRcodeDebug


void printQRcode(){


    for (int i = 0; i< QRcodeArray.size() ; i++ ){
        
        
        for (int j = 0; j< QRcodeArray[i].size() ; j++ ){


            QRcodeArray[i][j] == 1 ? printBlack() : printWhite();
        
        }
        cout << endl;

    }


}// PrintQRcode

};//QRcode class



