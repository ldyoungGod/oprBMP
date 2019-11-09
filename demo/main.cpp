#include <iostream>
#include "COperateBMP.h"
using namespace std;

int main(int argc, char *argv[])
{
    BMPIMG img;
    COperateBMP opr;
    opr.ReadImgBMP("bmp.bmp",img);
    opr.OutPutImgBMP(img,"bmp_bak.bmp");
    opr.OutPutImgBmpNbitGray(img,"gray.bmp");
    return 0;
}
