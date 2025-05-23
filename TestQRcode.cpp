
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


    Errorcorrection Errorcorrection = Q;
    QRcodeMode QRcodeModeInput = Numeric;
    string message = "HELLO WORLD";

    unsigned short version = 2;
    QRcode TestQRcode(version, Errorcorrection, QRcodeModeInput , message);

    TestQRcode.printOccupyDebug();

    TestQRcode.printQRcodeDebug();

    TestQRcode.printQRcode();
    cout << endl << endl << endl;


    return 0;
}