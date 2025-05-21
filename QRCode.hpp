#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Important file header 
// also includes QRCodeHelp.hpp for all the tables and help functions for the QRcode class
#include "QRCode.h"


class QRcode : public QRcodeHeader{

public:

string URL;

private:

    // Go look QRCodeHelp.hpp or the documentation on the Variables
    short DimensionQRcode= 41;
    unsigned short Version = 6; // (max version is 40)
    Errorcorrection ErrorCorrection = L; // Light for now
    QRcodeMode Mode = Byte; // Byte is the standard for urls


    // allocate an dynamic array initialized for version 6 QRcode for the standard url
    vector<vector<bool>> QRcodeArray = vector<vector<bool>>(DimensionQRcode,(vector<bool>(DimensionQRcode, 0)));

    // Allocate the same QRcode Size for places that CANT be modified because of the Function paterns (1 in the Array means occupied)
    vector<vector<bool>> OccupiedVector = vector<vector<bool>>(DimensionQRcode,(vector<bool>(DimensionQRcode, 0)));


    // ////////////////////////////////////////////////////////////////////////////////////////////////


    // double cout the char to make the the QRcode in the terminal a square
    void printBlack(){
        cout << "░░";
    }
    void printWhite(){
        cout << "▓▓";
    }

    void OccupySquare(CoordXY Coords){
        OccupiedVector[Coords.Y][Coords.X] = 1;
    }

    //place a square in the QRcode 
    void InitialiseBlack(CoordXY Coords){
        //place is not occupied
        if (OccupiedVector[Coords.Y][Coords.X] == 0){
            QRcodeArray[Coords.Y][Coords.X] = 1;
        }
        //is occupied (debugging)
        else{
            cout << "(" << Coords.X << "," <<  Coords.Y << ") Occupied" << endl;
        }
        
    }

    // TODO : for the Corner, timing and alignment patterns you dont have to initialise them because they are already white
    void InitialiseWhite(CoordXY Coords){
        //place is not occupied
        if (OccupiedVector[Coords.Y][Coords.X] == 0){
            QRcodeArray[Coords.Y][Coords.X] = 0;
        }
        //is occupied (debugging)
        else{
            cout << "(" << Coords.X << "," <<  Coords.Y << ") Occupied" << endl;
        }
    }


    



    // initialise the corner without the white borders 
    void InitialiseCorner(CoordXY TopLeftCoords){


        /*
        Finding pattern

        ░░ ░░ ░░ ░░ ░░ ░░ ░░ 
        ░░ ▓▓ ▓▓ ▓▓ ▓▓ ▓▓ ░░ 
        ░░ ▓▓ ░░ ░░ ░░ ▓▓ ░░ 
        ░░ ▓▓ ░░ ░░ ░░ ▓▓ ░░ 
        ░░ ▓▓ ░░ ░░ ░░ ▓▓ ░░ 
        ░░ ▓▓ ▓▓ ▓▓ ▓▓ ▓▓ ░░ 
        ░░ ░░ ░░ ░░ ░░ ░░ ░░ 
        */




        // top left corner always the same (0,0) -> (7,7)
        for(int Row = 0; Row<=6; Row++){
            
            CoordXY Cursor(TopLeftCoords.X, TopLeftCoords.Y + Row);
            
            // Always starts with black
            InitialiseBlack(Cursor);
            OccupySquare(Cursor);
            Cursor.X++;

            // column [1,5]

            // all black row 
            if (Row == 0 || Row == 6){
                    
                //░░ ░░ ░░ ░░ ░░ ░░ ░░ 
                for (int Column = Cursor.X; Column < (TopLeftCoords.X + 6); Column++) {
                    InitialiseBlack(Cursor);
                    OccupySquare(Cursor);
                    Cursor.X++;
                }
                    
            }
            // all white row
            else if (Row == 1 || Row == 5){
                    
                //░░ ▓▓ ▓▓ ▓▓ ▓▓ ▓▓ ░░ 
                for (int Column = Cursor.X; Column < (TopLeftCoords.X +6); Column++) {
                    InitialiseWhite(Cursor);
                    OccupySquare(Cursor);
                    Cursor.X++;
                }
                    
            }

            // White and black row
            else {

                // ░░ ▓▓ ░░ ░░ ░░ ▓▓ ░░ 
                InitialiseWhite(Cursor);
                OccupySquare(Cursor);
                Cursor.X++;

                for (int Column = Cursor.X; Column< (TopLeftCoords.X +5); Column++) {
                    InitialiseBlack(Cursor);
                    OccupySquare(Cursor);
                    Cursor.X++;
                }
                InitialiseWhite(Cursor);
                OccupySquare(Cursor);
                Cursor.X++;
            }

            // Finishes with Black White
            InitialiseBlack(Cursor);
            OccupySquare(Cursor);

        }

    }



