
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
    unsigned short version = 10;
    QRcode TestQRcode(version);

    TestQRcode.printOccupyDebug();

    TestQRcode.printQRcodeDebug();

    TestQRcode.printQRcode();
    cout << endl << endl << endl;


    return 0;
}