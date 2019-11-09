#ifndef COPERATEBMP_H
#define COPERATEBMP_H
#include "CFileOperate.h"
#include <vector>
#include <string.h>
#include <cmath>
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned int   LONG;
typedef unsigned char  BYTE;

#pragma pack(2)
struct tagBmpFileHeader//文件头
{
    WORD    bfType;//标识该文件为bmp文件,判断文件是否为bmp文件，即用该值与0x4d42
    DWORD   bfSize;//整个bmp文件大小
    WORD    bfReserved1;//预保留位
    WORD    bfReserved2;//预保留位
    DWORD   bfOffBits;//图像数据区的起始位置(不包括文件头，信息头，调色板)
};//14字节
struct tagBmpInfoHeader//信息头
{
    DWORD       biSize;//tagBmpInfoHeader大小==40
    LONG        biWidth;//宽度像素
    LONG        biHeight;//高度像素（如果是正数代表图像是倒向的，负数代表正向的）
    WORD        biPlanes;//为1
    WORD        biBitCount;//像素位数，1,4,8-灰度图；24-真彩色
    DWORD       biCompression;//压缩方式（0：不压缩）
    DWORD       biSizeImage;//图像区数据大小，当biCompression为0时，可以设置为0
    LONG        biXPelsPerMeter;//水平分辨率，像素每米
    LONG        biYPelsPerMeter;
    DWORD       biClrUsed;//说明位图实际使用的彩色表中的颜色索引数，为0代表使用所有调色板项
    DWORD       biClrImportant;//位图显示过程，重要的颜色数；0--所有都重要
};//40字节
struct tagRGBPallete//调色板
{
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    alpha; //预保留位
};

#pragma pack()

struct BMPIMG
{
    tagBmpFileHeader _fileHead;
    tagBmpInfoHeader _infoHead;
    std::vector<tagRGBPallete> _pallete;
    char* buff;
};

class COperateBMP
{
public:
    COperateBMP();
    bool OutPutImgBmpNbitGray(int imgW,int imgH,int biCount/*1,4,8*/,const char *buffer,const int buffLen,const char *fileName);//1,4,8bit,灰度图像
    bool OutPutImgBmpNbitGray(const BMPIMG &img, const char* fileName);
    bool OutPutImgBMP24bit(int imgW, int imgH, const char *buffer,const int buffLen, const char *fileName);//真彩色RGB
    bool OutPutImgBMP(const BMPIMG& img,const char* fileName);
    bool ReadImgBMP(const char* fileName, BMPIMG& img);
};

#endif // COPERATEBMP_H
