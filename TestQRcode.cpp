
#include <iostream>
using namespace std;

// import the QRcode class
#include "QRCode.hpp"



int main(){

    /*
    // will make a default QRcode of version 6 that can fit [195-85 characters]
    QRcode TestQRcodeDefault;

    TestQRcodeDefault.printOccupyDebug();

    TestQRcodeDefault.printQRcodeDebug();

    TestQRcodeDefault.printQRcode();

    */

    //  https://www.qrcode.com/en/about/version.html


    /*
    Errorcorrection Errorcorrection = Q;
    QRcodeMode QRcodeModeInput = Alphanumeric;
    string message = "HELLO WORLD";
    unsigned short version = 1;
    short mask = 9; (mask 6)
    QRcode TestQRcode(version, Errorcorrection, QRcodeModeInput , message , mask);

    */

    /*
    Errorcorrection Errorcorrection = M;
    QRcodeMode QRcodeModeInput = Byte;
    string message = "www.youtube.com/veritasium";
    unsigned short version = 2;
    short mask = 8;
    QRcode TestQRcode(version, Errorcorrection, QRcodeModeInput , message , mask);

    */
    

    // well documented video for my testing 

    Errorcorrection Errorcorrection = L;
    QRcodeMode QRcodeModeInput = Byte;
    string message = "https://www.youtube.com/mattbatwings";
    unsigned short version = 3;
    //unsigned short version = 6;
    short mask = 0;
    QRcode TestQRcode(version, Errorcorrection, QRcodeModeInput , message , mask);




    TestQRcode.printOccupyDebug();

    TestQRcode.printQRcodeDebug();

    TestQRcode.printQRcode();
    cout << endl << endl << endl;


    return 0;
}