    // Step 3: Add the Alignment Patterns
    //https://www.thonky.com/qr-code-tutorial/module-placement-matrix
    void InitialiseAlignementPattern(CoordXY Middle){

        /*
        Alignment Pattern

        ░░ ░░ ░░ ░░ ░░ 
        ░░ ▓▓ ▓▓ ▓▓ ░░ 
        ░░ ▓▓ ░░ ▓▓ ░░ 
        ░░ ▓▓ ▓▓ ▓▓ ░░ 
        ░░ ░░ ░░ ░░ ░░ 
        
        */

        //cursor to the top left square
        CoordXY Cursor(Middle.X-2 , Middle.Y -2);

        // ░░ ░░ ░░ ░░ ░░ 
        for (int i = 0 ; i < 5 ; ++i){
            
            InitialiseBlack(Cursor);
            OccupySquare(Cursor);
            Cursor.X++;
        }

        Cursor.Y++;
        Cursor.X = Middle.X-2;

        //  ░░ ▓▓ ▓▓ ▓▓ ░░ X 3
        
        for (int i = 0 ; i < 3 ; ++i ){

            

            InitialiseBlack(Cursor);
            OccupySquare(Cursor);

            for (int X = 0 ; X < 3 ; ++X){

                Cursor.X++;
                
                // black square in the middle of the alignment pattern
                if (Cursor.X == Middle.X && Cursor.Y == Middle.Y){
                    InitialiseBlack(Cursor);
                    OccupySquare(Cursor);
                    continue;
                }
                InitialiseWhite(Cursor);  
                OccupySquare(Cursor);
            }

            Cursor.X++;
            InitialiseBlack(Cursor);
            OccupySquare(Cursor);

            // go to the first collumn and the next row
            Cursor.X = Middle.X -2;
            Cursor.Y++;
            
        }


        // ░░ ░░ ░░ ░░ ░░ 
        for (int i = 0 ; i < 5 ; ++i){
            
            InitialiseBlack(Cursor);
            OccupySquare(Cursor);
            Cursor.X++;
        }


    }

    ////////////////////////////////////////////////////////////////////////////////////////////////


