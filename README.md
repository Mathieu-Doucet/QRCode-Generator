# URL to QR Code Generator
A personal project that gives you a QR code in the terminal when you enter your URL

## How to use 

Their are different [versions](https://www.thonky.com/qr-code-tutorial/introduction#history-and-information) of QRcode. Version 1 is the smallest with the dimensions of (21x21) and the biggest is version 40 with dimensions of (177X177). When you initiate the class with the string and the error correction you want, it will automatically chose the minimum version of QRcode to fit your URL.

You can choose what the QRcode will store with the [QRcode Mode](https://www.thonky.com/qr-code-tutorial/data-encoding#step-2-determine-the-smallest-version-for-the-data) and what kind of [Error Correction](https://www.thonky.com/qr-code-tutorial/data-encoding#step-1-choose-the-error-correction-level) you want.

 | QRcode Mode  | Error Correction | 
| ------------- |:-------------:| 
| Numeric      | L (7%) |
| AplhaNumeric | M (15%)|
| Byte         | Q (20%)|
| Kanji        | H (30%)|


### Inspiration and Reference

- I built a QR code with redstone to find out how it works [ mattbatwings ]
https://youtu.be/ZizmvuZ3EFk?si=qsV3VweIx_CGVQyK

-  I built a QR code with my bare hands to see how it works  [ Veritasium ]
https://youtu.be/w5ebcowAJD8?si=2lxczz_dLhvlZPIr

- Reference pour des expliquations techniques 
https://www.thonky.com/qr-code-tutorial/




