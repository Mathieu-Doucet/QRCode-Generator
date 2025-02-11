
#include <iostream>
using namespace std;

// import the QRcode class
#include "QRCode.hpp"



int main(){


    QRcode TestQRcode;

    TestQRcode.printQRcode();
    cout << endl << endl << endl;

    TestQRcode.printQRcodeDebug();

    return 0;
}