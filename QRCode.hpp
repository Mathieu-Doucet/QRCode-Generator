#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Important file header 
// also includes QRCodeHelp.hpp for all the tables and help functions for the QRcode class
#include "QRCode.h"


class QRcode : public QRcodeHeader{


private:

    //TODO make the parameters changable with the initiators? (first make something that works)
    // Go look QRCodeHelp.hpp or the documentation on the Variables
    const short DimensionQRcode= 41;
    const unsigned short Version = 6; // (max version is 40)
    const Errorcorrection ErrorCorrection = H; // Light for now
    const QRcodeMode Mode = Byte; // Byte is the standard for urls
    short MaskPattern = 3; // can change the mask pattern (every 3rd column inverted) // (column) mod 3 == 0
    string Message = ""; 


    // allocate an dynamic array initialized for version 6 QRcode for the standard url
    vector<vector<bool>> QRcodeArray = vector<vector<bool>>(DimensionQRcode,(vector<bool>(DimensionQRcode, 0)));

    // Allocate the same QRcode Size for places that CANT be modified because of the Function paterns (1 in the Array means occupied)
    vector<vector<bool>> OccupiedVector = vector<vector<bool>>(DimensionQRcode,(vector<bool>(DimensionQRcode, 0)));

    //Data encoding 
    // TODO make modular [https://www.thonky.com/qr-code-tutorial/error-correction-table]
    // 134 charcters and +2 bytes for padding, character count and QRcode mode = 136 bytes
    //vector<bool> MessageDataBits = vector<bool>(136*8, 0);
    vector<bool> MessageDataBits;
    


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




    //                                insert a funciton to be the condition of the mask (lambda)
    void HelpApplyMaskWithCondition( bool (*condition) (int , int)) {

        
        // check every square in the QRcode if its not occupied and the condition is true
        for (int row = 0 ; row < DimensionQRcode ; ++row){

            for (int column = 0; column < DimensionQRcode ; ++column){

                // if not occupied and the condition pointeur for the mask is true
                if (OccupiedVector[row][column] == 0 && condition(row , column)){

                    // flip the square in the QRcode
                    QRcodeArray[row][column] = !QRcodeArray[row][column];

                    //TODO : change the value of the message bit?

                }

            }

        }



    }



