
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



I built a QR code with my bare hands to see how it works - Veritasium 
https://www.youtube.com/watch?v=ZizmvuZ3EFk


 I built a QR code with redstone to find out how it works - mattbatwings
https://www.youtube.com/watch?v=w5ebcowAJD8



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



//TODO : make a 2D array to for the 4 error correction if i want to do them in the future(L,M,Q,H)

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



        //if i want to use bigger version with ECC L
        /*
        
            {156	,20,	2,	78},        //7-L	
            {194	,24,	2,	97	},      //8-L	
            {232	,30,	2,	116	},      //9-L	
            {274	,18,	2,	68,	2,	69},//10-L
        	{324	,20,	4,	81},        //11-L
        	{370	,24,	2,	92,	2,	93},    //12-L
        	{428	,26,	4,	107},	         //13-L
        	{461	,30,	3,	115,	1,	116},//14-L	
        	{523	,22,	5,	87,	1,	88},     //15-L
        	{589	,24,	5,	98,	1,	99},     //16-L
        	{647	,28,	1,	107,	5,	108},//17-L
        	{721	,30,	5,	120,	1,	121},//18-L 
        	{795	,28,	3,	113,	4,	114},//19-L
        	{861	,28,	3,	107,	5,	108},//20-L
        	{932	,28,	4,	116,	4,	117},//21-L
        	{1006	,28,	2,	111,	7,	112},//22-L
        	{1094	,30,	4,	121,	5,	122},//23-L
        	{1174	,30,	6,	117,	4,	118},//24-L
        	{1276	,26,	8,	106,	4,	107},//25-L
        	{1370	,28,	10,	114,	2,	115},//26-L 	
        	{1468	,30,	8,	122,	4,	123},//27-L 	
        	{1531	,30,	3,	117,	10,	118},//28-L
        	{1631	,30,	7,	116,	7,	117},//29-L
        	{1735	,30,	5,	115,	10,	116},//30-L
        	{1843	,30,	13,	115,	3,	116},//31-L
        	{1955	,30,	17,	115},	         //32-L
        	{2071	,30,	17,	115,	1,	116},//33-L
        	{2191	,30,	13,	115,	6,	116},//34-L
        	{2306	,30,	12,	121,	7,	122},//35-L
        	{2434	,30,	6,	121,	14,	122},//36-L
        	{2566	,30,	17,	122,	4,	123},//37-L
        	{2702	,30,	4,	122,	18,	123},//38-L
        	{2812	,30,	20,	117,	4,	118},//39-L
        	{2956	,30,	19,	118,	6,	119} //40-L
        
        //*/

};


////////////////////////////////////////////////////////////////////////////////////////////