    void InitialiseQRcode(){

         // the steps to initialising a QRcode
        //https://www.thonky.com/qr-code-tutorial/module-placement-matrix

        ////////////////////////////////////////////////
        // Step 1: Add the Finder Patterns
        ////////////////////////////////////////////////

    
        // the coordanate of the most top left black square for the finding paterns
        CoordXY Topleft(0,0);
        CoordXY TopRight(DimensionQRcode-7, 0);
        CoordXY Bottomleft(0, DimensionQRcode-7);
        
        // finding patterns
        InitialiseCorner(Topleft);
        InitialiseCorner(TopRight);
        InitialiseCorner(Bottomleft);
        

        ////////////////////////////////////////////////
        // Step 2: Add the Separators
        ////////////////////////////////////////////////
        // the QR code is initiated as white so the separateurs are already there but needs to be *occupied*

        //occupy the white separators in the corners
        short Offset = DimensionQRcode-8;
        for (int i = 0 ; i <= 7 ; ++i){
            
            //Vertical
            // [Y][X]
            OccupiedVector[i][7] = 1;
            OccupiedVector[i][Offset] = 1;
            OccupiedVector[i+Offset][7] = 1;

            //Horizontal
            // [Y][X]
            OccupiedVector[7][i] = 1;
            OccupiedVector[Offset][i] = 1;
            OccupiedVector[7][i+Offset] = 1;
        }

        ////////////////////////////////////////////////
        // Step 3: Add the Alignment Patterns
        ////////////////////////////////////////////////
    
        //TODO
        //-Make a aligment patterne function that initialises a single alignement patern (in the QRcode and the funtionpattern) 

        // Each permutation of the numbers in the table is the center coordinate for an aligment pattern
        //  Version 2 : 6 and 18   ----> (6, 6), (6, 18), (18, 6) and (18, 18).

        vector<short> PossibleCoordinates = AlignmentTable[Version - 2];


        for (int X = 0; X < PossibleCoordinates.size() ; ++X){

            for (int Y = 0 ; Y < PossibleCoordinates.size() ; ++Y){

                CoordXY AlignmentSquare(PossibleCoordinates[X] ,PossibleCoordinates[Y] );

                if(AlignmentPatternIsSafe(AlignmentSquare , OccupiedVector)){

                    InitialiseAlignementPattern(AlignmentSquare);

                }

            }

        }




        ////////////////////////////////////////////////
        // Step 4: Add the Timing Patterns
        ////////////////////////////////////////////////

        //modifided the order because if you had the Timing patern before the alignement paterne you dont have to check if their is an al

        // starts form the bottomright finding pattern and between the separators
        CoordXY CursorVertical = {6,8};
        CoordXY CursorHorizontal = {8,6};

        

        //                   (DimensionQRcode - 8) -8 -1 = length between the two white separators (-1 first index = 0)
        for (int i = 0 ; i <= DimensionQRcode - 17 ; i++){

            if ( i % 2 == 0){
                InitialiseBlack(CursorHorizontal);
                OccupySquare(CursorHorizontal);
        
                InitialiseBlack(CursorVertical);
                OccupySquare(CursorVertical);
            }
            else{
                InitialiseWhite(CursorHorizontal);
                OccupySquare(CursorHorizontal);

                InitialiseWhite(CursorVertical);
                OccupySquare(CursorVertical);
                
            }
            CursorHorizontal.X++;
            CursorVertical.Y++;
        }
        

    }


    ////////////////////////////////////////////////////////////////////////////////////////////////


public:

    // insert your own QRcode Size
    //https://www.qrcode.com/en/about/version.html
    QRcode(unsigned short QRcodeVersion) : DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) {InitialiseQRcode();}
    

    // TODO : automatic with the string input 


    // TODO : add what error correction you want 

    // TODO : byte Alpha and kanji? 

    // TODO : all the combination of signateurs


    // initiations of the QRcode  (default version 6)
    QRcode(){

        InitialiseQRcode();
       
    }//QRcode



    // print the occupied squares of the QRcode
    void printOccupyDebug(){

        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
        cout << "printOccupyDebug() \n" ;
        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

        // counters for dimensions
        int Rows = 0;
        int Columns = 0;
    
        // iteration for the columns
        cout << "   ";
        for (int i = 0; i< OccupiedVector.size() ; i++ ){
            cout << Columns;
    
            // aligment
            cout << (Columns <= 9 ? " " : "");
            Columns++;
    
        }
        cout << endl;
    
        // Normal PrintQRcode()
        for (int i = 0; i< OccupiedVector.size() ; i++ ){
            
            // Iteration for the rows
            cout << Rows << (Rows <= 9 ? "  " : " ");
            Rows++;
    
            for (int j = 0; j< OccupiedVector[i].size() ; j++ ){
    
    
                OccupiedVector[i][j] == 1 ? printBlack() : printWhite();
            
            }
            cout << endl;
    

        }

        cout << "\n \n \n";

    }//printOccupyDebug()


    void printQRcodeDebug(){
        
        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
        cout << "printQRcodeDebug() \n" ;
        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;

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

        cout << "\n \n \n";

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



