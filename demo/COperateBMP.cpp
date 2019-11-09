#include "COperateBMP.h"

COperateBMP::COperateBMP()
{

}

bool COperateBMP::OutPutImgBmpNbitGray(int imgW, int imgH, int biCount, const char *buffer,const int buffLen, const char *fileName)
{
    if(buffer == nullptr || fileName == nullptr || imgW < 1 || imgH < 1 || buffLen < 1)
        return false;
    if(biCount == 1 || biCount == 4 || biCount == 8)
    {
        int colorDeep = std::pow(2,biCount);
        colorDeep > 256 ? colorDeep = 0 : colorDeep;

        tagBmpFileHeader  headFile;
        headFile.bfType = 0x4D42;
        headFile.bfSize = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader) + sizeof(tagRGBPallete)*colorDeep + buffLen;
        headFile.bfReserved1 = headFile.bfReserved2 = 0;
        headFile.bfOffBits = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader)+ sizeof(tagRGBPallete)*colorDeep;

        tagBmpInfoHeader headInfo;
        headInfo.biSize = sizeof(tagBmpInfoHeader);
        headInfo.biWidth = imgW;
        headInfo.biHeight = imgH;
        headInfo.biPlanes = 1;
        headInfo.biBitCount = biCount;
        headInfo.biCompression = 0;
        headInfo.biSizeImage = buffLen;
        headInfo.biXPelsPerMeter = headInfo.biYPelsPerMeter = 0;
        headInfo.biClrUsed = 0;
        headInfo.biClrImportant = 0;

        std::ofstream outFile(fileName,std::ios::binary);
        outFile.write((char*)(&headFile),sizeof(headFile));
        outFile.write((char*)(&headInfo),sizeof(headInfo));

        for(int i = 0;i<colorDeep;i++)
        {
            tagRGBPallete tt;
            tt.rgbBlue = i;
            tt.rgbGreen = i;
            tt.rgbRed = i;
            tt.alpha = 0;
            outFile.write((char*)(&tt),sizeof(tt));
        }

        outFile.write(buffer,buffLen);
        outFile.close();
    }
    else if(biCount == 24 && buffLen%3 == 0)
    {
        int colorDeep = 256;

        tagBmpFileHeader  headFile;
        headFile.bfType = 0x4D42;
        headFile.bfSize = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader) + sizeof(tagRGBPallete)*colorDeep + buffLen/3;
        headFile.bfReserved1 = headFile.bfReserved2 = 0;
        headFile.bfOffBits = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader)+ sizeof(tagRGBPallete)*colorDeep;

        tagBmpInfoHeader headInfo;
        headInfo.biSize = sizeof(tagBmpInfoHeader);
        headInfo.biWidth = imgW;
        headInfo.biHeight = imgH;
        headInfo.biPlanes = 1;
        headInfo.biBitCount = 8;
        headInfo.biCompression = 0;
        headInfo.biSizeImage = buffLen/3;
        headInfo.biXPelsPerMeter = headInfo.biYPelsPerMeter = 0;
        headInfo.biClrUsed = 0;
        headInfo.biClrImportant = 0;

        std::ofstream outFile(fileName,std::ios::binary);
        outFile.write((char*)(&headFile),sizeof(headFile));
        outFile.write((char*)(&headInfo),sizeof(headInfo));

        for(int i = 0;i<colorDeep;i++)
        {
            tagRGBPallete tt;
            tt.rgbBlue = i;
            tt.rgbGreen = i;
            tt.rgbRed = i;
            tt.alpha = 0;
            outFile.write((char*)(&tt),sizeof(tt));
        }

        for(int i = 0; i< buffLen/3; ++i)
        {
            byte b = (byte)buffer[3 * i];
            byte g = (byte)buffer[3 * i + 1];
            byte r = (byte)buffer[3 * i + 2];
            byte grayLevel = 0.299*r + 0.587*g + 0.114*b;
            outFile.write((char*)&grayLevel,sizeof(char));
        }
        outFile.close();
    }
    return true;
}

