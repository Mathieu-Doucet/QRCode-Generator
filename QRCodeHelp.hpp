
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
https://www.youtube.com/watch?v=w5ebcowAJD8

I built a QR code with redstone to find out how it works - mattbatwings
https://www.youtube.com/watch?v=ZizmvuZ3EFk



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

//QR Code Log Antilog Table for Galois Field 256
//https://www.thonky.com/qr-code-tutorial/log-antilog-table


//https://www.thonky.com/qr-code-tutorial/error-correction-coding 
//these tables are ESSENTIAL for the error correction part of the QRcode Read the error correction coding part to have a little understanding on how you need them [The Galois Field]



// how to use: like in the table if you have a^5 and want to place it to GF(256) just put AlphaToInterger[5] and it will give 32



/*
Exemple : Step 1a: Multiply the Generator Polynomial by the Lead Term of the Message Polynomial

ɑ5x25 + ɑ1x24 + ɑ72x23 + ɑ51x22 + ɑ66x21 + ɑ123x20 + ɑ75x19 + ɑ69x18 + ɑ99x17 + ɑ37x16 + ɑ50x15

into:

32x25 + 2x24 + 101x23 + 10x22 + 97x21 + 197x20 + 15x19 + 47x18 + 134x17 + 74x16 + 5x15

*/


vector<uint8_t> AlphaToInterger = 
    {   1 , 2 , 4 , 8 , 16 , 32 , 64 , 128 , 29 , 58 , 116 , 232 , 205 , 135 , 19 , 38 , 76 , 152 , 45 , 90 , 180 , 117 ,
        234 , 201 , 143 , 3 , 6 , 12 , 24 , 48 , 96 , 192 , 157 , 39 , 78 , 156 , 37 , 74 , 148 , 53 , 106 , 212 , 181 ,
        119 , 238 , 193 , 159 , 35 , 70 , 140 , 5 , 10 , 20 , 40 , 80 , 160 , 93 , 186 , 105 , 210 , 185 , 111 , 222 ,
        161 , 95 , 190 , 97 , 194 , 153 , 47 , 94 , 188 , 101 , 202 , 137 , 15 , 30 , 60 , 120 , 240 , 253 , 231 , 211 ,
        187 , 107 , 214 , 177 , 127 , 254 , 225 , 223 , 163 , 91 , 182 , 113 , 226 , 217 , 175 , 67 , 134 , 17 , 34 , 
        68 , 136 , 13 , 26 , 52 , 104 , 208 , 189 , 103 , 206 , 129 , 31 , 62 , 124 , 248 , 237 , 199 , 147 , 59 , 118 , 
        236 , 197 , 151 , 51 , 102 , 204 , 133 , 23 , 46 , 92 , 184 , 109 , 218 , 169 , 79 , 158 , 33 , 66 , 132 , 21 , 
        42 , 84 , 168 , 77 , 154 , 41 , 82 , 164 , 85 , 170 , 73 , 146 , 57 , 114 , 228 , 213 , 183 , 115 , 230 , 209 , 
        191 , 99 , 198 , 145 , 63 , 126 , 252 , 229 , 215 , 179 , 123 , 246 , 241 , 255 , 227 , 219 , 171 , 75 , 150 , 
        49 , 98 , 196 , 149 , 55 , 110 , 220 , 165 , 87 , 174 , 65 , 130 , 25 , 50 , 100 , 200 , 141 , 7 , 14 , 28 , 56 , 
        112 , 224 , 221 , 167 , 83 , 166 , 81 , 162 , 89 , 178 , 121 , 242 , 249 , 239 , 195 , 155 , 43 , 86 , 172 , 69 , 
        138 , 9 , 18 , 36 , 72 , 144 , 61 , 122 , 244 , 245 , 247 , 243 , 251 , 235 , 203 , 139 , 11 , 22 , 44 , 88 , 176 ,
        125 , 250 , 233 , 207 , 131 , 27 , 54 , 108 , 216 , 173 , 71 , 142 , 1 
    };






// how to use: like in the table if you have 32 and want to place it to GF(256) just put IntergerToALpha[32] and it will give 5 equal to [a^5 in the tutorial]
/*
Exemple : Step 1a: Multiply the Generator Polynomial by the Lead Term of the Message Polynomial

32x^25 

into 
a^5 to add with the alpha polynomial

*/

// IntergerToAlpha[0] is an error and shoudnt be used
vector<uint8_t> IntergerToAlpha = 
    {0 , 0 , 1 , 25 , 2 , 50 , 26 , 198 , 3 , 223 , 51 , 238 , 27 , 104 , 199 , 75 , 4 , 100 , 224 , 14 , 52 , 141 , 239 ,
    129 , 28 , 193 , 105 , 248 , 200 , 8 , 76 , 113 , 5 , 138 , 101 , 47 , 225 , 36 , 15 , 33 , 53 , 147 , 142 , 218 , 
    240 , 18 , 130 , 69 , 29 , 181 , 194 , 125 , 106 , 39 , 249 , 185 , 201 , 154 , 9 , 120 , 77 , 228 , 114 , 166 , 6 , 
    191 , 139 , 98 , 102 , 221 , 48 , 253 , 226 , 152 , 37 , 179 , 16 , 145 , 34 , 136 , 54 , 208 , 148 , 206 , 143 , 150 ,
    219 , 189 , 241 , 210 , 19 , 92 , 131 , 56 , 70 , 64 , 30 , 66 , 182 , 163 , 195 , 72 , 126 , 110 , 107 , 58 , 40 , 
    84 , 250 , 133 , 186 , 61 , 202 , 94 , 155 , 159 , 10 , 21 , 121 , 43 , 78 , 212 , 229 , 172 , 115 , 243 , 167 , 87 , 
    7 , 112 , 192 , 247 , 140 , 128 , 99 , 13 , 103 , 74 , 222 , 237 , 49 , 197 , 254 , 24 , 227 , 165 , 153 , 119 , 38 , 
    184 , 180 , 124 , 17 , 68 , 146 , 217 , 35 , 32 , 137 , 46 , 55 , 63 , 209 , 91 , 149 , 188 , 207 , 205 , 144 , 135 , 
    151 , 178 , 220 , 252 , 190 , 97 , 242 , 86 , 211 , 171 , 20 , 42 , 93 , 158 , 132 , 60 , 57 , 83 , 71 , 109 , 65 , 
    162 , 31 , 45 , 67 , 216 , 183 , 123 , 164 , 118 , 196 , 23 , 73 , 236 , 127 , 12 , 111 , 246 , 108 , 161 , 59 , 82 , 
    41 , 157 , 85 , 170 , 251 , 96 , 134 , 177 , 187 , 204 , 62 , 90 , 203 , 89 , 95 , 176 , 156 , 169 , 160 , 81 , 11 , 
    245 , 22 , 235 , 122 , 117 , 44 , 215 , 79 , 174 , 213 , 233 , 230 , 231 , 173 , 232 , 116 , 214 , 244 , 234 , 168 , 
    80 , 88 , 175 };


////////////////////////////////////////////////////////////////////////////////////////////

