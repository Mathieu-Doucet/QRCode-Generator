
#include <iostream>
#include <vector>
#include <string>
using namespace std;



struct CoordXY{
    short X;
    short Y;

    CoordXY(short x, short y){
        X=x;   Y=y;
    }
};


//                     7% 15% 25% 30%
enum Errorcorrection { L , M , Q , H}; // https://www.thonky.com/qr-code-tutorial/data-encoding

enum QRcodeMode {Numeric , AlphaNumeric, Byte, Kanji}; // https://www.thonky.com/qr-code-tutorial/data-analysis

// Alighment table for the aligments locations
// https://www.thonky.com/qr-code-tutorial/alignment-pattern-locations

// Each permutation of the numbers in the table is the center coordenate for an aligment pattern
//  Version 2 : 6 and 18   ----> (6, 6), (6, 18), (18, 6) and (18, 18).

// This table isnt nesessary but for future expansion its useful. their is a small formula but its a pain to code with all the exeptions
vector<vector<int>> AlignmentTable = {
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


////////////////////////////////////////////////////////////////////////////////////////////

