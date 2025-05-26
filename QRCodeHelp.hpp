
#include <iostream>
#include <vector>
#include <string>
using namespace std;


/*
important tables for information in the future

How many characters can fit in each version of a QRcode
https://www.thonky.com/qr-code-tutorial/character-capacities


Size and howmany characteurs can fit in a QRcode
https://www.qrcode.com/en/about/version.html


-Error correction tables Blocks and groups
https://www.thonky.com/qr-code-tutorial/error-correction-table
https://www.thonky.com/qr-code-tutorial/log-antilog-table

-format and Version table (not useful can code my own algoright that can generate the Ecc for the format corner)
https://www.thonky.com/qr-code-tutorial/format-version-tables

-Alphanumeric table (letters availible in the alphanumeric mode)
https://www.thonky.com/qr-code-tutorial/alphanumeric-table

-Alignment pattern middle coordanates table 
https://www.thonky.com/qr-code-tutorial/alignment-pattern-locations

*/




// TODO : funtion that returns the encoded of each mode (alphanumeric numeric and byte) (no kanji) (maybe ECI???)

//https://www.thonky.com/qr-code-tutorial/alphanumeric-mode-encoding
//https://www.thonky.com/qr-code-tutorial/numeric-mode-encoding
//https://www.thonky.com/qr-code-tutorial/byte-mode-encoding





struct CoordXY{
    short X;
    short Y;

    CoordXY(short x, short y){
        X=x;   Y=y;
    }
};



//Step 3: Add the alignment patterns
//https://www.thonky.com/qr-code-tutorial/module-placement-matrix

bool AlignmentPatternIsSafe (CoordXY Middle, const vector<vector<bool>> & FunctionPattern){


    CoordXY TopLeftAlignment(Middle.X - 2, Middle.Y - 2);
    CoordXY TopRightAlignment(Middle.X - 2, Middle.Y + 2);
    CoordXY BottomLeftAlignment(Middle.X + 2, Middle.Y + 2);


    // TODO: Optimisation posible?

    if (FunctionPattern[Middle.Y][Middle.X] == 1){
        return false;
    }

    else if (FunctionPattern[TopLeftAlignment.Y][TopLeftAlignment.X] == 1){
        return false;
    }

    else if (FunctionPattern[TopRightAlignment.Y][TopRightAlignment.X] == 1){
        return false;
    }

    else if (FunctionPattern[BottomLeftAlignment.Y][BottomLeftAlignment.X] == 1){
        return false;
    }

    return true;
    


}

//https://www.thonky.com/qr-code-tutorial/format-version-information 
//Error correction section says L = 1 , M = 0 , Q = 3 and H = 2
//                     15%  7%  30% 25%
enum Errorcorrection {  M , L , H , Q  }; // https://www.thonky.com/qr-code-tutorial/data-encoding

//https://www.youtube.com/watch?v=w5ebcowAJD8 [14:46]
// Numeric = 0001 , AlphaNumeric = 0010 , Binary = 0100 and Kanji = 1000
enum QRcodeMode {Numeric = 1 , Alphanumeric = 2, Byte = 4, Kanji = 8}; // https://www.thonky.com/qr-code-tutorial/data-analysis

// Alighment table for the aligments locations
// https://www.thonky.com/qr-code-tutorial/alignment-pattern-locations

// Each permutation of the numbers in the table is the center coordinate for an aligment pattern
//  Version 2 : 6 and 18   ----> (6, 6), (6, 18), (18, 6) and (18, 18).

// This table isnt nesessary for the algnment patterns but for future expansion its useful. *their is probably a formula for the coords but its a pain to code with all the exeptions
vector<vector<short>> AlignmentTable = {
    {6,18},//2
    {6,22},//3
    {6 ,26},//4
    {6 ,30},//5
    {6 ,34},//6
    {6 ,22,38},//7
    {6 ,24,42},//8
    {6 ,26,46},//9
    {6 ,28,50},//10
    {6 ,30,54},//11
    {6 ,32,58},//12
    {6 ,34,62},//13
    {6 ,26,46,66},//14
    {6 ,26,48,70},//15
    {6 ,26,50,74},//16
    {6 ,30,54,78},//17
    {6 ,30,56,82},//18
    {6 ,30,58,86},//19
    {6 ,34,62,90},//20
    {6 ,28,50,72,94 },//21
    {6 ,26,50,74,98 },//22
    {6 ,30,54,78,102 },//23
    {6 ,28,54,80,106 },//24
    {6 ,32,58,84,110 },//25
    {6 ,30,58,86,114 },//26
    {6 ,32,62,90,118 },//27
    {6 ,26,50,74,98,122 },//28
    {6 ,30,54,78,102,126 },//29
    {6 ,26,52,78,104,130 },//30
    {6 ,30,52,78,108,130 },//31
    {6 ,34,60,86,112,138 },//32
    {6 ,30,58,86,114,142 },//33
    {6 ,34,62,90,118,146 },//34
    {6 ,30,54,78,102,126,150},//35
    {6 ,24,50,76,102,128 ,154 },//36
    {6 ,28,54,80,106,132 ,158 },//37
    {6 ,32,58,84,110,136 ,162 },//38
    {6 ,36,54,82,110,138 ,166 },//39
    {6 ,30,58,86,114,142 ,170 }//40
};




// for the format string information you need to calculte the ECC for the 5 bits and do a copy instead of copying it ill make a table
/*
    L	0	111011111000100
    L	1	111001011110011
    L	2	111110110101010
    L	3	111100010011101
    L	4	110011000101111
    L	5	110001100011000
    L	6	110110001000001
    L	7	110100101110110

*/

//in QRcode.hpp uses [0 for first]
vector<vector<bool>> FormatStringsTable = {

    {1,1,1,0,1,1,1,1,1,0,0,0,1,0,0}, //L-0     mattbatwings Redstone QRcode
    {1,1,1,0,0,1,0,1,1,1,1,0,0,1,1}, //L-1	
    {1,1,1,1,1,0,1,1,0,1,0,1,0,1,0}, //L-2	
    {1,1,1,1,0,0,0,1,0,0,1,1,1,0,1}, //L-3	
    {1,1,0,0,1,1,0,0,0,1,0,1,1,1,1}, //L-4	
    {1,1,0,0,0,1,1,0,0,0,1,1,0,0,0}, //L-5	
    {1,1,0,1,1,0,0,0,1,0,0,0,0,0,1}, //L-6
    {1,1,0,1,0,0,1,0,1,1,1,0,1,1,0}, //L-7
    // /* future debugging
    {1,0,1,1,1,1,0,0,1,1,1,1,1,0,0}, //M-2 [Veritasium video]
    {0,1,0,1,1,1,0,1,1,0,1,1,0,1,0} // Final QRcode in thonky v1-Q-6 helloworld example (mask patern 6)
    // */



};




//https://www.thonky.com/qr-code-tutorial/error-correction-table

vector<vector<int>> DataCodeWord = {

                    //version - ECC
    	{19	,7	,1,	19},//1-L
    	{34	,10	,1,	34},//2-L
    	{55	,15	,1,	55}, //3-L [Mattbatwings]
    	{80	,20	,1,	80},//4-L
    	{108,26, 1,	108},//5-L
    	{136,18, 2,	68},//6-L [Moi]
    	{28	,16	,1,	28},//2-M [Veritasium]
        {13,13,	1,	13} //// Final QRcode in thonky v1-Q helloworld example 

};

////////////////////////////////////////////////////////////////////////////////////////////