bool COperateBMP::OutPutImgBmpNbitGray(const BMPIMG &img, const char* fileName)
{
    this->OutPutImgBmpNbitGray(img._infoHead.biWidth,img._infoHead.biHeight,img._infoHead.biBitCount,img.buff,img._infoHead.biSizeImage,fileName);
}

bool COperateBMP::OutPutImgBMP24bit(int imgW, int imgH, const char *buffer, const int buffLen, const char *fileName)
{
    if(buffer == nullptr || fileName == nullptr || imgW < 1 || imgH < 1 || buffLen < 1)
        return false;

    tagBmpFileHeader  headFile;
    headFile.bfType = 0x4D42;
    headFile.bfSize = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader) + buffLen;
    headFile.bfReserved1 = headFile.bfReserved2 = 0;
    headFile.bfOffBits = sizeof(tagBmpFileHeader ) + sizeof(tagBmpInfoHeader);

    tagBmpInfoHeader headInfo;
    headInfo.biSize = sizeof(tagBmpInfoHeader);
    headInfo.biWidth = imgW;
    headInfo.biHeight = imgH;
    headInfo.biPlanes = 1;
    headInfo.biBitCount = 24;
    headInfo.biCompression = 0;
    headInfo.biSizeImage = buffLen;
    headInfo.biXPelsPerMeter = headInfo.biYPelsPerMeter = 0;
    headInfo.biClrUsed = 0;
    headInfo.biClrImportant = 0;

    std::ofstream outFile(fileName,std::ios::binary);
    outFile.write((char*)(&headFile),sizeof(headFile));
    outFile.write((char*)(&headInfo),sizeof(headInfo));

    outFile.write(buffer,buffLen);
    outFile.close();
    return true;
}

bool COperateBMP::OutPutImgBMP(const BMPIMG &img,const char* fileName)
{
    std::ofstream outFile(fileName,std::ios::binary);
    outFile.write((char*)(&img._fileHead),sizeof(tagBmpFileHeader));
    outFile.write((char*)(&img._infoHead),sizeof(tagBmpInfoHeader));
    for(int i = 0; i< img._pallete.size(); ++i)
    {
        outFile.write((char*)(&img._pallete[i]),sizeof(tagRGBPallete));
    }
    outFile.write(img.buff,img._fileHead.bfSize - img._fileHead.bfOffBits);
    outFile.close();
    return true;
}

bool COperateBMP::ReadImgBMP(const char *fileName, BMPIMG& img)
{
    CFileOperate _file;
    size_t len = _file.readFileBinary(fileName,nullptr);
    char* fileChar = new char[len];
    _file.readFileBinary(fileName,fileChar);
    if(len < sizeof(tagBmpFileHeader) + sizeof(tagBmpInfoHeader))
        return false;
    memcpy(&img._fileHead,fileChar,sizeof(tagBmpFileHeader));
    memcpy(&img._infoHead,fileChar + sizeof(tagBmpFileHeader),sizeof(tagBmpInfoHeader));
    if(img._fileHead.bfType != 0x4D42)
        return false;
    if(len != img._fileHead.bfSize)
        return false;
    int palleteLen = img._fileHead.bfOffBits - sizeof(tagBmpFileHeader) - sizeof(tagBmpInfoHeader);
    int palleteCount = palleteLen/sizeof(tagRGBPallete);

    for(int i = 0; i< palleteCount; ++i)
    {
        tagRGBPallete tmp;
        memcpy(&tmp,fileChar + sizeof(tagBmpFileHeader) + sizeof(tagBmpInfoHeader) + i * sizeof(tagRGBPallete),sizeof(tagRGBPallete));
        img._pallete.push_back(tmp);
    }
    int bufferLen = /*len - img._fileHead.bfOffBits*/img._infoHead.biSizeImage;
    img.buff = new char[bufferLen];
    memcpy(img.buff,fileChar + sizeof(tagBmpFileHeader) + sizeof(tagBmpInfoHeader) + palleteCount * sizeof(tagRGBPallete),bufferLen);
    return true;
}