    void ApplyMaskPattern(short Mask){

        // mask patterns dont invert the Occupied spaces (finding patterns , separators , allignment patters, timing patterns and format strings)

        // MaskPatterns[Veritasium].png Visual representation for how each mask looks
        // https://www.thonky.com/qr-code-tutorial/mask-patterns


        // for every mask i define a lambda function for the condition in its respective case to save code space
        switch (Mask){

            //checkerboard pattern
            case 0: {

                //(row + column) mod 2 == 0
                // define the condition using a lambda funciton
                auto function = [] (int row , int column) { return (row + column) % 2 == 0 ;} ;

                HelpApplyMaskWithCondition(function);


            }break;
            

            // every even horizontal row is flipped
            case 1: {
                //(row) mod 2 == 0
                // define the condition using a lambda funciton
                auto function = [] (int row , int column) { return row  % 2 == 0 ;} ;

                HelpApplyMaskWithCondition(function);

        
            }break;
            
            //every third column is inverted
            case 2: {
                //(column) mod 3 == 0
                // define the condition using a lambda funciton
                auto function = [] (int row , int column) { return column % 3 == 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;

            // checker board pattern every 3 row
            case 3: {

                //(row + column) mod 3 == 0
                auto function = [] (int row , int column) { return (row + column) % 3 == 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;

            // big checkerboard pattern (2x3)
            case 4: {
                //( floor(row / 2) + floor(column / 3) ) mod 2 == 0
                auto function = [] (int row , int column) { return ((row/2) + (column / 3)) % 2 == 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;


            /*
            // plus in square
            *the black inverts*
            ░░ ░░ ░░ ░░ ░░ ░░ ░░
            ░░ ▓▓ ▓▓ ▓▓ ▓▓ ▓▓ ░░
            ░░ ▓▓ ▓▓ ░░ ▓▓ ▓▓ ░░
            ░░ ▓▓ ░░ ▓▓ ░░ ▓▓ ░░
            ░░ ▓▓ ▓▓ ░░ ▓▓ ▓▓ ░░
            ░░ ▓▓ ▓▓ ▓▓ ▓▓ ▓▓ ░░
            ░░ ░░ ░░ ░░ ░░ ░░ ░░
            */
            case 5: {
                //((row * column) mod 2) + ((row * column) mod 3) == 0

                auto function = [] (int row , int column) { return (((row * column) % 2) + ((row * column)% 3))== 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;




            /*
            // amd logo
            *the black inverts*
            ░░ ░░ ░░ ░░ ░░ ░░ ░░
            ░░ ░░ ░░ ▓▓ ▓▓ ▓▓ ░░
            ░░ ░░ ▓▓ ░░ ░░ ▓▓ ░░
            ░░ ▓▓ ░░ ▓▓ ░░ ▓▓ ░░
            ░░ ▓▓ ░░ ░░ ▓▓ ░░ ░░
            ░░ ▓▓ ▓▓ ▓▓ ░░ ░░ ░░
            ░░ ░░ ░░ ░░ ░░ ░░ ░░
            */

            case 6: {
                //( ((row * column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
                auto function = [] (int row , int column) { return (((row * column) % 2) + ((row * column)% 3)) % 2 == 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;




            // interlinked spining top
            /*
            *the black inverts*
            ░░ ▓▓ ░░ ▓▓ 
            ▓▓ ▓▓ ▓▓ ░░ 
            ░░ ▓▓ ▓▓ ▓▓ 
            ▓▓ ░░ ▓▓ ░░ 
            ░░ ░░ ░░ ▓▓ 
            ▓▓ ░░ ░░ ░░ 
            ░░ ▓▓ ░░ ▓▓ 
            */

            case 7: {
                //( ((row + column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
                auto function = [] (int row , int column) { return (((row + column) % 2) + ((row * column)% 3)) % 2 == 0 ;} ;
                HelpApplyMaskWithCondition(function);

            }break;



            default:
                cout << "mask failed?" <<  endl;

    

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
        

        //debug mask
        AddFormatString(MaskPattern);



    }//InitialiseQRcode


    

    //https://www.thonky.com/qr-code-tutorial/format-version-tables

    //TODO : Generate Error Correction Bits for Format String (ECC) [https://www.thonky.com/qr-code-tutorial/format-version-information]

    /*
    Because we are just using ECC L *for now* we dont really have to calculate the ECC bit because of the table given (table in QRcodehelp.hpp)
    L	0	111011111000100
    L	1	111001011110011
    L	2	111110110101010
    L	3	111100010011101
    L	4	110011000101111
    L	5	110001100011000
    L	6	110110001000001
    L	7	110100101110110
    
    */
    
    //FormatStringLayout.png for a visual Reference [https://www.thonky.com/qr-code-tutorial/format-version-information]
    void AddFormatString(short MaskPatternInput){

        // always the same place
        CoordXY TopLeftCursor(0,8);
        //changes how low with the dimension
        CoordXY BottomLeftCursor(8,DimensionQRcode-1);

        vector<bool> FormatString = FormatStringsTable[MaskPatternInput];


        for (int Bit = 0 ; Bit <15 ; ++Bit){

            //encountered the vertical timing pattern
            if(Bit == 6){
                TopLeftCursor.X++;
            }
            //encountered the horizontal timing pattern
            if (Bit == 9){
                TopLeftCursor.Y--;
            }

            //DarkSquare in the bottom left [https://www.youtube.com/watch?v=w5ebcowAJD8][14:00] Veritatium 
            if (Bit == 7){
                CoordXY BlackSquare(8,DimensionQRcode-8);
                InitialiseBlack(BlackSquare);
                OccupySquare(BlackSquare);

                //Go to the TopRight finding pattern for the bottomleft cursor[postion 7]
                BottomLeftCursor.X = DimensionQRcode-8;
                BottomLeftCursor.Y = 8;
    
                
            }


            // Place the Bit ()
            if(FormatString[Bit] == 1) {
                InitialiseBlack(TopLeftCursor); 
                InitialiseBlack(BottomLeftCursor);
            }
            //if in the future i want to overwrite / change QRcode
            else{
                InitialiseWhite(TopLeftCursor);
                InitialiseWhite(BottomLeftCursor);
            }
            //always occupy (wont be affected by the mask pattern)
            OccupySquare(TopLeftCursor);
            OccupySquare(BottomLeftCursor);


            // change the cursor positions

            //Top right move horizontaly and Bottom left Veritcaly
            if(Bit < 7){

                TopLeftCursor.X++;
                BottomLeftCursor.Y--;

            }

            //Top right move Vertically and Bottom left horizontaly
            else{
                
                TopLeftCursor.Y--;
                BottomLeftCursor.X++;

            }

        }




    }



    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Data encoding
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    void AppendToDataBits(uint8_t Character){

        //using big edian so the first bit is 2^7
        uint8_t Checker = 0b10000000 ; 

        for (int Bit = 7 ; Bit >= 0 ; --Bit){

            // if the bit is presently a 1 in the charcter 
            if ((Checker & Character) > 0){
                MessageDataBits.push_back(true);
            }
            else{
                MessageDataBits.push_back(false);
            }

            // move the bit to the right
            Checker = (Checker >> 1);

        }


    }



    //https://www.youtube.com/watch?v=ZizmvuZ3EFk [7:52] 
    //https://www.thonky.com/qr-code-tutorial/data-encoding

    //TODO : Determine the Smallest Version for the Data 

    //Step 1: Choose the Error Correction Level (choosed level L because a computer wont corrupted data that much)

    //Step 2: Determine the Smallest Version for the Data
    //with L, version 6 you can have 134 of used characteurs 

    


   

    void MakeTheDataString (){

        //TODO make the mode modular (Alpha, numeric, byte)

        //firt four bits is the QRcode mode (Byte mode for default 0100)
        //Step 3: Add the Mode Indicator
        uint8_t Checker = Mode;
        
        // 1000 -> 0100 -> 0010 -> 0001 
        for (int Bit = 8 ; Bit > 0 ; Bit = Bit >> 1){

            // if the bit is presently a 1 in the mode then place one
            if ((Bit & Checker) > 0){
                MessageDataBits.push_back(true);
            }
            else{
                MessageDataBits.push_back(false);
            }
        }


         ///*
        //Add the lenght of the message in the data string (8bits long)
        // Step 4: Add the Character Count Indicator


        uint8_t CharacterCount = Message.length();
        AppendToDataBits(CharacterCount);

        // add the characteurs to the Data String

        //Step 5: Encode Using the Selected Mode

        for (int Character = 0 ;Character < Message.size() ; ++Character ){

            AppendToDataBits(uint8_t(Message[Character]));
        }  

        // Add a Terminator of 0s if Necessary
        // TODO : *if Necessary* make a check if you are in numeric or other modes?

        // with byte mode you always add 4 0's to make it a multiple of 8
        for (int i = 0 ; i <4 ; ++i){MessageDataBits.push_back(false);}


        //TODO : to expand the QRcode function you need to know when to stop adding padding make a table using this link
        // https://www.thonky.com/qr-code-tutorial/error-correction-table 
        // 6-L = 136 data codewords 
        // padding bytes = 136 - 2 - Message.size()     [2 is for the mode character count and 4 0's at the end of the message]

        //Add Pad Bytes if the String is Still too Short
        int PaddingByte = DataCodeWord[Version-1][0] -2 - Message.size();

        // padding patterns
                // 11101100 00010001
        uint8_t Pair = 236;
        uint8_t Odd = 17;

        for (int i = 0 ; i < PaddingByte ; ++i){

            if (i % 2 == 0){
                AppendToDataBits(Pair);
            }
            else{
                AppendToDataBits(Odd);

            }

        }
        //*/
    }

    


public:

    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Initialisation 

    ////////////////////////////////////////////////////////////////////////////////////////////////

    //Default QRcode setting for this class
    /*
    short DimensionQRcode= 41;
    unsigned short Version = 6; // (max version is 40)
    Errorcorrection ErrorCorrection = L; // Light easiest
    QRcodeMode Mode = byte; // byte is the standard for urls
    string Message = ""; 
    */

    

    /*
    // insert your own QRcode Version [2-40]
    //https://www.qrcode.com/en/about/version.html
    QRcode(unsigned short QRcodeVersion) : DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) {InitialiseQRcode();}
    
    //DEBUG (NO MESSAGE)
    // TODO : add what error correction you want 
    QRcode(Errorcorrection ErrorcorrectionInput) : ErrorCorrection(ErrorcorrectionInput) {InitialiseQRcode();}

    // DEBUG (NO MESSAGE)
    // change just the QRcode 
    QRcode(QRcodeMode QRcodeModeInput) : Mode(QRcodeModeInput) {InitialiseQRcode();}

    //DEBUG (ALPHANUMERIC ONLY)
    // TODO : automatiquly scale what is the best QRcode mode for the string lenght
    //  Default setting with just the string input 
    QRcode(string stringInput) : Message(stringInput) {InitialiseQRcode();}

    


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Double input
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // L and Alphanumeric 
    QRcode(unsigned short QRcodeVersion , string stringInput) : DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) , Message(stringInput) {InitialiseQRcode();}

    // No input and alphanumeric
    QRcode(unsigned short QRcodeVersion, Errorcorrection ErrorcorrectionInput) : DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) {InitialiseQRcode();}



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Triple input
    ////////////////////////////////////////////////////////////////////////////////////////////////


    // TODO : if the user choses a incompatible version or string check and stop the execusion of the class
    // use stdexept to give a messge and quit the proggrame?

    // full control on what QRcode you want
    QRcode(unsigned short QRcodeVersion, Errorcorrection ErrorcorrectionInput , QRcodeMode QRcodeModeInput ,string stringInput) : 
        DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) , ErrorCorrection(ErrorcorrectionInput) , Mode(QRcodeModeInput), Message(stringInput) 
    
        {InitialiseQRcode();}



    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    */

     // full control on what QRcode you want
     QRcode(unsigned short QRcodeVersion, Errorcorrection ErrorcorrectionInput , QRcodeMode QRcodeModeInput ,string stringInput , short mask) : 
     DimensionQRcode((QRcodeVersion-1)*4 + 21) , Version(QRcodeVersion) , ErrorCorrection(ErrorcorrectionInput) , Mode(QRcodeModeInput), Message(stringInput) , MaskPattern(mask) 
 
     {InitialiseQRcode();}

    //DEBUG default (L version 6 and bytemode)
    // TODO : automatiquly scale what is the best QRcode mode for the string lenght
    //  Default setting with just the string input 
    QRcode(string stringInput) : Message(stringInput) {InitialiseQRcode();}


    // initiations of the QRcode  (default)
    QRcode(){

        InitialiseQRcode();
       
    }//QRcode
    //**************************************************************** 
    
    //TODO : add more customisation to initialise the QRcode 
    // if the user wants to specifie what they want to do with the QRcode (mode, ECC and version) it could be a const in the class and will always take priority 
    // For now i will focus on making something that works so i will do (L, Byte and version 6) for the default because its the most rational for this project 
    // L because its a computer output and it cant really get damaged, Byte because we are doing urls and their can be a lot of symbols :P and version 6 because their is just one alignment pattern and 
    //it wont be a headache for when you place the message in the QRcode and when you get to Version 7 you need to add "Reserve the Version Information Area" [https://www.thonky.com/qr-code-tutorial/module-placement-matrix]


    //TODO function to input a string in the QRcode



    
    // print the occupied squares of the QRcode
    void printOccupyDebug(){

        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
        cout << "printOccupyDebug() \n" ;
        cout << "////////////////////////////////////////////////////////////////////////////////////////////////" << endl << endl;



        // DEBUG mask
        /////////////////////////////////////////////////
        //ApplyMaskPattern(7);
        /////////////////////////////////////////////////



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

        cout << "Dimensions : " << DimensionQRcode << endl;
        cout << "Version : " << Version << endl;
        cout << "ErrorCorrection : " << ErrorCorrection << endl;
        cout << "QRcodeMode : " << Mode << endl;
        cout << "Message : " << Message << endl << endl << endl << endl;


        MakeTheDataString();

        //DEBUG Data bits
        /////////////////////////////////////////////////
        cout << "DataBit message string : " << endl;
        for (int i = 0 ; i < MessageDataBits.size() ; ++i){

            cout << (MessageDataBits[i] == true ) ? '1' : '0' ;
            if (i != 0 && (i+1) % 8 == 0 ){cout << " ";} // space out every byte

            if (i != 0 && (i+1) % 80 == 0) {cout << endl;} // new line every 10 byte
        }
        cout << endl << endl << endl;

        /////////////////////////////////////////////////


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



