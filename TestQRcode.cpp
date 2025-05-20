
#include <iostream>
using namespace std;

// import the QRcode class
#include "QRCode.hpp"



int main(){


    QRcode TestQRcode;

    TestQRcode.printOccupyDebug();

    TestQRcode.printQRcodeDebug();

    TestQRcode.printQRcode();
    cout << endl << endl << endl;


    return 0;
}