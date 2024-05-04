//
//  crypto.c
//  XLuaC
//
//  Created by Bepic on 2024/5/4.
//

#include "crypto.h"

char llndex[12]; // char llndex[] = {85, 87, 105, 74, 93, 92, 108, 96, 112, 72, 99, 120}; // absSecretKey
char lindex[13]; // char lindex[] = {95, 105, 86, 89, 74, 93, 92, 108, 96, 112, 72, 99, 120}; // luacSecretKey

void initCrypto(void) {
    secretKeyA();
    secretKeyB();
    secretKeyC();
    secretKeyD();
    secretKeyE();
    secretKeyF();
    secretKeyG();
    secretKeyH();
    secretKeyI();
    secretKeyJ();
    secretKeyK();
    secretKeyL();
}

void secretKeyA(void) {
   llndex[0] = 85;    lindex[11] = 99;
}
void secretKeyB(void) {
   llndex[1] = 87;    lindex[10] = 72;
}
void secretKeyC(void) {
   llndex[2] = 105;    lindex[9] = 112;
}
void secretKeyD(void) {
   llndex[3] = 74;    lindex[8] = 96;
}
void secretKeyE(void) {
   llndex[4] = 93;    lindex[7] = 108;
}
void secretKeyF(void) {
   llndex[5] = 92;    lindex[6] = 92;
}
void secretKeyG(void) {
   llndex[6] = 108;    lindex[5] = 93;
}
void secretKeyH(void) {
   llndex[7] = 96;    lindex[4] = 74;
}
void secretKeyI(void) {
   llndex[8] = 112;    lindex[3] = 89;
}
void secretKeyJ(void) {
   llndex[9] = 72;    lindex[2] = 86;
}
void secretKeyK(void) {
   llndex[10] = 99;    lindex[1] = 105;
}
void secretKeyL(void) {
   llndex[11] = 120;    lindex[0] = 95;
}

char* get_llndex(void) {
    return llndex;
}
char* get_lindex(void) {
    return lindex;
}
